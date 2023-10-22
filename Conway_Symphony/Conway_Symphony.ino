// Conway's Game of Life cellular automata symphony //

#include <SoftwareSerial.h>

SoftwareSerial mySerial(2, 3);

#define RST           4
#define BPM           120
#define WIDTH         16
#define HEIGHT        16
#define SCR           (WIDTH * HEIGHT)
#define crNum(x,y,z)  ((x)+(y))%z
#define generation    38

  bool newCells[SCR];
  bool cells[SCR];
  int counter;
  bool world[SCR] = {
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 1, 0, 0, 1, 0, 0, 1, 0, 0, 1, 0, 0, 0,
    0, 0, 0, 0, 1, 0, 0, 1, 1, 0, 0, 1, 0, 0, 0, 0,
    0, 0, 0, 0, 1, 0, 0, 1, 1, 0, 0, 1, 0, 0, 0, 0,
    0, 0, 0, 1, 0, 0, 1, 0, 0, 1, 0, 0, 1, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
};

void reset(){

  memset(cells, 0, sizeof(cells));
  memset(newCells, 0, sizeof(newCells));

  for(int k = 0; k < SCR; k++) cells[k] = world[k];

}

void next(){

  memcpy(newCells, cells, sizeof(newCells));

  for(int y = 0; y < HEIGHT; y++){

    for(int x = 0; x < WIDTH; x++){
    
      int surrounding = 0;
     
      bool isAlive = false;

      cells[x+y*WIDTH] ? isAlive = true : isAlive = false;
      
      for(int j = -1; j < 2; j++) {
        for(int i = -1; i < 2; i++) surrounding += cells[crNum(x,i,WIDTH)+crNum(y,j,HEIGHT)*WIDTH];
      }

      surrounding -= cells[x+y*WIDTH];
      if((surrounding < 2 || surrounding > 3) && isAlive) newCells[x+y*WIDTH] = 0;
      if(surrounding == 3 && !isAlive) newCells[x+y*WIDTH] = 1;
      
    }

  }

  memcpy(cells, newCells, sizeof(cells));

}

void setup() {

  mySerial.begin(31250);

  pinMode(RST, OUTPUT);
  digitalWrite(RST, LOW);
  delay(100);
  digitalWrite(RST, HIGH);
  delay(100);

  reset();

  control(0, 10, 32);
  control(1, 10, 58);
  control(2, 10, 96);
  control(3, 10, 64);
  control(4, 10, 32);
  control(5, 10, 58);
  control(6, 10, 96);
  control(7, 10, 64);
  control(8, 10, 32);
  control(10, 10, 58);
  control(11, 10, 96);
  control(12, 10, 64);
  control(13, 10, 32);
  control(14, 10, 58);
  control(15, 10, 96);

  for(int i = 0; i < WIDTH; i++) control(i, 7, 64);   // channel 0-15 volume
  for(int i = 0; i < WIDTH; i++) control(i, 91, 64);  // channel 0-15 reverb
  for(int i = 0; i < WIDTH; i++) program(i, i);       // channel 0-15 instruments
  
}

void loop() {

  for(int y = 0; y < HEIGHT; y++){ 
    
    for(int x = 0; x < WIDTH; x++) { 

      uint16_t note = 24 + rand()%64;
      uint16_t vel = rand()%32;
      
      if (cells[x+y*WIDTH] == 1) {

        noteon(x, note, vel);

      }

      else {

        noteoff(x, note, vel);

      }

    }

  }
  
  uint16_t tempo = BPM;
  uint16_t delay_ms = 60000 / tempo;
    
  delay(delay_ms / 8);

  next();

  if(counter == generation){

    reset();
    counter = 0;

  }

  counter++;

}

void noteon(byte channel, byte note, byte attack_velocity) { comand( (0x90 | channel), note, attack_velocity); }
void noteoff(byte channel, byte note, byte release_velocity) { comand( (0x80 | channel), note, release_velocity); }
void program(byte channel, byte prog_number) { comand( (0xC0 | channel), prog_number, 0); }
void control(byte channel, byte cntrl, byte cntrl_value) { comand( (0xB0 | channel), cntrl, cntrl_value); }
void comand(byte cmd, byte data1, byte data2) { mySerial.write(cmd); mySerial.write(data1); if( (cmd & 0xF0) <= 0xB0) mySerial.write(data2); }
