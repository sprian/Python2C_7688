Python2C v0.9.11a - azureiotd
- Concurrent Direct Method replies (queued), using *_Ex API
- UCI tunables:
  * azure.device.dm_timeout : seconds (1..600), default 30
  * azure.device.dm_max     : max in-flight (1..256), default 32
- Socket API: SUB C2D | SUB DM | REPLY <rid> <code> <json> | SEND <json> | STATUS | EXIT
