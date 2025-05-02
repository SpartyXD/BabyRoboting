#include <objects.h>
//=====================================

//PINS
#define BUZZERPIN 10
#define REDLEDPIN 5
#define BLUELEDPIN 6

//===================================
//OBJECTS
Screen screen;
PassiveBuzzer speaker;
Led red, blue;

//===================================

#define DEBG_MODE false

void setup() {
    Serial.begin(115200);
    speaker.init(BUZZERPIN);
    screen.init(speaker);
    red.init(REDLEDPIN);
    blue.init(BLUELEDPIN);
    
    screen.loading_screen();
    red.toggle();
    delay(1000);
    blue.toggle();
    delay(2000);
}


void loop(){
  Serial.println("alo");
  screen.clear();
  screen.printCentered("Alo");
  screen.show();
  delay(1000);

}


