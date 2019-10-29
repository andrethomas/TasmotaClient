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
    uint16_t spare3 : 1;
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

TasmotaSlave::TasmotaSlave(HardwareSerial *device, uint32_t baud)
{
  serial = device;
  serial->begin(baud);
  serial->setTimeout(50);
  Settings.features_version = TASMOTA_SLAVE_LIB_VERSION;
  Settings.features.func_json_append = 0;
  Settings.features.func_every_second = 0;
  Settings.features.func_every_100_msecond = 0;
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

void TasmotaSlave::ProcessCommand(void)
{
  char buffer[sizeof(Command)];
  uint8_t len = serial->readBytesUntil(char(CMND_END), buffer, sizeof(buffer));
  if (len == sizeof(Command)) {
    memcpy(&Command, &buffer, sizeof(Command));
    switch (Command.command) {
      case CMND_FEATURES:
        sendFeatures();
        break;
      case CMND_FUNC_JSON:
        if (Settings.features.func_json_append) {
          FUNC_JSON();
        }
        break;
      case CMND_FUNC_EVERY_SECOND:
        if (Settings.features.func_every_second) {
          FUNC_EVERY_SECOND();
        }
	  case CMND_FUNC_EVERY_100_MSECOND:
	    if (Settings.features.func_every_100_msecond) {
          FUNC_EVERY_100_MSECOND();
		}
        break;
      default:
        break;
    }
  }
}

void TasmotaSlave::process(void)
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
