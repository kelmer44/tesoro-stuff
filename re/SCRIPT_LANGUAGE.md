# Dialogue / cutscene script language (fresh, 28 Oct 2026)

Source: `DlgScript_Load` (00404810, parser), `DlgScript_RunSteps` (00405a60), `DlgScript_Update` (00405850), `DlgScript_Link` (00405630), `DlgScript_Start` (00405fd0).
Confidence: **step layout, executor behaviour and the parser's line dispatch: read** (28 Oct 2026). Sub-command spelling beyond the first letters is inferred from the sscanf formats; exact casing of command words is not recorded because the parser matches only the first character(s).

## Text grammar fragments found in the executable (read order, 0042d0a4..0042d1fc)
`ERROR AL LEER COMANDO`, `%[^'\n']s`, `...mp %s` (jump), `DERECHA`, `IZQUIERDA`, `ABAJO`, `ARRIBA`, `...irar %s`, `...ra %i %i`, `wait`, `loop`, `stop`, `play`,
`...lay %i %i %s`, `...ctivate o`, `...how o`, `LEVEL%i`, `INVENTARIO`, `...n %s`, `...ndmessage`, `...eep %i`, `%[^,]s`, `%i`, `%[^']s`, `...lay %s`, `if v%i == %i jmp %s`, `%i = %i`, `%s.WAV`.
The leading characters of several fragments are the tail of the previous string and are missing from Ghidra's names; full words need the raw strings (e.g. "Mirar %s", "Sleep %i", "Sendmessage ").

## Compiled step (24 bytes, array at script +0xbc; program counter +0xcc; loop start +0xa0)
Dword 0 = step type. Executor semantics read from `RunSteps`/`Update`:
| Type | Behaviour |
|---|---|
| 0, 1, 10 | speak a phrase: actor chosen from the actor table by 1-based index in field +8, phrase record in +4, flag +0xc; then the player object narrates a subtitle line |
| 2 | timed wait: records the start time (`Update` waits for the duration in +4) |
| 3 | stop / end: if the loop start is negative, remove script from the main list, release the cursor, reset actors; otherwise jump to the loop start |
| 4 | unconditional jump to step +4 |
| 5 | label/loop start: remember current step as loop start, continue |
| 7 | if script variable[+8] == +9 then jump to step +4 (variables are bytes) |
| 8 | call an event on an object (`DlgScript_Start` on the target), special-cased when the key equals a fixed name key |
| 9 | actor command by key (+8,+0xc): sub-type in +0x10: 0 select animation, 1 walk to (x,y) with this script as target, 2 set facing, 3 set active, 4 set visible |
| 0xb | variable[+4] = +5 |
| 0xc | virtual slot 28 of the script object with argument +4 |
| 0xd | play the DirectShow movie |
| 0xe | play a sound named `%s.WAV` from the archive |
| 0xf | CD audio: play track +4 |
Blocking steps (handled in `Update`): 1 (wait until the speaker is done + 200 ms), 2, 5/6 (choice menu via the player object), 8, 9.

## Open points
Exact keyword -> type mapping, the choice-menu step encoding, how variables are declared and persisted in saves, the meaning of fields +0xa0/+0xa4/+0xd0, the full actor table format, `LEVEL%i` and `INVENTARIO` handling.


## Parser line dispatch (`DlgScript_Load`, read)
A script block starts with a header read by `fscanf`: a name (hashed to the script's 64-bit key at +0x44), an optional second token, a count of actor names and then that many actor keys (stored at +0x2d, resolved later by `DlgScript_Start`). Each following line is dispatched on its **first character**:
| First char | Step type | Syntax read | Meaning |
|---|---|---|---|
| space | 1 | ` <phrase>, <n>` (phrase name via `PhraseLookup`, optional list continuing after `,`) | speak a phrase line |
| `.` | 10 | | end of this script block (also executed as a say/advance in `RunSteps`) |
| `D` | 0xc | | decision/option marker; each `D` gets the next index (counter) |
| `_` | label | `_<name>` | defines a label: key of the name plus the current step index (12-byte label entries, count at +0xc8) |
| `b` | 3 | | stop/return |
| `i` | 7 | `if v<n> == <m> jmp <label>` | conditional jump on script variable n |
| `j` | 4 | `jmp <label>` | unconditional jump |
| `m` | 5 or 6 | `m` alone = 5; `m[n|...] [!]v<n>` = 6 with flag 1 for `n` and a variable condition (`v<n>` true, `!v<n>` negated, 0xff when none) | menu: 5 starts/ends the menu, 6 is a menu option |
| `p` | 0xd, 0xe, 0xf | `pa` movie, `ps <name>` sound (`<name>.WAV`), `pc <n>` CD track | play |
| `r` | 0 or 8 | `re` = 0 (reply/say as the player); `ru 'obj' 'event'` = 8, two 64-bit keys | run: call event/script on an object |
| `s` | 2 or 9 | `sl<eep> <n>` = 2 (timed wait ms); `se<ndmessage> '<actor>' <cmd>` = 9 | sleep / send message to an actor |
| `v` | 0xb | `v<n> = <m>` | assign script variable |
| other | | | message box `ERROR AL LEER COMANDO` and the load aborts |

### `sendmessage` target and commands (type 9, sub-type at step +0x10)
Target: `'<name>'` hashed to a key, or `INVENTARIO` (interface level object), or `LEVEL<n>` (entry n of the level table), or an actor index digit into the script's actor table.
| Sub | Command | Syntax | Notes |
|---|---|---|---|
| 0 | `p` play animation | `play <bank> <range> <name?>` (`%i %i %s`) | selects animation bank/range |
| 1 | `i` go to | `ira <x> <y>` | walk the actor; script waits |
| 2 | `m` look | `mirar ARRIBA|ABAJO|IZQUIERDA|DERECHA` | facing 1, 2, 3, 4 (matches Jim's facing numbering: 1 up, 2 down, 3 left, 4 right in world terms) |
| 3 | `a` activate | `activate on|off` | active flag |
| 4 | `s` show | `show on|off` | visible flag |
Directions are Spanish: ARRIBA, ABAJO, IZQUIERDA, DERECHA.

### Corrections to the executor table above
Type 0 comes from `re` (player reply) and 10 from `.`; type 5 is the *menu header* (the executor records the loop start there so that the script returns to the menu after an option) and 6 is a *menu option*; 0xc is the `D` marker.
Types 0xd/0xe/0xf are the three `p` variants. The earlier guess that types 5/6 are labels was wrong for the parser's output: labels are not steps, they live in the label table.
