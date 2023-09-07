#include <Wire.h>
#include <VL53L0X.h>

const int threshhold = 100; // centimeters

// xshut pins
const int sl_xpin = 12;
const int fl_xpin = 10;

// sensor address
const int sl_addr = 42;
const int fl_addr = 43;

VL53L0X sIr;
VL53L0X fIr;

void setup(void)
{
    Serial.begin(9600);

    pinMode(sl_xpin, OUTPUT);
    pinMode(fl_xpin, OUTPUT);

    // set address
    Wire.begin();
    pinMode(sl_xpin, INPUT);
    delay(10);
    fIr.setAddress(fl_addr);
    pinMode(fl_xpin, INPUT);
    delay(10);
    sIr.setAddress(sl_addr);

    // initialize
    sIr.init();
    fIr.init();
    sIr.setTimeout(500);
    fIr.setTimeout(500);
    sIr.startContinuous();
    fIr.startContinuous();
}

void loop(void)
{
    int s_dist = sIr.readRangeContinuousMillimeters();
    int f_dist = fIr.readRangeContinuousMillimeters();

    // Serial.println(s_dist);
    // Serial.println(f_dist);

    if ((s_dist != 0) && (s_dist < threshhold))
    {
      Serial.println("S");
    }
    if ((f_dist != 0) && (f_dist < threshhold))
    {
      Serial.println("F");
    }

    delay(10);
}