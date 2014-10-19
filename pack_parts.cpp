#include <ProtonPack.h>
#include "pack_parts.h"


Powercell::Powercell(int offset) : PackComponent(offset, 15) {
}

void Powercell::onPackInitStart(Pack pack) {
  _initializing = true;
}

void Powercell::onPackInitComplete(Pack pack) {
  _initializing = false;
}

void Powercell::onUpdate(Pack pack) {
    if (_initializing) {
        for(int i=0; i < _num_leds; i++) {
            if (i == _current_led) {
                setLed(i, _MAX_BRIGHTNESS);
            } else {
                setLed(i, 0);
            }

        }
        if (_current_led > 0) {
            setLed(_current_led, _MAX_BRIGHTNESS / 2);
        }

        if (_current_led > 1) {
            setLed(_current_led, _MAX_BRIGHTNESS / 4);
        }
    } else {
        for (int i=0; i<= _num_leds; i++) {
            if (i >= _current_led) {
              setLed(i, _MAX_BRIGHTNESS);
            } else {
              setLed(i, 0);
            }
        }
    } 
    _current_led = pp_decrement(_current_led, _num_leds - 1);
    if ( _current_led == 0 ) {
      callAgainIn(120);
    } else {
      callAgainIn(40);
    }
}

void Powercell::reset(Pack pack) {
    _current_led = 0;
    _initializing = true;
    for(int i=0; i < _num_leds; i++) {
        setLed(i, 0);
    }
}

Cyclotron::Cyclotron(int offset) : PackComponent(offset, 14) {
  _last_switched_at = millis();
}

void Cyclotron::onUpdate(Pack pack) {
    static int BRIGHTNESS_INCREMENT = 80;
    int since_last_switched = pack.now - _last_switched_at;
    if (since_last_switched >= 1000) {
        _current_brightness = 0;
        setLed(_current_led, _current_brightness);
        _current_led = pp_mod_increment(_current_led, 4);
        _last_switched_at = pack.now;
    } else if ( since_last_switched <= 500 ) {
        _current_brightness += BRIGHTNESS_INCREMENT;
    } else if (_current_brightness > 0) {
        _current_brightness -= BRIGHTNESS_INCREMENT;
    }
    
    setLed(_current_led, _current_brightness);    
    callAgainIn(20);
   
}

void Cyclotron::reset(Pack pack) {
    _current_led = 0;
    _current_brightness = 0;
    _last_switched_at = pack.now;
    for (int i=0; i < 4; i++) {
        setLed(i, 0);
    }
}


Graph::Graph(int offset) : PackComponent(offset, 15) {
  _direction = 1;
}

void Graph::onPackInitStart(Pack pack){
  _iteration = _num_leds - 1;
  _fill_pos = 0;
  _is_initializing = true;
  Serial.println("GRAPH INIT");
}
void Graph::onPackInitComplete(Pack pack){
  reset(pack);
}
void Graph::onFiringStart(Pack pack){
  reset(pack);
}
void Graph::onFiringStop(Pack pack){
  reset(pack);
}

void Graph::onUpdate(Pack pack) {
    const int MAX_LEDS_ON_IDLE = 14;
    const int BRIGHT = 4000;
    const int O = 1000;
    const int o = BRIGHT;
    const int _ = 0;
    const int firing_sequence[][15] = {
        {_, _, _, _, _, _, _, O, _, _, _, _, _, _, _},
        {_, _, _, _, _, _, o, O, o, _, _, _, _, _, _},
        {_, _, _, _, _, o, O, _, O, o, _, _, _, _, _},
        {_, _, _, _, o, O, _, _, _, O, o, _, _, _, _},
        {_, _, _, o, O, _, _, _, _, _, O, o, _, _, _},
        {_, _, o, O, _, _, _, _, _, _, _, O, o, _, _},
        {_, o, O, _, _, _, _, _, _, _, _, _, O, o, _},
        {o, O, _, _, _, _, _, _, _, _, _, _, _, O, o},
    };
 
    if(pack.is_firing) {
        for(int i=0; i < _num_leds; i++) {
            setLed(i, firing_sequence[_iteration][i]);
        }
        _iteration = pp_mod_increment(_iteration, 7);
        callAgainIn(40);
    } else if (_is_initializing) {
      if ( _fill_pos == _num_leds - 1) {
        callAgainIn(50);
        return;
      }
      
      for(int i=0; i < _fill_pos; i++) {
        setLed(i, BRIGHT);
      }
      setLed(_iteration, BRIGHT);
      setLed(_iteration + 1, 0);
      _iteration = pp_decrement(_iteration, _num_leds - 1);
      if(_iteration == _fill_pos) {
        _fill_pos = pp_mod_increment(_fill_pos, _num_leds - 1);
        _iteration = _num_leds - 1;
      }
      callAgainIn(44);
    } else {
        for(int i=0; i < _num_leds; i++) {
          if (i < _iteration) { 
              setLed(i, BRIGHT);
           } else {
              setLed(i, 0);
           }
        }
        _iteration += _direction;
        if (_iteration == 0) {
          _direction = 1;
        } else if (_iteration == MAX_LEDS_ON_IDLE) {
           _direction = -1;
        }
        
        callAgainIn(50);
    } 
}

