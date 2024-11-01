#include <Arduino.h>
#include <RF24.h>

#define CE_PIN 9
#define CSN_PIN 10

RF24 radio(CE_PIN, CSN_PIN);
const uint8_t num_channels = 126;
uint8_t values[num_channels];

const uint8_t noiseAddress[][2] = { { 0x55, 0x55 }, { 0xAA, 0xAA }, { 0xA0, 0xAA }, { 0xAB, 0xAA }, { 0xAC, 0xAA }, { 0xAD, 0xAA } };
const int num_reps = 100;   

void performMeasurment();

void setup() {
  if (!radio.begin()) {
    while (true) {}
  }

  radio.stopConstCarrier();  
  radio.setAutoAck(false);
  radio.disableCRC();
  radio.setAddressWidth(2);

  for (uint8_t i = 0; i < 6; ++i) {radio.openReadingPipe(i, noiseAddress[i]);}

  radio.setDataRate(RF24_1MBPS);

  radio.startListening();
  radio.stopListening();
  radio.flush_rx();

  //Serial.begin(9600); //for debbuging
}

void loop() {
  performMeasurment();


//for debbuging
  /*for (int i = 0; i < num_channels; ++i) {
    Serial.print(values[i]);
    Serial.print(" ");
    }
    Serial.println();*/
}

void performMeasurment() {

    memset(values, 0, sizeof(values));

    int rep_counter = num_reps;
    while (rep_counter--) {
      int i = num_channels;
      while (i--) {
        radio.setChannel(i);
        radio.startListening();
        delayMicroseconds(128);
        bool foundSignal = radio.testRPD();
        radio.stopListening();

        if (foundSignal || radio.testRPD() || radio.available()) {
          ++values[i];
          radio.flush_rx();
        }
      }
    }

}