import socket
import time
import json
import os
import sys


PORT = 1234


class UnsupportedMethod(RuntimeError):
    pass


class InvalidData(ValueError):
    pass


class ValidationError(ValueError):
    pass

class Server:
    def __init__(self):
        self.init_recv_worker_sock()
    
    def log(self, msg):
        print(msg)
    
    def init_recv_worker_sock(self):
        self.recv_worker_sock = socket.socket(
            socket.AF_INET,
            socket.SOCK_STREAM,
        )
        self.recv_worker_sock.bind((
            socket.gethostname(), PORT,
        ))
        self.recv_worker_sock.listen()

        self.log('receive worker socket is ready')

    def run(self):
        while True:
            time.sleep(1)

            self.log('recv: accept...')
            client_sock, addr = self.recv_worker_sock.accept()

            try:
                self.receive(client_sock)
            except (
                ConnectionResetError,
                BrokenPipeError,
                socket.timeout,
                UnsupportedMethod,
                InvalidData,
                ValidationError,
            ) as e:
                print('Error:', e, file=sys.stderr)
    
    def receive(self, sock):
        try:
            data = sock.recv(1024)
        except (
            ConnectionResetError,
            BrokenPipeError,
            socket.timeout,
        ) as e:
            raise e

        sdata = data.decode()
        d = json.loads(sdata)

        self.receive_json(sock, d)
    
    def receive_json(self, sock, d):
        method = d.get('method', '')
        if method == 'POST':
            self.receive_post(sock, d)
        else:
            raise UnsupportedMethod(f'invalid method {method}')
    
    def receive_post(self, sock, d):
        fname = d.get('filename', None)
        fsize = d.get('filesize', None)
        if fname is None or fsize is None:
            raise InvalidData('invalid post data')
        
        bdata = b''
        while len(bdata) < fsize:
            try:
                bdata += sock.recv(1024)
            except (
                ConnectionResetError,
                BrokenPipeError,
                socket.timeout,
            ) as e:
                raise e
        
        self.save_data(fname, bdata)
    
    def save_data(self, fname, bdata):
        self.validate_fname(fname)

        dirname = 'received'
        if not os.path.exists(dirname):
            os.mkdir(dirname)
        
        path = os.path.join(dirname, fname)

        with open(path, 'wb') as fout:
            fout.write(bdata)
        
        self.log(f'saved {path}')
    
    def validate_fname(self, fname):
        if '..' in fname or \
            '/' in fname or \
            '\\' in fname:
            raise ValidationError(f'invalid file name {fname}')


Server().run()
