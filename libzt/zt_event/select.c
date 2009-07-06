#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include <sys/select.h>
#include <sys/time.h>
#include <errno.h>

#include <libzt/zt.h>
#include <libzt/zt_assert.h>
#include <libzt/zt_log.h>
#include <libzt/adt/zt_tree.h>
#include <libzt/zt_time.h>

#include "private.h"
#include "gtimer.h"

struct zt_event_elt {
	zt_rbt_node		  node;
	
	zt_event_enum	  type;
	int	  			  fd;
	
	long			  tod;
	
	zt_event_io_cb	* read_cb;
	zt_event_io_cb	* write_cb;
	zt_event_io_cb	* except_cb;
	
	void			* read_data;
	void			* write_data;
	void			* except_data;
};

static int elt_cmp(zt_rbt_node *n1, zt_rbt_node *n2);
static int gtimer_cmp(zt_rbt_node *n1, zt_rbt_node *n2);
static void recalc_min_interval(zt_event_sys sys);

typedef struct zt_event_sys_select zt_event_sys_select;
struct zt_event_sys_select {
	struct zt_event_sys	  sys;

	int					  max_fd;
	
	fd_set				  read_set;
	fd_set				  write_set;
	fd_set				  except_set;

	zt_rbt				* io_head;
	
	zt_rbt				* timer_events;
	zt_rbt				* signal_events;

};

static void destroy( zt_event_sys sys)
{
	XFREE(sys);
}


static int
register_io(zt_event_sys sys, int fd, zt_event_enum type, zt_event_io_cb cb, void *data)
{
	struct zt_event_sys_select	* ess = (struct zt_event_sys_select *)sys;
	struct zt_event_elt			* eltp;
	struct zt_event_elt			  elt;
	zt_rbt_node					* nodep;

	elt.fd = fd;
	
	nodep = zt_rbt_find(&ess->io_head, &elt.node, elt_cmp);
	
	if(nodep) {
		eltp = zt_rbt_data(nodep, struct zt_event_elt, node);
	} else {
		eltp = XCALLOC(struct zt_event_elt, 1);
		eltp->fd = fd;
	}
	
	if(type & ZT_READ_EVENT) {
		FD_SET(fd, &ess->read_set);
		eltp->type |= ZT_READ_EVENT;
		eltp->read_data = data;
		eltp->read_cb = cb;
	}

	if(type & ZT_WRITE_EVENT) {
		FD_SET(fd, &ess->write_set);
		eltp->type |= ZT_WRITE_EVENT;
 		eltp->write_data = data;
		eltp->write_cb = cb;
	}

	if(type & ZT_EXCEPT_EVENT) {
		FD_SET(fd, &ess->except_set);
		eltp->type |= ZT_EXCEPT_EVENT;
		eltp->except_data = data;
		eltp->except_cb = cb;
	}

	zt_rbt_insert(&ess->io_head, &eltp->node, elt_cmp);
	
	ess->max_fd = MAX(fd, ess->max_fd);
	
	return 0;
}

static int
remove_io(zt_event_sys sys, int fd, zt_event_enum type) /* , zt_event_io_cb cb, void *data) */
{
	struct zt_event_sys_select	* ess = (struct zt_event_sys_select *)sys;
	struct zt_event_elt			  elt;
	struct zt_event_elt			* eltp;
	zt_rbt_node				* nodep;
	
	elt.fd = fd;	
	nodep = zt_rbt_find(&ess->io_head, &elt.node,  elt_cmp);
	
	if(nodep) {
		eltp = zt_rbt_data(nodep, struct zt_event_elt, node);
		
		zt_rbt_remove(&ess->io_head, nodep);
		
		if(eltp->type & ZT_READ_EVENT) {
			FD_CLR(fd, &ess->read_set);
			eltp->type &= ~(ZT_READ_EVENT);
			eltp->read_cb = 0;
			eltp->read_data = 0;
		}

		if(eltp->type & ZT_WRITE_EVENT) {
			FD_CLR(fd, &ess->write_set);
			eltp->type &= ~(ZT_READ_EVENT);
			eltp->write_cb = 0;
			eltp->write_data = 0;
		}

		if(eltp->type & ZT_EXCEPT_EVENT) {
			FD_CLR(fd, &ess->except_set);
			eltp->type &= ~(ZT_READ_EVENT);
			eltp->except_cb = 0;
			eltp->except_data = 0;
		}

		if(eltp->type == 0) {
			
			if(eltp->fd == ess->max_fd) {
				struct zt_event_elt	* e;
			
				nodep = zt_rbt_max(&ess->io_head);
			
				if(nodep) {
					e = zt_rbt_data(nodep, struct zt_event_elt, node);
					ess->max_fd = e->fd;
				} else {
					ess->max_fd = 0;
				}
			}
		
			XFREE(eltp);
		}
		
	} else {
		zt_log_printf(zt_log_err, "attempt to remove io event which was never added for fd: %d", fd);
	}	
	
	return 0;
}

	
static int
register_signal(zt_event_sys sys, int signal, zt_event_signal_cb cb, void *data)
{
	return 0;
}

