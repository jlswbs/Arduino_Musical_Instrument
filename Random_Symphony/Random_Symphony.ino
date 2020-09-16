// Random symphony //

#include <SoftwareSerial.h>

SoftwareSerial mySerial(2, 3);

#define RST 4

void setup() {

  mySerial.begin(31250);

  pinMode(RST, OUTPUT);
  digitalWrite(RST, LOW);
  delay(100);
  digitalWrite(RST, HIGH);
  delay(100);

  // Volume cc = 7
  control(0, 7, 96);
  control(1, 7, 96);
  control(2, 7, 96);
  control(9, 7, 96);

  // Pan cc = 10
  control(0, 10, 32);
  control(1, 10, 58);
  control(2, 10, 96);
  control(9, 10, 64);

  // Reverb cc = 91
  control(0, 91, 64);
  control(1, 91, 64);
  control(2, 91, 64);
  
}

void loop() {

  program(0, rand()%20);
  program(1, 90+rand()%30);
  program(2, 50+rand()%30); 

  int note1 = 24+rand()%36;
  int note2 = 48+rand()%24;
  int note3 = 36+rand()%24;
 
  noteon(0, note1, rand()%32);
  noteon(1, note2 , rand()%32);
  noteon(2, note3 , rand()%32);
  noteon(9, 36+rand()%36 , rand()%28);
    
  delay(40);

  noteoff(0, note1, 96);
  noteoff(1, note2, 96);
  noteoff(2, note3, 96);
    
  delay(200);

}

void noteon(byte channel, byte note, byte attack_velocity) { comand( (0x90 | channel), note, attack_velocity); }
void noteoff(byte channel, byte note, byte release_velocity) { comand( (0x80 | channel), note, release_velocity); }
void program(byte channel, byte prog_number) { comand( (0xC0 | channel), prog_number, 0); }
void control(byte channel, byte cntrl, byte cntrl_value) { comand( (0xB0 | channel), cntrl, cntrl_value); }
void comand(byte cmd, byte data1, byte data2) { mySerial.write(cmd); mySerial.write(data1); if( (cmd & 0xF0) <= 0xB0) mySerial.write(data2); }
