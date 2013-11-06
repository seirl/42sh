#!/usr/bin/env python3

import json
import os
import os.path
import unittest
import subprocess

class QDTestSuite(unittest.TestSuite):
    def __init__(self, category, *args, **kwargs):
        super().__init__(*args, **kwargs)

        self.category = category

class QDTestCase(unittest.TestCase):
    def __init__(self, category, test_name, *args, **kwargs):
        super().__init__(*args, **kwargs)
        self.category = category
        self.test_name = test_name

    def setUp(self):
        self.program_name = "../42sh"

    def get_test_name(self):
        return self.category + '_' + self.test_name

    def start_shell(self, args):
        self._shell = subprocess.Popen(["valgrind",
            "--xml=yes",
            "--xml-file={}.memcheck".format(self.get_test_name()),
            "--log-file={}.log".format(self.get_test_name()),
            self.program_name] + args,
            stdin=subprocess.PIPE,
            stdout=subprocess.PIPE,
            stderr=subprocess.PIPE)

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

        category = os.path.basename(directory)
        test_class_name = "Test{}{}".format(
                category.title(),
                test_filename.title())
        test_method_name = 'test_' + test_filename
        test_class_methods = {test_method_name: test_it}
        test_case_class = type(test_class_name,
                (QDTestCase, ),
                test_class_methods,)

        return test_case_class(methodName=test_method_name, category=category,
                test_name=test_filename.rstrip(".test"))

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
