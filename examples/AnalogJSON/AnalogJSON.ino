#include <Arduino.h>
#include <TasmotaSlave.h>

TasmotaSlave slave(&Serial, 57600);

void user_FUNC_JSON(void)
{
  uint8_t a = 0;
  char myjson[100];
  sprintf(myjson,"{\"A0\": %u, \"A0\": %u, \"A0\": %u, \"A0\": %u, \"A0\": %u, \"A0\": %u, \"A0\": %u, \"A0\": %u}", analogRead(A0), analogRead(A1), analogRead(A2), analogRead(A3), analogRead(A4), analogRead(A5), analogRead(A6), analogRead(A7));
  slave.sendJSON(myjson);
}

void setup() {
  Serial.begin(57600);
  pinMode(LED_BUILTIN, OUTPUT);
  slave.attach_FUNC_JSON(user_FUNC_JSON);
}

void loop() {
  slave.process();
}
