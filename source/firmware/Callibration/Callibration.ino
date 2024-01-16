#include "arduino.h"
#include "arduinoFFT.h"

/*Filter and Sampling*/
#define SAMPLES 128               //Must be a power of 2
#define SAMPLING_FREQUENCY 10000  //Hz, must be less than 10000 due to ADC

/*Threshold and Hysterese*/
#define HYST 0.003
#define THRESHOLD 0.016

/*Digial Pins*/
#define MOTOR_PIN 8

arduinoFFT FFT = arduinoFFT();

/*FFT Variables*/
int vRealmeasurement[SAMPLES];
double vReal[SAMPLES];
double vImag[SAMPLES];

unsigned int sampling_period_us;
unsigned long microseconds;
float energy;

void setup() {
  Serial.begin(9600);

  pinMode(LED_BUILTIN, OUTPUT);
  pinMode(MOTOR_PIN, OUTPUT);
  pinMode(0, INPUT);

  sampling_period_us = round(1000000 * (1.0 / SAMPLING_FREQUENCY));
}

void loop() {
  digitalWrite(MOTOR_PIN, LOW);
  /*SAMPLING*/
  for (int i = 0; i < SAMPLES; i++) {
    microseconds = micros();  //Overflows after around 70 minutes!

    vReal[i] = analogRead(A0)*(3.3/1023);
    vImag[i] = 0;
    
    while (micros() < (microseconds + sampling_period_us)) {
    }
  }
 
  /*FFT https://www.norwegiancreations.com/2017/08/what-is-fft-and-how-can-you-implement-it-on-an-arduino/*/
  FFT.Windowing(vReal, SAMPLES, FFT_WIN_TYP_HAMMING, FFT_FORWARD);
  FFT.Compute(vReal, vImag, SAMPLES, FFT_FORWARD);
  FFT.ComplexToMagnitude(vReal, vImag, SAMPLES);
  double peak = FFT.MajorPeak(vReal, SAMPLES, SAMPLING_FREQUENCY);

  energy = 0;
 
  for (int i =2; i < (SAMPLES/2); i++) {
    /*View all these three lines in serial terminal to see which frequencies has which amplitudes*/
    //Serial.print((i * 1.0 * SAMPLING_FREQUENCY) / SAMPLES, 1);
    //Serial.print(" ");
    //Serial.println(vReal[i], 4);    //View only this line in serial plotter to visualize the bin
    //Serial.print(vReal[i],4);
    //if(i<(SAMPLES/2-1)){
      //Serial.print(",");
    //}
    energy += vReal[i];
    
  }
  //Serial.println();
  energy = energy / (128*(SAMPLES-4)/2);
  Serial.println(energy,4);
  

  /*Touch detection with hysteresis*/
  if (energy >= THRESHOLD + HYST/2) {
    digitalWrite(LED_BUILTIN, HIGH);
    //digitalWrite(MOTOR_PIN, HIGH);
  }
  if (energy <= THRESHOLD - HYST/2) {
    digitalWrite(LED_BUILTIN, LOW);
    //digitalWrite(MOTOR_PIN, LOW);
  }
  delay(100);  //Repeat the process every second OR:
}