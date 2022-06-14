import tkinter as tk
import threading
import time


class App(tk.Tk):
    def __init__(self):
        super().__init__()
        self.title('口パク')
        self.geometry('400x400')

        self.images = [
            tk.PhotoImage(file='00.png'),
            tk.PhotoImage(file='01.png'),
            tk.PhotoImage(file='02.png'),
        ]
        self.images_index = 0

        self.label = tk.Label(
            self,
            image=self.images[0],
        )
        self.label.pack(
            expand=True,
            fill=tk.BOTH,
        )

        self.thread = threading.Thread(
            target=self.update,
            daemon=True,
        )
        self.thread.start()
    
    def update(self):
        while True:
            time.sleep(0.1)

            image = self.images[self.images_index]
            self.label.config(image=image)

            self.images_index = int(
                (self.images_index + 1) % len(self.images),
            )
    

App().mainloop()
