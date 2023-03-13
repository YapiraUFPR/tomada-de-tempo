#include <HCSR04.h>
#include <LiquidCrystal.h>

// start line sensors pins
const int sl_ul_tpin = 2;
const int sl_ul_epin = 3;
const int sl_ur_tpin = 2;
const int sl_ur_epin = 3;

// finish line sensors pins
const int fl_ul_tpin = 4;
const int fl_ul_epin = 5;
const int fl_ur_tpin = 4;
const int fl_ur_epin = 5;

// reset button pin
const int rst_btn_pin = 7;

// lcd pins
const int rs = 8, en = 9, d4 = 10, d5 = 11, d6 = 12, d7 = 13;

// threshold for the ultrassonic sensors reading
const int US_THRSH = 5;

// code state
enum LINE_STATE {INIT, START, COUNTING, FINISH};

//initialisation class HCSR04 (trig pin , echo pin)
HCSR04 sl_ur(sl_ur_tpin, sl_ur_epin);
HCSR04 sl_ul(sl_ul_tpin, sl_ul_epin); 
HCSR04 fl_ur(fl_ur_tpin, fl_ur_epin);
HCSR04 fl_ul(fl_ul_tpin, fl_ul_epin);
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);
LINE_STATE state = INIT;

unsigned int start_timestamp = 0;
unsigned int time_elapsed = 0;

void printToLcd(String first_line, String second_line)
{
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print(first_line);
    lcd.setCursor(0, 1);
    lcd.print(second_line);
}

bool checkArc(float right_dist, float left_dist)
{
    return (((right_dist != 0) &&  (right_dist < US_THRSH)) || ((left_dist != 0) && (left_dist < US_THRSH)));
}

LINE_STATE initTrs(bool rst_btn_state)
{
    if (rst_btn_state)
    {
        return START;
    }

    printToLcd("STANDBY", "");
    
    //Serial.println("STANDBY");

    return INIT;
}


LINE_STATE startTrs(bool rst_btn_state, bool start_line_state)
{
    if (rst_btn_state)
    {
        return INIT; 
    }
    if (start_line_state)
    {
        start_timestamp = millis();
        return COUNTING;
    }   
    
    time_elapsed = 0;
    printToLcd("READY", String(time_elapsed));

    //Serial.print("READY ");
    //Serial.println(time_elapsed);

    return START;
}

LINE_STATE countTrs(bool finish_line_state, bool rst_btn_state)
{
    if (rst_btn_state)
    {
        return INIT; 
    }
    if (finish_line_state)
    {
        Serial.print("RESULT:");
        Serial.println(time_elapsed);
        return FINISH;
    }
    
    time_elapsed = millis() - start_timestamp;
    printToLcd("COUNTING", String(time_elapsed));
    
    //Serial.print("COUNTING ");
    //Serial.println(time_elapsed);

    return COUNTING;
}


LINE_STATE finishTrs(bool rst_btn_state)
{
    if (rst_btn_state == 1)
    {
        return INIT;
    }

    printToLcd("FINISHED", String(time_elapsed));

    //Serial.print("FINISHED ");
    //Serial.println(time_elapsed);

    return FINISH;
}

bool checkBtn(void)
{
    bool btn_state = digitalRead(rst_btn_pin);
    if (btn_state == HIGH)
    {
        while(digitalRead(rst_btn_pin) == HIGH);
        return true;
    }
    return false;
}

void setup(void) {
    pinMode(A0, OUTPUT);
    analogWrite(A0,75);
    pinMode(rst_btn_pin, INPUT);
    lcd.begin(16, 2);
    Serial.begin(9600);
}

void loop(void) {

    bool rst_btn_state = checkBtn();

    if (state == INIT)
    {
       state = initTrs(rst_btn_state);
    } else if (state == START)
    {
       bool start_line_state =  checkArc(sl_ur.dist(), sl_ul.dist());
       state = startTrs(rst_btn_state, start_line_state);    
    } else if (state == COUNTING)
    {
       bool finish_line_state =  checkArc(fl_ur.dist(), fl_ul.dist());
       state = countTrs(finish_line_state, rst_btn_state);
    } else if (state == FINISH)
    {
       state = finishTrs(rst_btn_state);
    }

    delay(60);
}