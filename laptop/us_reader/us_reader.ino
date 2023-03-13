#include <HCSR04.h>

// start line sensors pins
const int sl_tpin = 2;
const int sl_epin = 3;

// finish line sensors pins
const int fl_tpin = 6;
const int fl_epin = 7;

//initialisation class HCSR04 (trig pin , echo pin)
HCSR04 slu(sl_tpin, sl_epin);
HCSR04 flu(fl_tpin, fl_epin);

void setup(void)
{
    Serial.begin(9600);
}

void loop(void)
{
    char buffer[20];
    sprintf(buffer, "%f %f\n", slu.dist(), flu.dist());
    Serial.print(buffer);
    delay(60);
}
