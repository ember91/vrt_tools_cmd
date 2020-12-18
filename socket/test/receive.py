#!/usr/bin/python3

# Receive data on socket

import socket
import signal
import sys
import time

if len(sys.argv) != 3:
    print("Usage: {} <UDP/TCP> <service>".format(sys.argv[0]))
    exit(0)

use_udp = True
if sys.argv[1].lower() == "udp":
    use_udp = True
elif sys.argv[1].lower() == "tcp":
    use_udp = False
else:
    print("Protocol type must be either UDP or TCP")
    exit(1)

service = int(sys.argv[2])

total_bytes   = 0
total_packets = 0

# Count total received bytes at SIGINT
def signal_handler(sig, frame):
    print("Received a total of {} packets, totalling {} B".format(total_packets, total_bytes))
    sys.exit(0)
signal.signal(signal.SIGINT, signal_handler)

# Address
IP = "localhost"

# Setup listener
sock = socket.socket(socket.AF_INET, socket.SOCK_DGRAM if use_udp else socket.SOCK_STREAM)
sock.bind((IP, service))
if not use_udp:
    sock.listen(1)
    (conn, addr) = sock.accept()

while True:
    if use_udp:
        (data, addr) = sock.recvfrom(1024)
    else:
        data = conn.recv(1024)
    if len(data) == 0:
        time.sleep(0.1)
    else:
        total_packets += 1
        bytes = len(data)
        total_bytes += bytes
        print("Received {} B from {}: ".format(bytes, addr))
