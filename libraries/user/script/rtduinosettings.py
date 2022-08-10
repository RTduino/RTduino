import Tkinter as tk

def rtduino_setting_gui():
    window = tk.Tk()
    window.title('RTduino Settings')
    window.geometry('327x272')
    window.iconbitmap('./script/rt-thread-studio.ico')
    window.wm_attributes('-topmost', 1)
    window.wm_attributes('-topmost', 0)
    window.mainloop()
