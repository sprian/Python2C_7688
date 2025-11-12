# CheckResult — instructionDef vs v0.9.9
Date: 2025-10-14

## Summary
- instructionDef.py only defines opcode constants and two enums; it doesn't touch UCI or define return semantics.
- Updated shim now sends **raw opcodes** to `lcdsvc` (`INSTR 0xNN`), so constants map 1:1.
- UCI is still consumed only by the daemon; no coupling with instructionDef.

## Opcode support
- 0x01 CLEAR → supported (`CLEAR`/`INSTR 0x01`).
- 0x02 HOME → supported (`INSTR 0x02`).
- 0x08/0x09/0x0A/0x0C → supported (`INSTR`).
- 0x10/0x14 cursor moves → supported.
- 0x18/0x1C shift → supported.
- 0x30 Function Set → sent by init (0x30×2), still callable later.
- 0x34/0x36 EXT GRAPHIC → controller-dependent (NOP on HD44780); safe to send.

## Return values
- Shim returns **0 on OK**, **-1 on error** for `write_instructions`, `write_letters`, `LCD_clr`, `set_ddram`.
