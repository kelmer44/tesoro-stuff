// Recover animation playback, frame advancement and object animation dispatch.
// Run with analyzeHeadless -postScript RecoverAnimation.java <output-directory>.
// @category Tesoro
import ghidra.app.script.GhidraScript;
import ghidra.app.decompiler.DecompInterface;
import ghidra.app.decompiler.DecompileResults;
import ghidra.program.model.address.Address;
import ghidra.program.model.listing.Function;
import ghidra.program.model.listing.Instruction;
import ghidra.program.model.listing.InstructionIterator;
import ghidra.program.model.symbol.Reference;
import ghidra.program.model.symbol.ReferenceIterator;
import ghidra.program.model.symbol.SourceType;
import java.io.File;
import java.io.PrintWriter;
import java.nio.charset.StandardCharsets;
import java.nio.file.Files;
import java.util.LinkedHashMap;
import java.util.Map;

public class RecoverAnimation extends GhidraScript {
    private final Map<Long, String> targets = new LinkedHashMap<>();

    private void target(long address, String name) { targets.put(address, name); }

    @Override
    public void run() throws Exception {
        String[] args = getScriptArgs();
        if (args.length != 1) throw new IllegalArgumentException("Expected output directory");
        String expectedHash = "0fcd56b306b857283347e705dcce41a4b44dd4d9ef7c9226694114bba80792ef";
        if (!expectedHash.equalsIgnoreCase(currentProgram.getExecutableSHA256()))
            throw new IllegalArgumentException("These addresses apply only to the verified TESORO.EXE");
        File output = new File(args[0]);
        output.mkdirs();
        target(0x401580L, "tesoro_clone_animation_bank");
        target(0x401a50L, "tesoro_pause_animation");
        target(0x401860L, "tesoro_advance_animation");
        target(0x401740L, "tesoro_set_animation_playback");
        target(0x401a70L, "tesoro_resume_animation");
        target(0x401aa0L, "tesoro_initialize_base_object");
        target(0x401dc0L, "tesoro_restore_object_animation");
        target(0x401de0L, "tesoro_play_object_animation");
        try (PrintWriter index = new PrintWriter(new File(output, "index.tsv"), "UTF-8")) {
            index.println("address\tname\tstatus");
            for (Map.Entry<Long, String> entry : targets.entrySet()) {
                monitor.checkCancelled();
                Address address = toAddr(entry.getKey());
                Function function = getFunctionAt(address);
                if (function == null) {
                    Function containing = getFunctionContaining(address);
                    if (containing != null) {
                        index.printf("%s\t%s\tinside_existing_function_%s%n", address,
                            entry.getValue(), containing.getEntryPoint());
                        continue;
                    }
                    disassemble(address);
                    function = createFunction(address, entry.getValue());
                }
                if (function == null) {
                    index.printf("%s\t%s\tcreate_failed%n", address, entry.getValue());
                    continue;
                }
                function.setName(entry.getValue(), SourceType.USER_DEFINED);
                index.printf("%s\t%s\tready%n", address, entry.getValue());
            }
        }
        // Newly identified virtual functions can expose switch tables and more callees.
        analyzeChanges(currentProgram);
        DecompInterface decompiler = new DecompInterface();
        decompiler.openProgram(currentProgram);
        int complete = 0;
        try (PrintWriter status = new PrintWriter(new File(output, "decompile-status.tsv"), "UTF-8")) {
            status.println("address\tname\tcompleted\terror");
            for (Map.Entry<Long, String> entry : targets.entrySet()) {
                monitor.checkCancelled();
                Address address = toAddr(entry.getKey());
                Function function = getFunctionAt(address);
                if (function == null) continue;
                String stem = String.format("%08x_%s", entry.getKey(), entry.getValue());
                DecompileResults result = decompiler.decompileFunction(function, 90, monitor);
                boolean ok = result != null && result.decompileCompleted();
                String error = result == null ? "no result" : result.getErrorMessage();
                status.printf("%s\t%s\t%s\t%s%n", address, entry.getValue(), ok,
                    error == null ? "" : error.replace('\n', ' '));
                if (ok) {
                    String heading = "/* Ghidra output, not original source. Inferred types and names may be wrong.\n"
                        + " * TESORO.EXE SHA256: 0fcd56b306b857283347e705dcce41a4b44dd4d9ef7c9226694114bba80792ef\n"
                        + " * Entry: " + address + "\n */\n\n";
                    Files.write(new File(output, stem + ".raw.c").toPath(),
                        (heading + result.getDecompiledFunction().getC()).getBytes(StandardCharsets.UTF_8));
                    complete++;
                }
                try (PrintWriter assembly = new PrintWriter(new File(output, stem + ".asm"), "UTF-8")) {
                    InstructionIterator instructions = currentProgram.getListing().getInstructions(function.getBody(), true);
                    while (instructions.hasNext()) {
                        Instruction instruction = instructions.next();
                        assembly.printf("%s  %s%n", instruction.getAddress(), instruction);
                    }
                }
                try (PrintWriter refs = new PrintWriter(new File(output, stem + ".xrefs.tsv"), "UTF-8")) {
                    refs.println("from\tto\ttype");
                    ReferenceIterator references = currentProgram.getReferenceManager().getReferencesTo(address);
                    while (references.hasNext()) {
                        Reference reference = references.next();
                        refs.printf("%s\t%s\t%s%n", reference.getFromAddress(), reference.getToAddress(), reference.getReferenceType());
                    }
                }
            }
        } finally {
            decompiler.dispose();
        }
        println("TESORO_TARGETS_DECOMPILED=" + complete + "/" + targets.size());
    }
}
