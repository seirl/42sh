Howto develop for 42sh
======================

Testing
-------

<blockquote>

    We want you to write more tests. Yes, you. You've already been told that
    tests are the safety net that protects you when you need to refactor your
    code, or when another developer adds features. You even know that tests can
    help with the design of your code.

    -- http://googletesting.blogspot.fr/2007/01/introducing-testing-on-toilet.html

</blockquote>

Tests are organized in tests files. Each test file is located in a sub
directory of `check/` that describe what is being tested. A test file always
end in `.test`.

### How to test?

    hs24$ make check

or

    hs24$ make
    hs24$ cd build/check
    hs24$ ./run_tests.py

### Formats of the tests

General fields:

\code{.py}
{
    "type": "42sh",        # type of test
    "desc": "test 42sh",   # description of the test
    "retval": 0,           # check return value
    "timeout" 4.2,         # custom timeout
    "with_valgrind": True, # check for memory errors
}
\endcode

#### Lexer

\code{.py}
{
    "type": "lexer",
    "desc": "Test WORD token",
    "input": "A",
    "output": """A      WORD
        EOF
"""
}
\endcode

#### Unittest

You can also write unit test with the unittest module. Create a file named
test_XXX.py in `check/`.

Example with `test_foo.py`:

\code{.py}
import unittest

class TestFoo(unittest.TestCase):

    def test_foo(self):
        self.assertEqual(42, 42, "yes")
\endcode

#### Options of run_tests.py

    hs24$ ./run_tests.py --help
    Usage: run_tests.py [options]

    Options:
      -h, --help            show this help message and exit
      --failfast            Stop on first fail or error
      --catch               Catch ctrl-C and display results so far
      -b, --buffer          Buffer stdout and stderr during tests
      -v, --verbose         Verbose output
      -q, --quiet           Verbose output
      -c, --categories      Display the categories and the percentage of
                            successful tests.
      -e SELECT, --select=SELECT
                            Select category of tests. Will only display categories
                            name and percentage of success.
      -f, --final           Display the percentage of successful tests.
      -n, --number          Display the number of successful test.
      -a, --all             Execute the test suite on all categories.
      -t TIMEOUT, --timeout=TIMEOUT
                            General timeout in seconds for each test.
      -m, --without-valgrind
                            Disable running tests under valgrind (memcheck).


Hash Table
----------

\code{.c}
#include "hashtbl.h"
HASHTBL(char*, char*, my_table);

int foo()
{
    s_my_table *t;
    //table, size, hash, cmp, free function
    HASHTBL_INIT(t, 10, hash_string, cmp_string, NULL);
    //table, value, key
    HASHTBL_SET(t, "foo", "bar");
    int changed;
    char *ret;
    //table, key, res, bool
    HASHTBL_GET(t, "bar", ret, changed)
    if (changed)
        printf("%s = %s\n", bar, ret);
    HASHTBL_DEL(t, "bar");
    HASHTBL_FREE(t);
}
\endcode
