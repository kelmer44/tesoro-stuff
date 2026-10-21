# Repeatable headless Ghidra exports

Verified 21 October 2026 on this machine: Ghidra 11.4.2 and Homebrew OpenJDK
23.0.2. Existing project: `analysis/ghidra-project/TesoroAnalysis.gpr` inside
the research repository. Do not run concurrent headless/GUI writers on it.
The project and runtime directories are local artifacts, not required game files.

## Re-export the startup without saving database changes

This exact command completed all 11 targets. The scripts check the project's
recorded executable SHA-256 against the known Tesoro edition. First verify the
input edition separately if originals have changed. `-readOnly` discards database
changes on exit; exports still write to the specified output directory.

Run from `/Users/gabriel/Desktop/source/tesoro`:

```sh
mkdir -p /private/tmp/tesoro-plan-ghidra/cache \
  /private/tmp/tesoro-plan-ghidra/settings \
  /private/tmp/tesoro-plan-ghidra/tmp \
  /private/tmp/tesoro-plan-ghidra/home \
  /private/tmp/tesoro-plan-ghidra/export

env JAVA_HOME=/opt/homebrew/opt/openjdk/libexec/openjdk.jdk/Contents/Home \
  JAVA_TOOL_OPTIONS='-Duser.home=/private/tmp/tesoro-plan-ghidra/home -Dapplication.cachedir=/private/tmp/tesoro-plan-ghidra/cache -Dapplication.settingsdir=/private/tmp/tesoro-plan-ghidra/settings -Dapplication.tempdir=/private/tmp/tesoro-plan-ghidra/tmp -Djava.io.tmpdir=/private/tmp/tesoro-plan-ghidra/tmp' \
  /opt/homebrew/Caskroom/ghidra/11.4.2-20250826/ghidra_11.4.2_PUBLIC/support/analyzeHeadless \
  /Users/gabriel/Desktop/source/tesoro/analysis/ghidra-project TesoroAnalysis \
  -process TESORO.EXE -readOnly -noanalysis \
  -scriptPath /Users/gabriel/Desktop/source/tesoro/ghidra \
  -postScript RecoverStartup.java /private/tmp/tesoro-plan-ghidra/export \
  -log /private/tmp/tesoro-plan-ghidra/headless.log \
  -scriptlog /private/tmp/tesoro-plan-ghidra/script.log
```

Expected: `TESORO_TARGETS_DECOMPILED=11/11`, exit 0, and 11 `true` rows in
`export/decompile-status.tsv`, with `.raw.c`, `.asm`, `.xrefs.tsv` and index.
Inspect status rows and function boundaries; success means the decompiler
finished, not that inferred signatures/semantics are correct.

`-noanalysis` avoids a fresh automatic whole-program pass. The Java exporters
explicitly call `analyzeChanges` after identifying functions, so targeted changes
can still be analyzed during the run. Read-only mode discards those changes.
For a deliberately persistent recovery pass, use an appropriate project backup
and omit `-readOnly`; avoid changing modes merely to re-export existing evidence.

## Why the runtime properties matter

The earlier startup console log showed `Operation not permitted` while Ghidra
maintained `/var/tmp/gabriel-ghidra/fscache2/.lastmaint`, despite redirecting
`user.home` and `java.io.tmpdir`. The tested invocation additionally redirects
`application.cachedir`, `application.settingsdir` and `application.tempdir` to
writable task directories. Its cache cleanup completed without that error.
These properties are documented by the installed `support/launch.properties`.
Use unique writable task directories for separate runs. Do not change `$HOME`
or installed global launch properties to work around this.

## Add a focused recovery batch

Adapt `ghidra/RecoverStartup.java` or `RecoverTesoro.java`: retain the executable
hash guard; add only addresses supported by callers/vtables/data; identify
functions, analyze changes, then export C, assembly, xrefs and completion status.
An existing containing function requires manual boundary investigation. Inferred
names are investigation labels, not recovered symbols. Preserve earlier exports
or record why a corrected type/boundary supersedes them.

Use the [official headless guide](https://github.com/NationalSecurityAgency/ghidra/blob/master/Ghidra/RuntimeScripts/support/analyzeHeadlessREADME.md)
for options, with the installed version's `support/analyzeHeadlessREADME.html`
as the local reference. Recheck installation/JDK paths after upgrades.
