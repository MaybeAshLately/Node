#include <Arduino.h>
#include <RF24.h>
#include <avr/sleep.h>
#include <Wire.h>

const int CE_PIN=9;
const int CSN_PIN=10;
const int interruptPin=2;
const int slaveAddress=10;

RF24 radio(CE_PIN, CSN_PIN);
const uint8_t num_channels = 126;
uint8_t values[num_channels];
const uint8_t noiseAddress[][2] = { { 0x55, 0x55 }, { 0xAA, 0xAA }, { 0xA0, 0xAA }, { 0xAB, 0xAA }, { 0xAC, 0xAA }, { 0xAD, 0xAA } };
const int num_reps = 100;  

volatile boolean flag;
int messagePart=0;

void goToSleep();
void performMeasurment();
void requestEvent();
void setFlag();


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

  Wire.begin(slaveAddress);
  Wire.onRequest(requestEvent);

  pinMode(interruptPin, INPUT);

  attachInterrupt(digitalPinToInterrupt(interruptPin), setFlag, RISING);

  flag=false;

}


void loop() {
  if (flag) {
    flag=false;
    goToSleep();
  }
}


void goToSleep() {
  set_sleep_mode(SLEEP_MODE_PWR_DOWN);
  sleep_enable();
  attachInterrupt(digitalPinToInterrupt(interruptPin), setFlag, RISING);
  sleep_mode();
  sleep_disable();
  detachInterrupt(0);
}
  

void setFlag() {
  flag = true;
}


void requestEvent() {

if(messagePart==0) performMeasurment();
if(messagePart<3)
{
  for(int i=0;i<32;i++)
  {
    Wire.write(values[messagePart*32+i]);
  }
  messagePart++;
}
else //messegePart==3
{
  for(int i=0;i<30;i++)
  {
    Wire.write(values[messagePart*32+i]);
  }
  messagePart=0;
}
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