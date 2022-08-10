import os
from SCons.Script import AddOption, GetOption
from rtduinosettings import *

def cmd_rtduino_settings():
    rtduino_setting_gui()
    exit()

def cmd_rtduino_help():
    print('RTduino is Arduino Application and Ecosystem Compatibility Layer for RT-Thread')
    print('scons --rtduino => open Arduino Libraries Manager')
    exit()

def rtduinolib_init():
    AddOption('--rtduino',
                help='Arduino Libraries Manager',
                action='store_true',
                default=False,
                dest='rtduino')

    AddOption('--rtduino-h', '--rtduino-help',
                help='RTduino Help',
                action='store_true',
                default=False,
                dest='rtduino-help')

    if GetOption('rtduino'):
        cmd_rtduino_settings()
    elif GetOption('rtduino-help'):
        cmd_rtduino_help()

