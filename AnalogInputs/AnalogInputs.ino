/*
 * Provides all 8 analog inputs on an UNO / Pro Mini via serial
 */

#include "interface.h"
#include "settings.h"

bool ledstate = false;

void setup() {
  pinMode(LED_BUILTIN, OUTPUT);
  Settings.features_version = 20191026;
  Settings.features.json = 1;
  Settings.features.spare1 = 0;
  Settings.features.spare2 = 0;
  Settings.features.spare3 = 0;
  Settings.features.spare4 = 0;
  Settings.features.spare5 = 0;
  Settings.features.spare6 = 0;
  Settings.features.spare7 = 0;
  Settings.features.spare8 = 0;
  Settings.features.spare9 = 0;
  Settings.features.spare10 = 0;
  Settings.features.spare11 = 0;
  Settings.features.spare12 = 0;
  Settings.features.spare13 = 0;
  Settings.features.spare14 = 0;
  Settings.features.spare15 = 0;
  Serial.begin(57600);
}

void SendFeatures(void)
{
  char buffer[sizeof(Settings)];
  memcpy(&buffer, &Settings, sizeof(Settings));
  Serial.write(char(PARAM_DATA_START));
  for (uint8_t ca = 0; ca < sizeof(buffer); ca++) {
    Serial.write(char(buffer[ca]));
  }
  Serial.write(char(PARAM_DATA_END));
}

void SendJSON(void)
{
  uint16_t a0 = analogRead(A0);
  uint16_t a1 = analogRead(A1);
  uint16_t a2 = analogRead(A2);
  uint16_t a3 = analogRead(A3);
  uint16_t a4 = analogRead(A4);
  uint16_t a5 = analogRead(A5);
  uint16_t a6 = analogRead(A6);
  uint16_t a7 = analogRead(A7);
  String json = (String)char(PARAM_DATA_START)+"{\"A0\":" + a0 + ",\"A1\":"+a1 + ",\"A2\":"+a2 + ",\"A3\":"+a3 + ",\"A4\":"+a4 + ",\"A5\":"+a5 + ",\"A6\":"+a6 + ",\"A7\":"+a7 + "}"+char(PARAM_DATA_END);
  Serial.println(json);
}

void ProcessCommand(void)
{
  char buffer[sizeof(Command)];
  uint8_t len = Serial.readBytesUntil(char(CMND_END), buffer, sizeof(buffer));
  if (len == sizeof(Command)) {
    memcpy(&Command, &buffer, sizeof(Command));
    switch (Command.command) {
      case CMND_FEATURES:
             SendFeatures();
             break;
      case CMND_JSON:
             SendJSON();
             break;
      default:
             break;
    }
  }
}

void loop() {
  if (Serial.available()) {
    uint8_t cmnd = Serial.read();
    switch (cmnd) {
      case CMND_START:
        ProcessCommand();
        break;
      default:
        break;
    }
  }
}
