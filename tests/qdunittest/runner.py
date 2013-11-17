import sys
import unittest

from .result import QDTestResult

from .termcolors import colorize

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

    def __init__(self, options):
        self.stream = _WritelnDecorator(sys.stderr)
        self.final = options.final
        self.number = options.number

        self.options = options

    def _make_result(self):
        return QDTestResult(self.options, self.stream)

    def run_single(self, test):
        result = self._make_result()
        test(result)
        result.print_summary()
        return result

    def run_suite(self, suite):
        failures = []
        errors = []
        skipped = []
        tests_run = 0

        # No tests, return dummy result
        if len(suite._tests) == 0:
            return self._make_result()

        for subtest in suite._tests:
            if not self.final:
                if hasattr(subtest, "category"):
                    self.stream.writeln("\nCategory: " +
                            colorize(subtest.category, fg="blue"))
                    if not self.options.categories:
                        self.stream.write("\n")
                else:
                    try:
                        category = subtest._tests[0]._tests[0].__class__.__name__
                        self.stream.writeln("\nCategory: " +
                                colorize(category, fg="blue") + "\n")
                    except:
                        pass
            result = self._make_result()
            subtest(result)
            if not self.final:
                result.print_summary(self.number)

            failures.extend(result.failures)
            errors.extend(result.errors)
            skipped.extend(result.skipped)
            tests_run += result.testsRun

            result = self._make_result()

        if not self.final:
            self.stream.write("\n")
        self.stream.write("Final results: ")
        result = self._make_result()
        result.failures = failures
        result.errors = errors
        result.skipped = skipped
        result.testsRun = tests_run
        result.print_summary(self.number, show_errors=False)

        return result

    def run(self, test):
        "Run the given test case or test suite."

        if isinstance(test, unittest.TestSuite):
            return self.run_suite(test)
        else:
            return self.run_single(test)