static int
remove_signal(zt_event_sys sys, int signal, zt_event_signal_cb cb, void *data)
{
	return 0;
}

	
static int
register_timer(zt_event_sys sys, struct timeval *time, zt_event_timer_cb cb, void *data, zt_event_timer_flags flags)
{
	struct zt_event_sys_select	* ess = (struct zt_event_sys_select *)sys;
	struct zt_timer_node		* new;
	zt_rbt_node					* nodep;
	struct timeval				  ntime;
	
	zt_assert(ess);
	zt_assert(time);

	if(flags & ZT_EVENT_TIMER_ABSOLUTE) {
		zt_copy_timeval(&ntime, time);
		/* absolute timers are always one shot timers */
		flags |= ZT_EVENT_TIMER_ONCE;
		
	} else {
		zt_add_time(&ntime, &sys->tod, time);
	}
		    
	new = gtimer_new(&ntime, time, cb, data, flags);
		    
	nodep = zt_rbt_insert(&ess->timer_events, &new->node, gtimer_cmp);
	
	if(nodep != NULL) {
		gtimer_free(new);
		return -1;
	}

	if(zt_cmp_time(&sys->min_timeout, time) > 0) {
		zt_copy_timeval(&sys->min_timeout, time);
	}
	
	return 0;
}

static int
remove_timer(zt_event_sys sys, struct timeval *time, zt_event_timer_cb cb, void *data)
{
	struct zt_event_sys_select	* ess = (struct zt_event_sys_select *)sys;
	struct zt_timer_node		  node;
	struct zt_timer_node		* eltp;
	zt_rbt_node					* nodep;
	
	zt_assert(time);
	zt_assert(ess);
	
	node.when.tv_sec = time->tv_sec;
	node.when.tv_usec = time->tv_usec;

	nodep = zt_rbt_remove(&ess->timer_events, &node.node);
	
	if(nodep) {
		eltp = zt_rbt_data(nodep, struct zt_timer_node, node);
		gtimer_free(eltp);
	}

	recalc_min_interval(sys);
	
	return 0;
}


static void reset_fd_sets(fd_set *read_set, fd_set *orig_read_set,
						  fd_set *write_set, fd_set *orig_write_set,
						  fd_set *except_set, fd_set *orig_except_set,
						  zt_event_flags flags)
{

	FD_ZERO(read_set);
	FD_ZERO(write_set);
	FD_ZERO(except_set);
	
	if(flags & ZT_READ_EVENTS) {
		*read_set = *orig_read_set;
		/* FD_COPY(orig_read_set, read_set); */
	}

	if(flags & ZT_WRITE_EVENTS) {
		*write_set = *orig_write_set;
		/* FD_COPY(orig_write_set, write_set); */
	}

	if(flags & ZT_EXCEPT_EVENTS) {
		*except_set = *orig_except_set;
		/* FD_COPY(orig_except_set, except_set); */
	}

}

