import tkinter as tk
from tkinter import ttk
from tkinter.scrolledtext import ScrolledText
import socket
import threading
import time


MESSAGE_PORT = 1234
BROADCAST_PORT = 2234


class Client(tk.Tk):
    def __init__(self):
        super().__init__()
        self.title('Client')

        self.text = ScrolledText(self)
        self.text.pack(side=tk.TOP, expand=True, fill=tk.BOTH)

        self.bottom_frame = tk.Frame(self)
        self.bottom_frame.pack(side=tk.TOP, fill=tk.X)

        self.message_entry = tk.Entry(self.bottom_frame)
        self.message_entry.pack(side=tk.LEFT, expand=True, fill=tk.X)

        self.message_btn = tk.Button(
            self.bottom_frame,
            text='メッセージ送信',
            command=self.message_btn_clicked,
        )
        self.message_btn.pack(side=tk.LEFT)

        self.init_message_sock()
        self.init_broadcast_sock()

        self.broadcast_worker_thread = threading.Thread(
            target=self.broadcast_worker,
            daemon=True,
        )
        self.broadcast_worker_thread.start()
    
    def log(self, msg):
        self.text.insert(tk.END, msg)
        self.text.see(tk.END)

    def broadcast_worker(self):
        while True:
            time.sleep(1)

            data = self.broadcast_sock.recv(1024)
            data = data.decode()
            self.log(f'{data}\n')
            
    def init_message_sock(self):
        self.message_sock = socket.socket(
            socket.AF_INET,
            socket.SOCK_STREAM,
        )
        self.message_sock.connect((
           socket.gethostname(),
           MESSAGE_PORT, 
        ))
        self.log('message socket connect\n')

    def init_broadcast_sock(self):
        self.broadcast_sock = socket.socket(
            socket.AF_INET,
            socket.SOCK_STREAM,
        )
        self.broadcast_sock.connect((
            socket.gethostname(),
            BROADCAST_PORT,
        ))
        self.log('broadcast socket connect\n')

    def message_btn_clicked(self):
        msg = self.message_entry.get()
        msg = msg.encode()
        self.message_sock.send(msg)


if __name__ == '__main__':
    client = Client()
    client.mainloop()
