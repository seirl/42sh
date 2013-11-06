#!/usr/bin/env python3

import unittest

class TestProgram(unittest.TestProgram):
    """
    A variation of the unittest.TestProgram. Please refer to the base
    class for command line parameters.
    """

    def parseArgs(self, *args, **kwargs):
        super().parseArgs(*args, **kwargs)

    def runTests(self):
        if self.testRunner is None:
            #self.testRunner = QDTestRunner(verbosity=self.verbosity)
            self.testRunner = None
        unittest.TestProgram.runTests(self)

main = TestProgram

if __name__ == "__main__":
    main(module=None)
