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
- 4 Nov 2026: Windows 95 boots from the clone (the log shows APM and Plug and Play coming up). The game has not been started yet.
- **Blocked on host permissions:** from Claude's shell, `screencapture` fails ("could not create image from display") and sending keystrokes through
  System Events is refused (error -1743). DOSBox-X's own screenshot key (Ctrl+F5) writes into `re/captures` when pressed by hand.
  [windowid.swift](windowid.swift) prints the id of DOSBox-X's window for `screencapture -l` once Screen Recording is granted.

- First boot: Windows 95 runs its Add New Hardware wizard (it asked for printer drivers). Cancel it; the parallel and serial ports are disabled in the config so it should stop after a restart.

## Not yet tried
Installing or starting the game inside Windows 95 (the disk may already have it), CD audio and the Indeo movies in this setup, the DOSBox-X debugger for
reading memory, scripted input.
