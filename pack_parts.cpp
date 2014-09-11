#include <ProtonPack.h>
#include "pack_parts.h"


Powercell::Powercell(int offset) : PackComponent(offset) {
}

void Powercell::onPackInitStart(Pack pack) {
  _initializing = true;
}

void Powercell::onPackInitComplete(Pack pack) {
  Serial.println("init stop");
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
        _current_led = pp_decrement(_current_led, _num_leds - 1);
        _current_led %= _num_leds;        
    } else {
        if (_current_led == _num_leds - 1) {
            for (int i=0; i<= _num_leds; i++) {
                setLed(i, 0);
            }
        }
        setLed(_current_led, _MAX_BRIGHTNESS);
        _current_led = pp_decrement(_current_led, _num_leds - 1);        
    }
    callAgainIn(40);
}

void Powercell::reset(Pack pack) {
    _last_updated = 0;
    _current_led = 0;
    _initializing = true;
    for(int i=0; i < _num_leds; i++) {
        setLed(i, 0);
    }
}

Cyclotron::Cyclotron(int offset) : PackComponent(offset) {
  _last_switched_at = millis();
}

void Cyclotron::onUpdate(Pack pack) {
    static int BRIGHTNESS_INCREMENT = 83;
    int since_last_switched = pack.now - _last_switched_at;
    if (since_last_switched >= 1000) {
        _current_brightness = 0;
        _current_led = pp_increment_to_max(_current_led, 4);
        _last_switched_at = pack.now;
    } else if ( since_last_switched < 500 ) {
        _current_brightness += BRIGHTNESS_INCREMENT;
    } else if (_current_brightness > 0) {
        _current_brightness -= BRIGHTNESS_INCREMENT;
    }
    setLed(_current_led, _current_brightness);    
    callAgainIn(20);
   
}

void Cyclotron::reset(Pack pack) {
    _last_updated = 0;
    _current_led = 0;
    _current_brightness = 0;
    _last_switched_at = pack.now;
    for (int i=0; i < 4; i++) {
        setLed(i, 0);
    }
}


Graph::Graph(int offset) : PackComponent(offset) {
  _direction = 1;
}

    void Graph::onPackInitStart(Pack pack){}
    void Graph::onPackInitComplete(Pack pack){}
    void Graph::onFiringStart(Pack pack){}
    void Graph::onFiringStop(Pack pack){}

void Graph::onUpdate(Pack pack) {

    int O = 1000;
    int o = 4000;
    int _ = 0;
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
    const int normal_sequence[][15] = {
        {_, _, _, _, _, _, _, _, _, _, _, _, _, _, _},
        {o, _, _, _, _, _, _, _, _, _, _, _, _, _, _},
        {O, o, _, _, _, _, _, _, _, _, _, _, _, _, _},
        {O, O, o, _, _, _, _, _, _, _, _, _, _, _, _},
        {O, O, O, o, _, _, _, _, _, _, _, _, _, _, _},
        {O, O, O, O, o, _, _, _, _, _, _, _, _, _, _},
        {O, O, O, O, O, o, _, _, _, _, _, _, _, _, _},
        {O, O, O, O, O, O, o, _, _, _, _, _, _, _, _},
        {O, O, O, O, O, O, O, o, _, _, _, _, _, _, _},
        {O, O, O, O, O, O, O, O, o, _, _, _, _, _, _},    
    };

    if(pack.is_firing) {
        for(int i=0; i < _num_leds; i++) {
            setLed(i, firing_sequence[_iteration][i]);
        }
        _iteration++;
        _iteration %= 8;
        callAgainIn(40);
    } else {
        for(int i=0; i < _num_leds; i++) {
            setLed(i,normal_sequence[_iteration][i]);
        }
        _iteration += _direction;
        if (_iteration == 0) {
          _direction = 1;
        } else if (_iteration == 8) {
          _direction = -1;
        }
        callAgainIn(60);
    }
}

void Graph::reset(Pack pack) {
    _last_updated = 0;
    _iteration = 0;
    for(int i=0; i < _num_leds; i++) {
        setLed(i, 0);
    }
}

Nozzle::Nozzle(int offset) : PackComponent(offset) {
}

void Nozzle::onUpdate(Pack pack) {
    setLed(0, 4000);
}

void Nozzle::reset(Pack pack) {
    _last_updated = 0;
    setLed(0, 0);
}
