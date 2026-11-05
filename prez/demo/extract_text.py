#!/usr/bin/env python3
"""Extract the .text section of a 32-bit COFF object into a flat file."""
import struct
import sys

def main(obj_path, out_path):
    data = open(obj_path, "rb").read()
    machine, nsec, _, _, _, opt, _ = struct.unpack_from("<HHIIIHH", data, 0)
    if machine != 0x14C:
        sys.exit("not an i386 COFF object")
    off = 20 + opt
    for i in range(nsec):
        name, _, _, size, ptr, reloc_ptr, _, nreloc, _, _ = struct.unpack_from("<8sIIIIIIHHI", data, off + 40 * i)
        if name.rstrip(b"\0") == b".text":
            if nreloc:
                sys.exit("function has relocations; keep it self-contained")
            open(out_path, "wb").write(data[ptr:ptr + size])
            print(f"{out_path}: {size} bytes")
            return
    sys.exit("no .text section")

if __name__ == "__main__":
    main(sys.argv[1], sys.argv[2])
