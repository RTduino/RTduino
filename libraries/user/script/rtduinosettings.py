import Tkinter as tk

def rtduino_setting_gui():
    window = tk.Tk()
    window.title('Arduino Libraries Manager')
    window.geometry('327x272')
    window.iconbitmap('./script/logo/arduino.ico')
    window.wm_attributes('-topmost', 1)
    window.wm_attributes('-topmost', 0)
    window.mainloop()
