# -*- coding: utf-8 -*-
from ctypes import CDLL, c_char_p, c_int
import os

def _load():
    cand=[os.environ.get("LIBEVI_PATH"),"libevi.so",os.path.join(os.path.dirname(__file__),"..","src","libevi.so"),"libevi_caption.so"]
    err=None
    for p in cand:
        if not p: continue
        try: return CDLL(p)
        except Exception as e: err=e
    raise OSError("Cannot load libevi: %r"%err)
lib=_load()
lib.evi_version.restype=c_char_p

def version():
    v=lib.evi_version(); return v.decode("ascii") if isinstance(v,bytes) else v
lib.evi_caption_version.restype=c_char_p

def caption_version():
    v=lib.evi_caption_version(); return v.decode("ascii") if isinstance(v,bytes) else v
lib.evi_caption_initial.argtypes=[]; lib.evi_caption_initial.restype=None
lib.evi_caption_system_loading.argtypes=[]; lib.evi_caption_system_loading.restype=None
lib.evi_caption_info1.argtypes=[c_char_p,c_char_p]; lib.evi_caption_info1.restype=None
lib.evi_caption_info2.argtypes=[c_char_p,c_char_p]; lib.evi_caption_info2.restype=None
lib.evi_caption_info3.argtypes=[c_char_p,c_char_p]; lib.evi_caption_info3.restype=None
lib.evi_caption_info4.argtypes=[c_char_p,c_char_p]; lib.evi_caption_info4.restype=None
lib.evi_caption_standby.argtypes=[c_int]; lib.evi_caption_standby.restype=None

def initial(): lib.evi_caption_initial()

def systemLoading(): lib.evi_caption_system_loading()

def info1(mcu,soc): lib.evi_caption_info1(mcu.encode("utf-8"), soc.encode("utf-8"))

def info2(sn,pn):   lib.evi_caption_info2(sn.encode("utf-8"), pn.encode("utf-8"))

def info3(mv,mc):   lib.evi_caption_info3(mv.encode("utf-8"), mc.encode("utf-8"))

def info4(m2,c2):   lib.evi_caption_info4(m2.encode("utf-8"), c2.encode("utf-8"))

def standby(t):     lib.evi_caption_standby(int(t))
lib.evi_caption_preparing.argtypes=[]; lib.evi_caption_preparing.restype=None
lib.evi_caption_authWaiting.argtypes=[c_int]; lib.evi_caption_authWaiting.restype=None
lib.evi_caption_plugInWaiting.argtypes=[c_int]; lib.evi_caption_plugInWaiting.restype=None
lib.evi_caption_serverConn.argtypes=[]; lib.evi_caption_serverConn.restype=None
lib.evi_caption_serverConnFailed.argtypes=[]; lib.evi_caption_serverConnFailed.restype=None
lib.evi_caption_serverConnLost.argtypes=[]; lib.evi_caption_serverConnLost.restype=None
lib.evi_caption_getMValue.argtypes=[c_char_p,c_char_p]; lib.evi_caption_getMValue.restype=None
lib.evi_caption_dispComplete.argtypes=[c_char_p,c_char_p]; lib.evi_caption_dispComplete.restype=None

def preparing(): lib.evi_caption_preparing()

def authWaiting(sec): lib.evi_caption_authWaiting(int(sec))

def plugInWaiting(sec): lib.evi_caption_plugInWaiting(int(sec))

def serverConn(): lib.evi_caption_serverConn()

def serverConnFailed(): lib.evi_caption_serverConnFailed()

def serverConnLost(): lib.evi_caption_serverConnLost()

def getMValue(item, value): lib.evi_caption_getMValue(item.encode("utf-8"), value.encode("utf-8"))

def dispComplete(item, value): lib.evi_caption_dispComplete(item.encode("utf-8"), value.encode("utf-8"))
