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
