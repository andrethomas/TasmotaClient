/*
  TasmotaSlave.h - Library for microcontrollers enslaved by Tasmota
  
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

#ifndef __TASMOTASLAVE_H__
#define __TASMOTASLAVE_H__

#include <Arduino.h>

/*************************************************\
 * TasmotaSlave Configuration Defaults
\*************************************************/

#define TASMOTA_SLAVE_LIB_VERSION      20191026

/*************************************************\
 * TasmotaSlave Command definitions
\*************************************************/

#define CMND_START                     0xFC
#define CMND_END                       0xFD

#define CMND_FEATURES                  0x01
#define CMND_FUNC_JSON                 0x02
#define CMND_FUNC_EVERY_SECOND         0x03
#define CMND_FUNC_EVERY_100_MSECOND    0x04

/*************************************************\
 * TasmotaSlave Parameter defintions
\*************************************************/

#define PARAM_DATA_START               0xFE
#define PARAM_DATA_END                 0xFF

/*************************************************\
 * TasmotaSlave Class
\*************************************************/

typedef void (*callbackFunc) (void);

class TasmotaSlave {
    public:
     TasmotaSlave(HardwareSerial *device, uint32_t baud);
     void sendFeatures(void);
     void sendJSON(char *json);
     void attach_FUNC_JSON(callbackFunc func = nullptr);
     void attach_FUNC_EVERY_SECOND(callbackFunc func = nullptr);
     void attach_FUNC_EVERY_100_MSECOND(callbackFunc func = nullptr);
     void ProcessCommand(void);
     void process(void);
    private:
     HardwareSerial *serial;
     callbackFunc FUNC_JSON;
     callbackFunc FUNC_EVERY_SECOND;
     callbackFunc FUNC_EVERY_100_MSECOND;
};

#endif // __TASMOTASLAVE_H__
