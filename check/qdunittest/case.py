import os
import subprocess
import unittest

class QDTestCase(unittest.TestCase):
    def __init__(self, category, test_name, description, *args, **kwargs):
        super().__init__(*args, **kwargs)
        self.category = category
        self.test_name = test_name
        self.description = description
        self.shell_name = "../42sh"

    def __str__(self):
        return "{}/{}".format(self.category, self.test_name)

    def get_test_name(self):
        return self.category + '_' + self.test_name

    def get_test_path(self):
        return os.path.join(self.category, self.test_name)

    def start_program(self, args):
        return subprocess.Popen(["valgrind",
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
            "--log-file={}.log".format(self.get_test_path())] + args,
            stdin=subprocess.PIPE,
            stdout=subprocess.PIPE,
            stderr=subprocess.PIPE)

    def start_shell(self, args):
        return self.start_program([self.shell_name] + args)

def new_test_run_42sh(test, gtimeout):
    def test_42sh(subself):
        timeout = test.get('timeout', gtimeout)

        shell_args = test.get('args', [])
        shell = subself.start_shell(shell_args)

        stdin_buf = test.get('stdin', None)
        stdoutdata, stderrdata = shell.communicate(stdin_buf, timeout)

        if 'stdout' in test:
            subself.assertMultiLineEqual(stdoutdata.decode(), test['stdout'],
                             "stdout differ")
        if 'stderr' in test:
            subself.assertMultiLineEqual(stderrdata.decode(), test['stderr'],
                             "stderr differ")
        else:
            subself.assertMultiLineEqual(stderrdata.decode(), b"", "stderr differ")

        retval = test.get('retval', 0)
        subself.assertEqual(shell.returncode, retval, "return value differ")

    return test_42sh

def new_test_run_lexer(test, gtimeout):
    def test_lexer(subself):
        timeout = test.get('timeout', gtimeout)
        input_string = test.get('input', "")
        lexer_commands = test.get('lexer', "")

        lexer = subself.start_program(["./test_lexer",
            input_string, lexer_commands])
        stdoutdata, stderrdata = lexer.communicate(b"", timeout)

        if 'output' in test:
            subself.assertMultiLineEqual(stdoutdata.decode(), test['output'],
                             "stdout differ")

        retval = test.get('retval', 0)
        subself.assertEqual(lexer.returncode, retval, "return value differ")

    return test_lexer
