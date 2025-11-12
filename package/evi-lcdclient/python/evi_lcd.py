from __future__ import print_function
import ctypes, os
_lib = None
cands = ['/usr/lib/libevi_lcd.so', os.path.join(os.path.dirname(__file__), 'libevi_lcd.so')]
for c in cands:
    if os.path.exists(c):
        try:
            _lib = ctypes.CDLL(c); break
        except Exception: pass
if _lib is None:
    _lib = ctypes.CDLL('libevi_lcd.so')

_lib.evi_lcd_init.restype = ctypes.c_int
_lib.evi_lcd_clear.restype = ctypes.c_int
_lib.evi_lcd_home.restype = ctypes.c_int
_lib.evi_lcd_instr.argtypes = [ctypes.c_uint8]; _lib.evi_lcd_instr.restype = ctypes.c_int
_lib.evi_lcd_ddram.argtypes = [ctypes.c_uint8]; _lib.evi_lcd_ddram.restype = ctypes.c_int
_lib.evi_lcd_put.argtypes = [ctypes.c_char_p]; _lib.evi_lcd_put.restype = ctypes.c_int
_lib.evi_lcd_set_socket.argtypes = [ctypes.c_char_p]

def set_socket(path): _lib.evi_lcd_set_socket(path.encode('utf-8'))
def init(): return _lib.evi_lcd_init()
def clear(): return _lib.evi_lcd_clear()
def home(): return _lib.evi_lcd_home()
def instr(v): return _lib.evi_lcd_instr(ctypes.c_uint8(v & 0xFF))
def ddram(addr): return _lib.evi_lcd_ddram(ctypes.c_uint8(addr & 0x7F))
def put(s):
    if isinstance(s, str): s = s.encode('utf-8')
    return _lib.evi_lcd_put(ctypes.c_char_p(s))

if __name__ == '__main__':
    print('init:', init())
    print('put:', put('Hello from Python'))