void Graph::reset(Pack pack) {
    _iteration = 0;
    _direction = 1;
    _is_initializing = false;
    _fill_pos = 0;
    for(int i=0; i < _num_leds; i++) {
        setLed(i, 0);
    }
}

Nozzle::Nozzle(int offset) : PackComponent(offset, 1) {
}

void Nozzle::onFiringStart(Pack pack) {
  _is_firing = true;
}

void Nozzle::onFiringStop(Pack pack) {
  _is_firing = false;
  setLed(0, 0);
}

void Nozzle::onUpdate(Pack pack) {
  if ( _is_firing) {
    if (random(0, 100) >= 45) {
      setLed(0, 4000);
    } else { 
      setLed(0, 0);
    }
  }
  callAgainIn(50);
}

void Nozzle::reset(Pack pack) {
    _is_firing = false;
}


WandLights::WandLights(int offset) : PackComponent(offset, 4) {

}

void WandLights::reset(Pack pack) {
  _is_on = false;
  _is_firing = false;
  _is_initializing = true;
  setLed(SLO_BLO, 0);
  setLed(TOP_INDICATOR, 1000);
  setLed(FRONT_LIGHT, 1000);
  setLed(INTERNAL_LIGHT, 4095);
}

void WandLights::onFiringStart(Pack pack) {
  _is_firing = true;
  callAgainIn(1);
}

void WandLights::onFiringStop(Pack pack) {
  _is_firing = false;
}

void WandLights::onPackInitStart(Pack pack) {
  _is_initializing = true;

}
void WandLights::onPackInitComplete(Pack pack){
  _is_initializing = false;
}

void WandLights::onUpdate(Pack pack) {
  if (_is_initializing) {
    setLed(SLO_BLO, 0);
    callAgainIn(200); 
  } else {
    if (_is_on) {
      setLed(SLO_BLO, 0);
    } else {
      setLed(SLO_BLO, 400);
    }
    _is_on = !_is_on;
    if (_is_firing) {
      callAgainIn(500);
    } else if (_is_on) {
      callAgainIn(2500);
    } else {
      callAgainIn(250);
    }
  }
}

Sound::Sound(): PackListener() {
  pinMode(POWER_RELAY, OUTPUT);
  pinMode(FIRING_START_RELAY, OUTPUT);
  pinMode(FIRING_STOP_RELAY, OUTPUT);
    digitalWrite(POWER_RELAY, HIGH);
  digitalWrite(FIRING_START_RELAY, HIGH);
  digitalWrite(FIRING_STOP_RELAY, HIGH);
}

void Sound::reset(Pack pack) {
  digitalWrite(POWER_RELAY, HIGH);
  digitalWrite(FIRING_START_RELAY, HIGH);
  digitalWrite(FIRING_STOP_RELAY, HIGH);
}

void Sound::onFiringStart(Pack pack) {
  digitalWrite(FIRING_START_RELAY, LOW);
  digitalWrite(FIRING_STOP_RELAY, HIGH);
}
 
void Sound::onFiringStop(Pack pack) {
  digitalWrite(FIRING_START_RELAY, HIGH);
  digitalWrite(FIRING_STOP_RELAY, LOW);
}
  
void Sound::onPackInitStart(Pack pack) {
  digitalWrite(POWER_RELAY, LOW);
}

void Sound::onPackInitComplete(Pack pack) {
  
}

