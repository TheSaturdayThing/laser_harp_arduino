#include <SoftwareSerial.h>
// LED outputs
#define L1 24
#define L2 12
#define L3 11
#define L4 10
#define L5 9
#define L6 8
#define L7 7
#define L8 6
#define L9 5
#define L10 22
// Photoresistor inputs
const int HP1 = A0;
const int HP2 = A1;
const int HP3 = A2;
const int HP4 = A3;
const int HP5 = A4;
const int HP6 = A5;
const int HP7 = A6;
const int HP8 = A7;
const int HP9 = A8;
const int HP10 = A9;

// took from instructables
//music shield from sparkfun.com (music instrument shield)
SoftwareSerial mySerial(2, 3); // RX, TX

int sensorValue[10]; // Photoresistor values
int sensorHigh[10]; // Max photoresistor values

//controlla quali note dovrebbe suonare questo turno
boolean noteBuffer[10] = {false, false, false, false, false, false, false, false, false, false};  // checks which notes should play this round
//questo controlla se la nota sta suonando
boolean notes[10] = {false, false, false, false, false, false, false, false, false, false}; // this checks to see if a note is currently playing
const int lightBuffer[10] = {L1, L2, L3, L4, L5, L6, L7, L8, L9, L10 };
const int photoBuffer[10] = {HP1, HP2, HP3, HP4, HP5, HP6, HP7, HP8, HP9, HP10};
//inserisci il valore delle 10 note qui NON MODIFICARE QUESTO CON FUNZIONI
byte noteValue[10] = {60, 62, 64, 65, 67, 69, 71, 72, 74, 76}; // put the values of the 10 notes here DO NOT MODIFY THIS WITH FUNCTIONS
                    //48, 51, 53, 54, 55, 58  Blues Scale
int t;
// Il valore della nota MIDI che deve essere suonata
int note = 0; //The MIDI note value to be played
//Collegat al VS1053 Reset line
byte resetMIDI = 4; //Tied to VS1053 Reset line
byte ledPin = 13; //MIDI traffic inidicator
int loopCount=0;
int button=0;
int instrument = 1;



void setup() {
  Serial.begin(57600);

  //Setup soft serial for MIDI control
  mySerial.begin(31250);
  for(int i=0; i<10; i++){
    pinMode(lightBuffer[i],OUTPUT);
  }
   
  //Reset the VS1053
  pinMode(resetMIDI, OUTPUT);
  digitalWrite(resetMIDI, LOW);
  delay(100);
  digitalWrite(resetMIDI, HIGH);
  delay(100);
  //0xB0 è un messaggio canale, imposta il volume del canale verso il massimo (127)
  talkMIDI(0xB0, 0x07, 120); //0xB0 is channel message, set channel volume to near max (127)


  for(int i = HP1; i <= HP10; i++){
    pinMode(i,INPUT);
  }

  for(int i=0;i<10;i++){
    sensorHigh[i] = 0;
  }

  for(int i=0, j=HP1; i<10, j <= HP10; i++, j++){              //sostituite j = 1 e j<=9        
     t=millis();
     do{
      sensorValue[i]=analogRead(j);
      if(sensorValue[i]>sensorHigh[i]){
         sensorHigh[i]=sensorValue[i]; 
      }
      Serial.println(sensorValue[i]);
     }while(millis()-t<=1000);
     Serial.println(sensorHigh[i]);
  }

  for(int i = 0; i<10; i++){
    noteBuffer[i] = true;
  }
  playNotes();


  for(int i = 0; i<10; i++){
    noteBuffer[i] = false;
  }
  
  delay(1000);
}

void loop() {

  talkMIDI(0xB0, 0, 0x79); //Bank select Melodic //Default bank GM1
  talkMIDI(0xC0, instrument, 0); //Set instrument number. 0xC0 is a 1 data byte command
  for(int i=0; i<10; i++){
    sensorValue[i] = analogRead(photoBuffer[i]);
    if(sensorValue[i]>sensorHigh[i]+100 ){
      noteBuffer[i] = true;
    }
  }
  playNotes();
  Serial.println(" A0: " + String(sensorValue[0])+" "+String(sensorHigh[0]) +" A1: " + String(sensorValue[1])+" "+String(sensorHigh[1])+ " A2: " + String(sensorValue[2])+" "+String(sensorHigh[2]) + " A3: " + String(sensorValue[3])+" "+String(sensorHigh[3]) + " A4: " + String(sensorValue[4])+" "+String(sensorHigh[4]) + " A5: " + String(sensorValue[5])+" "+String(sensorHigh[5]) + " A6: " + String(sensorValue[6])+" "+String(sensorHigh[6]) + " A7: " + String(sensorValue[7])+" "+String(sensorHigh[7]) + " A8: " + String(sensorValue[8])+" "+String(sensorHigh[8]) + " A9: " + String(sensorValue[9])+" "+String(sensorHigh[9]));   
  delay(50);
}

// i = 0
// playMyNote(noteValue[0], 0);
// inside playMyNote: note = 60, boolValue = 0

void playMyNote(int i){
    // check to see if note is playing
    if(!notes[i]){
      //play note
      noteOn(0, noteValue[i], 126);
      // array reflects note playing
      notes[i] = true;
      }
  }
void stopMyNote(int i){
    if(notes[i]){
      noteOff(0, noteValue[i], 126);
      notes[i] = false;
      digitalWrite(lightBuffer[i], LOW);
      }
  }

void stopAllOtherNotes(byte note){
  for(int i = 0; i<10; i++){
    if(noteValue[i] != note){
      stopMyNote(i);
      
    }
  }
}

void playNotes(){
  // traverse buffer
  for(int i = 0; i<10; i++){
    if(noteBuffer[i]){
      playMyNote(i);
      digitalWrite(lightBuffer[i], HIGH);
    } else {
      digitalWrite(lightBuffer[i], LOW);
      stopMyNote(i);
    }
  }
   // clear buffer
  for(int i = 0; i<10; i++){
      noteBuffer[i] = false;
    
    }
  }

  
void noteOn(byte channel, byte note, byte attack_velocity) {
  talkMIDI( (0x90 | channel), note, attack_velocity);
}

//Send a MIDI note-off message.  Like releasing a piano key
void noteOff(byte channel, byte note, byte release_velocity) {
  talkMIDI( (0x80 | channel), note, release_velocity);
}

//suona la nota MIDI. Non controlla se cmd è più grande di 127, oppurese quei valori son inferiori di 127
//Plays a MIDI note. Doesn't check to see that cmd is greater than 127, or that data values are less than 127
void talkMIDI(byte cmd, byte data1, byte data2) {
  digitalWrite(ledPin, HIGH);
  mySerial.write(cmd);
  mySerial.write(data1);

  //Some commands only have one data byte. All cmds less than 0xBn have 2 data bytes 
  //(sort of: http://253.ccarh.org/handout/midiprotocol/)
  if( (cmd & 0xF0) <= 0xB0)
    mySerial.write(data2);

  digitalWrite(ledPin, LOW);
 }
void turnNotesOff(){
  for(int i = 0; i < 200; i++){
    noteOff(0, i, 120);
  }
}
