#include <ProtonPack.h>

ProtonPack pack(A0, A1, 16, 0, 32);

void setup(){
  pack.initialize();
  Serial.begin(9600);
}

void loop(){
  pack.update();
  delay(10);
}
