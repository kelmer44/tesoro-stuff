# Dialogue / cutscene script language (fresh, 28 Oct 2026)

Source: `DlgScript_Load` (00404810, parser), `DlgScript_RunSteps` (00405a60), `DlgScript_Update` (00405850), `DlgScript_Link` (00405630), `DlgScript_Start` (00405fd0).
Confidence: **step layout and executor behaviour: read**; **mapping of text keywords to step types: inferred from string order, not verified**.

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
