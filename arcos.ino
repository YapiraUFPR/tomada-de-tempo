#include <HCSR04>
#include <LiquidCrystal.h>

// start line sensors pins
const int sl_ul_tpin = 7
const int sl_ul_epin = 7;
const int sl_ur_tpin = 8;
const int sl_ur_epin = 8;

// finish line sensors pins
const int fl_ul_tpin = 9;
const int fl_ul_epin = 9;
const int fl_ur_tpin = 10;
const int fl_ur_epin = 10;

// reset button pin
const int rst_btn_pin = 11;

// threshold for the ultrassonic sensors reading
const int US_THRSH = 50;

// code state
enum LINE_STATE {INIT, START, COUNTING, FINISH};

unsigned int start_timestamp = 0;
unsigned int time_elapsed = 0;

LINE_STATE initTrs(bool rst_btn_state)
{
    if (rst_btn_state == 1)
    {
        return START;
    }

    lcd.setCursor(0, 0);
    lcd.print("STANDBY");

    return INIT;
}


LINE_STATE startTrs(bool rst_btn_state, bool start_line_state)
{
    if (rst_btn_state == 1)
    {
        return INIT; 
    }
    if (start_line_state == 1)
    {
        start_timestamp = millis();
        return COUNTING;
    }   
    
    time_elapsed = 0;
    lcd.setCursor(0, 0);
    lcd.print("READY");
    lcd.setCursor(0, 1);
    lcd.print(time_elapsed);
    return START;
}

LINE_STATE countTrs(bool finish_line_state, bool rst_btn_state)
{
    if (rst_btn_state == 1)
    {
        return INIT; 
    }
    if (finish_line_state == 1)
    {
        return FINISH;
    }
    
    time_elapsed = millis() - start_timestamp;

    lcd.setCursor(0, 0);
    lcd.print("COUNTING");
    lcd.setCursor(0, 1);
    lcd.print(time_elapsed);
    
    return COUNTING;
}


LINE_STATE finishTrs(bool rst_btn_state)
{
    if (rst_btn_state == 1)
    {
        return INIT;
    }

    lcd.setCursor(0, 0);
    lcd.print("FINISHED");
    lcd.setCursor(0, 1);
    lcd.print(time_elapsed);

    return FINISH;
}

void setup() {
    //initialisation class HCSR04 (trig pin , echo pin)
    HCSR04 sl_ur(sl_ur_tpin, sl_ur_epin);
    HCSR04 sl_ul(sl_ul_tpin, sl_ul_epin); 
    HCSR04 fl_ur(fl_ur_tpin, fl_ur_epin);
    HCSR04 fl_ul(fl_ul_tpin, fl_ul_epin);

    pinMode(rst_btn_pin, INPUT);

    LiquidCrystal lcd(rs, en, d4, d5, d6, d7);
    lcd.begin(16, 2);
    
    LINE_STATE state = INIT;
}

void loop() {

    bool rst_btn_state = digitalRead(rst_btn_pin);
    bool start_line_state =  (sl_ul.dist() < US_THRSH) || (sl_ur.dist() < US_THRSH);
    bool finish_line_state =  (fl_ul.dist() < US_THRSH) || (fl_ur.dist() < US_THRSH);

    switch (state)
    {
        case INIT:
            state = initTrs(rst_btn_state);
            break;

        case START:
            state = startTrs(rst_btn_state, start_line_state);    
            break;

        case COUNTING:
            state = countTrs(finish_line_state, rst_btn_state);
            break;

        case FINISH:
            state = finishTrs(rst_btn_state);
            break;
    }

    delay(60);
}