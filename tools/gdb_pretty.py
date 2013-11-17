import gdb
import gdb.printing

class tokenPrinter:
    """Print a token* object."""

    def __init__(self, val):
        self.val = val

    def to_string(self):
        return gdb.execute("call token_print({})".format(
            self.val.address))

def build_pretty_printer():
    pp = gdb.printing.RegexpCollectionPrettyPrinter(
        "42sh")
    pp.add_printer('token', '^token$', tokenPrinter)
    return pp


gdb.printing.register_pretty_printer(gdb.current_objfile(),
        build_pretty_printer())
