import PySimpleGUI as sg
from arcs import TimingArc
from serial import Serial

# arduino port settings
PORT = "/dev/ttyUSB0"
BAUD_RATE = 9600

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
        sensors = Serial(PORT, BAUD_RATE, timeout=0) # sensor readings(0)
    except Exception:
        print(f"Coudn't stabilish connecting to port {PORT}")
        return

    arc = TimingArc(10)  # timer
    while not sensors.closed:
        event, _ = window.read(30)

        # process sensor and button values to update timer
        us_reading = sensors.readline()
        slu, flu = us_reading.split(" ") # start line and finish line sensors
        bgn = (event == "-BGN-")
        rst = (event == "-RST-")
        arc.update([float(slu)], [float(flu)], bgn, rst)

        # update timestamp infos
        ts = arc.getTimeElaps()
        window["-TS1-"].update(ts[0], text_color = "white")
        window["-TS2-"].update(ts[1], text_color = "white")
        window["-TS3-"].update(ts[2], text_color = "white")
        window["-STT-"].update(arc.getState())
        cts = arc.cts + 1
        window[f"-TS{cts}-"].update(text_color = "yellow")

        print(ts) # for failsafe

        # close the window correctly
        if event == sg.WINDOW_CLOSED:
            break

    window.close()

try:
    main()
except KeyboardInterrupt:
    print("Exiting...")