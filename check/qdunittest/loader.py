import json
import os
import os.path
import unittest

from .case import QDTestCase
from .suite import QDTestSuite

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
