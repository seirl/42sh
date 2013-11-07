#!/usr/bin/env python3

import json
import os
import os.path
import subprocess
import sys
import time
import unittest
import warnings

from termcolors import colorize

class QDTestSuite(unittest.TestSuite):
    def __init__(self, category, *args, **kwargs):
        super().__init__(*args, **kwargs)

        self.category = category

class QDTestCase(unittest.TestCase):
    def __init__(self, category, test_name, description, *args, **kwargs):
        super().__init__(*args, **kwargs)
        self.category = category
        self.test_name = test_name
        self.description = description

    def __str__(self):
        return "{}/{}".format(self.category, self.test_name)

    def setUp(self):
        self.program_name = "../42sh"

    def get_test_name(self):
        return self.category + '_' + self.test_name

    def get_test_path(self):
        return os.path.join(self.category, self.test_name)

    def start_shell(self, args):
        self._shell = subprocess.Popen(["valgrind",
            "--xml=yes",
            "--xml-file={}.memcheck".format(self.get_test_path()),
            "--log-file={}.log".format(self.get_test_path()),
            self.program_name] + args,
            stdin=subprocess.PIPE,
            stdout=subprocess.PIPE,
            stderr=subprocess.PIPE)

class _WritelnDecorator(object):
    """Used to decorate file-like objects with a handy 'writeln' method."""
    def __init__(self,stream):
        self.stream = stream

    def __getattr__(self, attr):
        if attr in ('stream', '__getstate__'):
            raise AttributeError(attr)
        return getattr(self.stream,attr)

    def writeln(self, arg=None):
        if arg:
            self.write(arg)
        self.write('\n') # text-mode streams translate to \r\n if needed

class QDTestResult(unittest.TestResult):
    """A test result class that can print formatted text results to a stream.

    Used by QDTestRunner.
    """
    separator1 = '=' * 70
    separator2 = '-' * 70

    def __init__(self, stream, descriptions, verbosity):
        super().__init__(stream, descriptions, verbosity)
        self.stream = stream
        self.showAll = verbosity > 1
        self.dots = verbosity == 1
        self.descriptions = descriptions

    def getDescription(self, test):
        doc_first_line = test.shortDescription()
        if self.descriptions and doc_first_line:
            return '\n'.join((str(test), doc_first_line))
        else:
            return str(test)

    def startTest(self, test):
        super().startTest(test)
        if self.showAll:
            self.stream.write(self.getDescription(test))
            self.stream.write(" ... ")
            self.stream.flush()

    def addSuccess(self, test):
        super().addSuccess(test)
        if self.showAll:
            if self.stream.isatty():
                self.stream.writeln(colorize("ok", fg="green"))
            else:
                self.stream.writeln("ok")
        elif self.dots:
            self.stream.write('.')
            self.stream.flush()

    def addError(self, test, err):
        super().addError(test, err)
        if self.showAll:
            if self.stream.isatty():
                self.stream.writeln(colorize("ERROR", bg="white", fg="red"))
            else:
                self.stream.writeln("ok")
        elif self.dots:
            self.stream.write('E')
            self.stream.flush()

    def addFailure(self, test, err):
        super().addFailure(test, err)
        if self.showAll:
            if self.stream.isatty():
                self.stream.writeln(colorize("FAIL", fg="red"))
            else:
                self.stream.writeln("ok")
        elif self.dots:
            self.stream.write('F')
            self.stream.flush()

    def printErrors(self):
        if self.dots or self.showAll:
            self.stream.writeln()
        self.printErrorList('ERROR', self.errors)
        self.printErrorList('FAIL', self.failures)

    def printErrorList(self, flavour, errors):
        for test, err in errors:
            self.stream.writeln(self.separator1)
            self.stream.writeln("%s: %s" % (flavour,self.getDescription(test)))
            self.stream.writeln(self.separator2)
            self.stream.writeln("%s" % err)

    def print_summary(self):
        self.stream.writeln()

        rate = 100 - len(self.failures + self.errors) / self.testsRun * 100
        rate_str = colorize("{:.0f}".format(rate),
                fg="green" if rate == 100 else "red")
        self.stream.writeln(
            "Ran: {} Failure: {} Errors: {} Rate: {}%".format(
                self.testsRun,
                len(self.failures),
                len(self.errors),
                rate_str))

class QDTestRunner:
    """A test runner class that displays results in textual form.

    It prints out the names of tests as they are run, errors as they
    occur, and a summary of the results at the end of the test run.
    """
    resultclass = QDTestResult

    def __init__(self, stream=None, descriptions=True, verbosity=1,
                 failfast=False, buffer=False, resultclass=None,
                 warnings=None):
        if stream is None:
            stream = sys.stderr
        self.stream = _WritelnDecorator(stream)
        self.descriptions = descriptions
        self.verbosity = verbosity
        self.failfast = failfast
        self.buffer = buffer
        self.warnings = warnings
        if resultclass is not None:
            self.resultclass = resultclass

    def _makeResult(self):
        return self.resultclass(self.stream, self.descriptions, self.verbosity)
        result.failfast = self.failfast
        result.buffer = self.buffer

    def run_single(self, test):
        result = self._makeResult()
        test(result)
        return result

    def run_suite(self, suite):
        failures = []
        errors = []

        for subtest in suite._tests:
            if hasattr(subtest, "category"):
                self.stream.writeln("\nCategory: " +
                        colorize(subtest.category, fg="blue") + "\n")
            result = self._makeResult()
            subtest(result)
            result.print_summary()
            failures.extend(result.failures)
            errors.extend(result.errors)
            result = self._makeResult()

        result = self._makeResult()
        result.failures = failures
        result.errors = errors

        return result

    def run(self, test):
        "Run the given test case or test suite."

        if isinstance(test, unittest.TestSuite):
            return self.run_suite(test)
        else:
            return self.run_single(test)

