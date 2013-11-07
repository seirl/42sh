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
        result.print_summary()
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
