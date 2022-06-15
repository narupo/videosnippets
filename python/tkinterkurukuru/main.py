import tkinter as tk
import threading
import time
import math


class App(tk.Tk):
    def __init__(self):
        super().__init__()
        self.title('くるくる')
        self.geometry('200x200')

        self.x = self.y = 0
        self.cx = 1280 / 2 - 100
        self.cy = 720 / 2 - 100
        self.r = 300
        self.deg = 0

        self.photo = tk.PhotoImage(file='img.png')
        self.label = tk.Label(self, image=self.photo)
        self.label.pack(expand=True, fill=tk.BOTH)

        self.thread = threading.Thread(
            target=self.update,
            daemon=True,
        )
        self.thread.start()

    def deg_to_rad(self, deg):
        return deg * (math.pi / 180)

    def update(self):
        while True:
            time.sleep(1 / 60)

            rad = self.deg_to_rad(self.deg)
            self.x = int(self.cx + self.r * math.cos(rad))
            self.y = int(self.cy + self.r * math.sin(rad))
            self.deg = (self.deg + 1) % 360
            self.geometry(f'200x200+{self.x}+{self.y}')


App().mainloop()
