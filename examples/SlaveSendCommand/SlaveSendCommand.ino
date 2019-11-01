#include <Arduino.h>
#include <TasmotaSlave.h>

TasmotaSlave slave(&Serial);

void setup() {
  pinMode(LED_BUILTIN, OUTPUT);
  Serial.begin(57600);
  slave.attach_FUNC_JSON(user_FUNC_JSON);
  slave.attach_FUNC_COMMAND_SEND(user_FUNC_RECEIVE);
}

void user_FUNC_RECEIVE(void)
{
  if (!strcmp(slave.receive_buffer, "ON")) {
    digitalWrite(LED_BUILTIN, HIGH);
  }
  if (!strcmp(slave.receive_buffer, "OFF")) {
    digitalWrite(LED_BUILTIN, LOW);
  }
}

void user_FUNC_JSON(void)
{
  uint8_t a = 0;
  char myjson[100];
  sprintf(myjson,"{\"A0\": %u, \"A1\": %u, \"A2\": %u, \"A3\": %u, \"A4\": %u, \"A5\": %u, \"A6\": %u, \"A7\": %u}", analogRead(A0), analogRead(A1), analogRead(A2), analogRead(A3), analogRead(A4), analogRead(A5), analogRead(A6), analogRead(A7));
  slave.sendJSON(myjson);
}

void loop() {
  slave.loop();
}
