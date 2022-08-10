import os
import Tkinter as tk
from SCons.Script import AddOption, GetOption

def rtduinolib_gui():
    window = tk.Tk()
    window.title('RTduino Settings')
    window.geometry('327x272')
    window.iconbitmap('./script/rt-thread-studio.ico')
    window.wm_attributes('-topmost', 1)
    window.wm_attributes('-topmost', 0)
    window.mainloop()

def rtduinolib_init():
    AddOption('--rtduino',
                help='RTduino Settings',
                action='store_true',
                default=False,
                dest='rtduino')

    if GetOption('rtduino'):
        rtduinolib_gui()
