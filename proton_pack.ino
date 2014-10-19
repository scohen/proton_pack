#include <ProtonPack.h>
#include "pack_parts.h"

ProtonPack pack(8, 7);
Powercell cell(0);
Cyclotron cyclo(17);
Graph graph(32);
Nozzle nozzle(48);
WandLights wand(49);
Sound sound;

void setup(){
  Serial.begin(9600);
  pack.addComponent(&cell);
  pack.addComponent(&cyclo);
  pack.addComponent(&graph);
  pack.addComponent(&nozzle);
  pack.addComponent(&wand);
  pack.addComponent(&sound);
  pack.initialize();
}

void loop(){
  pack.update();
}

