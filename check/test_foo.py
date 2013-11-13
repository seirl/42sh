import unittest

class TestFoo(unittest.TestCase):

    def test_foo(self):
        self.assertEqual(42, 42, "yes")
