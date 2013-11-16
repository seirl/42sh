import unittest

from .loader import QDTestLoader
from .runner import QDTestRunner

class TestProgram(unittest.TestProgram):
    """
    A variation of the unittest.TestProgram. Please refer to the base
    class for command line parameters.
    """

    def __init__(self, categories=False, select=None, final=False,
            number=False, all=True, timeout=None, *args, **kwargs):
        self.categories = categories
        self.select = select
        self.final = final
        self.number = number
        self.all = all
        self.timeout = timeout
        super().__init__(*args, **kwargs)

    def _setAttributesFromOptions(self, options):
        """Store options in instance attributes."""
        super()._setAttributesFromOptions(options)
        self.verbosity = 1 if options.categories else self.verbosity
        self.options = options

    def _getOptParser(self):
        """Setup and run option parser."""
        parser = super()._getOptParser()
        # Will redefine some options
        parser.set_conflict_handler("resolve")

        # Redefine default verbosity to verbose
        parser.add_option('-v', '--verbose', dest='verbose', default=True,
            help='Verbose output', action='store_true')
        parser.add_option('-q', '--quiet', dest='verbose', default=True,
            help='Verbose output', action='store_false')

        parser.add_option("-c", "--categories", action="store_true",
            help="Display the categories and the percentage of successful tests.")
        parser.add_option("-e", "--select",
            help="Select category of tests. Will only display categories name and percentage of success.")
        parser.add_option("-f", "--final", action="store_true", default=False,
            help="Display the percentage of successful tests.")
        parser.add_option("-n", "--number", action="store_true", default=False,
            help="Display the number of successful test.")
        parser.add_option("-a", "--all", action="store_true", default=True,
            help="Execute the test suite on all categories.")

        parser.add_option("-t", "--timeout", default=5.0, type=float,
            help="General timeout in seconds for each test.")

        parser.add_option("-m", "--without-valgrind", default=False,
            action="store_true",
            help="Disable running tests under valgrind (memcheck).")

        return parser

    def _do_discovery(self, argv, *args, **kwargs):
        # handle command line args for test discovery
        self.progName = '%s discover' % self.progName
        parser = self._getOptParser()
        self._addDiscoveryOptions(parser)

        options, args = parser.parse_args(argv)
        if len(args) > 3:
            self.usageExit()

        for name, value in zip(('start', 'pattern', 'top'), args):
            setattr(options, name, value)

        self._setAttributesFromOptions(options)

        start_dir = options.start
        pattern = options.pattern
        top_level_dir = options.top

        loader = QDTestLoader(options)
        self.test = loader.discover(start_dir, pattern, top_level_dir)

    def _addDiscoveryOptions(self, parser):
        """Change -t option to -T"""
        parser.add_option('-s', '--start-directory', dest='start', default='.',
            help="Directory to start discovery ('.' default)")
        parser.add_option('-p', '--pattern', dest='pattern',
            default='test*.py',
            help="Pattern to match tests ('test*.py' default)")
        parser.add_option('-T', '--top-level-directory', dest='top',
            default=None,
            help='Top level directory of project (defaults to start directory)')

    def createTests(self):
        # Monkey patch to add our testLoader
        self.testLoader = QDTestLoader(options=self.options)
        super().createTests()

    def runTests(self):
        self.testRunner = QDTestRunner(options=self.options)
        super().runTests()


