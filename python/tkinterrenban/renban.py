import tkinter as tk
from tkinter import filedialog
from tkinter import messagebox
import os
import sys


class FolderSelector(tk.Frame):
    def __init__(self, master):
        super().__init__(master)
        
        self.label = tk.Label(self, text='選択されていません。')
        self.label.pack(side=tk.LEFT, expand=True, fill=tk.X)

        self.btn = tk.Button(
            self,
            text='フォルダを選択する',
            command=self.on_clicked,
        )
        self.btn.pack(side=tk.LEFT)

        self.folder_path = None
    
    def on_clicked(self):
        idir = os.path.expanduser('~')
        self.folder_path = filedialog.askdirectory(initialdir=idir)
        if len(self.folder_path):
            self.label.config(text=self.folder_path)
        else:
            self.folder_path = None
            self.label.config(text='選択されていません。')
        

class MainFrame(tk.Frame):
    def __init__(self, master):
        super().__init__(master)
        
        self.folder_selector = FolderSelector(self)
        self.folder_selector.pack(expand=True, fill=tk.X)

        tk.Frame(self).pack(pady=10)

        self.exec_btn = tk.Button(
            self,
            text='フォルダ内のファイルを連番にする',
            command=self.on_exec_clicked,
        )
        self.exec_btn.pack(
            expand=True,
            fill=tk.X,
            ipady=10,
            ipadx=20,
        )
    
    def on_exec_clicked(self):
        fldpath = self.folder_selector.folder_path
        if fldpath is None:
            return messagebox.showerror('ERROR', 'フォルダが選択されていません。')

        files = os.listdir(fldpath)
        files = sorted(files, key=lambda fname: fname[0])

        for i, fname in enumerate(files):
            path = os.path.join(fldpath, fname)
            name, ext = os.path.splitext(path)
            name += f'_{i:04d}'
            new_path = name + ext
            os.rename(path, new_path)
            print('rename to', new_path)


class App(tk.Tk):
    def __init__(self):
        super().__init__()
        self.title('連番くん')

        self.main_frame = MainFrame(self)
        self.main_frame.pack(
            expand=True,
            fill=tk.BOTH,
            pady=10,
            padx=10,
        )


App().mainloop()
