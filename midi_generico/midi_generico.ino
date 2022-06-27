#include <Audio.h>
#include <USBHost_t36.h>

#define LED 13

unsigned long tiempoAnterior = 0;
const int INTERVALO_MS = 500;

USBHost myusb;
MIDIDevice midi1(myusb);

#include <Audio.h>
#include <Wire.h>
#include <SPI.h>
#include <SD.h>
#include <SerialFlash.h>

// GUItool: begin automatically generated code
AudioSynthWaveformSine   sine1;          //xy=334.2857322692871,427.61905097961426
AudioSynthWaveformSine   sine2;          //xy=335.71431732177734,497.6190605163574
AudioMixer4              mixerL;         //xy=592.857177734375,444.76194763183594
AudioMixer4              mixerR;         //xy=594.2857818603516,516.1905059814453
AudioOutputI2S           salidaI2S;           //xy=774.2858619689941,491.28592252731323
AudioConnection          patchCord1(sine1, 0, mixerL, 0);
AudioConnection          patchCord2(sine2, 0, mixerR, 0);
AudioConnection          patchCord3(mixerL, 0, salidaI2S, 0);
AudioConnection          patchCord4(mixerR, 0, salidaI2S, 1);
AudioControlSGTL5000     sgtl5000_1;     //xy=602.857177734375,610.4762725830078
// GUItool: end automatically generated code


int ultimaNota = -1;

void setup() {
  AudioMemory(20);

  sgtl5000_1.enable();
  sgtl5000_1.volume(0.6);
  
  //Serial.begin(115200);
  delay(500);

  myusb.begin();
  midi1.setHandleNoteOn(myNoteOn);
  midi1.setHandleNoteOff(myNoteOff);
  midi1.setHandleControlChange(myControlChange);
  
  mixerL.gain(0, 0.5);
  mixerR.gain(0, 0.5);

  sine1.frequency(110);
  sine1.amplitude(0.0);
  sine2.frequency(110);
  sine2.amplitude(0.0);
 
  pinMode(LED, OUTPUT);
}

void loop() {
  myusb.Task();
  midi1.read();   // aca se leen las posibles notas

  if (hayEvento()){
    // evento cada INTERVALO_MS
    // Serial.println( tempmonGetTemp() );
  }  

}


boolean hayEvento(){
  unsigned long tiempoActual = millis();
  boolean evento = false;  // valor por defecto
  
  if ((tiempoActual - tiempoAnterior ) >= INTERVALO_MS) {
     evento = true;
     tiempoAnterior = tiempoActual;
  }
  return evento;
}

/*
void setearVolumenMidi(int valor){
    mixer1.gain(0, (float)valor / 127.0);
    mixer1.gain(1, (float)valor / 127.0);
}
*/

void myNoteOn(byte channel, byte note, byte velocity) {
    sine1.amplitude((float)velocity / 127);
    sine1.frequency(midiToFreq(note));
    
    sine2.frequency(midiToFreq(note)*1.004);
    sine2.amplitude((float)velocity / 127);

    ultimaNota = note;
    digitalWrite(LED, HIGH);
}

void myNoteOff(byte channel, byte note, byte velocity) {
    if (note == ultimaNota){
      sine1.amplitude(0.0);
      sine2.amplitude(0.0);
      digitalWrite(LED, LOW);
    }
}

void myControlChange(byte channel, byte control, byte value) {
    if (control == 0) {
      mixerL.gain(0, (float)value/127.0);
      mixerR.gain(0, (float)value/127.0);  
    }
    //if (control == 1);
}

float midiToFreq (int notaMidi){
  float frecuencia = (440.0/32) * pow(2.0, ((notaMidi - 9.0) / 12.0));
  return frecuencia;
}
