import os
import subprocess
import unittest

class QDTestCase(unittest.TestCase):
    def __init__(self, category, test_name, description, *args, **kwargs):
        super().__init__(*args, **kwargs)
        self.category = category
        self.test_name = test_name
        self.description = description

    def __str__(self):
        return "{}/{}".format(self.category, self.test_name)

    def setUp(self):
        self.program_name = "../42sh"

    def get_test_name(self):
        return self.category + '_' + self.test_name

    def get_test_path(self):
        return os.path.join(self.category, self.test_name)

    def start_shell(self, args):
        self._shell = subprocess.Popen(["valgrind",
            "--xml=yes",
            "--xml-file={}.memcheck".format(self.get_test_path()),
            "--log-file={}.log".format(self.get_test_path()),
            self.program_name] + args,
            stdin=subprocess.PIPE,
            stdout=subprocess.PIPE,
            stderr=subprocess.PIPE)
