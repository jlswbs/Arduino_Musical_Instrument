// Random chaos symphony //

#include <SoftwareSerial.h>

SoftwareSerial mySerial(2, 3);

#define RST  4
#define BPM  120  // tempo
#define POLY 16   // channels

  float r = 3.5699456f;
  float x = 0.1f;
  float nx;
  int outx;

void setup() {

  mySerial.begin(31250);

  pinMode(RST, OUTPUT);
  digitalWrite(RST, LOW);
  delay(100);
  digitalWrite(RST, HIGH);
  delay(100);
  
}

void loop() {

  nx = x;
  x = r * nx * (1.0f - nx);
  
  outx = (1UL << 31) * x;

  srand(outx);

  program(rand()%POLY, rand()%123);

  control(rand()%POLY, rand()%128, rand()%128);

  noteon(rand()%POLY, 24 + rand()%64, rand()%24);
    
  uint16_t tempo = BPM;
  uint16_t delay_ms = 60000 / tempo;
    
  delay(delay_ms / 8);

}

void noteon(byte channel, byte note, byte attack_velocity) { comand( (0x90 | channel), note, attack_velocity); }
void noteoff(byte channel, byte note, byte release_velocity) { comand( (0x80 | channel), note, release_velocity); }
void program(byte channel, byte prog_number) { comand( (0xC0 | channel), prog_number, 0); }
void control(byte channel, byte cntrl, byte cntrl_value) { comand( (0xB0 | channel), cntrl, cntrl_value); }
void comand(byte cmd, byte data1, byte data2) { mySerial.write(cmd); mySerial.write(data1); if( (cmd & 0xF0) <= 0xB0) mySerial.write(data2); }
