#include "arduinoFFT.h"
#include <ArduinoBLE.h>

/*Filter and Sampling*/ 
#define SAMPLES 128         //Must be a power of 2
#define SAMPLING_FREQUENCY 10000 //Hz, must be less than 10000 due to ADC

/*Threshold and Hysterese*/
#define HYST 0.006
#define THRESHOLD 0.024

/*Digial Pins*/
#define MOTOR_PIN 8
 
arduinoFFT FFT = arduinoFFT();
/*FFT Variables*/
double vReal[SAMPLES];
double vImag[SAMPLES];

unsigned int sampling_period_us;
unsigned long microseconds;
float averageEnergy ;


// variables for button
bool oldPinchState = false;
bool pinchState = false;

void setup() {
  Serial.begin(9600);

    pinMode(LED_BUILTIN, OUTPUT);
    pinMode(MOTOR_PIN,OUTPUT);
    pinMode(0,INPUT);

    sampling_period_us = round(1000000*(1.0/SAMPLING_FREQUENCY));

  // initialize the Bluetooth® Low Energy hardware
  BLE.begin();

  Serial.println("Bluetooth® Low Energy Central - LED control");

  // start scanning for peripherals
  BLE.scanForUuid("19b10000-e8f2-537e-4f6c-d104768a1214");
}

void loop() {
  //digitalWrite(LED_BUILTIN, LOW);
  // check if a peripheral has been discovered
  BLEDevice peripheral = BLE.available();

  if (peripheral) {
    // discovered a peripheral, print out address, local name, and advertised service
    Serial.print("Found ");
    Serial.print(peripheral.address());
    Serial.print(" '");
    Serial.print(peripheral.localName());
    Serial.print("' ");
    Serial.print(peripheral.advertisedServiceUuid());
    Serial.println();
    
    if (peripheral.localName() != "Ring") {
      return;
    }

    // stop scanning
    BLE.stopScan();

    controlLed(peripheral);

    // peripheral disconnected, start scanning again
    BLE.scanForUuid("19b10000-e8f2-537e-4f6c-d104768a1214");
  }
}

void controlLed(BLEDevice peripheral) {
  // connect to the peripheral
  Serial.println("Connecting ...");

  if (peripheral.connect()) {
    Serial.println("Connected");
  } else {
    Serial.println("Failed to connect!");
    return;
  }

  // discover peripheral attributes
  Serial.println("Discovering attributes ...");
  if (peripheral.discoverAttributes()) {
    Serial.println("Attributes discovered");
  } else {
    Serial.println("Attribute discovery failed!");
    peripheral.disconnect();
    return;
  }

  // retrieve the LED characteristic
  BLECharacteristic ledCharacteristic = peripheral.characteristic("19b10001-e8f2-537e-4f6c-d104768a1214");

  if (!ledCharacteristic) {
    Serial.println("Peripheral does not have LED characteristic!");
    peripheral.disconnect();
    return;
  } else if (!ledCharacteristic.canWrite()) {
    Serial.println("Peripheral does not have a writable LED characteristic!");
    peripheral.disconnect();
    return;
  }

  while (peripheral.connected()) {
    // while the peripheral is connected

    /*SAMPLING*/
    for(int i=0; i<SAMPLES; i++)
    {
        microseconds = micros();    //Overflows after around 70 minutes!
     
        vReal[i] = analogRead(A0)*(3.3/1023.0);
        vImag[i] = 0;
     
        while(micros() < (microseconds + sampling_period_us)){
        }
    }
 
    /*FFT https://www.norwegiancreations.com/2017/08/what-is-fft-and-how-can-you-implement-it-on-an-arduino/*/
    FFT.Windowing(vReal, SAMPLES, FFT_WIN_TYP_HAMMING, FFT_FORWARD);
    FFT.Compute(vReal, vImag, SAMPLES, FFT_FORWARD);
    FFT.ComplexToMagnitude(vReal, vImag, SAMPLES);
   
    averageEnergy = 0;
    for(int i=1; i<(SAMPLES/2); i++)
    {

        averageEnergy += vReal[i];
    }
    
    averageEnergy =averageEnergy/(128*(SAMPLES-2)/2);
    Serial.println(averageEnergy,4);
    /*Touch detection with hysteresis*/
    if (averageEnergy >= THRESHOLD+HYST/2){
      digitalWrite(LED_BUILTIN, HIGH);
      digitalWrite(MOTOR_PIN, HIGH);
      pinchState = true;
    }
    if (averageEnergy <= THRESHOLD-HYST/2){
      digitalWrite(LED_BUILTIN, LOW);
      digitalWrite(MOTOR_PIN, LOW);
      pinchState = false;
    }
    if (oldPinchState != pinchState) {
      // pinch changed
      oldPinchState = pinchState;

      if (pinchState) {
        Serial.println("pinch");

        // button is pressed, write 0x01 to turn the LED on
        ledCharacteristic.writeValue((byte)0x01);
      } else {
        Serial.println("pinch released");

        // button is released, write 0x00 to turn the LED off
        ledCharacteristic.writeValue((byte)0x00);
      }
    }
  }

  Serial.println("Peripheral disconnected");
}
