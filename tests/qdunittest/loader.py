import os
import os.path
import unittest

from .case import (QDTestCase,
        new_test_run_42sh,
        new_test_run_fnmatch,
        new_test_run_utils,
        new_test_run_interface,
        new_test_run_lexer,
        new_test_run_parser,
        new_test_run_env,
    )
from .suite import QDTestSuite

class QDTestLoader(unittest.TestLoader):
    """Find and load tests."""

    test_methods = {
            'lexer': new_test_run_lexer,
            'parser': new_test_run_parser,
            'utils': new_test_run_utils,
            'env': new_test_run_env,
            'interface': new_test_run_interface,
            'fnmatch': new_test_run_fnmatch,
            '42sh': new_test_run_42sh,
            }

    def __init__(self, options, *args, **kwargs):
        super().__init__(*args, **kwargs)
        self.options = options

    def _make_test(self, test, directory, test_filename):
        test_type = test.get('type',
                directory if directory in self.test_methods else '42sh')
        test_func = self.test_methods[test_type]
        test_class, test_method = test_func(test, self.options)

        category = os.path.basename(directory)
        test_class_name = "Test{}{}".format(
                category.title(),
                test_filename.replace(".test", "")\
                        .title()\
                        .replace("_", ""))
        test_method_name = 'test_' + test_filename

        if test.get('skip', False):
            test_method = unittest.expectedFailure(test_method)

        # We create the method here to give it the right name
        test_class_methods = {test_method_name: test_method}
        test_case_class = type(test_class_name,
                (test_class, ),
                test_class_methods,)

        yield test_case_class(methodName=test_method_name,
                category=category,
                test_name=test_filename,
                test=test)

    def _load_test_case(self, directory, test_filename):
        """Return a TestCase generated from the file test_file, expected to
        contain a json structure reprensenting the test."""
        with open(os.path.join(directory, test_filename)) as f:
            tests = eval(f.read())
            if isinstance(tests, dict):
                tests = [tests]

            for test in tests:
                yield from self._make_test(test, directory, test_filename)


    def _load_test_suite(self, directory, filenames):
        """Return a TestSuite for the directory ``directory``."""
        test_suite = QDTestSuite(directory)
        empty = True

        for filename in filenames:
            if filename.endswith(".test"):
                test_suite.addTests(self._load_test_case(directory, filename))
                empty = False

        if empty:
            return None
        else:
            return test_suite

    def discover(self, start_dir, pattern, top_level_dir):
        """Add discovery of test from files ending with .test. Filter
        categories if select is != None.
        """

        # Discover python scripts
        if not self.options.select:
            test = super().discover(start_dir, pattern, top_level_dir)
        else:
            test = unittest.TestSuite()
        for dirpath, dirnames, filenames in os.walk(start_dir):
            directory = os.path.basename(dirpath)
            if self.options.select and directory != self.options.select:
                continue
            filenames.sort()
            test_suite = self._load_test_suite(directory, filenames)
            if test_suite:
                test.addTest(test_suite)

        return test
