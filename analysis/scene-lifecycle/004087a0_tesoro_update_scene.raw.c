/* Ghidra output, not original source. Inferred types and names may be wrong.
 * TESORO.EXE SHA256: 0fcd56b306b857283347e705dcce41a4b44dd4d9ef7c9226694114bba80792ef
 * Entry: 004087a0
 */


undefined4 __fastcall tesoro_update_scene(int *param_1)

{
  undefined4 uVar1;
  
  if ((char)param_1[0x25] != '\0') {
    *(undefined1 *)(param_1 + 0x25) = 0;
                    /* WARNING: Could not recover jumptable at 0x004087b3. Too many branches */
                    /* WARNING: Treating indirect jump as call */
    uVar1 = (**(code **)(*param_1 + 0x34))();
    return uVar1;
  }
  tesoro_animate_scene_objects((int)param_1);
  return 1;
}

