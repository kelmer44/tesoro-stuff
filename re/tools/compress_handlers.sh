#!/bin/bash
# Compressed view of a pretty_handlers.py dump: declarations and the plain script-start plumbing are dropped, returns are kept.
grep -v '^  undefined\|^{$\|^}$\|pvVar[0-9]* = \*(void \*\*)\|findscript(L4420b8\|local_[0-9a-z]* = (int)\|uStack_4 = param_1;\|^  *\(char\|int\|uint\|void\|byte\|ulonglong\|longlong\|undefined[0-9]*\|short\|bool\) [*a-zA-Z0-9_]*;$' "$1" | sed 's/(int)(uVar[0-9] >> 0x20)//g; s/(int)//g'
