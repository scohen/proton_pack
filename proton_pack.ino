#include <ProtonPack.h>
#include "pack_parts.h"

ProtonPack pack(8, 7);
Cyclotron cyclo(0);
Powercell cell(16);
Graph graph(32);
Nozzle nozzle(48);
WandLights wand(49);

void setup(){
  Serial.begin(9600);
  pack.addComponent(&cyclo);
  pack.addComponent(&cell);
  pack.addComponent(&graph);
  pack.addComponent(&nozzle);
  pack.addComponent(&wand);
  pack.initialize();
}

void loop(){
  pack.update();
}

