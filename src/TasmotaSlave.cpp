/*
  TasmotaSlave.cpp - Library for microcontrollers enslaved by Tasmota
  
  Copyright (C) 2019  Andre Thomas
  
  This program is free software: you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation, either version 3 of the License, or
  (at your option) any later version.

  This program is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#include <Arduino.h>
#include <TasmotaSlave.h>

/*************************************************\
 * TasmotaSlave shared structures
\*************************************************/

typedef union {
  uint16_t data;
  struct {
    uint16_t func_json_append : 1;               // Supports FUNC_JSON_APPEND callback
    uint16_t func_every_second : 1;              // Supports FUNC_EVERY_SECOND callback (No JSON)
    uint16_t func_every_100_msecond : 1;         // Supports FUNC_EVERY_100_MSECOND callback (No JSON)
    uint16_t func_command_send : 1;
    uint16_t spare4 : 1;
    uint16_t spare5 : 1;
    uint16_t spare6 : 1;
    uint16_t spare7 : 1;
    uint16_t spare8 : 1;
    uint16_t spare9 : 1;
    uint16_t spare10 : 1;
    uint16_t spare11 : 1;
    uint16_t spare12 : 1;
    uint16_t spare13 : 1;
    uint16_t spare14 : 1;
    uint16_t spare15 : 1;
  };
} FeatureCfg;

struct FEATURES {
  uint32_t features_version;
  FeatureCfg features;
  uint16_t spare4;
} Settings;

struct COMMAND {
  uint8_t command;
  uint8_t parameter;
  uint8_t unused2;
  uint8_t unused3;
} Command;

TasmotaSlave::TasmotaSlave(HardwareSerial *device)
{
  serial = device;
  Settings.features_version = TASMOTA_SLAVE_LIB_VERSION;
  Settings.features.func_json_append = 0;
  Settings.features.func_every_second = 0;
  Settings.features.func_every_100_msecond = 0;
  Settings.features.func_command_send = 0;
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
}

void TasmotaSlave::sendFeatures(void)
{
  char buffer[sizeof(Settings)];
  memcpy(&buffer, &Settings, sizeof(Settings));
  serial->write(char(PARAM_DATA_START));
  for (uint8_t ca = 0; ca < sizeof(buffer); ca++) {
    serial->write(char(buffer[ca]));
  }
  serial->write(char(PARAM_DATA_END));
}

void TasmotaSlave::sendJSON(char *json)
{
  serial->write(char(PARAM_DATA_START));
  for (uint8_t ca = 0; ca < strlen(json); ca++) {
    serial->write(json[ca]);
  }
  serial->write(char(PARAM_DATA_END));
}

void TasmotaSlave::attach_FUNC_JSON(callbackFunc func)
{
  Settings.features.func_json_append = 1;
  FUNC_JSON = func;
}

void TasmotaSlave::attach_FUNC_EVERY_SECOND(callbackFunc func)
{
  Settings.features.func_every_second = 1;
  FUNC_EVERY_SECOND = func;
}

void TasmotaSlave::attach_FUNC_EVERY_100_MSECOND(callbackFunc func)
{
  Settings.features.func_every_100_msecond = 1;
  FUNC_EVERY_100_MSECOND = func;
}

void TasmotaSlave::attach_FUNC_COMMAND_SEND(callbackFunc func)
{
  Settings.features.func_command_send = 1;
  FUNC_SEND = func;
}

uint8_t TasmotaSlave::waitforbytes(uint16_t num, uint16_t timeout)
{
  uint16_t timer = 0;
  while (timeout >= timer) {
    if (serial->available() >= num) {
      return 1;
    }
    delay(1);
    timer++;
  }
  return 0;
}

void TasmotaSlave::ProcessSend(uint8_t sz)
{
  if (waitforbytes(sz+2,50)) {
    serial->read(); // read leading character
    for (uint8_t idx = 0; idx < sz; idx++) {
      receive_buffer[idx] = serial->read();
    }
    serial->read(); // read trailing byte
    receive_buffer[sz] = '\0';
    FUNC_SEND();
  }
}


void TasmotaSlave::ProcessCommand(void)
{
  if (waitforbytes(sizeof(Command)+1, 100)) {
    char buffer[sizeof(Command)];
    for (uint8_t idx = 0; idx < sizeof(Command); idx++) {
      buffer[idx] = serial->read();
    }
    serial->read(); // Remove end of command character
    memcpy(&Command, &buffer, sizeof(Command));
    switch (Command.command) {
      case CMND_FEATURES:
        sendFeatures();
        break;
      case CMND_FUNC_JSON:
        FUNC_JSON();
        break;
      case CMND_FUNC_EVERY_SECOND:
        FUNC_EVERY_SECOND();
        break;
      case CMND_FUNC_EVERY_100_MSECOND:
        FUNC_EVERY_100_MSECOND();
        break;
      case CMND_COMMAND_SEND:
        ProcessSend(Command.parameter);
        break;
      default:
        break;
    }
  }
}

void TasmotaSlave::loop(void)
{
  if (serial->available()) {
    uint8_t cmnd = serial->read();
    switch (cmnd) {
      case CMND_START:
        ProcessCommand();
        break;
      default:
        break;
    }
  }
}
