// Builds ../tesoro_talk.pptx. Run: node build_deck.js
const path = require("path");
const pptxgen = require("pptxgenjs");
const React = require("react");
const ReactDOMServer = require("react-dom/server");
const sharp = require("sharp");
const fa = require("react-icons/fa");

const SKILL_DIR = process.env.PPTX_SKILL_DIR ||
  "/Users/gabriel/Library/Application Support/Claude/local-agent-mode-sessions/skills-plugin/2751e9c9-ae29-4a7d-8936-3e88a619981a/eb4fc988-ec22-468a-94f8-f32c965d6c52/skills/pptx";
const { applyTheme } = require(path.join(SKILL_DIR, "scripts/apply_theme.js"));

const OUT = path.join(__dirname, "..", "tesoro_talk.pptx");
const MONO = "Courier New";

// Palette: ink and artichoke green with treasure gold.
const HEX = {
  ink: "14232A", white: "FFFFFF", green: "3D5A47", tint: "EEF2EC",
  gold: "D9A92E", leaf: "6B9A55", red: "C8553D", teal: "2E7D8C", grey: "5E6B70", code: "D8E2DC",
};
const THEME = {
  name: "Tesoro Talk",
  headFontFace: "Cambria",
  bodyFontFace: "Calibri",
  colors: {
    dk1: HEX.ink, lt1: HEX.white, dk2: HEX.green, lt2: HEX.tint,
    accent1: HEX.gold, accent2: HEX.leaf, accent3: HEX.red,
    accent4: HEX.teal, accent5: HEX.grey, accent6: "5B4B8A",
    hlink: HEX.teal, folHlink: HEX.grey,
  },
};

const pres = new pptxgen();
pres.layout = "LAYOUT_16x9"; // 10 x 5.625 in
pres.theme = { headFontFace: THEME.headFontFace, bodyFontFace: THEME.bodyFontFace };
pres.title = "Reading a program with no source";
const C = pres.SchemeColor;

pres.defineSlideMaster({
  title: "DARK",
  background: { color: C.text1 },
  slideNumber: { x: 9.0, y: 5.2, w: 0.6, h: 0.3, fontSize: 10, color: C.accent5 },
  objects: [
    { placeholder: { options: { name: "title", type: "title", x: 0.5, y: 0.35, w: 9, h: 1.0, fontSize: 34, bold: true, color: C.background1, margin: 0, valign: "middle", align: "left" }, text: "" } },
  ],
});
pres.defineSlideMaster({
  title: "LIGHT",
  background: { color: C.background1 },
  slideNumber: { x: 9.0, y: 5.2, w: 0.6, h: 0.3, fontSize: 10, color: C.accent5 },
  objects: [
    { placeholder: { options: { name: "title", type: "title", x: 0.5, y: 0.3, w: 8.0, h: 0.8, fontSize: 34, bold: true, color: C.text1, margin: 0, valign: "middle", align: "left" }, text: "" } },
  ],
});

async function icon(Comp, hex, size = 256) {
  const svg = ReactDOMServer.renderToStaticMarkup(React.createElement(Comp, { color: "#" + hex, size: String(size) }));
  const buf = await sharp(Buffer.from(svg)).png().toBuffer();
  return "image/png;base64," + buf.toString("base64");
}

// ---------- helpers (each returns fresh option objects) ----------
function begin(section, layout, title, notes) {
  const s = pres.addSlide({ masterName: layout, sectionTitle: section });
  s.addText(title, { placeholder: "title" });
  if (notes) s.addNotes(notes);
  return s;
}
function chip(s, text, fill) {
  s.addText(text, {
    x: 7.7, y: 0.42, w: 1.8, h: 0.36, shape: pres.shapes.ROUNDED_RECTANGLE, rectRadius: 0.08,
    fill: { color: fill || C.accent1 }, color: C.text1, fontSize: 12, bold: true, align: "center", valign: "middle",
    margin: 0, isTextBox: true, objectName: "Chip",
  });
}
// A dark code panel; lines is an array of strings or arrays of {text, color} runs.
function code(s, x, y, w, h, lines, fontSize, name) {
  const runs = [];
  lines.forEach((ln, i) => {
    const parts = typeof ln === "string" ? [{ text: ln }] : ln;
    parts.forEach((p, j) => {
      runs.push({ text: p.text, options: { color: p.color || HEX.code, breakLine: j === parts.length - 1 && i < lines.length - 1 } });
    });
  });
  s.addText(runs, {
    x, y, w, h, shape: pres.shapes.ROUNDED_RECTANGLE, rectRadius: 0.08, fill: { color: C.text1 },
    fontFace: MONO, fontSize: fontSize || 12, valign: "top", margin: 0.15, isTextBox: true, objectName: name || "Code",
  });
}
function card(s, x, y, w, h, name) {
  s.addShape(pres.shapes.ROUNDED_RECTANGLE, { x, y, w, h, rectRadius: 0.1, fill: { color: C.background2 }, line: { color: C.background2, width: 0.5 }, objectName: name || "Card" });
}
function text(s, t, o) {
  s.addText(t, Object.assign({ isTextBox: true, margin: 0, valign: "top", fontSize: 16, color: C.text1 }, o));
}
function arrow(s, x, y, w, h, opts) {
  s.addShape(pres.shapes.LINE, Object.assign({ x, y, w, h, line: { color: C.accent5, width: 2, endArrowType: "triangle" } }, opts));
}