static int
handle_events(zt_event_sys sys,  zt_event_flags flags)
{
	struct zt_event_sys_select	* ess = (struct zt_event_sys_select *)sys;
	fd_set				  except_set;
	fd_set				  read_set;
	int				  	  result;
	int				  	  run_once;
	struct timeval		  timeout;
	fd_set				  write_set;
	struct timeval 		  zero_timeout = {0, 0};
	
	zt_assert(ess);

	run_once = flags & ZT_RUN_ONCE;	
	if(flags & ZT_NON_BLOCK) {
		zt_copy_timeval(&timeout, &zero_timeout);
	} else {
		zt_copy_timeval(&timeout, &sys->min_timeout);
	}	

	reset_fd_sets(&read_set, &ess->read_set,
				  &write_set, &ess->write_set,
				  &except_set, &ess->except_set,
				  flags);
	
	while((result = select(ess->max_fd + 1, &read_set, &write_set,
						   &except_set, &timeout)) != -1) {
		
		gettimeofday(&sys->tod, NULL);
		
		if(result == 0) {
			/* timeout */
			goto TIMEOUT;
		} else {
			/* events ready */
			zt_rbt_node	* iter;
			
			/*
			 * not very effecient but will probably have to do for the
			 * moment as there is no way, that I know of, to query the
			 * select sets to get which objects were set...
			 */
			zt_rbt_for_each(&ess->io_head, iter){
				struct zt_event_elt	* eltp = zt_rbt_data(iter, struct zt_event_elt, node);

				if((flags & ZT_READ_EVENTS) &&
				   (eltp->type & ZT_READ_EVENT) &&
				   (FD_ISSET(eltp->fd, &read_set))) {
					eltp->read_cb(sys, eltp->fd, ZT_READ_EVENT, eltp->read_data);
				}

				if((flags & ZT_WRITE_EVENTS) &&
				   (eltp->type & ZT_WRITE_EVENT) &&
				   (FD_ISSET(eltp->fd, &write_set))) {
					eltp->write_cb(sys, eltp->fd, ZT_WRITE_EVENT, eltp->write_data);
				}
				
				if((flags & ZT_EXCEPT_EVENTS) &&
				   (eltp->type & ZT_EXCEPT_EVENT) &&
				   (FD_ISSET(eltp->fd, &except_set))) {
					eltp->except_cb(sys, eltp->fd, ZT_EXCEPT_EVENT, eltp->except_data);
				}
			}	
		}
	TIMEOUT:
		if(flags & ZT_TIMER_EVENTS) {
			zt_rbt_node	* nodep;
			zt_rbt_node	* nextp;
			long		  sec;
			long		  usec;
			
			sec = sys->tod.tv_sec;
			usec = sys->tod.tv_usec;

			/* FIXME: we might as well be using a list here... as we
			 * are not using the tree to eleminate any
			 * checks... change that. Keep in mind that this is
			 * because we comapair the data and callback as part of
			 * the timer. maybe (time -> [(cb, data), (cb, data), ...]
			 */
			zt_rbt_for_each_safe(&ess->timer_events, nodep, nextp) {
				struct zt_timer_node	* te;
				te = zt_rbt_data(nodep, struct zt_timer_node, node);
				if((te->when.tv_sec < sec) ||
				   ((te->when.tv_sec == sec) &&
				    (te->when.tv_usec <= usec))) {
					/* execute  */
					te->cb(sys, &te->when, te->data);
					
					if(te->flags & ZT_EVENT_TIMER_ONCE) {
						zt_rbt_remove(&ess->timer_events, nodep);
						sys->num_events--;
						gtimer_free(te);
					}
				}
			}
			recalc_min_interval(sys);
		}
		
		if(run_once) {
			return result;
		}
		
		reset_fd_sets(&read_set, &ess->read_set,
					  &write_set, &ess->write_set,
					  &except_set, &ess->except_set,
					  flags);
	}
	
	
	return 0;
}

static zt_event_vtbl vtbl = {
	sizeof(zt_event_sys_select),
	register_io,
	remove_io,
	register_signal,
	remove_signal,
	register_timer,
	remove_timer,
	handle_events,
	destroy,
};

zt_event_sys 
zt_event_select(void) 
{
	zt_event_sys	sys;
	
	sys = zt_event_new(&vtbl);
	gettimeofday(&sys->tod, NULL);
	return sys;
}


static int elt_cmp(zt_rbt_node *n1, zt_rbt_node *n2)
{
	struct zt_event_elt	* e1;
	struct zt_event_elt	* e2;
	
	zt_assert(n1 && n2);
	
	e1 = zt_rbt_data(n1, struct zt_event_elt, node);
	e2 = zt_rbt_data(n2, struct zt_event_elt, node);

	if(e1->fd < e2->fd) {
		return -1;
	} else if (e1->fd > e2->fd) {
		return 1;
	}
	
	return 0;
}

static int gtimer_cmp(zt_rbt_node *n1, zt_rbt_node *n2)
{
	struct zt_timer_node	* g1;
	struct zt_timer_node	* g2;
	
	zt_assert(n1 && n2);
	
	g1 = zt_rbt_data(n1, struct zt_timer_node, node);
	g2 = zt_rbt_data(n2, struct zt_timer_node, node);

	if(g1->when.tv_sec < g2->when.tv_sec) {
		return -1;
	} else if (g1->when.tv_sec > g2->when.tv_sec) {
		return 1;
	}

	if(g1->when.tv_usec < g2->when.tv_usec) {
		return -1;
	} else if (g1->when.tv_usec > g2->when.tv_usec) {
		return 1;
	}

	/* 
     * if(g1->cb > g2->cb) {
	 * 	return -1;
	 * } else if (g1->cb < g2->cb) {
	 * 	return 1;
	 * }
     */

	if(g1->data > g2->data) {
		return -1;
	} else if (g1->data < g2->data) {
		return 1;
	}
	
	return 0;
}

static void recalc_min_interval(zt_event_sys sys) {
	struct zt_event_sys_select	* ess = (struct zt_event_sys_select *)sys;
	zt_rbt_node					* nodep;
		
	nodep = zt_rbt_min(&ess->timer_events);
	if(nodep) {
		struct zt_timer_node	* ne;
		ne = zt_rbt_data(nodep, struct zt_timer_node, node);

		if(zt_cmp_time(&sys->tod, &ne->when) > 0) {
			sys->min_timeout.tv_sec = 0;
			sys->min_timeout.tv_usec = 0;
		} else {
			zt_diff_time(&sys->min_timeout, &sys->tod, &ne->when);
		}
	} else {
		sys->min_timeout.tv_sec = 0;
		sys->min_timeout.tv_usec = 0;
	}
}
