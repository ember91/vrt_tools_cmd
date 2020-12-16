#!/usr/bin/python3

# Receive data on socket

import socket
import signal
import sys

total_bytes   = 0
total_packets = 0

# Count total received bytes at SIGINT
def signal_handler(sig, frame):
    print("Received a total of {} packets totalling {} B".format(total_packets, total_bytes))
    sys.exit(0)
signal.signal(signal.SIGINT, signal_handler)

# Address and port
UDP_IP   = "127.0.0.1"
UDP_PORT = 20001

# Setup listener
sock = socket.socket(socket.AF_INET, # Internet
                    socket.SOCK_DGRAM) # UDP
sock.bind((UDP_IP, UDP_PORT))

while True:
    data, addr = sock.recvfrom(1024) # buffer size is 1024 bytes
    total_packets += 1
    bytes = len(data)
    total_bytes += bytes
    print("Received {} B from {}: ".format(bytes, addr))
