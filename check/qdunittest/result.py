import unittest

from termcolors import colorize

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

    def addError(self, test, err):
        super().addError(test, err)
        if self.showAll:
            if self.stream.isatty():
                self.stream.writeln(colorize("ERROR", bg="white", fg="red"))
            else:
                self.stream.writeln("ERROR")

    def addFailure(self, test, err):
        super().addFailure(test, err)
        if self.showAll:
            if self.stream.isatty():
                self.stream.writeln(colorize("FAIL", fg="red"))
            else:
                self.stream.writeln("FAIL")

    def printErrors(self):
        if self.showAll:
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
            self.stream.writeln("%s: %s" % (flavour, self.getDescription(test)))
            self.stream.writeln(self.separator2)
            self.stream.writeln("%s" % err)

    def print_summary(self, number=True):
        self.printErrors()

        if number:
            self.stream.writeln(
                "Success: {}/{}".format(
                    self.testsRun - (len(self.failures) + len(self.errors)),
                    self.testsRun))
        else:
            rate = 100 - len(self.failures + self.errors) / self.testsRun * 100
            rate_str = colorize("{:.0f}".format(rate),
                    fg="green" if rate == 100 else "red")
            self.stream.writeln( "Success: {}%".format( rate_str))
