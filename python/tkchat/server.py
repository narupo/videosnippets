import tkinter as tk
from tkinter import ttk
from tkinter.scrolledtext import ScrolledText
import threading
import time
import socket


MESSAGE_PORT = 1234
BROADCAST_PORT = 2234


class Server(tk.Tk):
    def __init__(self):
        super().__init__()
        self.title('Server')

        self.text = ScrolledText(self)
        self.text.pack(side=tk.TOP, expand=True, fill=tk.BOTH)

        self.broadcast_socks = []

        self.init_message_worker_sock()
        self.init_broadcast_worker_sock()

        self.message_worker_thread = threading.Thread(
            target=self.message_worker,
            daemon=True,
        )
        self.message_worker_thread.start()

        self.broadcast_worker_thread = threading.Thread(
            target=self.broadcast_worker,
            daemon=True,
        )
        self.broadcast_worker_thread.start()

    def log(self, msg):
        self.text.insert(tk.END, msg)
        self.text.see(tk.END)
    
    def init_broadcast_worker_sock(self):
        self.broadcast_worker_sock = socket.socket(
            socket.AF_INET,
            socket.SOCK_STREAM,
        )
        self.broadcast_worker_sock.bind(
            (socket.gethostname(), BROADCAST_PORT),
        )
        self.broadcast_worker_sock.listen()
        self.log('broadcast worker socket is ready\n')

    def init_message_worker_sock(self):
        self.message_worker_sock = socket.socket(
            socket.AF_INET,
            socket.SOCK_STREAM,
        )
        self.message_worker_sock.bind(
            (socket.gethostname(), MESSAGE_PORT),
        )
        self.message_worker_sock.listen()
        self.log('message worker socket is ready\n')
    
    def broadcast_worker(self):
        while True:
            time.sleep(1)

            self.log('broadcast: accept...\n')
            client_sock, addr = self.broadcast_worker_sock.accept()
            client_sock.settimeout(3)

            self.broadcast_socks.append(client_sock)

            self.log('accept new broadcast client socket\n')

    def message_worker(self):
        while True:
            time.sleep(1)

            self.log('message: accept...\n')
            client_sock, addr = self.message_worker_sock.accept()
            client_sock.settimeout(10)

            th = threading.Thread(
                target=self.message_client_worker,
                args=(client_sock, ),
                daemon=True,
            )
            th.start()
            self.log('accept new message client socket\n')
    
    def message_client_worker(self, client_sock):
        while True:
            time.sleep(1)

            try:
                msg = client_sock.recv(1024)
            except ConnectionResetError:
                break
            except socket.timeout:
                continue
            
            if not len(msg):
                continue
        
            msg = msg.decode()
            self.log(f'msg: {msg}\n')

            self.broadcast(msg)
    
    def broadcast(self, msg):
        msg = msg.encode()

        for sock in self.broadcast_socks:
            try:
                sock.send(msg)
            except (
                BrokenPipeError,
                ConnectionResetError,
                socket.timeout,
            ):
                continue
            
            self.log(f'done broadcast message: {msg}\n')


if __name__ == '__main__':
    server = Server()
    server.mainloop()
