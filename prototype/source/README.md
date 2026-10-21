# Preserved Tesoro prototype

This directory contains the exact working engine source from ScummVM commit
`56e25ababf42611c91fa35ba1c716f3a6e703b74`, saved before resetting the `tesoro`
branch to the unmodified output of `create_engine tesoro` on 21 October 2026.
The ten original files are under `engines/tesoro/`; `manifest.json` records their
SHA-256 hashes and original commit/base. This is reference source, not an active
ScummVM checkout. Original game assets are not included.

The source includes the office/pen sequence, archive and image readers, script
interpreter, voices, subtitles, animations, inventory switches and harbour exit.
Its existing limitations still apply; see [prototype notes](../README.md).

## Copying pieces into the fresh engine

| Files | Purpose |
| --- | --- |
| `engines/tesoro/resources.h`, `resources.cpp` | TRE/DAT archive access, LVL/ANM parsing, image decoding and scene records. |
| `engines/tesoro/script.h`, `script.cpp` | Dialogue/text catalogue and resumable script execution. |
| `engines/tesoro/tesoro.h`, `tesoro.cpp` | Prototype host, rendering, speech, movement, object requests and validation. |
| `engines/tesoro/detection.cpp` | Detection hashes for the supplied Spanish release. |
| `engines/tesoro/metaengine.cpp`, `module.mk`, `configure.engine` | Original prototype wiring and build requirements. |

Start with the resource reader, then the script layer. Add copied `.cpp` files
to the fresh engine's `module.mk` as you introduce them. Port the runtime piece
by piece: the generated engine and this prototype use different class/member
layouts, so copying a method alone may also require its declarations, includes
and state. The old wiring files are reference material; replacing all of them
would restore the entire prototype rather than the incremental starting point.

The archive is intentionally unchanged. Make new engine edits in the linked
worktree at `../scummvm-tesoro` relative to the repository root, not here.
A local Git backup branch `archive/tesoro-prototype` also retains the old commit
in the shared `scummvm-fork` repository.

The local packaged `prototype/Tesoro Prototype.app` retains the old playable
binary. Rebuilding the active `tesoro` branch builds only the generated starter
until you port functionality back. Do not repackage that starter over the old
app if you want to retain the playable binary.
