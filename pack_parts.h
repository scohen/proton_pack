#ifndef pack_parts_h
#define pack_parts_h

#include "Arduino.h"
#include <ProtonPack.h>

class Powercell: public PackComponent {
public:
    Powercell(int offset);
    void onUpdate(Pack pack);
    void onPackInitStart(Pack pack);
    void onPackInitComplete(Pack pack);
    void reset(Pack pack);

protected:
    const static int _num_leds = 14;
    const static int _UPDATE_RATE = 30;
    const static int _MAX_BRIGHTNESS = 2000;
    int _current_led;
    bool _initializing;
};

class Cyclotron: public PackComponent {
public:
    Cyclotron(int offset);
    int _last_switched_at;
    int _current_led;
    int _current_brightness;
    virtual void onUpdate(Pack pack);
    virtual void reset(Pack pack);
};

class Graph: public PackComponent {
  public:
    Graph(int offset);
    void onUpdate(Pack pack);
    void reset(Pack pack);
    void onPackInitStart(Pack pack);
    void onPackInitComplete(Pack pack);
    void onFiringStart(Pack pack);
    void onFiringStop(Pack pack);
    
  protected:
    const static int _num_leds = 15;
    int _iteration;
    int _direction;
    bool _is_firing;
    bool _is_initializing;
};

class Nozzle: public PackComponent {
public:
    Nozzle(int offset);
    void onUpdate(Pack pack);
    void reset(Pack pack);
};

#endif
