 ###############################################################################
# client-python.py
# Name:
# NetId:
###############################################################################

import sys
import socket

SEND_BUFFER_SIZE = 2048

def client(server_ip, server_port):
    """TODO: Open socket and send message from sys.stdin"""
    with socket.socket() as s:
        s.connect((server_ip, server_port)) 
        data = sys.stdin.buffer.read(SEND_BUFFER_SIZE)

        while len(data) > 0:
            s.send(data)
            data = sys.stdin.buffer.read(SEND_BUFFER_SIZE)


def main():
    """Parse command-line arguments and call client function """
    if len(sys.argv) != 3:
        sys.exit("Usage: python client-python.py [Server IP] [Server Port] < [message]")
    server_ip = sys.argv[1]
    server_port = int(sys.argv[2])
    client(server_ip, server_port)

if __name__ == "__main__":
    main()
