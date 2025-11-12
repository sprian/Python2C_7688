import ctypes, json, socket
SOCK = "/var/run/azureiotd.sock"
_lib = None
try:
    _lib = ctypes.CDLL("/usr/lib/libevi_azure.so")
    _lib.evi_azure_send_json.argtypes = [ctypes.c_char_p]
    _lib.evi_azure_send_json.restype  = ctypes.c_int
except Exception:
    _lib = None

def _send_sock(line: str) -> int:
    try:
        s = socket.socket(socket.AF_UNIX, socket.SOCK_STREAM)
        s.connect(SOCK)
        s.sendall((line+"\n").encode("utf-8","ignore"))
        data = s.recv(128); s.close()
        return 0 if data and data.startswith(b"OK") else -1
    except Exception:
        return -1

def send(payload: dict) -> int:
    js = json.dumps(payload, separators=(",",":"))
    if _lib:
        return int(_lib.evi_azure_send_json(js.encode("utf-8")))
    return _send_sock(f"SEND {js}")

def status() -> str:
    try:
        s = socket.socket(socket.AF_UNIX, socket.SOCK_STREAM)
        s.connect(SOCK); s.sendall(b"STATUS\n")
        d = s.recv(128); s.close()
        return d.decode("utf-8","ignore").strip()
    except Exception:
        return "ERR"

def subscribe_c2d():
    s = socket.socket(socket.AF_UNIX, socket.SOCK_STREAM)
    s.connect(SOCK); s.sendall(b"SUB C2D\n")
    s.recv(64)  # OK
    buf = b""
    try:
        while True:
            ch = s.recv(4096)
            if not ch: break
            buf += ch
            while b"\n" in buf:
                ln, buf = buf.split(b"\n", 1)
                yield ln.decode("utf-8","ignore")
    finally:
        s.close()
