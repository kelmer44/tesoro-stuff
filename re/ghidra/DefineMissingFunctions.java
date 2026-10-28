// Phase 1 of the Tesoro restart: define functions Ghidra missed (virtual methods, table callbacks, unreached code).
// Input: a text file with one hex address per line (re/new_functions.txt). Creates a function at each address that
// is not already one, then follows direct calls from the new functions and defines their targets too.
// Output: a log file next to the input (re/define_missing_functions.log).
// @category Tesoro
import ghidra.app.script.GhidraScript;
import ghidra.program.model.address.Address;
import ghidra.program.model.listing.Function;
import ghidra.program.model.listing.Instruction;
import ghidra.program.model.symbol.FlowType;
import java.io.*;
import java.util.*;

public class DefineMissingFunctions extends GhidraScript {
	@Override
	protected void run() throws Exception {
		File in = new File("/Users/gabriel/Desktop/source/tesoro/re/new_functions.txt");
		if (!in.exists()) in = askFile("Function start list", "Open");
		File logFile = new File(in.getParentFile(), "define_missing_functions.log");
		PrintWriter log = new PrintWriter(new FileWriter(logFile));
		int before = currentProgram.getFunctionManager().getFunctionCount();
		Deque<Address> todo = new ArrayDeque<>();
		try (BufferedReader r = new BufferedReader(new FileReader(in))) {
			String line;
			while ((line = r.readLine()) != null) {
				line = line.trim();
				if (!line.isEmpty()) todo.add(toAddr("0x" + line));
			}
		}
		Set<Address> created = new TreeSet<>();
		int already = 0, failed = 0, viaCalls = 0;
		while (!todo.isEmpty()) {
			Address a = todo.poll();
			if (getFunctionContaining(a) != null) {
				Function f = getFunctionAt(a);
				if (f == null) log.println("INSIDE   " + a + " (inside " + getFunctionContaining(a).getEntryPoint() + ")");
				else already++;
				continue;
			}
			disassemble(a);
			Function f = createFunction(a, null);
			if (f == null) { failed++; log.println("FAILED   " + a); continue; }
			created.add(a);
			log.println("CREATED  " + a + " " + f.getName() + " size " + f.getBody().getNumAddresses());
			for (Address ia : f.getBody().getAddresses(true)) {
				Instruction i = getInstructionAt(ia);
				if (i == null) continue;
				FlowType ft = i.getFlowType();
				if (ft.isCall() && !ft.isComputed()) {
					for (Address t : i.getFlows())
						if (getFunctionAt(t) == null && getFunctionContaining(t) == null && !todo.contains(t)) { todo.add(t); viaCalls++; }
				}
			}
		}
		int after = currentProgram.getFunctionManager().getFunctionCount();
		String s = "functions before " + before + ", after " + after + ", created " + created.size()
			+ ", already existing " + already + ", failed " + failed + ", found via calls " + viaCalls;
		log.println(s);
		log.close();
		println(s);
		println("log: " + logFile);
	}
}
