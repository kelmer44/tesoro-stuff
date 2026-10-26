# Inventory, held items and the mouse buttons (25 October 2026)

Engine branch `codex/inventory`, off `tesoro-engine` `2b030bbb844`. Exports in `analysis/inventory/`. **Status:** compiled; the production
objects and scripts were driven headless (`analysis/doors/scenarios/scenario4.*`); **not run in a window, not compared with the original game.**

## The mouse buttons (corrects earlier notes)

The input object keeps one state byte per button (`0x403700`): **8** = pressed, **4** = held, **1** = just released, **2** = released a moment ago (200 ms),
**0x10** = pressed again within that window (a **double click**), **0x20** = held for a very long time. So:
- Left press (`byte0 & 8`): the click (`slot 0x38`) and the click on the ground. Left **double** press (`byte0 & 0x10`): `slot 0x3c`. For ordinary objects
  `0x3c` just calls `0x38`; **for doors it puts Jim at the door and uses it at once** (`0x414d50`). Earlier notes called `0x3c` the right click; it is the
  double click (`CLICK_WALKING.md`, `DOORS_AND_VERBS.md`).
- The **right** button is the second byte: its press (`byte1 & 0x18`) opens the inventory on the main page and closes it on the inventory page
  (`0x4029f0`, `0x4025b0`, `0x4028c0`); its release (`byte1 & 1`) or a key ends the speech of a character that is talking (`0x4101f0`). The engine still skips speech with the
  left button (Gabriel's choice); not changed here. Nothing in the code resets the pointer to the arrow on a right click, so what was remembered
  from the original is probably the inventory.

## Items

Kind 13 of `INTERFACE.LVL` (46 items; the item number is byte 1 of the record, e.g. `IBOLI` 0, `IJARRON` 1, `IPLUMA` 2, `ICUERDA` 6, `IHALFDOBLON` 45).
Record (`0x406a90`): byte 0 first frame, byte 1 number, bytes 2..13 name in the dialogue tables, six 12-byte lines from 0xE (if the name is empty the lines start
at 2) for the verbs {0, 1, 3, 4, 5, 2}, `u16` at 0x62 = number of entries, then entries of 24 bytes from 0x7C: the name of another item or object and the line Jim says
when they meet. An item is **active when it is owned**; it starts inactive. Each item sits at a fixed place of the inventory page (the screen to the right
of the main one, x 640 to 1280); the page is `INVENTARIO` (kind 15), which only covers it and takes every click.
- **Activating an item (`0x407000`) puts it in Jim's hand**: the pointer takes its picture (cursor frame = number + 7, so items are frames 7 to 52; frames 5 and 6 are
  unused) and the item is the held one (`pointer + 0x12c`). Deactivating the held item puts it back and the pointer to the arrow. Scripts do this with
  `sendmessage 'IBOLI' in INVENTARIO activate on/off`; the engine's inventory scope now reaches the interface scene.
- **Clicking an item in the page** (`0x406d50`), by the pointer's frame: with a line of its own for the verb (frames 1-6), Jim says it. Otherwise frame 0 and frames 1-6
  without a handler **pick it up**. With an item in the hand (frame 7 or more): if either item has an entry naming the other, Jim says that line and the page closes (the
  per-item effects, `0x6c` overrides, then run when he has finished); otherwise Jim says `MODELO2` (a blank line).
- **Using an item on a room object** (`0x40f0d0`, default case): when Jim gets there the item goes back to the inventory, the pointer to the arrow, and
  the object answers with `0x6c(number)`. The default (`0x40f480`) says `ICON01T058` for item 0 and `MODELO2` for every other; the held item has been dropped first, so
  the entries of the item that name room objects (`CARTA`, `MAPA`, ...) are never read by the code recovered. The captain (item 0: `FIBOLI/capitan`) and the recruiter (items 0, 7, 45:
  `FIBOLI`, `FICNUDO`, `FIHLFDBLN` / `reclutador`) answer in their own way.
- Choosing a verb on the bar puts the item back (`0x415440`).
- **Opening the inventory**: the interface camera moves 640 to the right, and the strip, the shade and the verb icons move with it; an item in the hand is hidden
  from the page while it is open.

Item classes (by number) override a few verbs or the item-meets-item effect: look (`0x54`) for 19 `IRECETARIO`, 16 `IPLATO`, 28 `IMAPA`, 44 `IPROBERVIO`;
open (`0x60`) for 9 `ICOFRE`, 10 `ICIRUELA`, 15 `ILIBRO`, 19, 29 `IBOLSO`; effects (`0x6c`) for 16, 28, 33 `ICUBOPANT`, 36 `IRAMARESINA`, 37 `ICUBOMAR`, 42 `ISOGA`
(`0x417180`, `0x40c090`, `0x40c0e0`, `0x40c040`, `0x40c140`, `0x40c1f0`, `0x40c0f0`, `0x40c200`, `0x40c250`, `0x416ee0`, `0x417250`, `0x4174b0`, `0x417360`, `0x4173e0`, `0x417530`). The
items of the first scenes (0 to 8) have none.

## Ported

`ItemObject`, `InventoryPageObject` (`objects.h/.cpp`), `PointerHost::heldItem/holdItem/inventoryOpen/closeInventory`, the page toggle on the right button, the
interface scroll (`TesoroEngine::toggleInventory`, `ControlIconObject::shiftX`, camera bounds to x 1280, the pointer measured on the scrolled page), the double click as the
second click of objects (doors), items used on room objects (`InteractiveObject::itemUsed`), the captain's and recruiter's item answers, a verb choice dropping the item, and
`SceneObject::setActive` virtual so scripts activate items.

## Verification

Headless (the production code with a fake game): captain with his back turned -> the pen is taken (held, cursor frame 9); used on the letter it goes back and Jim says `MODELO2`;
an item clicked in the page is picked up or answers its look line; item meets item says `MODELO2` when no entry names the other; the ballpoint shown to the captain runs `FIBOLI/capitan` through
to the half doubloon (`IHALFDOBLON` held, frame 52).

## Gaps

1. The per-item classes above, and the effects after an item-meets-item line, are not ported (a pair that has a line says it and closes the page, nothing more).
2. The position of the page (the pointer, the bar) was ported from the interface offsets, not seen in a window; whether the page shows over the room or over a darkened room was not checked.
3. Double-click timing (200 ms from the release) is the value of the keyboard object; the mouse object's was not read. Unknown whether a double press also performs the walk.
4. The Jim scenes with the menu (`0x4025b0`'s three excluded scenes) are not excluded. The held item's depth (+2) is not applied.
5. The opening of the page by a script (the `tesoro_find` before the scroll, `0x40f680`) and `0x407aa0(Jim)` called by the open routine were not understood.

## Fix after the first play test (25 October 2026)

The interface was drawn into the room's pass with every sprite at depth 1, so objects at the same depth were drawn in the order of the scene's list and the first one
won: the parchment page (`PERGAMINO`, x 958) hid the items to its left (`IBOLI` 782, `IJARRON` 853, ...). The interface is now drawn in its own pass over the room
(`SceneRenderer::draw(..., clear = false)`) with the depths of its own objects (icons and items 5, shade 6, page 7; the pointer 0 in front). Rendered headless: the page shows the ballpoint, the
jar and the feather pen. The page is the parchment (`PERGAMINO`); items sit at fixed places on it; it opens with the right mouse button, and an item in the hand is not shown on it.
