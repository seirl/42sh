#!/usr/bin/env python3

import json
import os
import os.path
import unittest
import subprocess

class QDTestCase(unittest.TestCase):
    def setUp(self):
        self.program_name = "../42sh"

    def start_shell(self, args):
        self._shell = subprocess.Popen([self.program_name] + args,
                stdin=subprocess.PIPE, stdout=subprocess.PIPE,
                stderr=subprocess.PIPE)

class QDTestSuite(unittest.TestSuite):
    def __init__(self, category, *args, **kwargs):
        super().__init__(*args, **kwargs)

        self.category = category

def load_test_case(directory, test_filename):
    """Return a TestCase generated from the file test_file, expected to contain
    a json structure reprensenting the test."""
    with open(os.path.join(directory, test_filename)) as f:
        test = json.load(f)

        def test_it(self):
            shell_args = test.get('args', [])
            self.start_shell(shell_args)

            timeout = test.get('timeout', None)
            stdin_buf = test.get('stdin', None)
            stdoutdata, stderrdata = self._shell.communicate(stdin_buf,
                                                             timeout)

            if 'stdout' in test:
                self.assertEqual(stdoutdata, test['stdout'].encode())

            retval = test.get('retval', 0)
            self.assertEqual(self._shell.returncode, retval)

        test_method_name = 'test_' + test_filename

        new_test_case = type("Test" + test_filename, (QDTestCase, ),
            {test_method_name: test_it})(test_method_name)

        return new_test_case

def load_test_suite(directory, filenames):
    """Return a TestSuite for the directory ``directory``."""
    test_suite = QDTestSuite(directory)

    for filename in filenames:
        test_suite.addTest(load_test_case(directory, filename))

    return test_suite

def load_tests(loader, standard_tests, pattern):
    """Create a TestSuite by subdirectory and a TestCase by file."""
    for dirpath, dirnames, filenames in os.walk('.'):
        # Not test directories
        if dirpath == '.' or dirpath.startswith('__'):
            continue
        standard_tests.addTest(load_test_suite(dirpath,
            filter(lambda f: f.endswith('.test'), filenames)))

    return standard_tests
