import unittest
import subprocess

from .termcolors import colorize

class QDTestResult(unittest.TestResult):
    """A test result class that can print formatted text results to a stream.

    Used by QDTestRunner.
    """
    separator1 = '=' * 70
    separator2 = '-' * 70

    def __init__(self, options, stream):
        super().__init__()
        self.verbose = (options.verbose
                and not options.final
                and not options.categories)
        self.stream = stream
        self.failfast = options.failfast
        self.options = options

    def getShortDescription(self, test):
        return str(test)

    def getLongDescription(self, test):
        # qdtestcase
        if hasattr(test, 'get_test_path'):
            return '\n'.join((test.get_test_path(),
                test.shortDescription()))

        # unittest standard tests
        doc_first_line = test.shortDescription()
        if doc_first_line:
            return '\n'.join((str(test), doc_first_line))
        else:
            return str(test)

    def startTest(self, test):
        super().startTest(test)
        if self.verbose:
            self.stream.write(self.getShortDescription(test))
            self.stream.write(" ... ")
            self.stream.flush()

    def addSuccess(self, test):
        super().addSuccess(test)
        if self.verbose:
            if self.stream.isatty():
                self.stream.writeln(colorize("ok", fg="green"))
            else:
                self.stream.writeln("ok")

    def addSkip(self, test, reason):
        super().addSkip(test, reason)
        if self.verbose:
            if self.stream.isatty():
                self.stream.writeln(colorize("skip", fg="blue"))
            else:
                self.stream.writeln("skip")

    def addExpectedFailure(self, test, err):
        """Called when an expected failure/error occured."""
        self.expectedFailures.append(
            (test, self._exc_info_to_string(err, test)))
        super().addSkip(test, "broken")
        if self.verbose:
            if self.stream.isatty():
                self.stream.writeln(colorize("skip, expected failure",
                    fg="blue"))
            else:
                self.stream.writeln("skip, expected failure")

    def addUnexpectedSuccess(self, test):
        """Called when a test was expected to fail, but succeed."""
        super().addUnexpectedSuccess(test)

    def addError(self, test, err):
        super().addError(test, err)
        if self.verbose:
            if self.stream.isatty():
                self.stream.writeln(colorize("ERROR", bg="white", fg="red"))
            else:
                self.stream.writeln("ERROR")

        if self.failfast:
            self.shouldStop = True

    def addFailure(self, test, err):
        exctype, value, tb = err
        self.errors.append((test, str(value)))

        if self.verbose:
            if self.stream.isatty():
                self.stream.writeln(colorize("FAIL", fg="red"))
            else:
                self.stream.writeln("FAIL")

        if self.options.debug:
            args = ["cgdb", "--args"]
            args.extend(test.args)
            subprocess.call(args)

        if self.failfast:
            self.shouldStop = True

    def printErrors(self):
        if self.verbose:
            self.stream.writeln()

        error = (colorize("ERROR", fg="red") if
            self.stream.isatty() else "ERROR")
        fail = (colorize("FAIL", fg="red") if
            self.stream.isatty() else "FAIL")

        self.printErrorList(error, self.errors)
        self.printErrorList(fail, self.failures)

    def printErrorList(self, flavour, errors):
        for test, err in errors:
            self.stream.writeln(self.separator1)
            self.stream.writeln("{}: {}".format(flavour,
                self.getLongDescription(test)))
            self.stream.writeln(self.separator2)
            self.stream.writeln("{}".format(err))

    def print_summary(self, options, number=True, show_errors=True):
        if show_errors:
            self.printErrors()

        if number:
            self.stream.writeln(
                "Success: {}/{}".format(
                    self.testsRun
                    - (len(self.failures)
                        + len(self.errors) + len(self.skipped)),
                    self.testsRun))
        else:
            try:
                rate = (100 - len(self.failures + self.errors + self.skipped)
                        / self.testsRun * 100)
                rate_str = colorize("{:.0f}".format(rate),
                        fg="green" if rate == 100 else "red")
                self.stream.writeln(colorize("Success: {}%".format(rate_str),
                    opts=("bold",)))
            except ZeroDivisionError:
                self.stream.writeln("No test run.")

        if self.skipped and not options.categories:
            self.stream.writeln("Skipped: \n - "
                + "\n - ".join(t.get_test_path() for t, reason in self.skipped))

        if self.unexpectedSuccesses and not options.categories:
            self.stream.writeln(
            colorize("Unexpected successes (aka. bugfixes):", opts=("bold",))
                + "\n - "
                + "\n - ".join(
                    colorize(t.get_test_path(), bg='green')
                    for t in self.unexpectedSuccesses))

