# Reference environment: the original game under DOSBox-X

Authorized by Gabriel on 4 Nov 2026. Purpose: see the original run, to turn "matches the original's code" into "matches the original on screen"
(ledger status `original-compared`), and to read its memory and saved games.

## Setup
- DOSBox-X 2025.12.01 (`/opt/homebrew/bin/dosbox-x`), config [dosbox-x.conf](dosbox-x.conf), started with [run.sh](run.sh).
- Disk: a local APFS clone, `local-media/reference/win95-work.img` (ignored by git), made with `cp -c` from `~/Desktop/ags/TESORO/Win95.img`.
  The original image is never mounted. The image is an MBR with one active FAT16 partition (2 GB); geometry 512 B, 63 sectors, 64 heads, 1015 cylinders.
- Disc: `~/Desktop/ags/TESORO/Tesoro.cue` (1 data track and 19 audio tracks) mounted as drive D on the secondary IDE controller (`imgmount d ... -ide 2m`), so Windows 95 sees an ordinary ATAPI CD-ROM. The ports (serial, parallel) are disabled.
- Captures go to [../captures/](../captures/).

## Status
- 4 Nov 2026: Windows 95 boots from the clone and the game is installed in it (desktop icon "El Tesoro de la Isla Alcachofa", and the
  launcher from the disc offers play, DirectX 7.0, help, uninstall, quit). First capture: [../captures/first-look.png](../captures/first-look.png).
- Window captures work with [capture.sh](capture.sh) (`re/reference/capture.sh <name>`), which saves only DOSBox-X's own window.
  This needed Screen Recording for Claude.app (granted and Claude restarted). **Sending input is not possible** (no Accessibility permission),
  so clicks and keys come from Gabriel; Claude reads the captures.
- Windows 95's first boot runs the Add New Hardware wizard for the emulated ports; the config disables them (serial 1-4, parallel 1-3).

## Comparisons (original in DOSBox-X vs engine in ScummVM)
Tool: [compare.py](compare.py) (`re/.venv/bin/python re/reference/compare.py original.png engine.png`; needs pillow and numpy in `re/.venv`).
- **Main menu, 4 Nov 2026** ([original](../captures/now1.png), [engine](../captures/engine-main-menu.png), [difference](../captures/engine-main-menu-diff.png)):
  same sign, chains, colours, text and text positions; after aligning the window contents the difference is edge pixels only (mean 7.3, about 1% of pixels,
  from the two programs scaling 640x480 differently). Not settled: the pointer. The original shows its crosshair at the centre; in the engine capture it
  was at the bottom edge, which may only be where the host mouse was.
- **New-game menu, 4 Nov 2026** ([original](../captures/orig-2.png), [engine](../captures/engine-2.png), [difference](../captures/engine-2-diff.png)): the three
  modes with their taglines, same texts, same places, same look; difference is edge pixels only (mean 8.1, about 2% of pixels, text outlines scale differently).
  The fade between the two screens was not captured.
- Note: ScummVM first shows its "not yet fully supported" warning, which needs a click on "Start anyway".

## Findings from running the engine by hand
- 4 Nov 2026: the menu items warned "clearing the hover text is not ported yet" on every pointer leave (and their per-frame step was a warning stub);
  fixed the same day, since the original uses the plain base routines for both. Headless runs never reach this.

## Findings from the original's saved game
- 4 Nov 2026: `ISLA0001.SAV` from the original (a new game at its very start) and a ScummVM save of the engine at the same moment. Read with `fatread.py` (no mounting, no writes);
  the engine's save is gzip-compressed by ScummVM. Layout up to Jim confirmed. The original saves all 47 levels in order with no count or level numbers (ours: visited levels, with count and numbers).
  Level 0 is byte for byte identical; level 1 has the same records and differs only in animation frames. Run on the engine's loader (local instrumentation, reverted), the original's records
  parse for levels 0 to 23, so the per-object state formats are the original's. Details: 0040e000 contract.
- Recipe: put saves in a folder, name them `tesoro.00N` (the command line target is `tesoro`), and run
  `scummvm --path=<TESORO> --savepath=<folder> -x N tesoro`; the engine ignores a failed load silently, so a local print of the result is needed.

## Not yet tried
Installing or starting the game inside Windows 95 (the disk may already have it), CD audio and the Indeo movies in this setup, the DOSBox-X debugger for
reading memory, scripted input.
