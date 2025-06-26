#ifndef OBJECTS_H
#define OBJECTS_H

#include <Arduino.h>
#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SH110X.h>

#include <faces.h>
#define rep(i, n) for(int i=0; i<n; i++)

//SCREEN
#define OLED_RESET -1   //   QT-PY / XIAO
#define i2c_Address 0x3C //initialize with the I2C addr 0x3C Typically eBay OLED's
#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels
Adafruit_SH1106G display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

//Globals
#define MAX_ARDUINO_TIME 3294967295

unsigned long get_time(){
    return (millis()%MAX_ARDUINO_TIME);
}


//Format from seconds to MM:SS
String format_time(long seconds){
    String ans = "";
    //Calculate
    int m = seconds/60;
    int s = seconds - m*60;

    if(m < 10)
        ans += "0";
    ans += String(m);

    ans += ":";
    
    if(s < 10)
        ans += "0";
    ans += String(s);
    return ans;
}

//==========================================================

struct PassiveBuzzer{
    int pin = 0;

    PassiveBuzzer(){}

    void init(int pin){
        this->pin = pin;
        pinMode(pin, OUTPUT);    
    }

    void beep(unsigned int frec, unsigned int dur){
        tone(pin, frec);
        delay(dur);
        noTone(pin);
    }


    void startupBeep(){
        beep(700, 100);
        beep(900, 100);
    }

    void actionBeep(){
        beep(700, 100);
    }

    void alarmBeep(){
        beep(1000, 200);
        delay(100);
        beep(800, 300);
    }

    void successBeep(){
        beep(700, 100);
        delay(50);
        beep(1000, 100);
        delay(50);
        beep(1300, 100);
    }

    void gamblingBeep(){
        beep(700, 100);
        delay(50);
        beep(1000, 100);
        delay(200);
    }

    void sadBeep(){
        beep(1300, 100); 
        delay(50);
        beep(1000, 100); 
        delay(50);  
        beep(700, 100);
        delay(50);
        beep(500, 200);
    }

    void celebrationBeep(){
        beep(1000, 200);
        delay(300);
        beep(800, 300);
        delay(300);
        beep(600, 300);    
    }

    void angryBeep(){
        beep(600, 100);
        delay(50);
        beep(800, 100);
        delay(200);  
    }

};


struct Screen{
    int centerX = 42;
    int centerY = 26;
    PassiveBuzzer* spk;

    const int CHAR_WIDTH = SCREEN_WIDTH/20;
    const int CHAR_HEIGHT = SCREEN_HEIGHT/8;

    Screen(){}

    void init(PassiveBuzzer &spk){
        this->spk = &spk;
          // Inicializa la pantalla
        if(!display.begin(i2c_Address, true)){
            Serial.println("No se pudo inicializar la pantalla SH1106 :(");
            spk.alarmBeep();
            delay(1000);
        }

        display.clearDisplay();
        display.display();
        display.setTextSize(1);
        display.setTextColor(SH110X_WHITE);
        display.setCursor(0,0);
        delay(500);
    }

    void header(String title){
        display.setTextSize(1);
        display.setCursor(0, 0);
        printCentered(title, 1, false);
        display.drawLine (0,10,128,10, SH110X_WHITE);  
    }

    void moveCursor(int x=-1, int y=-1){
        int new_x = (x != -1) ? x : display.getCursorX();
        int new_y = (y != -1) ? y : display.getCursorY();
        display.setCursor(new_x, new_y);
    }

    void loading_screen(){
        for(int i=0; i<=100; i+=20){
            display.clearDisplay();
            header("Cargando...");
            display.setCursor(centerX-(String(i).length()*5)+15, centerY);
            print(String(i), 2);
            print("%", 2);
            display.display();

            delay(800);
        }

        display.clearDisplay();
        printCentered("Roboting UC");
        display.display();
        spk->startupBeep();
    }

    void print(String message, int sz=1){
        display.setTextSize(sz);
        display.print(message);
    }

    
    void printCentered(String message, int sz=1, bool absolute=true){
        int aux = max(0, int(message.length()-10));
        if(absolute)
            display.setCursor(max(0, int(centerX-(message.length()*2)+18-aux)), centerY);
        else
            display.setCursor(max(0, int(centerX-(message.length()*2)+18-aux)), display.getCursorY());

        print(message, sz);
    }


    void printCenteredNumber(int number, int sz=2){
        display.setTextSize(sz);
        display.setCursor(centerX-(String(number).length()*5)+20, centerY);
        display.print(number);
    }


    void printCenteredTextNumber(String text, int number){
        display.clearDisplay();
        header(text);
        printCenteredNumber(number);
        display.setCursor(0, 50);
    }


    void printClock(int seconds, String message="Focus time!", bool screen_on=true){
        //Status message
        display.clearDisplay();
        header(message);

        if(screen_on){
            display.setCursor(centerX-5, centerY);
            print(format_time(seconds), 2);
        }
        
        display.display();
    }


    void showFace(int idx){
        idx = constrain(idx, 0, N_FACES-1);
        display.clearDisplay();
        display.drawBitmap(0, 0, Faces[idx], 128, 64, SH110X_WHITE);
        display.display();
    }


    void show(){
        display.display();
    }


    void clear(){
        display.clearDisplay();
    }

};


struct Led{
    int pin = 0;
    int brightness = 255;
    bool turned_on = false;

    Led(){}

    void init(int pin){
        this->pin = pin;
        pinMode(pin, OUTPUT);
    }

    void _update(){
        if(turned_on)
            analogWrite(pin, brightness);
        else
            analogWrite(pin, 0);
    }

    void toggle(){
        turned_on = !turned_on;
        _update();
    }

    void adjust(bool on_off){
        turned_on = on_off;
        _update();
    }

    void change_brightness(int brightness){
        this->brightness = brightness;
        _update();
    }

};







#endif