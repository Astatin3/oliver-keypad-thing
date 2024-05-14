SERIAL_PORT = "COM3"
SERIAL_BUAD_RATE = 9600

AHK_PATH = "C:\\Program Files\\AutoHotkey\\v2\\AutoHotkey.exe"
AHK_SCRIPTS_DIR = "C:\\Users\\astatin3\\Documents\\AutoHotkey\\"

# Button Choord starts from 0
BUTTON_VALS = [
    {
        "pos": (1,1),
        "script": "test.ahk"
    }
]

# Slider ID starts from 1
SLIDER_VALS = [
    {
        "id": 1,
        "program": "master"
    },{
        "id": 2,
        "program": "firefox.exe"
    }
]

import math
import subprocess
from serial import Serial
from comtypes import CLSCTX_ALL
from pycaw.pycaw import AudioUtilities, IAudioEndpointVolume, ISimpleAudioVolume

sessions = AudioUtilities.GetAllSessions()

devices = AudioUtilities.GetSpeakers()
interface = devices.Activate(IAudioEndpointVolume._iid_, CLSCTX_ALL, None)
masterVolume = interface.QueryInterface(IAudioEndpointVolume)

def setMasterVolume(num: int):
    if num < 0 or num > 100: return
    masterVolume.SetMasterVolumeLevelScalar((num/100), None)

def setProgramVolume(num: int, program: str):
    if num < 0 or num > 100: return
    for session in sessions:
        volume = session._ctl.QueryInterface(ISimpleAudioVolume)
        if session.Process and session.Process.name() == program:
            volume.SetMasterVolume(num/100, None)
            # print(f"{session.Process.name()} volume.GetMasterVolume(): {volume.GetMasterVolume()}")

def run_ahk_script(script:str):
    subprocess.Popen([AHK_PATH, (AHK_SCRIPTS_DIR+script)])

def handle_button(pos:tuple):
    print(f"Pressed button ({pos[0]}, {pos[1]})", end="")
    for button in BUTTON_VALS:
        if button["pos"] != pos:
            continue

        print(f", and ran AHK Script: {AHK_SCRIPTS_DIR}{button['script']}", end="")
        run_ahk_script(button["script"])

    print("\n", end="")

def sliderUpdate(num: int, program: str):
    if program == "master":
        print(", and set master volume", end="")
        setMasterVolume(num)
    else:
        print(f", and set the volume of {program}", end="")
        setProgramVolume(num, program)

def handle_slider(id:int, pos:int):
    print(f"Slider {id} is at {pos}%", end="")
    for slider in SLIDER_VALS:
        if slider["id"] != id:
            continue
        if type(slider['program']) == list:
            for program in slider['program']:
                sliderUpdate(pos, program)
        elif type(slider['program']) == str:
                sliderUpdate(pos, slider['program'])
        

            
    print("\n", end="")

def serial_ports():
    import serial.tools.list_ports
    print("Open serial ports: " + str([port.name for port in serial.tools.list_ports.comports()]))

serial_ports()

ser = Serial(SERIAL_PORT, SERIAL_BUAD_RATE)

while ser.is_open:
    cc=str(ser.readline())
    val = cc[2:][:-5].split('|')
    match (int(val[0])):
        case 0: # Button
            pos = (int(val[1]), int(val[2]))
            handle_button(pos)
        case 1: # Slider
            level = (int(val[2])/1024)*100
            handle_slider(int(val[1]), level)
