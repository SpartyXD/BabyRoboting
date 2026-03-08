#include <objects.h>
//=====================================

//PINS
#define BUZZERPIN 7
#define REDLEDPIN 4
#define BLUELEDPIN 5

//===================================
//OBJECTS
OledScreen screen;
PassiveBuzzer speaker;
Led red, blue;

//===================================
//GLOBALS
unsigned long time_now=0, last_face_change=0, last_led_change=0;
int FACE_IDX = 0;

#define RANDOM_FACE_DELAY 10000
#define RANDOM_LED_DELAY 7000
#define TOGGLE_CHANCE 0.3

void startup_leds(void *params);
void change_face();
void change_led();
void receiveData();

//===================================

void setup(){
    Serial.begin(115200);
    speaker.init(BUZZERPIN);
    screen.init();
    red.init(REDLEDPIN);
    blue.init(BLUELEDPIN);

    xTaskCreatePinnedToCore(startup_leds, "Start", 2048, NULL, 1, NULL, 0);

    screen.loading_screen();
    speaker.welcomeBeep();
    vTaskDelay(1500);
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
  int red_brightness = random(60, 200);
  int blue_brightness = random(60, 200);

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
  
  screen.clear();
  String message = Serial.readStringUntil('\n');
  screen.printCenteredWrapped(message);
  screen.update();
  speaker.successBeep();
  vTaskDelay(3000);
}


void startup_leds(void *params){
  blue.change_brightness(0);
  red.change_brightness(0);

  vTaskDelay(250);

  blue.change_brightness(0);
  red.change_brightness(255);

  vTaskDelay(250);

  blue.change_brightness(255);
  red.change_brightness(255);

  vTaskDelay(250);

  blue.change_brightness(0);
  red.change_brightness(0);

  vTaskDelay(250);

  blue.change_brightness(255);
  red.change_brightness(255);

  vTaskDelete(NULL);
}