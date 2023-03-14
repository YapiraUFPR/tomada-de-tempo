import PySimpleGUI as sg
from arcs import TimingArc, fakeReading
from serial import Serial
from time import sleep

FPS = 60

# arduino port settings
PORT = "/dev/ttyACM0"
BAUD_RATE = 9600
TIMEOUT = 0.05

def main():
    # window layout
    sg.theme("DarkGrey2")
    ts_font = ("Digital-7", 200)
    ts_size = (13, 2)
    btn_font = ("Roboto Black", 60)
    state_font = ("Osmanya", 60)
    layout = [
        [sg.Text("00:00:000", font=ts_font, size=ts_size, key='-TS1-')],
        [sg.Text("00:00:000", font=ts_font, size=ts_size, key='-TS2-')],
        [sg.Text("00:00:000", font=ts_font, size=ts_size, key='-TS3-')],
        [sg.Text("STATE", font=state_font, key="-STT-", text_color="orange"), sg.Push(), sg.Button("Continue", font=btn_font, key='-BGN-'), sg.Button("Reset", button_color="red", font=btn_font, key='-RST-')],
    ]
    window = sg.Window("Display", layout).Finalize()
    #window.Maximize()
    
    try:
        sensors = Serial(PORT, BAUD_RATE, timeout=0.05) # sensor readings(0)
    except Exception:
        print(f"Couldn't stabilish connecting to port {PORT}")
        return

    arc = TimingArc()  # timer
    while True:
        event, _ = window.read(1000/FPS)

        # process sensor and button values to update timer
        us_reading = sensors.readline().decode().strip()
        #us_reading = fakeReading()

        slu = (us_reading == 'S')
        flu = (us_reading == 'F')

        bgn = (event == "-BGN-")
        rst = (event == "-RST-")
        arc.update(slu, flu, bgn, rst)

        # update timestamp infos
        ts = arc.getTimeElaps()
        window["-TS1-"].update(ts[0], text_color = "white")
        window["-TS2-"].update(ts[1], text_color = "white")
        window["-TS3-"].update(ts[2], text_color = "white")
        window["-STT-"].update(arc.getState())
        cts = arc.cts + 1
        window[f"-TS{cts}-"].update(text_color = "yellow")


        # close the window correctly
        if event == sg.WINDOW_CLOSED:
            break

    
    sensors.close()
    window.close()

try:
    main()
except KeyboardInterrupt:
    print("Exiting...")