# Original-game reference environment (Wine, set up 28 October 2026)

Local, ignored: `local-media/wine-test/` (`prefix/` Wine 9.0 prefix, `game/` copy of
`/Users/gabriel/Desktop/ags/TESORO`, `cdroot/` files of the CD data track extracted with
`7z x`, label file `cdroot/.windows-label` = `TESORO`). Drive `D:` -> `cdroot`, type cdrom
(`HKCU\Software\Wine\Drives`). macOS could not mount the ISO (hdiutil error 35), hence extraction.

## Findings (fresh analysis of `TESORO.EXE`)
- `TESORO.EXE` is both launcher and game. WinMain is `FUN_00415960`.
  - No argument: `FUN_00415b00` scans drives for volume label `TESORO` (skips floppies),
    checks `SETUP.EXE` exists, then the CD's installer menu runs. This is the install dialog
    seen when launching it directly.
  - Argument `_$_$_ ` + one byte: game start (byte stored in global `DAT_00442204`; looks like
    the CD drive letter, unverified). Argument `Uninstall`: uninstall path.
- No registry imports in the executable.
- The argument must reach the program unquoted. Plain `wine TESORO.EXE ...` quotes it, so use cmd:

```sh
cd local-media/wine-test/game
WINEPREFIX=$PWD/../prefix wine cmd /c 'TESORO.EXE _$_$_ D'
```
Status: process stays alive; one Wine `seh` exception logged. Visual result not yet confirmed.

## CD-audio dependency (found 28 October 2026)
The game cannot start without a working MCI CD-audio device. `FUN_00403b60` opens it
(`mciSendCommand` 0x803, device type 0x204) and shows "Error al inicializar el CD" if it fails,
leaving the track count at -1. `FUN_00403e00` only creates DirectSound when that count is >= 0,
so the failure shows "Error al inicializar el dispositivo de sonido" and the game exits
(`FUN_00413b90`). Wine on macOS has no audio-CD device for the extracted data folder.

Workaround (a modified COPY, not evidence): `game/TESORO_NOCD.EXE` = `TESORO.EXE` with the byte at
file offset 0x3273 (VA 0x403e73, `JGE` 7D -> `JMP` EB) changed, so DirectSound is created even
when the CD failed. Music is absent; CD calls fail silently. Runs with:
`WINEPREFIX=$PWD/../prefix wine cmd /c 'TESORO_NOCD.EXE _$_$_ D'` (from `game/`).
Do not treat behaviour that depends on CD state as original. A Win9x guest with a real
cue/bin (e.g. DOSBox-X) would avoid the patch.
Next obstacle: `NtUserChangeDisplaySettings` (640x480 mode switch) returns -2 under Wine/macOS.

## Display (28 October 2026)
Wine/macOS exposes only 32-bit modes (none at 640x480), so the game's `SetDisplayMode(640,480,16)`
fails ("DIRECTDRAW: error al cambiar la resolución…"), also in a Wine virtual desktop.
Fix: cnc-ddraw v7.1.0.0 (`cnc-ddraw.zip`, 2,688,079 bytes, SHA-256
`0b13ab89a64c9918189b1dadd449ef6ed3cb3b7b19cabd96d8adbd95505bb908`, from the project's GitHub
release; downloaded to `local-media/wine-test/dl-cncddraw/`), `ddraw.dll` + `ddraw.ini` copied into
`game/`, `WINEDLLOVERRIDES=ddraw=n,b`, `ddraw.ini` lines 17 `windowed=true`, 51 `renderer=gdi`.
Security: `Z:` removed from the prefix; the game copy is exposed as `F:` and the CD files as `D:`.
Launcher: `local-media/wine-test/run-tesoro.sh`. Fidelity caveat: this DDraw wrapper changes how
frames reach the screen; do not use it to judge original pixel output, only game logic/state.

## Status (28 October 2026, confirmed by Gabriel)
Original game runs with `run-tesoro.sh`: a black window for a while, then the intro movie and the
main menu. No mouse cursor is drawn (the game hides the system cursor and draws its own, input
via DirectInput). Whether clicks/keys work is not yet confirmed. No music (CD-audio patch).

## Mouse under Wine 9 (found 28 October 2026) and Wine 11
The game integrates DirectInput relative deltas (`cursor += lX/lY`, clamp 0..640 x 0..480, routine
`0x403aa0`, vtable `0x428074`, not a Ghidra function). Wine 9.0 on macOS 26 emulates relative axes by
warping the pointer to the window centre, but the warp is ignored, so it reports "distance from
centre" and the game's cursor pins to the bottom right. `MouseWarpOverride` = disable/force did not help.
Fix attempt: Gcenx `wine-devel-11.18-osx64.tar.xz` (190,974,384 bytes, SHA-256
`aa0ea4c82e636ae7bca2076387cb0a5affa26509ad13f119ecd0d62bd7ba6f82`, verified), installed as
`/Applications/Wine Devel.app` (unsigned; no quarantine flag), registered with Launch Services.
Prefix copy `prefix11/` (MouseWarpOverride removed). Launcher: `run-tesoro-wine11.sh`.
Status: starts and runs; mouse behaviour under Wine 11 not yet confirmed.

## Mouse: Wine 11 does not fix it; patched copy (28 October 2026)
Trace under Wine 11.18 (fresh `prefix11`, the first copy of the Wine 9 prefix crashed with a missing
`cryptbase` forward): still `dinput_mouse_hook` raw (776,480) -> reported (456,240) = raw minus window
centre (320,240); no warp attempted. Wine only recentres the pointer where the warp works, which it
does not here. The reading is therefore "distance from centre" forever.
`game/TESORO_NOCD_MOUSE.EXE` = `TESORO_NOCD.EXE` plus three edits, to make the game take absolute
positions: file 0x2a7d8 (DIDATAFORMAT.dwFlags at VA 0x42c1d0) 2 -> 1 (relative -> absolute axes);
0x2eec (VA 0x403aec, `add edx,eax` 03 D0) -> `mov edx,eax` 8B D0; 0x2ef1 (VA 0x403af1, `add eax,ecx`
03 C1) -> `mov eax,ecx` 8B C1. Launchers: `run-tesoro-wine11-mouse.sh`, `run-tesoro-wine9-mouse.sh`.
Not original behaviour: use only for hands-on testing, not for studying the game's mouse handling.
Wine 11 also fails to build video filters for the intro movie (quartz MPEG/AVI splitters).

## DOSBox-X Win95 reference (works, slow) — 28 October 2026
The AbandonSocios package (`~/Downloads/El Tesoro de Isla Alcachofa - DOSBox-Win95 CD/`) boots `Win95.img`
(game installed inside, original `TESORO.EXE`, no patches) with `Tesoro.cue` as D: (real CD audio).
Local setup: `local-media/dosbox/` = APFS clone of `Win95.img` (Windows writes to it; originals untouched),
`tesoro.conf` = package `dosboxAlcachofa.conf` with absolute image paths, `fullscreen=false`, `autolock=true`
(mouse captured by clicking in the window; Ctrl+F10 or wheel-click releases). Launcher: `run-dosbox.sh`
(`/Applications/dosbox-x.app`, version 2025.01.01). Mouse and music work here, so this is the faithful
reference; the Wine copies lack a cursor/music and need patches.
