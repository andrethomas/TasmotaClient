#include <Arduino.h>
#include <TasmotaSlave.h>

TasmotaSlave slave(&Serial);

void setup() {
  pinMode(LED_BUILTIN, OUTPUT);
  Serial.begin(57600);
  slave.attach_FUNC_COMMAND_SEND(user_FUNC_RECEIVE);
}

void user_FUNC_RECEIVE(char *data)
{
  if (!strcmp(data, "ON")) {
    digitalWrite(LED_BUILTIN, HIGH);
    char response[20];
    sprintf(response,"{\"LED\":\"ON\"}");
    slave.SendTele(response);
  }
  if (!strcmp(data, "OFF")) {
    digitalWrite(LED_BUILTIN, LOW);
    char response[20];
    sprintf(response,"{\"LED\":\"OFF\"}");
    slave.SendTele(response);
  }
}

void loop() {
  slave.loop();
}
