###############################################################################
# server-python.py
# Name:
# NetId:
###############################################################################

import sys
import socket

RECV_BUFFER_SIZE = 2048
QUEUE_LENGTH = 10

def server(server_port):
    """TODO: Listen on socket and print received message to sys.stdout"""
    s = socket.socket()
    s.bind(('', server_port))
    s.listen(QUEUE_LENGTH)

    while True:
        conn, addr = s.accept()
        data = conn.recv(RECV_BUFFER_SIZE)
        while len(data) > 0:
            sys.stdout.buffer.write(data)
            sys.stdout.flush()
            data = conn.recv(RECV_BUFFER_SIZE)


def main():
    """Parse command-line argument and call server function """
    if len(sys.argv) != 2:
        sys.exit("Usage: python server-python.py [Server Port]")
    server_port = int(sys.argv[1])
    server(server_port)

if __name__ == "__main__":
    main()
