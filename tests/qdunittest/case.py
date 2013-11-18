import os
import subprocess
import unittest

class QDTestCase(unittest.TestCase):
    def __init__(self, category, test_name, test, *args,
            **kwargs):
        super().__init__(*args, **kwargs)
        self.category = category
        self.test_name = test_name
        self.test = test

    def __str__(self):
        return self.test.get("desc", self.test_name)

    def get_test_name(self):
        return self.category + '_' + self.test_name

    def get_test_path(self):
        return os.path.join(self.category, self.test_name)

    def start_program(self, args, with_valgrind=True):
        if with_valgrind:
            command = ["valgrind",
                "--leak-check=full",
                "--show-reachable=yes",
                "--track-fds=yes",
                "--read-var-info=yes",
                "--track-origins=yes",
                "--partial-loads-ok=yes",
                "--malloc-fill=0x42",
                "--free-fill=0x43",
                "--xml=yes",
                "--xml-file={}.memcheck".format(self.get_test_path()),
                "--log-file={}.log".format(self.get_test_path())]
        else:
            command = []

        suppressions = os.getenv("VALGRIND_SUPP")
        if suppressions and with_valgrind:
            command.append(suppressions)

        command.extend(args)

        return subprocess.Popen(command,
            stdin=subprocess.PIPE,
            stdout=subprocess.PIPE,
            stderr=subprocess.PIPE)

class QDTestCaseShell(QDTestCase):

    def shortDescription(self):
        return "{}[args: {}]\nstdin: {}".format(
                self.test["desc"] + "\n" if "desc" in self.test else "",
                self.test.get('args', []),
                self.test.get('stdin', "None"),
            )

def new_test_run_42sh(test, options):
    def test_42sh(subself):
        timeout = test.get('timeout', options.timeout)

        shell_args = test.get('args', [])
        with_valgrind = test.get('with_valgrind', not options.without_valgrind)
        shell = subself.start_shell(["../42sh"] + shell_args,
                with_valgrind=with_valgrind)

        stdin_buf = test.get('stdin', None)
        stdoutdata, stderrdata = shell.communicate(stdin_buf, timeout)

        if 'stdout' in test:
            subself.assertMultiLineEqual(test['stdout'], stdoutdata.decode(),
                             "stdout differ")
        if 'stderr' in test:
            subself.assertMultiLineEqual(test['stderr'], stderrdata.decode(),
                             "stderr differ")
        else:
            subself.assertMultiLineEqual(b"", stderrdata.decode(),
                    "stderr differ")

        retval = test.get('retval', 0)
        subself.assertEqual(shell.returncode, retval, "return value differ")

    return QDTestCaseShell, test_42sh

class QDTestCaseLexer(QDTestCase):

    def shortDescription(self):
        return "{}[input: {}] [lexer: {}]".format(
                self.test["desc"] + "\n" if "desc" in self.test else "",
                self.test.get('input', "None"),
                self.test.get('lexer', "None"),
            )

def new_test_run_lexer(test, options):
    def test_lexer(subself):
        timeout = test.get('timeout', options.timeout)
        input_string = test.get('input', "")
        with_valgrind = test.get('with_valgrind', not options.without_valgrind)

        command = ["./test_lexer", input_string]
        if 'lexer' in test:
            command.extend(test['lexer'])

        lexer = subself.start_program(args=command,
                with_valgrind=with_valgrind)
        stdoutdata, stderrdata = lexer.communicate(b"", options.timeout)

        if 'output' in test:
            subself.assertMultiLineEqual(test['output'], stdoutdata.decode(),
                             "stdout differ")

        retval = test.get('retval', 0)
        subself.assertEqual(retval, lexer.returncode, "return value differ")

    return QDTestCaseLexer, test_lexer

class QDTestCaseFnmatch(QDTestCase):

    def shortDescription(self):
        return "{}[string: {}] [pattern: {}]".format(
                self.test["desc"] + "\n" if "desc" in self.test else "",
                self.test.get('string', "None"),
                self.test.get('pattern', "None"),
            )

