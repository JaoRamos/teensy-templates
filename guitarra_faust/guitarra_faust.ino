#include <Audio.h>
#include "GuitarraTeensy.h"

#define LED 13

unsigned long tiempoAnterior = 0;
const int INTERVALO_MS = 500;

GuitarraTeensy ampliGuitarra;

// GUItool: begin automatically generated code
AudioMixer4              mixerL;         //xy=592.857177734375,444.76194763183594
AudioMixer4              mixerR;         //xy=594.2857818603516,516.1905059814453
AudioOutputI2S           salidaI2S;           //xy=774.2858619689941,491.28592252731323
AudioInputI2S            entrada;           //xy=281.4285714285714,494.7618974958147

AudioConnection          patchCord0(entrada, 0, ampliGuitarra, 0);

AudioConnection          patchCord1(ampliGuitarra, 0, mixerL, 0);
AudioConnection          patchCord2(ampliGuitarra, 1, mixerR, 0);
AudioConnection          patchCord3(mixerL, 0, salidaI2S, 0);
AudioConnection          patchCord4(mixerR, 0, salidaI2S, 1);
AudioControlSGTL5000     sgtl5000_1;     //xy=602.857177734375,610.4762725830078
// GUItool: end automatically generated code


void setup() {
  AudioMemory(20);

  sgtl5000_1.enable();
  sgtl5000_1.volume(0.5);
  sgtl5000_1.inputSelect(AUDIO_INPUT_LINEIN);

  //Serial.begin(115200);
  delay(500);

  mixerL.gain(0, 0.5);
  mixerR.gain(0, 0.5);
 
  pinMode(LED, OUTPUT);
  
/*
    gain = hslider("Gain", 40, 1, 200, 0.1);
    prefiltro = hslider("Pasaaltos pre", 1400, 20, 1500, 10);
    agudos =  hslider("Parlante (pasabajos)", 3500, 100, 8000, 10);
    delaySlider =  hslider("Delay", 0.5, 0, 0.9, 0.01);
    blend = hslider("Blend Clean", 5, 0, 10, 0.01);
    master =  hslider("Volumen Master", 0.3, 0, 1, 0.01);
 */
 
    ampliGuitarra.setParamValue("Gain", 40.0);
    ampliGuitarra.setParamValue("Pasaaltos pre", 1400.0);
    ampliGuitarra.setParamValue("Parlante (pasabajos)", 3500.0);
    ampliGuitarra.setParamValue("Delay", 0.2);
    ampliGuitarra.setParamValue("Blend Clean", 5.0);
    ampliGuitarra.setParamValue("Volumen Master", 0.5);

}

void loop() {

  if (hayEvento()){
     ampliGuitarra.setParamValue("Gain", analogRead(A1)/1024.0 * 200.0);
     Serial.println( tempmonGetTemp() );
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

