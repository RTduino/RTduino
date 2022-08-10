import Tkinter as tk


def _cb_button_setonly():
    exit()

def ALM_GUI_Init():
    root = tk.Tk()
    root.title('Arduino Libraries Manager')
    root.geometry('327x272')
    root.iconbitmap('./ALM/logo/arduino.ico')

    button_setonly = tk.Button(root, text ="Set Only", command = _cb_button_setonly).pack()

    root.wm_attributes('-topmost', 1)
    root.wm_attributes('-topmost', 0)
    root.mainloop()

