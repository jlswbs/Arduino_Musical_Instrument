// Wolfram cellular automata drums generator //

#include <SoftwareSerial.h>

SoftwareSerial mySerial(2, 3);

#define RST 4

#define WIDTH 256

  uint8_t state[WIDTH];
  uint8_t newstate[WIDTH];
  int x, y, k;
  bool rules[8] = {0, 1, 1, 1, 1, 0, 0, 0};

void setup() {

  mySerial.begin(31250);

  pinMode(RST, OUTPUT);
  digitalWrite(RST, LOW);
  delay(100);
  digitalWrite(RST, HIGH);
  delay(100);

  // Volume cc = 7
  control(9, 7, 96);

  // Reverb cc = 91
  control(9, 91, 48);

  for (x=0; x<WIDTH; x++) state[x] = rand()%2;
  
}

void loop() {

  memset (newstate, 0, WIDTH);

  for (x=0;x<WIDTH;x++) {     
    k = 4*state[(x-1+WIDTH)%WIDTH] + 2*state[x] + state[(x+1)%WIDTH];
    newstate[x] = rules[k];
  }

  memcpy (state, newstate, WIDTH);
  
  if (state[0] == 1) noteon (9, 35, rand()%32);
  if (state[1] == 1) noteon (9, 36, rand()%32);
  if (state[2] == 1) noteon (9, 37, rand()%32);
  if (state[3] == 1) noteon (9, 38, rand()%32);
  if (state[4] == 1) noteon (9, 39, rand()%32);
  if (state[5] == 1) noteon (9, 40, rand()%32);
  if (state[6] == 1) noteon (9, 41, rand()%32);
  if (state[7] == 1) noteon (9, 42, rand()%32);
    
  uint16_t tempo = 120; // 120BPM
  uint16_t delay_ms = 60000 / tempo;
    
  delay(delay_ms / 2);

}

void noteon(byte channel, byte note, byte attack_velocity) { comand( (0x90 | channel), note, attack_velocity); }
void noteoff(byte channel, byte note, byte release_velocity) { comand( (0x80 | channel), note, release_velocity); }
void program(byte channel, byte prog_number) { comand( (0xC0 | channel), prog_number, 0); }
void control(byte channel, byte cntrl, byte cntrl_value) { comand( (0xB0 | channel), cntrl, cntrl_value); }
void comand(byte cmd, byte data1, byte data2) { mySerial.write(cmd); mySerial.write(data1); if( (cmd & 0xF0) <= 0xB0) mySerial.write(data2); }
