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
        return "{}/{}".format(self.category, self.test_name)

    def get_test_name(self):
        return self.category + '_' + self.test_name

    def get_test_path(self):
        return os.path.join(self.category, self.test_name)

    def start_program(self, args):
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

        suppressions = os.getenv("VALGRIND_SUPP")
        if suppressions:
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

def new_test_run_42sh(test, gtimeout):
    def test_42sh(subself):
        timeout = test.get('timeout', gtimeout)

        shell_args = test.get('args', [])
        shell = subself.start_shell(["../42sh"] + shell_args)

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

def new_test_run_lexer(test, gtimeout):
    def test_lexer(subself):
        timeout = test.get('timeout', gtimeout)
        input_string = test.get('input', "")
        lexer_commands = test.get('lexer', "")

        lexer = subself.start_program(["./test_lexer", input_string,
            lexer_commands])
        stdoutdata, stderrdata = lexer.communicate(b"", timeout)

        if 'output' in test:
            subself.assertMultiLineEqual(test['output'], stdoutdata.decode(),
                             "stdout differ")

        retval = test.get('retval', 0)
        subself.assertEqual(retval, lexer.returncode, "return value differ")

    return QDTestCaseLexer, test_lexer
