// Recover the script block linker, object list helpers and media helpers used by scripts.
// Run with analyzeHeadless -postScript RecoverScriptCompiler.java <output-directory>.
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

public class RecoverScriptCompiler extends GhidraScript {
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
        target(0x4067a0L, "tesoro_link_script_block");
        target(0x406110L, "tesoro_link_script_registry");
        target(0x405630L, "tesoro_link_script_ops");
        target(0x4060b0L, "tesoro_initialize_script_registry");
        target(0x404630L, "tesoro_copy_script_block_linked");
        target(0x413570L, "tesoro_load_text_table");
        target(0x4138b0L, "tesoro_text_table_lookup");
        target(0x4137c0L, "tesoro_sort_text_table");
        target(0x405850L, "tesoro_script_update");
        target(0x413a10L, "tesoro_current_frame_time");
        target(0x401ee0L, "tesoro_object_virtual_28");
        target(0x401f90L, "tesoro_object_virtual_2c");
        target(0x401fa0L, "tesoro_object_virtual_30");
        target(0x401dc0L, "tesoro_restore_object_animation_state");
        target(0x401a70L, "tesoro_resume_animation_bank");
        target(0x40ec90L, "tesoro_initialize_object_base");
        target(0x413940L, "tesoro_find_phrase_index");
        target(0x40fa10L, "tesoro_add_object_to_list");
        target(0x40fb20L, "tesoro_remove_object_from_list");
        target(0x404210L, "tesoro_audio_service");
        target(0x404110L, "tesoro_play_wav");
        target(0x403cb0L, "tesoro_play_cd_track");
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