def new_test_run_fnmatch(test, options):
    def test_fnmatch(subself):
        timeout = test.get('timeout', options.timeout)
        string = test.get('string', "")
        pattern = test.get('pattern', "")
        with_valgrind = test.get('with_valgrind', not options.without_valgrind)

        fnmatch = subself.start_program(["./test_fnmatch", pattern, string],
            with_valgrind=with_valgrind)
        stdoutdata, stderrdata = fnmatch.communicate(b"", options.timeout)

        if 'output' in test:
            subself.assertMultiLineEqual(test['output'], stdoutdata.decode(),
                             "stdout differ")

    return QDTestCaseFnmatch, test_fnmatch

class QDTestCaseParser(QDTestCase):

    def shortDescription(self):
        return "Description: {}Input: '{}'".format(
                self.test["desc"] + "\n" if "desc" in self.test else "",
                self.test.get('input', "None"),
            )

def new_test_run_parser(test, options):
    def test_parser(subself):
        timeout = test.get('timeout', options.timeout)
        input = test.get('input', "")
        with_valgrind = test.get('with_valgrind', not options.without_valgrind)

        parser = subself.start_program(["./test_parser", input],
            with_valgrind=with_valgrind)
        stdoutdata, stderrdata = parser.communicate(b"", options.timeout)

        if 'output' in test:
            subself.assertMultiLineEqual(test['output'], stdoutdata.decode(),
                             "stdout differ")

        if test.get('stderr') == ...:
            subself.assertNotEqual("", stderrdata.decode(), "stderr empty")
        else:
            subself.assertMultiLineEqual(test.get('stderr', ""),
                    stderrdata.decode(), "stderr differ")

        retval = test.get('retval', 0)
        subself.assertEqual(retval, parser.returncode, "return value differ")

    return QDTestCaseParser, test_parser

class QDTestCaseUtils(QDTestCase):

    def shortDescription(self):
        return "{}[input: {}] [utils: {}]".format(
                self.test["desc"] + "\n" if "desc" in self.test else "",
                self.test.get('input', "None"),
                self.test.get('utils', "None"),
            )

def new_test_run_utils(test, options):
    def test_utils(subself):
        timeout = test.get('timeout', options.timeout)
        input_string = test.get('input', "")
        with_valgrind = test.get('with_valgrind', not options.without_valgrind)

        command = ["./test_utils", input_string]
        if 'lexer' in test:
            command.extend(test['utils'])

        utils = subself.start_program(args=command,
                with_valgrind=with_valgrind)
        stdoutdata, stderrdata = utils.communicate(b"", options.timeout)

        if 'output' in test:
            subself.assertMultiLineEqual(test['output'], stdoutdata.decode(),
                             "stdout differ")

        retval = test.get('retval', 0)
        subself.assertEqual(retval, utils.returncode, "return value differ")

    return QDTestCaseUtils, test_utils

class QDTestCaseInterface(QDTestCase):

    def shortDescription(self):
        return "{}[input: {}] [interface: {}]".format(
                self.test["desc"] + "\n" if "desc" in self.test else "",
                self.test.get('input', "None"),
                self.test.get('interface', "None"),
            )

def new_test_run_interface(test, options):
    def test_interface(subself):
        timeout = test.get('timeout', options.timeout)
        input_string = test.get('input', "")
        with_valgrind = test.get('with_valgrind', not options.without_valgrind)

        command = ["./test_interface", input_string]
        if 'interface' in test:
            command.extend(test['interface'])

        interface = subself.start_program(args=command,
                with_valgrind=with_valgrind)
        stdoutdata, stderrdata = interface.communicate(b"", options.timeout)

        if 'stdout' in test:
            subself.assertRegexpMatches(stdoutdata.decode (), test['stdout'])

        if 'stderr' in test:
            subself.assertRegexpMatches(stderrdata.decode (), test['stderr'])

        retval = test.get('retval', 0)
        subself.assertEqual(retval, interface.returncode, "return value differ")

    return QDTestCaseInterface, test_interface
