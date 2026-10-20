// Recover selected Tesoro routines, including virtual targets missed by auto-analysis.
// Run with analyzeHeadless -postScript RecoverTesoro.java <output-directory>.
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

public class RecoverTesoro extends GhidraScript {
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
        target(0x403250L, "tesoro_open_archive");
        target(0x403380L, "tesoro_read_index");
        target(0x403500L, "tesoro_find_resource");
        target(0x401160L, "tesoro_load_animation_bank");
        target(0x4015a0L, "tesoro_load_image_buffer");
        target(0x404810L, "tesoro_parse_script_block");
        target(0x405a60L, "tesoro_execute_script");
        target(0x406190L, "tesoro_register_script_blocks");
        target(0x4070c0L, "tesoro_initialize_jim");
        target(0x4080e0L, "tesoro_load_scene");
        target(0x4132b0L, "tesoro_play_external_movie");
        target(0x401d20L, "tesoro_native_noop");
        target(0x4123e0L, "tesoro_fade_to_black");
        target(0x411f20L, "tesoro_reset_render_lists");
        target(0x401d80L, "tesoro_set_object_flag_bit2");
        target(0x408520L, "tesoro_update_scene_view");
        target(0x407b80L, "tesoro_place_jim_in_scene");
        target(0x408780L, "tesoro_find_scene_object");
        target(0x415c40L, "tesoro_encode_name");
        target(0x405fd0L, "tesoro_start_script");
        target(0x406140L, "tesoro_resume_script_after_action");
        target(0x409e50L, "tesoro_create_scene_object");
        target(0x40ed10L, "tesoro_initialize_interactive_object");
        target(0x40e5c0L, "tesoro_initialize_scenery");
        target(0x40ee70L, "tesoro_dispatch_pending_verb");
        target(0x40c3c0L, "tesoro_pen_pickup");
        target(0x40c880L, "tesoro_office_door_use");
        target(0x414be0L, "tesoro_initialize_door");
        target(0x414c40L, "tesoro_generic_door_use");
        // Script types 11 and 13 share the default no-op callback.
        long[] callbacks = {0x4197a0L, 0x416300L, 0x4198d0L, 0x4169a0L,
            0x4199a0L, 0x419a10L, 0x419ac0L, 0x419530L, 0x419b30L,
            0x419d90L, 0x401d20L, 0x419b90L, 0x401d20L, 0x4193b0L,
            0x419cf0L, 0x419d40L, 0x417c80L, 0x416b80L, 0x416c80L,
            0x419f10L, 0x416b50L};
        for (int i = 0; i < callbacks.length; i++) {
            if (callbacks[i] != 0x401d20L)
                target(callbacks[i], String.format("tesoro_native_type_%02d", i + 1));
        }
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
