#!/usr/bin/env python3.3
import os
from qdunittest.program import TestProgram

if __name__ == "__main__":
    os.chdir(os.path.dirname(__file__))
    TestProgram(module=None)
