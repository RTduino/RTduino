import Tkinter as tk
from SCons.Script import AddOption, GetOption

def rtduinolib_gui():
    window = tk.Tk()
    window.wm_attributes('-topmost', 1)
    window.wm_attributes('-topmost', 0)
    window.mainloop()

def rtduinolib_init():
    AddOption('--rtduino',
                help='RTduino configuration',
                action='store_true',
                default=False,
                dest='rtduino_configuration')

    if GetOption('rtduino_configuration'):
        rtduinolib_gui()
