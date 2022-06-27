#include <Audio.h>
#include <USBHost_t36.h>

#define LED 13

//extern float tempmonGetTemp(void);
unsigned long tiempoAnterior = 0;
const int INTERVALO_MS = 500;


USBHost myusb;
MIDIDevice midi1(myusb);

// GUItool: begin automatically generated code
AudioSynthWaveformSine   sine1;          //xy=173,289
AudioEffectEnvelope      envolvente1;      //xy=378,290
AudioOutputI2S           i2s1;           //xy=677,290
AudioMixer4              mixer1;         //xy=486,163
AudioSynthKarplusStrong  string1;        //xy=330,294
AudioFilterBiquad        filtro1;        //xy=483,188

AudioConnection          patchCord1(sine1, envolvente1);
AudioConnection          patchCord2(envolvente1, 0, mixer1, 0);
AudioConnection          patchCord3(string1, 0, mixer1, 1);
AudioConnection          patchCord4(mixer1, 0, filtro1, 0);
AudioConnection          patchCord5(filtro1, 0, i2s1, 0);
AudioConnection          patchCord6(filtro1, 0, i2s1, 1);

AudioControlSGTL5000     sgtl5000_1;     //xy=85,45
// GUItool: end automatically generated code

float volumenPote = 0.5;
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
  
  sine1.frequency(220);
  sine1.amplitude(0.6);
  envolvente1.attack(6.5);
  envolvente1.hold(25);
  envolvente1.decay(70);
  // envolvente1.sustain(0);
  filtro1.setLowpass(0, 20000, 0.3);
  mixer1.gain(0, 0.5);
  
  pinMode(LED, OUTPUT);
}

void loop() {
  myusb.Task();
  midi1.read();   // aca se leen las posibles notas

  if (hayEvento()){
    // evento cada INTERVALO_MS
    Serial.println( tempmonGetTemp() );
  }  

  //setearVolumen();
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

void setearVolumen(){
    volumenPote = analogRead(A1) / 1023.0;
    mixer1.gain(0, volumenPote);
    mixer1.gain(1, volumenPote);
}

void setearVolumenMidi(int valor){
    mixer1.gain(0, (float)valor / 127.0);
    mixer1.gain(1, (float)valor / 127.0);
}

void myNoteOn(byte channel, byte note, byte velocity) {
    sine1.amplitude((float)velocity / 127);
    sine1.frequency(midiToFreq(note));
    // envolvente1.noteOn();
    string1.noteOn(midiToFreq(note), velocity / 127.0);
    ultimaNota = note;
    digitalWrite(LED, HIGH);
}

void myNoteOff(byte channel, byte note, byte velocity) {
    if (note == ultimaNota){
      // envolvente1.noteOff();
      string1.noteOff(velocity / 127.0);
      digitalWrite(LED, LOW);
    }
}

void myControlChange(byte channel, byte control, byte value) {
    if (control == 0) setearVolumenMidi(value);
    if (control == 1) filtro1.setLowpass(0, 50 + pow(value, 2.04));  // exponencial
}

float midiToFreq (int notaMidi){
  float frecuencia = (440.0/32) * pow(2.0, ((notaMidi - 9.0) / 12.0));
  return frecuencia;
}
