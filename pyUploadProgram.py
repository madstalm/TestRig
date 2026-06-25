import socket, time

with open('/home/testrig/Documents/TestStation/program.dmc', 'r') as f:
    program = f.read()

s = socket.socket()
s.connect(('192.168.42.100', 23))
s.settimeout(1.0)
time.sleep(0.5)
try:
    print(s.recv(1024).decode())   # flush banner
except:
    pass

s.sendall(b'DL\r')
time.sleep(0.1)

for line in program.splitlines():
    s.sendall((line.strip() + '\r').encode())
    time.sleep(0.02)

s.sendall(b'\\\r')   # terminator
time.sleep(0.5)
try:
    print(s.recv(4096).decode())
except:
    pass
    
s.sendall(b'BP\r')   # burn program to EEPROM
time.sleep(1)
try:
    print(s.recv(1024).decode())
except:
    pass
s.close()
