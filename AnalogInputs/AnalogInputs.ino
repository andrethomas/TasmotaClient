/*
 * Provides all 8 analog inputs on an UNO / Pro Mini via serial
 */

void setup() {
  Serial.begin(57600);
}

void loop() {
  if (Serial.available()) {
    if (Serial.find("JSON")) {
      uint16_t a0 = analogRead(A0);
      uint16_t a1 = analogRead(A1);
      uint16_t a2 = analogRead(A2);
      uint16_t a3 = analogRead(A3);
      uint16_t a4 = analogRead(A4);
      uint16_t a5 = analogRead(A5);
      uint16_t a6 = analogRead(A6);
      uint16_t a7 = analogRead(A7);
      Serial.print((String)char(0xFE)+"{\"A0\":" + a0 + ",\"A1\":"+a1 + ",\"A2\":"+a2 + ",\"A3\":"+a3 + ",\"A4\":"+a4 + ",\"A5\":"+a5 + ",\"A6\":"+a6 + ",\"A7\":"+a7 + "}"+char(0xFF));
    }
  }
}
