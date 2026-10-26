// Export chosen functions (C, assembly, xrefs) and list virtual-call sites by vtable offset.
// Run with analyzeHeadless -postScript RecoverAddresses.java <output-directory> <spec>...
// A spec is 0xADDRESS=name (export that function) or vcall:0xOFFSET (list every
// "CALL [reg+OFFSET]" with its containing function in vcalls.tsv), or ref:0xOFFSET
// (list every instruction with a [reg+OFFSET] operand, in vcalls.tsv with mnemonic).
// text:STRING lists instructions whose text contains STRING.
// list:0xSTART:0xEND writes the functions that start in that range to functions.tsv.
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

public class RecoverAddresses extends GhidraScript {
    @Override
    public void run() throws Exception {
        String[] args = getScriptArgs();
        if (args.length < 2) throw new IllegalArgumentException("Expected output directory and specs");
        String expectedHash = "0fcd56b306b857283347e705dcce41a4b44dd4d9ef7c9226694114bba80792ef";
        if (!expectedHash.equalsIgnoreCase(currentProgram.getExecutableSHA256()))
            throw new IllegalArgumentException("These addresses apply only to the verified TESORO.EXE");
        File output = new File(args[0]);
        output.mkdirs();
        Map<Long, String> targets = new LinkedHashMap<>();
        java.util.List<String> offsets = new java.util.ArrayList<>();
        java.util.List<String> texts = new java.util.ArrayList<>();
        java.util.List<String> fieldRefs = new java.util.ArrayList<>();
        for (int i = 1; i < args.length; ++i) {
            if (args[i].startsWith("list:")) {
                String[] r = args[i].split(":");
                try (PrintWriter list = new PrintWriter(new File(output, "functions.tsv"), "UTF-8")) {
                    list.println("entry\tsize\tname");
                    ghidra.program.model.listing.FunctionIterator it = currentProgram.getListing()
                        .getFunctions(toAddr(Long.decode(r[1])), true);
                    while (it.hasNext()) {
                        Function f = it.next();
                        if (f.getEntryPoint().getOffset() > Long.decode(r[2])) break;
                        list.printf("%s\t%d\t%s%n", f.getEntryPoint(), f.getBody().getNumAddresses(), f.getName());
                    }
                }
                continue;
            }
            if (args[i].startsWith("text:")) { texts.add(args[i].substring(5).toLowerCase()); continue; }
            if (args[i].startsWith("ref:")) { fieldRefs.add(args[i].substring(4).toLowerCase()); continue; }
            if (args[i].startsWith("vcall:")) { offsets.add(args[i].substring(6).toLowerCase()); continue; }
            String[] parts = args[i].split("=", 2);
            targets.put(Long.decode(parts[0]), parts.length > 1 ? parts[1] : "function");
        }
        if (!offsets.isEmpty() || !fieldRefs.isEmpty() || !texts.isEmpty()) {
            try (PrintWriter calls = new PrintWriter(new File(output, "vcalls.tsv"), "UTF-8")) {
                calls.println("offset\tcall\tfunction\tinstruction");
                InstructionIterator all = currentProgram.getListing().getInstructions(true);
                while (all.hasNext()) {
                    monitor.checkCancelled();
                    Instruction instruction = all.next();
                    String text = instruction.toString().toLowerCase();
                    for (String needle : texts) {
                        if (text.contains(needle)) {
                            Function f = getFunctionContaining(instruction.getAddress());
                            calls.printf("text %s\t%s\t%s\t%s%n", needle, instruction.getAddress(),
                                f == null ? "-" : f.getEntryPoint().toString(), instruction);
                        }
                    }
                    for (String offset : fieldRefs) {
                        if (text.contains("+ " + offset + "]") || text.contains("+" + offset + "]")) {
                            Function f = getFunctionContaining(instruction.getAddress());
                            calls.printf("ref %s\t%s\t%s\t%s%n", offset, instruction.getAddress(),
                                f == null ? "-" : f.getEntryPoint().toString(), instruction);
                        }
                    }
                    if (!instruction.getMnemonicString().equals("CALL")) continue;
                    for (String offset : offsets) {
                        if (text.contains("+ " + offset + "]") || text.contains("+" + offset + "]")) {
                            Function f = getFunctionContaining(instruction.getAddress());
                            calls.printf("%s\t%s\t%s\t%s%n", offset, instruction.getAddress(),
                                f == null ? "-" : f.getEntryPoint().toString(), instruction);
                        }
                    }
                }
            }
        }
        try (PrintWriter index = new PrintWriter(new File(output, "index.tsv"), "UTF-8")) {
            index.println("address\tname\tstatus");
            for (Map.Entry<Long, String> entry : targets.entrySet()) {
                Address address = toAddr(entry.getKey());
                Function function = getFunctionAt(address);
                if (function == null) {
                    Function containing = getFunctionContaining(address);
                    if (containing != null) {
                        index.printf("%s\t%s\tinside_existing_function_%s%n", address, entry.getValue(), containing.getEntryPoint());
                        continue;
                    }
                    disassemble(address);
                    function = createFunction(address, entry.getValue());
                }
                if (function == null) { index.printf("%s\t%s\tcreate_failed%n", address, entry.getValue()); continue; }
                function.setName(entry.getValue(), SourceType.USER_DEFINED);
                index.printf("%s\t%s\tready%n", address, entry.getValue());
            }
        }
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
                status.printf("%s\t%s\t%s\t%s%n", address, entry.getValue(), ok, error == null ? "" : error.replace('\n', ' '));
                if (ok) {
                    String heading = "/* Ghidra output, not original source. Inferred types and names may be wrong.\n"
                        + " * TESORO.EXE SHA256: " + expectedHash + "\n * Entry: " + address + "\n */\n\n";
                    Files.write(new File(output, stem + ".raw.c").toPath(),
                        (heading + result.getDecompiledFunction().getC()).getBytes(StandardCharsets.UTF_8));
                    complete++;
                }
                try (PrintWriter assembly = new PrintWriter(new File(output, stem + ".asm"), "UTF-8")) {
                    InstructionIterator instructions = currentProgram.getListing().getInstructions(function.getBody(), true);
                    while (instructions.hasNext()) { Instruction in = instructions.next(); assembly.printf("%s  %s%n", in.getAddress(), in); }
                }
                try (PrintWriter refs = new PrintWriter(new File(output, stem + ".xrefs.tsv"), "UTF-8")) {
                    refs.println("from\tto\ttype");
                    ReferenceIterator references = currentProgram.getReferenceManager().getReferencesTo(address);
                    while (references.hasNext()) { Reference r = references.next(); refs.printf("%s\t%s\t%s%n", r.getFromAddress(), r.getToAddress(), r.getReferenceType()); }
                }
            }
        } finally { decompiler.dispose(); }
        println("TESORO_TARGETS_DECOMPILED=" + complete + "/" + targets.size());
    }
}
