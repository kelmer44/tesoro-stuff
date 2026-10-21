# Original-disc inspection

**21 October update:** movies 0 and 1 now play through ScummVM's Indeo decoder in
the active startup implementation; the full startup test decodes 461 frames.
See [startup validation](STARTUP_AND_LOOP.md). Other movies still need runtime checks.

20 October 2026. The Tesoro disc is already mounted at `/Volumes/TESORO`.

## Verified

- `Install/tesoro.dat` and `Install/tesoro.tre` exactly match the analysed copy by SHA-256. Existing resource and script findings therefore apply to these disc assets.
- `Install/bin/data00.bin` through `data12.bin` are 13 AVI videos using Indeo 5, all at 640 × 480. Some contain PCM audio at 22,050 Hz, with channel count and sample width varying by clip.
- FFmpeg successfully decoded the first video frame of every clip. This is an initial decode check, not full-file or ScummVM playback validation.
- The local ScummVM source includes an Indeo 5 decoder and selects it for AVI codec `IV50` when `USE_INDEO45` is enabled. This supplies a concrete reuse path; the Tesoro movies still need to be tested through ScummVM's player.
- The original HTML manual is present. It describes five main verbs, inventory use through right-clicking, an Escape menu for saves/loading/exiting, and keyboard skipping of dialogue/cutscenes. It explicitly describes background music as CD audio, distinct from the WAV speech volume.

Full hashes, movie metadata and first-frame decode results: [disc-inventory.json](analysis/disc-inventory.json).

## Executable variant

The disc's `Install/TESORO2.DAT` begins with the four ASCII bytes `DATA`, followed by an MZ executable. Its executable payload differs from the `TESORO.EXE` used for the current Ghidra project, although the payload length is the same. This is not a resource-data mismatch: the two resource archives match exactly.

Keep the variants separate. Current function addresses are established for the analysed executable only; do not apply them blindly to the disc executable. A reference runtime should record which executable it uses. The disc executable has not been launched or separately decompiled in this inspection.

## CD music recovered

The user supplied the explicit `Tesoro.cue` and `Tesoro.bin` paths. Those files are readable directly, resolving the earlier access problem with listing their Downloads folder. No relocation was needed.

The CUE describes one `MODE1/2352` data track and **19 AUDIO tracks, numbered 2–20**, in a single BIN. The BIN is 682,566,864 bytes, exactly 290,207 sectors of 2,352 bytes; the indexed track boundaries are consistent with its length.

All 19 audio tracks have been extracted to **44,100 Hz, 16-bit stereo PCM WAV**. They total **1,450.2 seconds (24 minutes 10.2 seconds)**. Audio bytes were preserved as stored in the image, without resampling, normalization, byte swapping or trimming. Sample waveform continuity strongly favors little-endian interpretation for this particular image. Each complete output WAV's PCM hash was checked against its source BIN range, and frame counts match the sector lengths. A separate FFprobe check of the first output identifies ordinary `pcm_s16le`, 44.1 kHz stereo.

Original BIN/CUE files remain unchanged. WAVs are under [local-media/cd-audio](local-media/cd-audio), excluded from future Git tracking. The [manifest](local-media/cd-audio/manifest.json) preserves original CUE text, hashes, sector positions, durations, sample statistics and output identities.

### Track numbering

The exports preserve **physical CD numbers**: `track02.wav` is physical track 2, and `track20.wav` is physical track 20. They are not renumbered to 1–19.

The original playback method at `0x403cb0` supplies the script's track number directly to Windows MCI in track/minute/second/frame format and uses the next physical track as the end boundary, except for the final track. This supports retaining physical numbering.

In the local ScummVM source, `DefaultAudioCDManager::play` searches extracted filenames using the supplied track argument, including `track%02d`. The future Tesoro adapter can preserve the same number when selecting these extracted files. Do not subtract one merely because track 1 is data; verify the chosen backend/adapter explicitly during implementation. Loop and stop semantics still need runtime testing.

There are **17 explicit `play cd` requests** across the readable scripts, using ten distinct physical tracks: 4, 6, 7, 9, 11, 13, 17, 18, 19 and 20. Every one resolves to an extracted WAV. Native-code music selection is not exhaustively mapped yet; all 19 tracks are available for it. See [cd-track-map.json](analysis/cd-track-map.json).

Extraction is repeatable with [extract_cd_audio.py](tools/extract_cd_audio.py). Use a fresh output directory; the script refuses to overwrite existing tracks. It intentionally supports this simple, single-BIN CUE layout and rejects unsupported index/pregap arrangements.

## Effect on the plan

The known movie and CD-music gaps are now resolved for this disc. Use the original `.bin` clips as AVI streams and the verified WAV track exports for music. Keep the first playable target unchanged: the office pen interaction and harbour transition. ScummVM playback validation, music looping, and original-executable variant comparison remain implementation checks.
