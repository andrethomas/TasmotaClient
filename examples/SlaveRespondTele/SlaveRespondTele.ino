/*
  SlaveRespondTele.ino - Example for TasmotaSlave to respond to SlaveSend ON and
                         SlaveSend OFF commands via console or telemetry.
                         In this example the ON and OFF is case sensitive so needs
                         to be sent in capital letters from the Tasmota device.
                         Upon receiving ON/OFF the slave will turn the LED on/off
                         respectively and respond with a telemetry message sent
                         back to Tasmota that will be published for telemetry
                         and rules processing on the Tasmota device.

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

TasmotaSlave slave(&Serial);

/*******************************************************************\
 * Normal setup() function for Arduino to configure the serial port
 * speed (which should match what was configured in Tasmota) and
 * attach any callback functions associated with specific requests
 * or commands that are sent by Tasmota.
\*******************************************************************/

void setup() {
  // Configure the LED pin as OUTPUT
  pinMode(LED_BUILTIN, OUTPUT);
  // Configure the serial port for the correct baud rate
  Serial.begin(57600);
  // Attach the callback function which will be called when Tasmota requests it
  slave.attach_FUNC_COMMAND_SEND(user_FUNC_RECEIVE);
}

/*******************************************************************\
 * Function which will be called when Tasmota sends a
 * SlaveSend command
\*******************************************************************/

void user_FUNC_RECEIVE(char *data)
{
  if (!strcmp(data, "ON")) { // SlaveSend ON
    digitalWrite(LED_BUILTIN, HIGH);
    char response[20];
    sprintf(response,"{\"LED\":\"ON\"}");
    slave.SendTele(response);
  }
  if (!strcmp(data, "OFF")) { // SlaveSend OFF
    digitalWrite(LED_BUILTIN, LOW);
    char response[20];
    sprintf(response,"{\"LED\":\"OFF\"}");
    slave.SendTele(response);
  }
}

void loop() {
  slave.loop(); // Call the slave loop function every so often to process incoming requests
}
