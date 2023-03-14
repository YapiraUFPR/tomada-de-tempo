#include <HCSR04.h>

const int threshhold = 10;

// start line sensors pins
const int sl_tpin = 2;
const int sl_epin = 3;

// finish line sensors pins
const int fl_tpin = 4;
const int fl_epin = 5;

//initialisation class HCSR04 (trig pin , echo pin)
HCSR04 slu(sl_tpin, sl_epin);
HCSR04 flu(fl_tpin, fl_epin);

void setup(void)
{
    Serial.begin(9600);
}

void loop(void)
{
    if (slu.dist() < threshhold)
    {
      Serial.println("S");
    } else if (flu.dist() < threshhold)
    {
      Serial.println("F");
    }
}
