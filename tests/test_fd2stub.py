"""Checks fd2stub.py on a small .fd file written for this test."""
import os
import subprocess
import sys
import tempfile
import unittest

HERE = os.path.dirname(os.path.abspath(__file__))
TOOL = os.path.join(HERE, "..", "tools", "fd2stub.py")

FD = """\
* test library
##base _TestBase
##bias 30
##public
First()()
Second(ptr,len)(a0,d2)
##private
Hidden()()
##public
Fourth(base)(a6)
Fifth(x)(d0)
##end
"""


class Fd2Stub(unittest.TestCase):
    def run_tool(self):
        tmp = tempfile.mkdtemp()
        fd = os.path.join(tmp, "test_lib.fd")
        with open(fd, "w") as f:
            f.write(FD)
        out = os.path.join(tmp, "out")
        subprocess.run([sys.executable, TOOL, out, fd], check=True)
        return out

    def read(self, out, name):
        with open(os.path.join(out, name + ".s")) as f:
            return f.read()

    def test_files(self):
        out = self.run_tool()
        # Private functions and ones that take an argument in a6 are skipped.
        self.assertEqual(sorted(os.listdir(out)),
                         ["Fifth.s", "First.s", "Second.s"])

    def test_offsets_and_registers(self):
        out = self.run_tool()
        self.assertIn("jsr (-30,%a6)", self.read(out, "First"))
        second = self.read(out, "Second")
        self.assertIn("jsr (-36,%a6)", second)
        # d2 is callee-saved, so it is saved with a6; arguments start past
        # the saved registers and the return address.
        self.assertIn("movem.l %d2/%a6,(%sp)", second)
        self.assertIn("move.l (12,%sp),%a0", second)
        self.assertIn("move.l (16,%sp),%d2", second)
        self.assertIn("move.l TestBase,%a6", second)
        # The bias still counts private and skipped functions.
        self.assertIn("jsr (-54,%a6)", self.read(out, "Fifth"))


if __name__ == "__main__":
    unittest.main()
