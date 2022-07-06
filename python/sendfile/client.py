import socket
import os
import sys
import json


PORT = 1234


def usage():
    print('Usage: client.py [filename]')


def send_file(fname):
    with open(fname, 'rb') as fin:
        data = fin.read()
    
    chunks = [data[i:i+1024] for i in range(0, len(data), 1024)]

    sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    sock.connect((socket.gethostname(), PORT))

    print('sending...')

    d = json.dumps({
        'method': 'POST',
        'filename': os.path.basename(fname),
        'filesize': len(data),
    })
    d = d.encode()
    sock.send(d)

    for chunk in chunks:
        sock.send(chunk)
    
    print('done')


def main():
    if len(sys.argv) < 2:
        usage()
        sys.exit(0)
    
    fname = sys.argv[1]
    send_file(fname)    


if __name__ == '__main__':
    main()
    