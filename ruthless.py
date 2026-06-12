import socket
import struct

s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
s.connect(("127.0.0.1", 8080))

# Force the socket to instantly abort (TCP RST)
s.setsockopt(socket.SOL_SOCKET, socket.SO_LINGER, struct.pack('ii', 1, 0))

# Send the request
s.sendall(b"GET / HTTP/1.1\r\nHost: localhost:8080\r\n\r\n")

# Instantly kill the connection before the server can reply
s.close()