# -*- coding: utf-8 -*-
"""displayCaptionFunc shim (compat layer)
- Keeps legacy function names used by older Python code.
- Delegates to evi.py when available, otherwise talks to lcdsvc over Unix socket.
- Supports raw instruction writes so that instructionDef.py constants work as expected.
"""
from __future__ import print_function
import socket
SOCK_PATH = "/var/run/evi-lcd.sock"
try:
    import evi as _evi
except Exception:
    _evi = None
_inited = False
def _ensure_init():
    global _inited
    if _inited:
        return
    if _evi and hasattr(_evi, 'init'):
        try:
            _evi.init()
        except Exception:
            pass
    _inited = True
def _send(cmd):
    try:
        s = socket.socket(socket.AF_UNIX, socket.SOCK_STREAM)
        s.connect(SOCK_PATH)
        if not cmd.endswith('\n'):
            cmd += '\n'
        s.sendall(cmd.encode('utf-8', 'ignore'))
        data = s.recv(256)
        s.close()
        if not data:
            return -1
        resp = data.decode('utf-8', 'ignore').strip()
        return 0 if resp.startswith('OK') else -1
    except Exception:
        return -1
def delay72u():
    return
def delay1600u():
    return
def write_instructions(val):
    _ensure_init()
    v = int(val) & 0xFF
    if _evi and hasattr(_evi, 'instr'):
        try:
            r = _evi.instr(v)
            return 0 if (r == 0 or r is None) else -1
        except Exception:
            pass
    return _send('INSTR 0x%02X' % v)
def write_letters(wordStr):
    _ensure_init()
    s = wordStr if isinstance(wordStr, (str, bytes)) else str(wordStr)
    if isinstance(s, bytes):
        try:
            s = s.decode('utf-8', 'ignore')
        except Exception:
            s = s.decode('latin-1', 'ignore')
    if _evi:
        for fname in ('dispComplete', 'setText', 'printText', 'put'):
            if hasattr(_evi, fname):
                try:
                    if fname == 'dispComplete':
                        r = getattr(_evi, fname)('text', s)
                    else:
                        r = getattr(_evi, fname)(s)
                    return 0 if (r == 0 or r is None) else -1
                except Exception:
                    pass
    return _send('PUT ' + s)
def write_graphic(data):
    return write_letters(data)
def LCD_clr():
    _ensure_init()
    if _evi and hasattr(_evi, 'clear'):
        try:
            r = _evi.clear()
            return 0 if (r == 0 or r is None) else -1
        except Exception:
            pass
    return _send('CLEAR')
def DDRAM_ADDR(pos):
    pos = int(pos)
    if pos < 12:
        addr = pos | 0x80
    elif pos < 24:
        addr = (pos - 12) | 0x90
    else:
        addr = (pos % 12) | (0x90 if pos >= 12 else 0x80)
    return addr
def GDRAM_ADDR(val):
    return (int(val) | 0x80)
def set_ddram(addr):
    a = int(addr) & 0xFF
    if _evi and hasattr(_evi, 'ddram'):
        try:
            r = _evi.ddram(a)
            return 0 if (r == 0 or r is None) else -1
        except Exception:
            pass
    return _send('DDRAM 0x%02X' % a)
def dispStartLogo(*args, **kwargs):
    _ensure_init()
    for fname in ('systemLoading', 'preparing'):
        if _evi and hasattr(_evi, fname):
            try:
                getattr(_evi, fname)(); return 0
            except Exception:
                pass
    write_letters('STARTING...')
    return 0
