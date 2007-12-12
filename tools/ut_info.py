#!/usr/bin/env python

import yaml

def stat_suite(suite):
    assertions = 0
    tests = 0
    failures = 0
    successes = 0

    test_section = suite['Tests']
    if test_section != None:
        for name, values in test_section.items():
            assertions += values['assertions']
            tests += 1
            if values['result'] == 'success':
                successes += 1
            else:
                failures += 1

    errors = suite.get('Errors', [])    
    return tests, assertions, failures, successes, errors
    
    
def stat_ut(fd):
    stats = []
    
    tests = yaml.load_all(fd) 
    for suite in tests:
        for suite_name, values in suite.items():
            sstats = stat_suite(values)
            stats.append(sstats)

    print_stat(stats)


def print_stat(stats):
    suites = len(stats)
    assertions = 0
    tests = 0
    successes = 0
    failures = 0
    errors = []

    for st, sa, sf, ss, er in stats:
        tests += st
        assertions += sa
        successes += ss
        failures += sf
        if er:
            errors.append(er)

    print """
---
Global Stats:
  suites                      : %d
  tests                       : %d
  successes                   : %d
  assertions                  : %d
  failures                    : %d
""" % (suites, tests, successes, assertions, failures),
    if errors:
        print "  errors:"
        for error in errors:
            print "\n".join(["    - %r" % x for x in error.values()])


if __name__ == '__main__':
    import optparse
    import sys

    parser = optparse.OptionParser(usage="usage: %prog [options] file")

    (options, args) = parser.parse_args()

    if len(args) != 1:
        parser.print_help()
        sys.exit(1)

    if args[0] == '-':
        stat_ut(sys.stdin)

    else:
        f = open(args[0], "r")
        stat_ut(f)
        f.close()
