// Recover opening composition, text, character initialization and virtual methods.
// Run with analyzeHeadless -postScript RecoverGameplayOpening.java <output-directory>.
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

public class RecoverGameplayOpening extends GhidraScript {
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
        target(0x412e10L, "tesoro_compose_render_rows");
        target(0x412bb0L, "tesoro_present_render_surface");
        target(0x4105a0L, "tesoro_queue_literal_spans");
        target(0x410cc0L, "tesoro_queue_solid_spans");
        target(0x4112d0L, "tesoro_queue_shadow_spans");
        target(0x40fec0L, "tesoro_initialize_character");
        target(0x407ce0L, "tesoro_start_narration");
        target(0x410330L, "tesoro_start_character_speech");
        target(0x413e60L, "tesoro_initialize_game");
        target(0x407770L, "tesoro_advance_character");
        target(0x407d00L, "tesoro_dialogue_choices");
        target(0x412120L, "tesoro_draw_scaled_sprite");
        target(0x412640L, "tesoro_apply_shadow_rows");
        target(0x411c20L, "tesoro_load_colour_lookup");
        target(0x407730L, "tesoro_jim_virtual_0c");
        target(0x407790L, "tesoro_jim_virtual_10");
        target(0x407ac0L, "tesoro_jim_virtual_18");
        target(0x407a80L, "tesoro_jim_virtual_74");
        target(0x408030L, "tesoro_jim_virtual_78");
        target(0x401c50L, "tesoro_recruiter_virtual_0c");
        target(0x4101f0L, "tesoro_recruiter_virtual_10");
        target(0x4102e0L, "tesoro_recruiter_virtual_18");
        target(0x4103d0L, "tesoro_recruiter_virtual_74");
        target(0x410130L, "tesoro_recruiter_virtual_78");
        target(0x40efa0L, "tesoro_draw_interactive_object");
        target(0x40ffa0L, "tesoro_queue_speech_text");
        target(0x411c80L, "tesoro_layout_text");
        target(0x412c50L, "tesoro_draw_text_overlay");
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
