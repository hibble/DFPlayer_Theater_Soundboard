#include <SoftwareSerial.h>
#include <DFPlayerMini_Fast.h>
#include <Adafruit_Keypad.h>

// softweere serial needed on arduino pro mini so usb serial is avaliable for diagnostic/development.
SoftwareSerial mySoftwareSerial(11, 10); // RX, TX  note: add 1k resistor to both
DFPlayerMini_Fast myMP3TheaterSoundboard;
// constants
const int StopButon = 12;
int VolumeSliderInputPin = A0; // A0 reads curent slider input

//variables
int VolumeSlider = 0; // variable to set volume
int StopButonState = 0;

//setup matrix keypad
const byte ROWS = 3; // rows
const byte COLS = 3; // columns
//define the symbols on the buttons
byte keys[ROWS][COLS] = {
  {1, 2, 3},
  {4, 5, 6},
  {7, 8, 9}
};
byte rowPins[ROWS] = {8, 7, 6}; //connect to the row pinouts of the buttons
byte colPins[COLS] = {2, 3, 4}; //connect to the column pinouts of the buttons

//initialize an instance of class NewKeypad
Adafruit_Keypad customKeypad = Adafruit_Keypad( makeKeymap(keys), rowPins, colPins, ROWS, COLS);

void setup()
{
  // initialize the pushbuttons pins as an inputs:
  pinMode(StopButon, INPUT_PULLUP);

  //setup serial
  mySoftwareSerial.begin(9600); // start softwere serial to send comands to DFPlayer mini mp3 modual
  Serial.begin(115200); // start arduino serial

  myMP3TheaterSoundboard.begin(mySoftwareSerial); //link mp3 modual with serial conection
  customKeypad.begin(); // start our custom matrix of buttons

  Serial.println("Setting volume to 5");
  myMP3TheaterSoundboard.volume(5);
  Serial.println("DFPlayer ready");
  delay(100);
}

void loop()
{
  customKeypad.tick(); // used to know when to compare values

  //Check if we should stop playback
  StopButonState = digitalRead(StopButon);
  // check if the pushbutton is pressed. If it is, the buttonState is LOW:
  if (StopButonState == LOW) {
    // stop playback:
    myMP3TheaterSoundboard.pause();
    Serial.println("DFPlayer paused by stop button");
  } else {
    // do nothing
  }

  // check if matrix button is pressed.
  while (customKeypad.available()) {
    keypadEvent e = customKeypad.read();
    //Serial.print((char)e.bit.KEY);
    Serial.print((int)e.bit.KEY);
    if (e.bit.EVENT == KEY_JUST_PRESSED) {
      Serial.println(" pressed");
      myMP3TheaterSoundboard.playFromMP3Folder((int)e.bit.KEY);
      Serial.print("DFPlayer playingTrack:");
      Serial.print((int)e.bit.KEY);
    }
    else if (e.bit.EVENT == KEY_JUST_RELEASED) Serial.println(" released");
  }

  // do we need delay? stops spamming volume update
  delay(10);

  //set volume to curent slider position
  VolumeSlider = analogRead(VolumeSliderInputPin) / 34;   // read the value from the volume slider:
  //  Serial.print("volumeSliser: ");
  //  Serial.println(VolumeSlider);
  myMP3TheaterSoundboard.volume(VolumeSlider);
}
