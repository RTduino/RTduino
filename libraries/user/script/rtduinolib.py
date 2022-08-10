import os
from SCons.Script import AddOption, GetOption
from rtduinosettings import *

def cmd_rtduino_settings():
    rtduino_setting_gui()
    exit()

def cmd_rtduino_help():
    print('RTduino is Arduino Application and Ecosystem Compatibility Layer for RT-Thread')
    print('scons --rtduino => RTduino Settings')
    exit()

def rtduinolib_init():
    AddOption('--rtduino-h', '--rtduino-help',
                help='RTduino Help',
                action='store_true',
                default=False,
                dest='rtduino-help')

    AddOption('--rtduino',
                help='RTduino Settings',
                action='store_true',
                default=False,
                dest='rtduino')

    if GetOption('rtduino'):
        cmd_rtduino_settings()
    elif GetOption('rtduino-help'):
        cmd_rtduino_help()

