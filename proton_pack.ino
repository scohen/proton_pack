#include <Protonpack.h>
#include <tlc_fades.h>
#include <tlc_config.h>
#include <tlc_animations.h>
#include <tlc_progmem_utils.h>
#include <Tlc5940.h>
#include <tlc_servos.h>
#include <tlc_shifts.h>

#include "Tlc5940.h"
#include "Protonpack.h"

Powercell cell;
Cyclotron cyclotron;
int now = 0;

void init_pack(){
  cell.last_updated = 0;
  cell.current_led = 0;
  cell.initializing = true;
  cell.current_brightness = 0;
  
  cyclotron.last_updated = 0;
  cyclotron.current_led = -1;
  cyclotron.current_brightness = 0;
  cyclotron.fade_started = 0;
  cyclotron.fade_duration = 250;
}

void setup(){
  Tlc.init();
  init_pack();
}

void update_pack(){
  now = millis();
  update_powercell();
  //update_cyclotron();
}

int increment_to_max(int current, int max_value){
  int incremented = current += 1;
  if (incremented < max_value){
    return incremented;
  }
  return 0;
}

void update_cyclotron(){
  static int BRIGHTNESS_INCREMENT = 80;

  int last_updated = now - cell.last_updated;

  if (last_updated > 1000) {
    cyclotron.current_brightness = 0;
    cyclotron.current_led = increment_to_max(cyclotron.current_led, 4);
    for (int i=0; i < 4; i++){      
        Tlc.set(i, 0);              
    }
    cell.last_updated = now;    
  } else if ( last_updated < 500 ){
    cyclotron.current_brightness += BRIGHTNESS_INCREMENT;
    Tlc.set(cyclotron.current_led, cyclotron.current_brightness);    
  } else if (cyclotron.current_brightness > 0) {
    cyclotron.current_brightness -= BRIGHTNESS_INCREMENT;
    Tlc.set(cyclotron.current_led, cyclotron.current_brightness);
  }
}

void update_powercell(){
  int last_updated = now - cell.last_updated;
  
  if (cell.initializing){
    if (last_updated > 75){
      cell.current_brightness += 10;
      for(int i=0; i < cell.num_leds; i++){
        Tlc.set(i, cell.current_brightness);
      }
      cell.last_updated = millis();
      if (cell.current_brightness >= 1024){
        cell.initializing = false;
        cell.current_brightness = 0;
        for (int i=0; i < cell.num_leds; i++){
          Tlc.set(i, 0);
        }
      }
    }
    
  } else {
    if (last_updated > 60){    
      for (int i=0; i<= cell.num_leds; i++){
        int val = 0;
        if (i < cell.current_led){
          val = 100;
        } 
        Tlc.set(i, val);
  
      }
      cell.current_led = increment_to_max(cell.current_led, cell.num_leds + 1);
      cell.last_updated = millis();
    }   
  }
}

void loop(){
  Tlc.update();
  update_pack();
  Tlc.update();
  delay(10);
}
