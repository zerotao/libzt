#!/usr/bin/env python

import types
import yaml
import os

class SuiteStat(object):
    assertions = 0
    tests = 0
    failures = 0
    successes = 0
    empty_tests = None
    errors = None

    def __init__(self):
        self.empty_tests = []
        self.errors = []


def stat_suite(suite):
    ss = SuiteStat()
    # assertions = 0
    # tests = 0
    # failures = 0
    # successes = 0
    # empty_tests = []

    test_section = suite['Tests']
    if test_section != None:
        for name, values in test_section.items():
            asserts = values['assertions']
            ss.assertions += asserts
            if asserts == 0:
                ss.empty_tests.append(name)

            ss.tests += 1
            if values['result'] == 'success':
                ss.successes += 1
            else:
                ss.failures += 1

    ss.errors = suite.get('Errors', [])
    return ss #tests, assertions, failures, successes, errors, empty_tests


def stat_ut(fd):
    stats = []

    try:
        tests = yaml.load_all(fd)

        for suite in tests:
            if type(suite) not in types.StringTypes:
                for suite_name, values in suite.items():
                    sstats = stat_suite(values)
                    stats.append(sstats)

    except Exception, e:
        print "Error parsing Unit Test input: %s" % e
        return

    print_stat(stats)


def print_stat(stats):
    suites = len(stats)
    assertions = 0
    tests = 0
    successes = 0
    failures = 0
    errors = []
    empty_tests = []

    for suite in stats:
        #st, sa, sf, ss, er, et
        tests += suite.tests
        assertions += suite.assertions
        successes += suite.successes
        failures += suite.failures
        if suite.errors:
            errors.append(suite.errors)

        empty_tests.extend(suite.empty_tests)
    print """
---
Global Stats:
  suites                      : %d
  tests                       : %d
  successes                   : %d
  assertions                  : %d
  failures                    : %d
  empty_tests                 : %s
""" % (suites, tests, successes, assertions, failures, ", ".join(empty_tests)),
    if empty_tests:
        print "  empty_tests:"
        print "\n".join(["     - %s" % x for x in empty_tests])
    if errors:
        print "  errors:"
        for error in errors:
            print "\n".join(["    - %r" % x for x in error.values()])


if __name__ == '__main__':
    import optparse
    import sys
    from StringIO import StringIO

    parser = optparse.OptionParser(usage="usage: %prog [options] prog [-- prog_args]")

    (options, args) = parser.parse_args()

    if len(args) < 1:
        parser.print_help()
        sys.exit(1)

    # run the command
    try:
        import subprocess as sp
        p = sp.Popen(args, shell=True,
                     stdin=sp.PIPE, stdout=sp.PIPE, stderr=sp.STDOUT,
                     close_fds=True)
        (stdin, stdout) = (p.stdin, p.stdout)
    except:
        stdin, stdout = os.popen4(args)

    input = StringIO()
    for line in stdout:
        print line,
        input.write(line)

    stdin.close()
    stdout.close()

    input.seek(0)
    stat_ut(input)
    input.close()
