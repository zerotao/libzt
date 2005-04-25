DOC   = libzt

SECTS = \
	glossary.sgml

FIGS  = 

DSSLPATH = /usr/lib/sgml/stylesheet/dsssl/docbook/nwalsh/
#/home/jshiffer/projects/other/docbook

# No need to alter from here
MAIN  = $(DOC).sgml
JPEGS = $(FIGS:.fig=.jpeg)
EPSS  = $(FIGS:.fig=.eps)
G_IMG = $(JPEGS) $(EPSS)
SRC   = builddate $(MAIN) $(SECTS) $(MAIN).sed $(addsufix .sed,$(SECTS) Makefile.sed  $(G_IMG) 

#genindex.sgml

all: 
	@robodoc --rc robodoc.rc --html
#	@robodoc --rc robodoc.rc --latex
#$(DOC).ps $(DOC)

genindex.sgml:
	perl $(DSSLPATH)/bin/collateindex.pl -N -o genindex.sgml
	jade -t sgml -d $(DSSLPATH)/html/docbook.dsl -V html-index -V nochunks $(MAIN)
	perl $(DSSLPATH)/bin/collateindex.pl -o genindex.sgml HTML.index

$(DOC).pdf: $(SRC)
	db2pdf $(MAIN)

$(DOC).ps: $(MAIN)
	db2ps $(MAIN)

$(DOC): $(SRC)
	-$(RM) -r $(DOC)
	db2html $(MAIN)
	(cd $@; for file in *.html; do \
	  mv $$file $$file.tmp; \
	  sed -e 's/&#8212;/--/g' $$file.tmp > $$file; \
	  rm $$file.tmp; \
	done)
	if [ "$(JEPGS)" != "" ]; then \
	  cp $(JPEGS) $@; \
	fi

builddate:
	date "+%e %B %Y" > $@

%.sed: %
	fold -w 72 $< | \
	  sed -e "s/$$(echo x | tr x \\t)/        /g" \
	      -e "s/&/\\&amp;/g" \
	      -e "s/</\\&lt;/g"  \
	      -e "s/>/\\&gt;/g"  \
	      -e "s/--/\\&mdash;/g" > $@

%.eps: %.fig
	fig2dev -Leps $< $@

%.jpeg: %.fig
	fig2dev -Ljpeg $< $@

.PHONY: all builddate


clean-local:
	$(RM) -r $(DOC) 

include $(top_srcdir)/Makefile.extra

MAINTAINERCLEANFILES += HTML.index genindex.sgml
CLEANFILES += *.log *.dvi *.aux *.tex *.sed *.ps builddate $(G_IMG)
DISTCLEANFILES += *.tar.gz 
