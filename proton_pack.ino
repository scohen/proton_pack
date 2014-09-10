#include <ProtonPack.h>
#include "pack_parts.h"

ProtonPack pack(8, 7);
Cyclotron cyclo(0);
Powercell cell(16);
Graph graph(32);

void setup(){
  Vector<PackComponent> components;
  pack.addComponent(&cyclo);
  pack.addComponent(&cell);
  pack.addComponent(&graph);
  Serial.begin(9600);
  pack.initialize();
}

void loop(){
  pack.update();
  delay(10);
}

