"""Read LHA archives (-lh0-, -lh5-, -lh6- and -lh7-), as Amiga software is
packed on Aminet. Pure Python, so the SDK needs no lha program.

    for name, data in lha.extract(archive_bytes, wanted=lambda n: ...):
        ...
"""
import struct

NC = 510  # literals, match lengths 3..256 and the block end
NT = 19
TBIT = 5
CBIT = 9


class BitReader:
    def __init__(self, data):
        self.data = data
        self.pos = 0  # in bits

    def peek(self, n):
        byte = self.pos >> 3
        chunk = int.from_bytes(self.data[byte:byte + 4].ljust(4, b"\0"), "big")
        return (chunk >> (32 - (self.pos & 7) - n)) & ((1 << n) - 1)

    def get(self, n):
        if n == 0:
            return 0
        v = self.peek(n)
        self.pos += n
        return v


def make_table(lengths, maxbits):
    """A table indexed by the next maxbits bits: (symbol, length)."""
    table = [None] * (1 << maxbits)
    code = 0
    for length in range(1, maxbits + 1):
        for sym, l in enumerate(lengths):
            if l == length:
                span = 1 << (maxbits - length)
                start = code << (maxbits - length)
                if start + span > len(table):
                    raise ValueError("bad Huffman table")
                for i in range(start, start + span):
                    table[i] = (sym, length)
                code += 1
        code <<= 1
    return table


class Code:
    """A Huffman code: either one symbol of no bits, or a table."""

    def __init__(self, lengths=None, single=None):
        self.single = single
        if single is None:
            self.maxbits = max(lengths)
            self.table = make_table(lengths, self.maxbits)

    def decode(self, br):
        if self.single is not None:
            return self.single
        entry = self.table[br.peek(self.maxbits)]
        if entry is None:
            raise ValueError("bad Huffman code")
        br.pos += entry[1]
        return entry[0]


def read_pt_len(br, nn, nbit, special):
    n = br.get(nbit)
    if n == 0:
        return Code(single=br.get(nbit))
    lengths = [0] * nn
    i = 0
    while i < n:
        c = br.peek(3)
        if c == 7:
            br.pos += 3
            while br.get(1):
                c += 1
        else:
            br.pos += 3
        lengths[i] = c
        i += 1
        if i == special:
            for _ in range(br.get(2)):
                lengths[i] = 0
                i += 1
    return Code(lengths)


def read_c_len(br, pt):
    n = br.get(CBIT)
    if n == 0:
        return Code(single=br.get(CBIT))
    lengths = [0] * NC
    i = 0
    while i < n:
        c = pt.decode(br)
        if c <= 2:
            if c == 0:
                count = 1
            elif c == 1:
                count = br.get(4) + 3
            else:
                count = br.get(CBIT) + 20
            i += count
        else:
            lengths[i] = c - 2
            i += 1
    return Code(lengths)


def decompress_lh(data, size, dicbit):
    np_ = dicbit + 1
    pbit = 4 if dicbit == 13 else 5
    br = BitReader(data)
    out = bytearray()
    blocksize = 0
    c_code = p_code = None
    while len(out) < size:
        if blocksize == 0:
            blocksize = br.get(16)
            pt = read_pt_len(br, NT, TBIT, 3)
            c_code = read_c_len(br, pt)
            p_code = read_pt_len(br, np_, pbit, -1)
        blocksize -= 1
        c = c_code.decode(br)
        if c < 256:
            out.append(c)
            continue
        length = c - 256 + 3
        j = p_code.decode(br)
        if j:
            j = (1 << (j - 1)) + br.get(j - 1)
        start = len(out) - j - 1
        if start < 0:
            raise ValueError("bad match distance")
        for k in range(length):
            out.append(out[start + k])
    return bytes(out[:size])


def crc16(data):
    crc = 0
    for b in data:
        crc ^= b
        for _ in range(8):
            crc = (crc >> 1) ^ 0xA001 if crc & 1 else crc >> 1
    return crc


def entries(archive):
    """Yield (name, method, packed bytes, original size, crc) per file."""
    p = 0
    while p < len(archive) and archive[p] != 0:
        level = archive[p + 20]
        method = archive[p + 2:p + 7].decode("latin-1")
        packed, size = struct.unpack_from("<II", archive, p + 7)
        name = b""
        dirname = b""
        if level in (0, 1):
            hsize = archive[p] + 2
            nlen = archive[p + 21]
            name = archive[p + 22:p + 22 + nlen]
            crc = struct.unpack_from("<H", archive, p + 22 + nlen)[0]
            q = p + hsize
            if level == 1:
                ext = struct.unpack_from("<H", archive, q - 2)[0]
                while ext:
                    kind = archive[q]
                    body = archive[q + 1:q + ext - 2]
                    if kind == 1:
                        name = body
                    elif kind == 2:
                        dirname = body
                    packed -= ext
                    q += ext
                    ext = struct.unpack_from("<H", archive, q - 2)[0]
        elif level == 2:
            hsize = struct.unpack_from("<H", archive, p)[0]
            crc = struct.unpack_from("<H", archive, p + 21)[0]
            q = p + 26
            ext = struct.unpack_from("<H", archive, p + 24)[0]
            while ext:
                kind = archive[q]
                body = archive[q + 1:q + ext - 2]
                if kind == 1:
                    name = body
                elif kind == 2:
                    dirname = body
                q += ext
                ext = struct.unpack_from("<H", archive, q - 2)[0]
            q = p + hsize
        else:
            raise ValueError("LHA header level %d is not supported" % level)
        full = dirname.replace(b"\xff", b"/") + name
        full = full.replace(b"\\", b"/").decode("latin-1")
        yield full, method, archive[q:q + packed], size, crc
        p = q + packed


DICBITS = {"-lh5-": 13, "-lh6-": 15, "-lh7-": 16}


def extract(archive, wanted=lambda name: True):
    """Yield (name, data) for each file that wanted(name) accepts."""
    for name, method, packed, size, crc in entries(archive):
        if name.endswith("/") or method == "-lhd-" or not wanted(name):
            continue
        if method == "-lh0-":
            data = packed[:size]
        elif method in DICBITS:
            data = decompress_lh(packed, size, DICBITS[method])
        else:
            raise ValueError("%s: method %s is not supported" % (name, method))
        if crc16(data) != crc:
            raise ValueError("%s: CRC error" % name)
        yield name, data
