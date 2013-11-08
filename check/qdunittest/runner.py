import sys
import unittest

from .result import QDTestResult

from termcolors import colorize

class _WritelnDecorator():
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

class QDTestRunner:
    """A test runner class that displays results in textual form.

    It prints out the names of tests as they are run, errors as they
    occur, and a summary of the results at the end of the test run.
    """

    def __init__(self, stream=None, descriptions=True, verbosity=1,
            final=False, number=True):
        if stream is None:
            stream = sys.stderr
        self.stream = _WritelnDecorator(stream)
        self.descriptions = descriptions
        self.verbosity = verbosity if not final else 0
        self.final = final
        self.number = number

    def _makeResult(self):
        return QDTestResult(self.stream, self.descriptions, self.verbosity)

    def run_single(self, test):
        result = self._makeResult()
        test(result)
        result.print_summary()
        return result

    def run_suite(self, suite):
        failures = []
        errors = []
        tests_run = 0

        for subtest in suite._tests:
            if not self.final:
                if hasattr(subtest, "category"):
                    self.stream.writeln("\nCategory: " +
                            colorize(subtest.category, fg="blue") + "\n")
                else:
                    try:
                        category = subtest._tests[0]._tests[0].__class__.__name__
                        self.stream.writeln("\nCategory: " +
                                colorize(category, fg="blue") + "\n")
                    except:
                        pass
            result = self._makeResult()
            subtest(result)
            if not self.final:
                result.print_summary(self.number)

            failures.extend(result.failures)
            errors.extend(result.errors)
            tests_run = result.testsRun

            result = self._makeResult()

        if not self.final:
            self.stream.write("\n")
        self.stream.write("Final results: ")
        result = self._makeResult()
        result.failures = failures
        result.errors = errors
        result.testsRun = tests_run
        result.print_summary(self.number)

        return result

    def run(self, test):
        "Run the given test case or test suite."

        if isinstance(test, unittest.TestSuite):
            return self.run_suite(test)
        else:
            return self.run_single(test)