(async () => {
  const icons = {
    plug: await icon(fa.FaPlug, HEX.white),
    museum: await icon(fa.FaLandmark, HEX.white),
    shield: await icon(fa.FaShieldAlt, HEX.white),
    puzzle: await icon(fa.FaPuzzlePiece, HEX.white),
    search: await icon(fa.FaSearch, HEX.white),
    rename: await icon(fa.FaTag, HEX.white),
    play: await icon(fa.FaPlay, HEX.white),
    list: await icon(fa.FaListUl, HEX.white),
    check: await icon(fa.FaCheck, HEX.white),
    bug: await icon(fa.FaBug, HEX.white),
  };
  const iconCircle = (s, img, x, y, d, fill) => {
    s.addShape(pres.shapes.OVAL, { x, y, w: d, h: d, fill: { color: fill || C.text2 }, line: { color: fill || C.text2, width: 0 }, objectName: "IconCircle" });
    s.addImage({ data: img, x: x + d * 0.25, y: y + d * 0.25, w: d * 0.5, h: d * 0.5, altText: "icon" });
  };

  // ================= Part 0: Hook =================
  pres.addSection({ title: "Hook" });

  // 1 Title
  {
    const s = begin("Hook", "DARK", "Reading a program with no source",
      "One line on what you do at work, then: this is a side project that taught me a lot.");
    text(s, "Reverse engineering by hand, with Ghidra, and with AI agents", { x: 0.5, y: 1.6, w: 8, h: 0.9, fontSize: 22, color: C.accent1 });
    text(s, "Gabriel", { x: 0.5, y: 2.7, w: 6, h: 0.4, fontSize: 16, color: C.background2 });
    const bytes = ["55", "89", "E5", "83", "EC", "08", "8B", "45", "08", "C7", "45", "FC", "00", "00", "00"];
    bytes.forEach((b, i) => {
      s.addText(b, {
        x: 0.5 + i * 0.6, y: 4.3, w: 0.5, h: 0.5, shape: pres.shapes.ROUNDED_RECTANGLE, rectRadius: 0.06,
        fill: { color: i % 3 === 0 ? C.accent1 : C.text2 }, color: i % 3 === 0 ? C.text1 : C.background1,
        fontFace: MONO, fontSize: 14, bold: true, align: "center", valign: "middle", margin: 0, isTextBox: true, objectName: "HexChip",
      });
    });
  }

  // 2 The goal
  {
    const s = begin("Hook", "LIGHT", "The goal",
      "A 1990s adventure game. I want it to run in ScummVM, faithfully. All I have is the executable and the disc.");
    s.addShape(pres.shapes.ROUNDED_RECTANGLE, { x: 0.5, y: 1.4, w: 5.0, h: 3.5, rectRadius: 0.1, fill: { color: C.text1 }, objectName: "ScreenshotPlaceholder" });
    text(s, "Screenshot or clip:\nEl Tesoro de Isla Alcachofa\nrunning in ScummVM", { x: 0.7, y: 2.7, w: 4.6, h: 1.0, fontSize: 18, color: C.background2, align: "center" });
    card(s, 5.8, 1.4, 3.7, 1.6, "HaveCard");
    text(s, "Have", { x: 6.0, y: 1.5, w: 3.3, h: 0.35, fontSize: 18, bold: true, color: C.text2 });
    text(s, "TESORO.EXE and the game disc", { x: 6.0, y: 1.95, w: 3.3, h: 0.9, fontSize: 16 });
    card(s, 5.8, 3.3, 3.7, 1.6, "LackCard");
    text(s, "Don't have", { x: 6.0, y: 3.4, w: 3.3, h: 0.35, fontSize: 18, bold: true, color: C.accent3 });
    text(s, "Source code, documentation, or the original developers", { x: 6.0, y: 3.85, w: 3.3, h: 0.9, fontSize: 16 });
  }

  // 3 The obvious approach (joke)
  {
    const s = begin("Hook", "DARK", "The obvious approach",
      "Pause a beat. Don't explain the joke.");
    s.addText("Hello Claude, implement this game in one shot, no errors.", {
      x: 0.8, y: 1.7, w: 7.0, h: 1.2, shape: pres.shapes.ROUNDED_RECTANGLE, rectRadius: 0.2,
      fill: { color: C.text2 }, color: C.background1, fontSize: 24, margin: 0.25, valign: "middle", isTextBox: true, objectName: "ChatBubble",
    });
    s.addShape(pres.shapes.OVAL, { x: 7.9, y: 1.6, w: 1.4, h: 1.4, fill: { color: C.accent3 }, line: { color: C.accent3, width: 0 }, objectName: "CrossCircle" });
    text(s, "X", { x: 7.9, y: 1.6, w: 1.4, h: 1.4, fontSize: 60, bold: true, color: C.background1, align: "center", valign: "middle" });
    text(s, "Narrator: it did not work.", { x: 0.8, y: 3.7, w: 8.4, h: 0.6, fontSize: 26, italic: true, color: C.accent1 });
  }

  // 4 Why that fails
  {
    const s = begin("Hook", "LIGHT", "Why that fails",
      "Close with: so what does a reliable process look like? That is the talk.");
    const rows = [
      ["The model can't see the binary", "So it guesses, and plausible guesses about game behaviour are often wrong."],
      ["Nothing is checked", "Mistakes compound silently."],
      ["No process", "No way to know what is done or what is trusted."],
    ];
    rows.forEach((r, i) => {
      const y = 1.4 + i * 1.2;
      s.addShape(pres.shapes.OVAL, { x: 0.6, y: y + 0.1, w: 0.7, h: 0.7, fill: { color: C.text2 }, line: { color: C.text2, width: 0 }, objectName: "NumCircle" });
      text(s, String(i + 1), { x: 0.6, y: y + 0.1, w: 0.7, h: 0.7, fontSize: 24, bold: true, color: C.background1, align: "center", valign: "middle" });
      text(s, r[0], { x: 1.6, y, w: 7.8, h: 0.45, fontSize: 22, bold: true });
      text(s, r[1], { x: 1.6, y: y + 0.45, w: 7.8, h: 0.6, fontSize: 16, color: C.accent5 });
    });
  }

  // ================= Part 1: What is RE =================
  pres.addSection({ title: "What is reverse engineering" });

  // 5 Definition and uses
  {
    const s = begin("What is reverse engineering", "LIGHT", "What is reverse engineering?",
      "One legal and ethical line: I own the game, the goal is preservation. Name one work-relevant example: an undocumented file format, a legacy system, a vendor library.");
    text(s, "Recovering how something works from the finished product.", { x: 0.5, y: 1.3, w: 9, h: 0.6, fontSize: 22, italic: true, color: C.text2 });
    const items = [
      [icons.plug, "Interoperability", "Talk to a system with no spec"],
      [icons.museum, "Preservation", "Keep old software alive"],
      [icons.shield, "Security", "Understand malware and flaws"],
      [icons.puzzle, "Compatibility", "Replace a component safely"],
    ];
    items.forEach((it, i) => {
      const x = 0.5 + i * 2.3;
      card(s, x, 2.3, 2.1, 2.6, "UseCard");
      iconCircle(s, it[0], x + 0.7, 2.5, 0.7);
      text(s, it[1], { x: x + 0.1, y: 3.35, w: 1.9, h: 0.4, fontSize: 16, bold: true, align: "center" });
      text(s, it[2], { x: x + 0.15, y: 3.8, w: 1.8, h: 0.9, fontSize: 14, color: C.accent5, align: "center" });
    });
  }

  // 6 Pipeline backwards
  {
    const s = begin("What is reverse engineering", "LIGHT", "The pipeline, run backwards",
      "The compiler throws information away: names, types, comments, structure. Everything after this is getting some of it back.");
    const boxes = ["Source code", "Compiler", "Machine code", "CPU"];
    boxes.forEach((b, i) => {
      const x = 0.5 + i * 2.35;
      const hi = i === 2;
      s.addText(b, {
        x, y: 1.7, w: 1.9, h: 1.0, shape: pres.shapes.ROUNDED_RECTANGLE, rectRadius: 0.1,
        fill: { color: hi ? C.accent1 : C.text2 }, color: hi ? C.text1 : C.background1,
        fontSize: 18, bold: true, align: "center", valign: "middle", margin: 0.05, isTextBox: true, objectName: "PipelineBox",
      });
      if (i < 3) arrow(s, x + 1.95, 2.2, 0.35, 0, {});
    });
    text(s, "what we have", { x: 0.5 + 2 * 2.35, y: 2.85, w: 1.9, h: 0.35, fontSize: 14, bold: true, color: C.accent1, align: "center" });
    arrow(s, 5.2, 3.9, 4.2, 0, { flipH: true, line: { color: C.accent3, width: 3, endArrowType: "triangle" } });
    text(s, "we work in this direction, recovering meaning", { x: 0.5, y: 4.15, w: 9, h: 0.5, fontSize: 18, color: C.accent3, align: "right" });
  }

  // ================= Part 2: Act 1 =================
  pres.addSection({ title: "Act 1: by hand" });

  // 7 DEMO 1
  {
    const s = begin("Act 1: by hand", "LIGHT", "Bytes and opcodes",
      "Type the function live (5 lines) into live/sum_to.c. Compile it for the Mac with ./build_native.sh and run ./build/native_demo 10 to get 55, a known answer. Then say why the next build is different: the game is 32-bit x86 Windows, so I compile the same C again for x86-win32 with ./build.sh. This Mac is arm64, so these are completely different bytes and the Mac cannot run them; file shows the difference (arm64 executable versus plain data). If asked why Python makes the .bin: it just cuts the code bytes out of the object file, as bare as a dump from a real program (objdump -h shows .text is 59 bytes). Show the bytes with xxd -g1, then explain why hex: one byte is exactly two hex digits (4 bits each), binary is unreadable and decimal does not line up with bits. Open the .bin in ImHex, which can colour and label bytes and edit them in place. Decode a few bytes by hand with the one-byte opcode map (80386 Programmer's Reference, Appendix A): high four bits pick the row, low four bits the column. Run the exact bytes in the emulator with an audience-chosen n. Then patch the copy: byte 0x1d (7f to 7d) turns i <= n into i < n, so the answer for 10 becomes 45; byte 0x2d (01 to 02) makes i++ into i += 2, so it becomes 25. This is real, and it is miserable. If short on time, drop the jump-offset arithmetic (0x1f + 0x14 = 0x33). The code panel on the slide is a reference for the audience; the real thing is typed on screen.");
    chip(s, "LIVE DEMO 1");
    code(s, 0.5, 1.35, 4.5, 2.4, [
      "int sum_to(int n)",
      "{",
      "  int total = 0;",
      "  for (int i = 1; i <= n; i++)",
      "    total += i;",
      "  return total;",
      "}",
    ], 15, "SourceCode");
    text(s, "Mac build first, then 32-bit x86", { x: 0.5, y: 3.9, w: 4.5, h: 0.35, fontSize: 14, color: C.accent5 });
    text(s, "59 bytes at -O0, 20 instructions", { x: 0.5, y: 4.3, w: 4.5, h: 0.4, fontSize: 18, bold: true, color: C.text2 });
    const hdr = (t) => ({ text: t, options: { bold: true, color: C.background1, fill: { color: C.text2 }, fontSize: 13 } });
    const cell = (t, mono) => ({ text: t, options: { fontSize: 13, color: C.text1, fontFace: mono ? MONO : undefined } });
    s.addTable([
      [hdr("Offset"), hdr("Bytes"), hdr("Meaning")],
      [cell("0x00", 1), cell("55", 1), cell("push ebp (start)")],
      [cell("0x03", 1), cell("83 ec 08", 1), cell("sub esp, 8")],
      [cell("0x1a", 1), cell("3b 45 08", 1), cell("cmp eax, [ebp+8]")],
      [cell("0x1d", 1), cell("7f 14", 1), cell("jg +0x14 (exit loop)")],
      [cell("0x31", 1), cell("eb e4", 1), cell("jmp back to the test")],
      [cell("0x3a", 1), cell("c3", 1), cell("ret")],
    ], { x: 5.3, y: 1.35, w: 4.2, colW: [0.8, 1.1, 2.3], rowH: 0.46, border: { type: "solid", pt: 0.5, color: "D5DDD2" }, fill: { color: C.background2 }, valign: "middle", margin: [0, 0.08, 0, 0.08] });
    text(s, "Patch one byte, run it again: 55 becomes 45 or 25", { x: 5.3, y: 4.7, w: 4.2, h: 0.3, fontSize: 12, color: C.accent5 });
  }

  // 8 Let tools decode
  {
    const s = begin("Act 1: by hand", "LIGHT", "Let a tool do the decoding",
      "A disassembler only translates bytes to mnemonics. It adds no meaning. Same bytes on both sides.");
    code(s, 0.5, 1.4, 4.7, 2.0, [
      [{ text: "00000000: ", color: "8FA39A" }, { text: "55 89 e5 83 ec 08 8b 45", color: HEX.gold }],
      [{ text: "00000008: ", color: "8FA39A" }, { text: "08 c7 45 fc 00 00 00 00", color: HEX.gold }],
      [{ text: "00000010: ", color: "8FA39A" }, { text: "c7 45 f8 01 00 00 00 8b", color: HEX.gold }],
      [{ text: "00000018: ", color: "8FA39A" }, { text: "45 f8 3b 45 08 7f 14 8b", color: HEX.gold }],
    ], 13, "HexDump");
    text(s, "xxd -g1 -c8: just the bytes", { x: 0.5, y: 3.5, w: 4.7, h: 0.35, fontSize: 14, color: C.accent5 });
    code(s, 5.5, 1.4, 4.0, 2.9, [
      "push ebp",
      "mov  ebp, esp",
      "sub  esp, 0x8",
      "mov  eax, [ebp+0x8]",
      "mov  [ebp-0x4], 0x0",
      "mov  [ebp-0x8], 0x1",
      "mov  eax, [ebp-0x8]",
      "cmp  eax, [ebp+0x8]",
      "jg   0x33",
    ], 13, "Disassembly");
    text(s, "objdump: the same bytes as instructions", { x: 5.5, y: 4.4, w: 4.0, h: 0.35, fontSize: 14, color: C.accent5 });
    s.addShape(pres.shapes.LINE, { x: 5.25, y: 2.4, w: 0.2, h: 0, line: { color: C.accent1, width: 3, endArrowType: "triangle" } });
  }

  // 9 Compiler reveal
  {
    const s = begin("Act 1: by hand", "LIGHT", "The compiler reveal",
      "Same five lines of C. At -O2 the loop is gone: the compiler worked out the sum is a formula (multiply, halve, add) and replaced the loop with it. The formula is n(n+1)/2: for n = 10 that is 55. For n = 1000 the unoptimised code runs 10,013 instructions and the optimised code runs 10, with the same answer. Keep in mind the test and jump at the top of the -O2 code: it returns 0 for n <= 0. That comes back in Act 3. Optimisers are why reverse engineering is not just reading.");
    card(s, 0.5, 1.4, 4.2, 2.6, "O0Card");
    text(s, "59", { x: 0.5, y: 1.55, w: 4.2, h: 1.4, fontSize: 80, bold: true, color: C.text2, align: "center" });
    text(s, "bytes at -O0", { x: 0.5, y: 2.9, w: 4.2, h: 0.4, fontSize: 20, align: "center" });
    text(s, "20 instructions, the loop you wrote", { x: 0.5, y: 3.4, w: 4.2, h: 0.4, fontSize: 14, color: C.accent5, align: "center" });
    card(s, 5.3, 1.4, 4.2, 2.6, "O2Card");
    text(s, "28", { x: 5.3, y: 1.55, w: 4.2, h: 1.4, fontSize: 80, bold: true, color: C.accent3, align: "center" });
    text(s, "bytes at -O2", { x: 5.3, y: 2.9, w: 4.2, h: 0.4, fontSize: 20, align: "center" });
    text(s, "12 instructions, no loop at all", { x: 5.3, y: 3.4, w: 4.2, h: 0.4, fontSize: 14, color: C.accent5, align: "center" });
    text(s, "n(n+1)/2", { x: 5.3, y: 3.7, w: 4.2, h: 0.3, fontSize: 13, bold: true, color: C.accent3, align: "center", fontFace: MONO });
    text(s, "Same answer. For n = 1000: 10,013 instructions versus 10.", { x: 0.5, y: 4.3, w: 9, h: 0.5, fontSize: 20, bold: true, color: C.text2, align: "center" });
  }

  // 10 What is lost
  {
    const s = begin("Act 1: by hand", "LIGHT", "What the compiler throws away",
      "The whole job is recovering meaning from behaviour.");
    card(s, 0.5, 1.4, 4.4, 3.5, "LostCard");
    text(s, "Lost", { x: 0.75, y: 1.55, w: 3.9, h: 0.45, fontSize: 24, bold: true, color: C.accent3 });
    text(s, [
      { text: "Names", options: { bullet: true, breakLine: true } },
      { text: "Types and structs", options: { bullet: true, breakLine: true } },
      { text: "Comments", options: { bullet: true, breakLine: true } },
      { text: "Loop and branch shapes", options: { bullet: true } },
    ], { x: 0.75, y: 2.15, w: 3.9, h: 2.5, fontSize: 18, paraSpaceAfter: 8 });
    card(s, 5.1, 1.4, 4.4, 3.5, "KeptCard");
    text(s, "Kept", { x: 5.35, y: 1.55, w: 3.9, h: 0.45, fontSize: 24, bold: true, color: C.text2 });
    text(s, [
      { text: "Behaviour", options: { bullet: true, breakLine: true } },
      { text: "Calling convention", options: { bullet: true, breakLine: true } },
      { text: "Constants and strings", options: { bullet: true, breakLine: true } },
      { text: "Who calls whom", options: { bullet: true } },
    ], { x: 5.35, y: 2.15, w: 3.9, h: 2.5, fontSize: 18, paraSpaceAfter: 8 });
  }

  // ================= Part 3: Act 2 =================
  pres.addSection({ title: "Act 2: Ghidra" });

  // 11 DEMO 2
  {
    const s = begin("Act 2: Ghidra", "LIGHT", "The same function in Ghidra",
      "Import the raw bytes of your own function with no symbols, create the function by hand with D then F, and say: in a real EXE Ghidra does that step itself. Also say you only choose the CPU and find the function by hand because a raw file has no header; a real EXE tells Ghidra the CPU and the entry point. Ghidra gives control flow, cross-references and a C-like view. Naming is still on you. Watch renames propagate through callers. The default names on this slide are from memory of how Ghidra labels things; swap the panels for a real screenshot of the decompiler once you have rehearsed it.");
    chip(s, "LIVE DEMO 2");
    text(s, "Ghidra's default names", { x: 0.5, y: 1.35, w: 4.3, h: 0.35, fontSize: 16, bold: true, color: C.accent5 });
    code(s, 0.5, 1.8, 4.0, 2.6, ["FUN_00000000", "param_1", "local_8", "local_c"], 16, "BeforeNames");
    arrow(s, 4.65, 3.1, 0.7, 0, { line: { color: C.accent1, width: 3, endArrowType: "triangle" } });
    text(s, "After you rename", { x: 5.5, y: 1.35, w: 4.0, h: 0.35, fontSize: 16, bold: true, color: C.text2 });
    code(s, 5.5, 1.8, 4.0, 2.6, [
      [{ text: "sum_to", color: HEX.gold }],
      [{ text: "n", color: HEX.gold }],
      [{ text: "total", color: HEX.gold }],
      [{ text: "i", color: HEX.gold }],
    ], 16, "AfterNames");
    text(s, "Same logic. Meaning is added by a person (or an agent).", { x: 0.5, y: 4.6, w: 9, h: 0.4, fontSize: 18, bold: true, color: C.text2 });
  }

  // 12 Scale
  {
    const s = begin("Act 2: Ghidra", "LIGHT", "The scale problem",
      "Switch to the real TESORO.EXE project: function list, a short function with its callers, call graph, strings and imports, all found by Ghidra itself. Caveat: Ghidra finds functions by following calls, so code reached only through function-pointer tables is invisible. A table of function pointers is a list of function addresses stored as data that the program looks up and calls, like verbs[choice](); there is no call to the function by name, so Ghidra does not see it. C++ virtual methods are the same thing built in (a hidden vtable per class). Its count went from 622 to 784 after 161 were defined with a script (re/PLAN.md). Doing the rest by hand is weeks of tedium per subsystem. One person is the bottleneck. These counts come from re/ledger.json; refresh before the talk.");
    card(s, 0.5, 1.4, 4.2, 3.2, "TotalCard");
    text(s, "706", { x: 0.5, y: 1.55, w: 4.2, h: 1.6, fontSize: 88, bold: true, color: C.text2, align: "center" });
    text(s, "functions in TESORO.EXE", { x: 0.5, y: 3.2, w: 4.2, h: 0.5, fontSize: 20, align: "center" });
    card(s, 5.1, 1.4, 4.4, 1.5, "GameCard");
    text(s, "447", { x: 5.3, y: 1.55, w: 1.6, h: 1.2, fontSize: 48, bold: true, color: C.accent1, valign: "middle" });
    text(s, "game code, the part that matters", { x: 6.9, y: 1.55, w: 2.5, h: 1.2, fontSize: 16, valign: "middle" });
    card(s, 5.1, 3.1, 4.4, 1.5, "LibCard");
    text(s, "259", { x: 5.3, y: 3.25, w: 1.6, h: 1.2, fontSize: 48, bold: true, color: C.accent5, valign: "middle" });
    text(s, "runtime and library code", { x: 6.9, y: 3.25, w: 2.5, h: 1.2, fontSize: 16, valign: "middle" });
  }

  // 13 Decompiler is a hypothesis
  {
    const s = begin("Act 2: Ghidra", "LIGHT", "Decompiler output is a guess",
      "It is excellent, but it is guessing. So how do you know a reading is right? That sets up Act 3.");
    text(s, "Decompiler output is not the truth.", { x: 0.5, y: 1.3, w: 9, h: 0.5, fontSize: 22, italic: true, color: C.text2 });
    const items = [
      ["Wrong types", "An int that is really a pointer or a short"],
      ["Wrong signedness", "A compare that is unsigned in the original"],
      ["Wrong calling convention", "Arguments in registers, not on the stack"],
    ];
    items.forEach((it, i) => {
      const x = 0.5 + i * 3.1;
      card(s, x, 2.1, 2.9, 2.5, "RiskCard");
      text(s, it[0], { x: x + 0.2, y: 2.3, w: 2.5, h: 0.8, fontSize: 20, bold: true, color: C.accent3 });
      text(s, it[1], { x: x + 0.2, y: 3.2, w: 2.5, h: 1.2, fontSize: 16 });
    });
  }

  // ================= Part 4: Act 3 =================
  pres.addSection({ title: "Act 3: verification" });

  // 14 Unicorn
  {
    const s = begin("Act 3: verification", "LIGHT", "Run the original bytes",
      "Unicorn is a CPU emulator: unit tests for a binary you have no source for. Load one function at its original address, build the stack by hand, run it, read the result. (Optional aside: Capstone, a disassembler library, is its scriptable companion. Ghidra is for humans, Capstone for scripts.)");
    const steps = [
      ["Map the bytes", "Load the function at its original address"],
      ["Build the stack", "The argument n, then a return address"],
      ["Run and read", "Execute until return, read the result from eax"],
    ];
    steps.forEach((st, i) => {
      const y = 1.4 + i * 1.15;
      s.addShape(pres.shapes.OVAL, { x: 0.5, y: y + 0.1, w: 0.6, h: 0.6, fill: { color: C.text2 }, line: { color: C.text2, width: 0 }, objectName: "NumCircle" });
      text(s, String(i + 1), { x: 0.5, y: y + 0.1, w: 0.6, h: 0.6, fontSize: 22, bold: true, color: C.background1, align: "center", valign: "middle" });
      text(s, st[0], { x: 1.3, y, w: 3.7, h: 0.4, fontSize: 20, bold: true });
      text(s, st[1], { x: 1.3, y: y + 0.42, w: 3.7, h: 0.6, fontSize: 14, color: C.accent5 });
    });
    code(s, 5.3, 1.4, 4.2, 3.4, [
      "uc.mem_write(CODE, code)",
      "",
      "# cdecl: push args,",
      "# then the return address",
      "for v in reversed(args):",
      "    esp -= 4",
      "    uc.mem_write(esp, v)",
      "",
      "uc.emu_start(CODE, RET)",
      "eax = uc.reg_read(EAX)",
    ], 13, "UnicornCode");
  }

  // 15 DEMO 3
  {
    const s = begin("Act 3: verification", "LIGHT", "A plausible claim, tested",
      "Type the claims live. First sum(range(1, n+1)) mirrors the loop and matches everywhere. Then the clever one, n*(n+1)//2: it looks like what the optimised code does, passes every non-negative input, and fails on negative n (n=-2: the real code returns 0, the formula says 1). The optimised code had the guard (test and jle at the top) all along. Finish live by typing the corrected claim, n*(n+1)//2 if n > 0 else 0, which matches 133/133 (this step is not on the slide): the compiler wrote an if n <= 0 return 0 guard and then the formula. This is what a confident AI explanation looks like.");
    chip(s, "LIVE DEMO 3");
    const G = "7FD18B", R = "FF8A75", D = "8FA39A";
    code(s, 0.5, 1.35, 9.0, 3.0, [
      [{ text: "$ verify.py sum_to_O2.bin --expr \"sum(range(1, n+1))\"", color: D }],
      [{ text: "133/133 inputs match", color: G }],
      "",
      [{ text: "$ verify.py ... --expr \"n*(n+1)//2\" --happy-path-only", color: D }],
      [{ text: "103/103 inputs match", color: G }, { text: "    (false confidence)", color: HEX.gold }],
      "",
      [{ text: "$ verify.py ... --expr \"n*(n+1)//2\"", color: D }],
      [{ text: "104/133 inputs match", color: R }],
      [{ text: "MISMATCH n=-2: emulator=0 hypothesis=1", color: R }],
    ], 14, "Terminal");
    text(s, "Happy-path tests pass. Negative inputs expose it.", { x: 0.5, y: 4.55, w: 9, h: 0.4, fontSize: 20, bold: true, color: C.text2 });
  }

  // 16 Oracle and limits
  {
    const s = begin("Act 3: verification", "LIGHT", "The oracle, and its limits",
      "A hypothesis is only as good as the inputs you tested it with. Pick edge cases deliberately. This is evidence, not proof.");
    card(s, 0.5, 1.4, 4.4, 3.5, "StrengthCard");
    text(s, "Strengths", { x: 0.75, y: 1.55, w: 3.9, h: 0.45, fontSize: 24, bold: true, color: C.text2 });
    text(s, [
      { text: "Runs the real bytes", options: { bullet: true, breakLine: true } },
      { text: "Deterministic", options: { bullet: true, breakLine: true } },
      { text: "Scales to many inputs", options: { bullet: true } },
    ], { x: 0.75, y: 2.15, w: 3.9, h: 2.5, fontSize: 18, paraSpaceAfter: 8 });
    card(s, 5.1, 1.4, 4.4, 3.5, "LimitCard");
    text(s, "Limits", { x: 5.35, y: 1.55, w: 3.9, h: 0.45, fontSize: 24, bold: true, color: C.accent3 });
    text(s, [
      { text: "Only the inputs you choose", options: { bullet: true, breakLine: true } },
      { text: "OS and hardware calls need stubs", options: { bullet: true, breakLine: true } },
      { text: "Evidence, not proof", options: { bullet: true } },
    ], { x: 5.35, y: 2.15, w: 3.9, h: 2.5, fontSize: 18, paraSpaceAfter: 8 });
  }

  // ================= Part 5: Act 4 =================
  pres.addSection({ title: "Act 4: AI agents" });

  // 17 What the agent can touch
  {
    const s = begin("Act 4: AI agents", "LIGHT", "What the agent can touch",
      "MCP gives the model hands in Ghidra. It reads real decompilation instead of guessing from a description.");
    const tools = [
      ["decompile_function", "Read the C view of one function"],
      ["disassemble_function", "Read the exact instructions"],
      ["get_xrefs_to", "Find who calls it"],
      ["rename_function", "Record a name"],
      ["set_function_prototype", "Record a signature"],
    ];
    tools.forEach((t, i) => {
      const y = 1.35 + i * 0.7;
      s.addText(t[0], {
        x: 0.5, y, w: 4.3, h: 0.56, shape: pres.shapes.ROUNDED_RECTANGLE, rectRadius: 0.08, fill: { color: C.text1 },
        color: HEX.gold, fontFace: MONO, fontSize: 14, bold: true, valign: "middle", margin: 0.15, isTextBox: true, objectName: "ToolName",
      });
      text(s, t[1], { x: 5.1, y, w: 4.4, h: 0.56, fontSize: 18, valign: "middle" });
    });
  }

  // 18 The loop
  {
    const s = begin("Act 4: AI agents", "LIGHT", "The loop",
      "The AI proposes, the emulator checks, the ledger records. The model is boxed in by evidence. A mismatch sends you back to reading, not forward to the next function.");
    const node = (n, label, col, row, fill) => {
      const x = 0.5 + col * 3.2, y = 1.45 + row * 1.9;
      s.addText([{ text: n + "  ", options: { bold: true, color: C.accent1 } }, { text: label, options: { color: C.background1 } }], {
        x, y, w: 2.7, h: 1.1, shape: pres.shapes.ROUNDED_RECTANGLE, rectRadius: 0.1, fill: { color: fill || C.text2 },
        fontSize: 16, valign: "middle", align: "center", margin: 0.1, isTextBox: true, objectName: "LoopNode",
      });
    };
    node("1", "Pick next function by dependency", 0, 0);
    node("2", "Agent reads decompile, callers, callees", 1, 0);
    node("3", "Write contract. Unknown stays unknown", 2, 0);
    node("4", "Port to C++", 2, 1);
    node("5", "Run original bytes vs port", 1, 1, HEX.teal);
    node("6", "Record status in ledger", 0, 1);
    arrow(s, 3.25, 2.0, 0.45, 0, {});
    arrow(s, 6.45, 2.0, 0.45, 0, {});
    arrow(s, 8.25, 2.6, 0, 0.75, {});
    arrow(s, 6.45, 3.9, 0.45, 0, { flipH: true });
    arrow(s, 3.25, 3.9, 0.45, 0, { flipH: true });
    arrow(s, 1.85, 2.6, 0, 0.75, { flipV: true });
    text(s, "mismatch: back to step 2", { x: 3.5, y: 4.65, w: 3.4, h: 0.35, fontSize: 14, bold: true, color: C.accent3, align: "center" });
  }

  // 19 The artifacts
  {
    const s = begin("Act 4: AI agents", "LIGHT", "The artifacts",
      "Contract before code. Every field has a confidence marker. The ledger is how a new session, or a new agent, picks up with no memory.");
    const cols = [
      ["Ledger", "One row per function: name, status, callers, callees, notes.", icons.list],
      ["Contracts", "One note per function. What it does, and what is still unknown.", icons.check],
      ["Struct map", "Recovered layouts, one confidence marker per field.", icons.search],
    ];
    cols.forEach((c, i) => {
      const x = 0.5 + i * 3.1;
      card(s, x, 1.4, 2.9, 2.0, "ArtifactCard");
      iconCircle(s, c[2], x + 0.2, 1.55, 0.6);
      text(s, c[0], { x: x + 0.95, y: 1.55, w: 1.8, h: 0.6, fontSize: 20, bold: true, valign: "middle" });
      text(s, c[1], { x: x + 0.2, y: 2.3, w: 2.5, h: 1.0, fontSize: 14 });
    });
    code(s, 0.5, 3.6, 9.0, 1.3, [
      [{ text: "addr: ", color: "8FA39A" }, { text: "00401000", color: HEX.gold }, { text: "   name: ", color: "8FA39A" }, { text: "inferred_ReturnThis", color: HEX.gold }],
      [{ text: "status: ", color: "8FA39A" }, { text: "ported", color: HEX.gold }, { text: "   callers: ", color: "8FA39A" }, { text: "[00401aa0]", color: HEX.gold }, { text: "   callees: ", color: "8FA39A" }, { text: "[]", color: HEX.gold }],
    ], 14, "LedgerRow");
  }

  // 20 Status ladder
  {
    const s = begin("Act 4: AI agents", "LIGHT", "Evidence moves a function up",
      "Decompiled is not done. The last rung is seeing it in the running original, in DOSBox-X.");
    const steps = ["untouched", "decompiled", "understood", "ported", "emu-compared", "original-compared"];
    steps.forEach((st, i) => {
      const w = 1.4, gap = 0.1, x = 0.5 + i * (w + gap);
      const h = 0.9 + i * 0.45;
      const last = i === steps.length - 1;
      s.addText(st, {
        x, y: 4.9 - h, w, h, shape: pres.shapes.RECTANGLE, fill: { color: last ? C.accent1 : C.text2 },
        color: last ? C.text1 : C.background1, fontSize: 14, bold: true, align: "center", valign: "top", margin: [0.1, 0.05, 0, 0.05], isTextBox: true, objectName: "LadderStep",
      });
    });
  }

  // 21 Where it stands (native chart)
  {
    const s = begin("Act 4: AI agents", "LIGHT", "Where it stands",
      "The restart was 28 October; these numbers are from 5 November (re/SUBSYSTEMS.md). Show the honest limit as proudly as the headline: nothing is yet seen as a function in the running original. Two menu screens and a saved game have been compared.");
    s.addChart(pres.charts.BAR, [{
      name: "Share of 447 game functions",
      labels: ["Read (decompiled)", "Understood (contract)", "Ported to C++", "Matched on original code", "Seen in running original"],
      values: [100, 96, 90, 79, 0],
    }], {
      x: 0.5, y: 1.3, w: 9.0, h: 3.0, barDir: "bar", catAxisOrientation: "maxMin",
      chartColors: [HEX.green, HEX.green, HEX.green, HEX.teal, HEX.red],
      showValue: true, dataLabelFormatCode: '0"%"', dataLabelPosition: "outEnd", dataLabelFontSize: 14, dataLabelColor: HEX.ink, dataLabelFontFace: "+mn-lt",
      catAxisLabelFontSize: 14, catAxisLabelColor: HEX.ink, catAxisLabelFontFace: "+mn-lt",
      valAxisHidden: true, valAxisMaxVal: 110, valAxisMinVal: 0, valGridLine: { style: "none" }, catGridLine: { style: "none" },
      showLegend: false, barGapWidthPct: 40,
    });
    text(s, "Honest limit: compared on screenshots of the original for two menu screens and a real saved game, not yet as running functions.", { x: 0.5, y: 4.45, w: 9, h: 0.6, fontSize: 14, color: C.accent5 });
  }

  // ================= Part 6: Lessons and close =================
  pres.addSection({ title: "Lessons and close" });

  // 22 The restart
  {
    const s = begin("Lessons and close", "LIGHT", "I threw it away and restarted",
      "Earlier work had drifted into conclusions that were plausible but unchecked. The restart rule: old notes are reference only, never evidence. Same lesson as the joke slide, at project scale.");
    card(s, 0.5, 1.4, 4.4, 3.4, "BeforeCard");
    text(s, "Before", { x: 0.75, y: 1.55, w: 3.9, h: 0.45, fontSize: 24, bold: true, color: C.accent3 });
    text(s, [
      { text: "Notes and conclusions", options: { bullet: true, breakLine: true } },
      { text: "Plausible, unchecked", options: { bullet: true, breakLine: true } },
      { text: "Hard to tell what was solid", options: { bullet: true } },
    ], { x: 0.75, y: 2.15, w: 3.9, h: 2.5, fontSize: 18, paraSpaceAfter: 8 });
    card(s, 5.1, 1.4, 4.4, 3.4, "AfterCard");
    text(s, "After", { x: 5.35, y: 1.55, w: 3.9, h: 0.45, fontSize: 24, bold: true, color: C.text2 });
    text(s, [
      { text: "One function at a time", options: { bullet: true, breakLine: true } },
      { text: "Every step has evidence", options: { bullet: true, breakLine: true } },
      { text: "Old notes are reference only", options: { bullet: true } },
    ], { x: 5.35, y: 2.15, w: 3.9, h: 2.5, fontSize: 18, paraSpaceAfter: 8 });
  }

  // 23 AI good and bad
  {
    const s = begin("Lessons and close", "LIGHT", "Where AI helped and misled",
      "Real example: reading the scene routines found a real bug in my own port, a harbour guard switched on and off the wrong way round. It was found by comparing against the original, not by asking the model.");
    card(s, 0.5, 1.4, 4.4, 2.3, "GoodCard");
    text(s, "Good at", { x: 0.75, y: 1.5, w: 3.9, h: 0.4, fontSize: 22, bold: true, color: C.text2 });
    text(s, "Naming, summarising, struct recovery, cross-referencing, tireless repetition", { x: 0.75, y: 2.0, w: 3.9, h: 1.5, fontSize: 16 });
    card(s, 5.1, 1.4, 4.4, 2.3, "BadCard");
    text(s, "Bad at", { x: 5.35, y: 1.5, w: 3.9, h: 0.4, fontSize: 22, bold: true, color: C.accent3 });
    text(s, "Plausible but wrong semantics, and confident gaps", { x: 5.35, y: 2.0, w: 3.9, h: 1.5, fontSize: 16 });
    s.addShape(pres.shapes.ROUNDED_RECTANGLE, { x: 0.5, y: 3.95, w: 9.0, h: 1.0, rectRadius: 0.1, fill: { color: C.text1 }, objectName: "BugCallout" });
    iconCircle(s, icons.bug, 0.7, 4.1, 0.7, HEX.red);
    text(s, "A real bug in my port: a harbour guard switched on and off the wrong way round. Found by comparing with the original, not by asking the model.", { x: 1.6, y: 4.0, w: 7.7, h: 0.9, fontSize: 16, color: C.background1, valign: "middle" });
  }

  // 24 The real prompt
  {
    const s = begin("Lessons and close", "DARK", "The real prompt",
      "AI doesn't replace understanding. It makes a systematic method affordable. Then take questions.");
    text(s, "Hello Claude, implement this game in one shot, no errors.", { x: 0.8, y: 1.6, w: 8.4, h: 0.6, fontSize: 20, color: C.accent5, strike: "sngStrike" });
    s.addText("Decompile the next function in the ledger, write its contract, and verify it against the emulator.", {
      x: 0.8, y: 2.4, w: 8.4, h: 1.4, shape: pres.shapes.ROUNDED_RECTANGLE, rectRadius: 0.15, fill: { color: C.text2 },
      color: C.accent1, fontSize: 24, bold: true, margin: 0.3, valign: "middle", isTextBox: true, objectName: "RealPrompt",
    });
    text(s, "Questions?", { x: 0.8, y: 4.2, w: 8.4, h: 0.7, fontSize: 36, bold: true, color: C.background1 });
  }

  await pres.writeFile({ fileName: OUT });
  await applyTheme(OUT, THEME);
  console.log("wrote", OUT);
})();
