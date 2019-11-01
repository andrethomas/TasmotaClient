#include <Arduino.h>
#include <TasmotaSlave.h>

TasmotaSlave slave(&Serial, 57600);

bool ledstate = false;

void user_FUNC_EVERY_SECOND(void)
{
  if (ledstate) {
    ledstate = false;
    digitalWrite(LED_BUILTIN, LOW);
  } else {
    ledstate = true;
    digitalWrite(LED_BUILTIN, HIGH);
  }
}

void setup() {
  Serial.begin(57600);
  pinMode(LED_BUILTIN, OUTPUT);
  slave.attach_FUNC_EVERY_SECOND(user_FUNC_EVERY_SECOND);
}

void loop() {
  slave.process();
}
