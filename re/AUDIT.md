# Audit: fresh port vs `tesoro-engine` (legacy engine, comparison only)

Each row compares a ported cluster with the analysis-derived implementation in `tesoro-engine` (`98415538c49`). Findings are not copied into contracts; where they disagree the fresh contract and original-code validation win, and the engine row is listed so Gabriel can decide what to port back.

## Increment 1: core primitives (29 Oct 2026)
| Area | Fresh port (`codex/fresh-core`) | `tesoro-engine` | Verdict |
|---|---|---|---|
| Game clock gap rule | step `< 500` ms accumulates, otherwise dropped; signed compare | `elapsed < 500` accumulates | agree |
| Clock runs in which states | every state: the original freezes and unfreezes the clock in every frame, menus and movies included | only `kAdventure` | **differs**: menus and movie time do not advance in tesoro-engine; matters for anything timed outside gameplay |
| Frozen time within a frame | all reads in a frame return the value captured by `freeze` | a single `_gameTime` updated once in `update()` | agree in effect |
| Suspend / resume | `pauseEngineIntern`: suspend, resume drops inactive time | `isPaused()` early return, `_lastHostTime` reset on resume | agree in effect |
| Randomness | time-driven `randomRange` (+ `randomRangeAlt`), validated against the original | seeded `Common::RandomSource` (`getRandomNumberRng`) | **differs**: original is not seeded or replayable; distribution over the range is equivalent only on average |
| Name key | `nameKey64` validated on 522 names | strings compared directly | not comparable until lookups use keys; real original lookups can collide (case-insensitive) |
Also noted in `tesoro-engine`: `testing.h` shortcuts (`TestingShortcuts::kStartInOffice` etc.) live in engine code, against the project rule for the submitted engine (not carried into `codex/fresh-core`).

## Archive (increment 2)
Fresh port vs `tesoro-engine`: not compared yet (to do after the port: check its index sort order against the descending order found here).
