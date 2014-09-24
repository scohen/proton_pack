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
    int _fill_pos;
    bool _is_firing;
    bool _is_initializing;
};

class Nozzle: public PackComponent {
public:
    Nozzle(int offset);
    void onUpdate(Pack pack);
    void onFiringStart(Pack pack);
    void onFiringStop(Pack pack);
    void reset(Pack pack);
protected:
  bool _is_firing;
  
};

class WandLights: public PackComponent {
public:
  WandLights(int offset);
  void onUpdate(Pack pack);
  void onFiringStart(Pack pack);
  void onFiringStop(Pack pack);
  void onPackInitStart(Pack pack);
  void onPackInitComplete(Pack pack);
  void reset(Pack pack);
  
protected:
  const static int SLO_BLO = 0;
  const static int TOP_INDICATOR = 1;
  const static int FRONT_LIGHT = 2;
  const static int INTERNAL_LIGHT = 3;
  bool _is_on;
  bool _is_initializing;
  bool _is_firing;
  
};

#endif
