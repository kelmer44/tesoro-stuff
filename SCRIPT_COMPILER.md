# Script compiler: how the original turns DLG text into ops

22 October 2026. Addresses apply to `TESORO.EXE` SHA-256
`0fcd56b306b857283347e705dcce41a4b44dd4d9ef7c9226694114bba80792ef`. Names are
research labels. Engine implementation: `engines/tesoro/script.h/.cpp` in
`../scummvm-tesoro` (branch `tesoro-engine`, uncommitted when written).
Exports: `ghidra/RecoverScriptCompiler.java` (15 functions, all completed; see the
read-only command in `skills/tesoro-re/references/headless-ghidra.md`).

## Load order and stages

`tesoro_initialize_game` (`0x413e60`) loads four speech tables, then four script
files, then links, in this fixed order:

| Step | Files | Routine |
| --- | --- | --- |
| Speech tables | `OBJETOS.TXT`, `ANIMS.TXT`, `CONVERS.TXT`, `ICONS.TXT` | `0x413570` load, `0x4137c0` sort |
| Script files | `OBJETOS.DLG`, `ANIMS.DLG`, `CONVERS.DLG`, `ICONS.DLG` | `0x406190` register, `0x404810` parse per block |
| Link | all registered blocks | `0x406110` calls `0x405630` per block |

`0x4067a0` is a plain field copy of the parsed block into the final 0xD8-byte
script object; it resolves nothing. The class (vtable) is chosen from the header
type, as recorded in `TECHNICAL_NOTES.md`.

## Parsing is C `scanf` driven and keyed on first letters

The parser reads the file as a character stream and uses `fscanf` with fixed
format strings (recovered from `.rdata`). Consequences that the engine reproduces:

- A line is recognised by its first character only (`' ' . D _ b i j m p r s v`),
  then the rest of the keyword is matched by the next format literal. Anything else
  raises the original's "ERROR AL LEER COMANDO" box and stops the parse. Blank or
  comment-only lines inside a block are therefore not accepted. The engine fails
  visibly in the same situations.
- After each command the rest of the line is discarded, which is why trailing
  `//` comments work.
- Block header: `.NAME`, optional ` type`, participant count, then that many
  names. The `\n` in `"%i\n"` / `"%s\n"` skips all following white space, including
  the leading space of a first speech line.
- Blocks are separated by exactly one extra line after `.END`; the register loop
  stops at the first block that does not start with `.` or when fewer than 8 bytes
  remain. All 145 blocks register.
- Labels, jump targets, run targets and participant names are compared by the
  64-bit name encoding (`0x415c40`), which keeps 12 characters and folds case.
- `%[0-í]s` (label) is a scanset from `'0'` to byte `0xA1`; a scanset does not skip
  leading white space.

### `menutiem0`

`CONVERS.DLG:7` (`menutiem0 next`) is accepted as an ordinary menu item. After the
leading `m` the parser reads the rest of the word and only tests whether it is
longer than three characters (`menu`) or not (`menuitem`); the digit and the exact
spelling are never read. Item order, not the digit, identifies items.

### Operand layout (24-byte ops)

Parser output is rewritten by the link pass, so the stored layouts differ:

| Op | Type | After link |
| --- | --- | --- |
| return / end | 0 / 10 | none |
| speech | 1 | +4 text entry, +8 speaker (0 = player), +0xC option (default 1, or `, n`) |
| sleep | 2 | +4 milliseconds |
| break | 3 | none; returns to the menu op |
| jmp | 4 | +4 op index of the label (0 if missing) |
| menu | 5 | none |
| menu item | 6 | +4 short index of the next item (-1 last), +6 variable (0xFF none), +7 invert (`!v`) |
| if | 7 | +4 op index, +8 variable byte, +9 value byte |
| run | 8 | +4 block pointer (0 for `FANMTRONO`), +8/+0xC label hash |
| sendmessage | 9 | +4/+8 object hash, +0xC scene, +0x10 sub-op, +0x14.. operands |
| assign | 0xB | +4 variable byte, +5 value byte |
| DO | 0xC | +4 ordinal within the block |
| avi / sound / cd | 0xD / 0xE / 0xF | +4 number or WAV name |

Values are bytes: `if v2 == -1` (CONVERS.DLG:1366) compares against 255. Play
`first` is a signed byte in the executor.

`sendmessage` sub-ops: 0 play, 1 walk (`ira x y`), 2 look (`mirar` ARRIBA 1, ABAJO 2,
IZQUIERDA 3, DERECHA 4), 3 activate, 4 show. A quoted object followed by `i` is
read as the `in <scope>` form (`INVENTARIO`, or `LEVELn`); a digit is a 1-based
participant. Play modes: stop 0, play 1, loop 2, wait 3.

### Link pass `0x405630`

- jmp/if: label hash to op index; a missing label becomes 0.
- menu: resets the chain; each item whose word was `next` is linked to the item
  that follows it, `stop` ends the chain, the last item stores -1.
- run: a name equal to `FANMTRONO` links to no block; any other name links to the
  first registered block with that hash.

### Speech texts

`ID, "text"` lines read with `" %[^,]s"` and `", \"%[^\"]s"`, two characters
discarded after the text. Each table is sorted by selection sort in descending
unsigned (low, high) order and searched by binary search with that order; the
encoded halves are 30-bit, so signed and unsigned comparisons agree. The four
tables are searched in the load order above and the first hit wins. A speech id
that is in no table raises the original's "frase no encontrada" box.

## Findings in the original data

- Three jumps go to labels that do not exist: `FDICKCOCINA` (`OBJETOS.DLG:854`,
  `ent2`, commented "nunca"), `FORZOGUEI` (line 1235) and `FTIENDA` (line 1673).
  The original sends them to op 0. The engine warns and does the same.
- 11 `run 'FANMTRONO'` ops link to no block and are special-cased by the executor.
- No duplicate speech ids across the four tables; no unresolved speech ids.

## Verification

Reproduce with the ignored driver `local-media/script-implementation/driver.cpp`
(links the production `script.o`, `resources.o` and ScummVM's `libcommon.a` and
`libformats.a`) and:

```sh
python3 tools/verify_script_compiler.py --game /Users/gabriel/Desktop/ags/TESORO \
  --driver local-media/script-implementation/driver \
  --data local-media/script-implementation/data \
  --output analysis/script-compiler-verification.json
```

Result on 22 October 2026: 145 blocks and 4,274 ops; all 4,419 output lines
(headers and ops with operands, source lines, jump/menu links, speech lookups) equal
an independent regular-expression reading of the original files; the op counts equal
the lexical counts in `analysis/full-game-catalogue.json`.

**Limits.** The comparison reads the same source files independently but does not
execute the original parser, so scanf corner cases on malformed lines are covered
only by reasoning, not by differential execution. No executor, wait/resume, menu
display or native `DO` behaviour is implemented or compared yet; this is the compiled
form only. No original-runtime session exists.
