import os
import Tkinter as tk


def _cb_button_setonly():
    exit()

# alm_path: ALM scripts' path
def ALM_GUI_Init(alm_path):
    root = tk.Tk()
    root.title('Arduino Libraries Manager')
    root.geometry('327x272')
    root.iconbitmap(os.path.join(alm_path, 'logo', 'arduino.ico'))

    button_setonly = tk.Button(root, text ="Set Only", command = _cb_button_setonly).pack()

    root.wm_attributes('-topmost', 1)
    root.wm_attributes('-topmost', 0)
    root.mainloop()

