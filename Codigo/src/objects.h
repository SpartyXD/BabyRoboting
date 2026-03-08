#ifndef OBJECTS_H
#define OBJECTS_H

#include <Arduino.h>
#include <SPI.h>
#include <Wire.h>
#include <U8g2lib.h>

#include <faces.h>
#define rep(i, n) for(int i=0; i<n; i++)

//SCREEN
#define OLED_RESET -1   //   QT-PY / XIAO
#define i2c_Address 0x3C //initialize with the I2C addr 0x3C Typically eBay OLED's
#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels

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
        tone(pin, frec, dur);
        vTaskDelay(dur);
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
        vTaskDelay(100);
        beep(800, 300);
    }

    void successBeep(){
        beep(700, 100);
        vTaskDelay(50);
        beep(1000, 100);
        vTaskDelay(50);
        beep(1300, 100);
    }

    void gamblingBeep(){
        beep(700, 100);
        vTaskDelay(50);
        beep(1000, 100);
        vTaskDelay(200);
    }

    void sadBeep(){
        beep(1300, 100); 
        vTaskDelay(50);
        beep(1000, 100); 
        vTaskDelay(50);  
        beep(700, 100);
        vTaskDelay(50);
        beep(500, 200);
    }

    void celebrationBeep(){
        beep(1000, 200);
        vTaskDelay(300);
        beep(800, 300);
        vTaskDelay(300);
        beep(600, 300);    
    }

    void angryBeep(){
        beep(600, 100);
        vTaskDelay(50);
        beep(800, 100);
        vTaskDelay(200);  
    }

    void hornBeep(){
        beep(440, 150);
        delay(50);
        beep(440, 150);
    }

    void welcomeBeep(){
        beep(523, 100); 
        delay(50);      
        beep(659, 100); 
        delay(50);
        beep(784, 100); 
        delay(50);
        beep(1047, 200);
    }
};


class OledScreen{
private:
    uint8_t address = 0x3C;
    uint8_t WIDTH = 128;
    uint8_t HEIGHT = 64;
    uint8_t centerX = WIDTH/2;
    uint8_t centerY = HEIGHT/2;
    String model = "SSD1306";
    U8G2* display = nullptr;

    const int CHAR_HEIGHT = 8;
    const int SPACE = CHAR_HEIGHT/2;

public:
    //Constructors
    OledScreen(){}
    ~OledScreen(){
        //Free memory
        if (display != nullptr) {
            delete display;
            display = nullptr;
        }
    }

    void init(String model= "SSD1306", uint8_t address=0x3C, const uint8_t* font=u8g2_font_ncenB08_tr){
        this->model = model;
        this->address = address;
        if(model == "SSD1306")
            display = new U8G2_SSD1306_128X64_NONAME_F_HW_I2C(U8G2_R0, U8X8_PIN_NONE);
        else
            display = new U8G2_SH1106_128X64_NONAME_F_HW_I2C(U8G2_R0, U8X8_PIN_NONE);

        display->begin();
        display->setFont(font);
    }

    void setFont(const uint8_t* font){
        display->setFont(font);
    }

    void update(){
        display->sendBuffer();
    }

    void clear(){
        display->clearBuffer();
    }

    void moveCursor(int x=-1, int y=-1, bool add_x=false, bool add_y=false){
        int new_x = (x != -1) ? x : display->getCursorX();
        int new_y = (y != -1) ? y : display->getCursorY();

        if(add_x)
            new_x = display->getCursorX()+x;
        if(add_y)
            new_y = display->getCursorY()+y;
        
        new_x = constrain(new_x, 0, SCREEN_WIDTH);
        new_y = constrain(new_y, 0, SCREEN_HEIGHT);
        display->setCursor(new_x, new_y);
    }

    void print(String msg) {
        display->drawStr(display->getCursorX(), display->getCursorY(), msg.c_str());
    }

    void print(int number){
        print(String(number));
    }

    void printCentered(String message, bool absolute=true){
        int width = display->getStrWidth(message.c_str());

        int x = max(0, centerX-(width/2));
        int y = absolute ? centerY : display->getCursorY();

        moveCursor(x, y);
        print(message);
    }

    void printCentered(int number, bool absolute=true){
        printCentered(String(number), absolute);
    }

    void renderCenteredLine(String line, int y) {
        int width = display->getStrWidth(line.c_str());
        int x = max(0, (128 - width) / 2); // Centrado horizontal
        display->setCursor(x, y);
        display->print(line);
    }

    int countLines(String message) {
        if (message.length() == 0) return 0;
        int count = 1;
        for (int i = 0; i < message.length(); i++) {
            if (message[i] == '\n') count++;
        }
        return count;
    }

    void printCenteredWrapped(String message) {
        int lineHeight = 12;
        int cursorY = centerY - ((countLines(message) - 1) * lineHeight / 2); 
        
        String word = "";
        String line = "";
        int i = 0;

        while (i <= message.length()) {
            if (i == message.length() || message[i] == ' ' || message[i] == '\n') {
                String testLine = line + (line.length() > 0 ? " " : "") + word;
                
                if (display->getStrWidth(testLine.c_str()) > 128 || (i < message.length() && message[i] == '\n')) {
                    renderCenteredLine(line, cursorY);
                    cursorY += lineHeight;
                    line = word;
                } else {
                    line = testLine;
                }
                word = "";
            } else {
                word += message[i];
            }
            i++;
        }
        renderCenteredLine(line, cursorY);
    }

    void header(String title){
        moveCursor(0, CHAR_HEIGHT);
        printCentered(title, false);
        display->drawLine(0, CHAR_HEIGHT+SPACE, 128, CHAR_HEIGHT+SPACE);  
    }

    void loading_screen(){
        for(int i=0; i<=100; i+=20){
            clear();
            header("Cargando...");
            printCentered(String(i) + " %");
            update();

            vTaskDelay(250);
        }

        clear();
        printCentered("Roboting UC");
        update();
    }

    void printCenteredTextNumber(String text, int number){
        clear();
        header(text);
        printCentered(number);
    }


    void printClock(int seconds, String message="Focus time!", bool screen_on=true){
        //Status message
        clear();
        header(message);

        if(screen_on)
            printCentered(format_time(seconds));
        
        update();
    }

    void fillRect(int x1, int y1, int w, int h){
        display->drawBox(x1, y1, w, h);
    }

    void fillTriangle(int x0, int y0, int x1, int y1, int x2, int y2){
        display->drawTriangle(x0, y0, x1, y1, x2, y2);
    }

    void fillCircle(int x, int y, int r){
        display->drawDisc(x, y, r);
    }

    void showFace(int idx){
        idx = constrain(idx, 0, N_FACES-1);
        clear();
        display->drawBitmap(0, 0, 16, 64, Faces[idx]);
        update();
    }
};


struct Led{
    int pin = 0;
    int brightness = 245;
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

    void turn(bool on_off){
        turned_on = on_off;
        _update();
    }

    void change_brightness(int brightness){
        this->brightness = brightness;
        _update();
    }

};







#endif