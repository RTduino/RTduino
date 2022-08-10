import os
from SCons.Script import AddOption, GetOption
from ALM_GUI import *

def cmd_libraries_manager():
    ALM_GUI_Init()

def cmd_help():
    print('RTduino is Arduino Application and Ecosystem Compatibility Layer for RT-Thread')
    print('scons --rtduino => open Arduino Libraries Manager')
    exit()

def ALM_Init():
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
        cmd_libraries_manager()
    elif GetOption('rtduino-help'):
        cmd_help()

