# ApiSampleDoc — instructionDef

## Python (legacy shim)
```python
from instructionDef import definition
import displayCaptionFunc as lcd
definition()
lcd.write_instructions(DISPLAY_CLEAR)
lcd.write_instructions(DISPLAY_ON)
lcd.write_instructions(lcd.DDRAM_ADDR(0x40))
lcd.write_letters('HELLO')
```

## CLI
```sh
lcdctl instr 0x01
lcdctl instr 0x0C
lcdctl instr 0x18
```
