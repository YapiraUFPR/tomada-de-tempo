#include <HCSR04.h>

const int threshhold = 10;

// start line sensors pins
const int sl_tpin = 3;
const int sl_epin = 4;

// finish line sensors pins
const int fl_tpin = 5;
const int fl_epin = 6;

//initialisation class HCSR04 (trig pin , echo pin)
HCSR04 slu(sl_tpin, sl_epin);
HCSR04 flu(fl_tpin, fl_epin);

void setup(void)
{
    Serial.begin(9600);
}

void loop(void)
{
    int sensorDist = slu.dist();
    int sensorDist2 = flu.dist();

    if ((sensorDist != 0) && (sensorDist < threshhold))
    {
      Serial.println("S");
    } else if ((sensorDist2 != 0) && (sensorDist2 < threshhold))
    {
      Serial.println("F");
    }
}
