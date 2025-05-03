#include <objects.h>
//=====================================

//PINS
#define BUZZERPIN 10
#define REDLEDPIN 6
#define BLUELEDPIN 5

//===================================
//OBJECTS
Screen screen;
PassiveBuzzer speaker;
Led red, blue;

//===================================
//GLOBALS
unsigned long time_now=0, last_face_change=0, last_led_change=0;
int FACE_IDX = 0;

#define RANDOM_FACE_DELAY 10000
#define RANDOM_LED_DELAY 6300
#define TOGGLE_CHANCE 0.5


void change_face();
void change_led();
void receiveData();

//===================================

void setup() {
    Serial.begin(115200);
    speaker.init(BUZZERPIN);
    screen.init(speaker);
    red.init(REDLEDPIN);
    blue.init(BLUELEDPIN);

    //Loading screen
    for(int i=0; i<=100; i+=20){
      screen.clear();
      blue.adjust(i > 40);
      red.adjust(i == 100);
      screen.header("Cargando...");
      display.setCursor(screen.centerX-(String(i).length()*5)+15, screen.centerY);
      screen.print(String(i), 2);
      screen.print("%", 2);
      screen.show();

      delay(800);
    }

    screen.clear();
    screen.printCentered("Roboting UC");
    screen.show();
    speaker.startupBeep();
    delay(1000);
    screen.showFace(IDLE);
}


void loop(){
  receiveData();

  //Check if time has passed
  time_now = get_time();

  if(time_now - last_face_change >= RANDOM_FACE_DELAY){
    change_face();
    last_face_change = time_now;
  }

  if(time_now - last_led_change >= RANDOM_LED_DELAY){
    change_led();
    last_led_change = time_now;
  }
}

//=======================================================

void change_face(){
  //Change face
  int new_idx = FACE_IDX;
  while(new_idx == FACE_IDX)
    new_idx = random(0, N_FACES);
  
  FACE_IDX = new_idx;
  screen.showFace(FACE_IDX);
}


void change_led(){
  //Change both leds to a random state
  int red_brightness = random(60, 255);
  int blue_brightness = random(60, 255);

  red.change_brightness(red_brightness);
  blue.change_brightness(blue_brightness);

  if(red.turned_on || blue.turned_on){
    //Red toggle
    if(random(1, 101) <= int(100*TOGGLE_CHANCE))
      red.toggle();

    //Blue toggle
    if(random(1, 101) <= int(100*TOGGLE_CHANCE))
      blue.toggle();
  }
  else{
    red.toggle();
    blue.toggle();
  }
}


void receiveData(){
  if(!Serial.available())
    return;
  
  String message = Serial.readStringUntil('\n');
  screen.printCentered(message);
  delay(3000);
}