class QDTestLoader(unittest.TestLoader):
    """Find and load tests."""

    def __init__(self, select, timeout, *args, **kwargs):
        super().__init__(*args, **kwargs)
        self.select = select
        self.timeout = timeout

    def _load_test_case(self, directory, test_filename):
        """Return a TestCase generated from the file test_file, expected to
        contain a json structure reprensenting the test."""
        with open(os.path.join(directory, test_filename)) as f:
            test = json.load(f)

            def test_it(subself):
                shell_args = test.get('args', [])
                subself.start_shell(shell_args)

                timeout = test.get('timeout', self.timeout)
                stdin_buf = test.get('stdin', None)
                stdoutdata, stderrdata = subself._shell.communicate(stdin_buf,
                                                                 timeout)

                if 'stdout' in test:
                    subself.assertEqual(stdoutdata,
                                     test['stdout'].encode(),
                                     "stdout differ")

                retval = test.get('retval', 0)
                subself.assertEqual(subself._shell.returncode, retval,
                                 "return value differ")

            category = os.path.basename(directory)
            test_class_name = "Test{}{}".format(
                    category.title(),
                    test_filename.title())
            test_method_name = 'test_' + test_filename
            # We create the method here to give it the right name
            test_class_methods = {test_method_name: test_it}
            test_case_class = type(test_class_name,
                    (QDTestCase, ),
                    test_class_methods,)
            test_description = test.get('description', "A test")

            return test_case_class(methodName=test_method_name,
                    category=category,
                    test_name=test_filename.rstrip(".test"),
                    description=test_description)

    def _load_test_suite(self, directory, filenames):
        """Return a TestSuite for the directory ``directory``."""
        test_suite = QDTestSuite(directory)
        empty = True

        for filename in filenames:
            if filename.endswith(".test"):
                test_suite.addTest(self._load_test_case(directory, filename))
                empty = False

        if empty:
            return None
        else:
            return test_suite

    def discover(self, start_dir, pattern, top_level_dir):
        """Add discovery of test from files ending with .test. Filter
        categories self.select is != None.
        """

        # Discover python scripts
        if not self.select:
            test = super().discover(start_dir, pattern, top_level_dir)
        else:
            test = unittest.TestSuite()
        for dirpath, dirnames, filenames in os.walk(start_dir):
            directory = os.path.basename(dirpath)
            if self.select and directory != self.select:
                continue
            test_suite = self._load_test_suite(directory, filenames)
            if test_suite:
                test.addTest(test_suite)

        return test

class TestProgram(unittest.TestProgram):
    """
    A variation of the unittest.TestProgram. Please refer to the base
    class for command line parameters.
    """

    def __init__(self, categories=False, select=None, final=False,
            number=False, all=True, timeout=None, *args, **kwargs):
        self.categories = categories
        self.select = select
        self.final = final
        self.number = number
        self.all = all
        self.timeout = timeout
        super().__init__(*args, **kwargs)

    def _setAttributesFromOptions(self, options):
        """Store options in instance attributes."""
        super()._setAttributesFromOptions(options)
        self.categories = options.categories
        self.select = options.select
        self.final = options.final
        self.number = options.number
        self.timeout = options.timeout
        self.all = options.all

    def _getOptParser(self):
        """Setup and run option parser."""
        parser = super()._getOptParser()
        # Will redefine some options
        parser.set_conflict_handler("resolve")

        # Redefine default verbosity to verbose
        parser.add_option('-v', '--verbose', dest='verbose', default=True,
                          help='Verbose output', action='store_true')
        parser.add_option('-q', '--quiet', dest='verbose', default=True,
                          help='Verbose output', action='store_false')

        parser.add_option("-c", "--categories", action="store_true",
            help="Display the categories and the percentage of successful tests.")
        parser.add_option("-e", "--select",
            help="Select category of tests. Will only display categories name and percentage of success.")
        parser.add_option("-f", "--final", action="store_true",
            help="Display the percentage of successful tests.")
        parser.add_option("-n", "--number", action="store_true",
            help="Display the number of successful test.")
        parser.add_option("-a", "--all", action="store_true",
            help="Execute the test suite on all categories.")

        parser.add_option("-t", "--timeout", default=None, type=float,
            help="General timeout in seconds for each test.")

        return parser

    def _do_discovery(self, *args, **kwargs):
        """Monkey patch testLoader."""
        self.testLoader = QDTestLoader(select=self.select, timeout=self.timeout)
        super()._do_discovery(*args, **kwargs)

    def _addDiscoveryOptions(self, parser):
        """Change -t option to -T"""
        parser.add_option('-s', '--start-directory', dest='start', default='.',
                help="Directory to start discovery ('.' default)")
        parser.add_option('-p', '--pattern', dest='pattern',
                default='test*.py',
                help="Pattern to match tests ('test*.py' default)")
        parser.add_option('-T', '--top-level-directory', dest='top',
                default=None,
                help='Top level directory of project (defaults to start directory)')

    def createTests(self):
        """Monkey patch testLoader."""
        self.testLoader = QDTestLoader(select=self.select,
                timetout=self.timeout)
        super().createTests()

    def runTests(self):
        self.testRunner = QDTestRunner(verbosity=self.verbosity)
        super().runTests()

main = TestProgram

if __name__ == "__main__":
    main(module=None)
