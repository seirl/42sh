import unittest

class QDTestSuite(unittest.TestSuite):
    """A TestSuite with category attribute."""

    def __init__(self, category, *args, **kwargs):
        super().__init__(*args, **kwargs)

        self.category = category
