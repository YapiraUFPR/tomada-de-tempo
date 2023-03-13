# Tomada de tempo

* Software for the time taking arcs for line follower robots. Has two versions:
  * An arduino standalone that uses a lcd display to show the timer;
  * And one which uses a computer with a GUI written in python (3.10) to display the timers.
* Both versions need at least two ultrassonic sensors for the starting and finishing arcs.

```bash
.
├── arcs
│   └── arcs.ino
├── display
│   ├── arcs.py
│   ├── fonts
│   │   └── digital-7.ttf
│   ├── main.py
│   ├── requirements.txt
│   └── us_reader
│       └── us_reader.ino
├── LICENSE
└── README.md
```

## Arduino Standalone version
* The folder *arcs* contains a arduino standalone code that uses a lcd display to display a single timer in milliseconds.

## GUI version
* The folder *display* contains a python software which uses the **PySerial** library to make an interface with the arduino serial port and read the ultrassonic sensor values. 
  * *display/us_reader/* folder contains the code for the arduino microcontroller, which is only responsable for reading and sending the sensor values to the serial port;
  * Class *arcs.py* is responsible for the time taking logic;
  * *main.py* is the main program which builds the GUI, using the **PySimpleGUI** library, comunicates with the serial port and calls the *Arcs* class.
  * It is recomended to install the fonts inside *display/fonts/* for a better visualization.

## Authors
* Gabriel de Oliveira Pontarolo
* Rian Marcos Sepulveda

## Special Thanks
* Luan Matheus Trindade Dalmazo
* Gabriel Nascarella Hishida do Nascimento
* https://open.spotify.com/playlist/3DbzVM500CO9llaYeNxMgo?si=933adffc20f94eff