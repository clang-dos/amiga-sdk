"""Checks lha.py on an archive built here, and, with AMIGA_SDK_NETWORK=1,
installs the NDK headers from Aminet with amiga-ndk.py --no-stubs."""
import os
import struct
import subprocess
import sys
import tempfile
import unittest

HERE = os.path.dirname(os.path.abspath(__file__))
sys.path.insert(0, os.path.join(HERE, "..", "tools"))
import lha  # noqa: E402


def stored_entry(name, data, level):
    """An -lh0- (stored) member with a level 0 or 1 header."""
    method = b"-lh0-"
    crc = lha.crc16(data)
    if level == 0:
        body = (method + struct.pack("<IIIBB", len(data), len(data), 0, 0x20, 0)
                + bytes([len(name)]) + name + struct.pack("<H", crc))
        return bytes([len(body), sum(body) & 0xFF]) + body + data
    # Level 1: an empty base name and the name in an extended header.
    ext = bytes([1]) + name + struct.pack("<H", 0)
    ext_size = len(ext)
    body = (method + struct.pack("<IIIBB", len(data) + ext_size, len(data), 0,
                                 0x20, 1)
            + bytes([0]) + struct.pack("<H", crc) + b"U"
            + struct.pack("<H", ext_size))
    return bytes([len(body), sum(body) & 0xFF]) + body + ext + data


class Lha(unittest.TestCase):
    def test_stored(self):
        archive = (stored_entry(b"a/one.h", b"one\n", 0)
                   + stored_entry(b"two.fd", b"##base _X\n", 1) + b"\0")
        got = dict(lha.extract(archive))
        self.assertEqual(got, {"a/one.h": b"one\n", "two.fd": b"##base _X\n"})
        got = dict(lha.extract(archive, lambda n: n.endswith(".fd")))
        self.assertEqual(list(got), ["two.fd"])

    def test_crc(self):
        bad = bytearray(stored_entry(b"x", b"data", 0) + b"\0")
        bad[-2] ^= 1  # the last data byte
        with self.assertRaises(ValueError):
            list(lha.extract(bytes(bad)))

    @unittest.skipUnless(os.environ.get("AMIGA_SDK_NETWORK"), "needs network")
    def test_ndk_headers(self):
        out = tempfile.mkdtemp()
        subprocess.run([sys.executable, os.path.join(HERE, "..", "tools",
                                                     "amiga-ndk.py"),
                        "--sysroot", out, "--no-stubs"], check=True)
        inc = os.path.join(out, "include")
        with open(os.path.join(inc, "proto", "dos.h")) as f:
            self.assertIn("extern struct DosLibrary *DOSBase;", f.read())
        with open(os.path.join(inc, "clib", "compiler-specific.h")) as f:
            self.assertIn("#define __REG__(r, p) p", f.read())
        self.assertFalse(os.path.exists(os.path.join(inc, "inline")))
        self.assertTrue(os.path.exists(os.path.join(inc, "exec", "types.h")))


if __name__ == "__main__":
    unittest.main()
