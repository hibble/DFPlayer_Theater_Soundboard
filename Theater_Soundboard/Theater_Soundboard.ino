#include <SoftwareSerial.h>
#include <DFPlayerMini_Fast.h>

// softweere serial needed on arduino pro mini so usb serial is avaliable for diagnostic/development.
SoftwareSerial mySoftwareSerial(10, 11); // RX, TX  note: add 1k resistor to both
DFPlayerMini_Fast myMP3TheaterSoundboard;
// constants
const int StopButon = 2;
const int T1Buton = 3;
const int T2Buton = 4;

//variables
int VolumeSlider = 0; // variable to set volume
int VolumeSliderInputPin = A0; // A0 reads curent slider input
int StopButonState = 0;
int T1ButonState = 0;
int T2ButonState = 0;

void setup()
{
  // initialize the pushbuttons pins as an inputs:
  pinMode(StopButon, INPUT);
  pinMode(T1Buton, INPUT);
  pinMode(T2Buton, INPUT);

  //setup serial
  Serial.begin(115200); // start arduino serial
  mySoftwareSerial.begin(9600); // start softwere serial to send comands to DFPlayer mini mp3 modual

  myMP3TheaterSoundboard.begin(mySoftwareSerial); //link mp3 modual with serial conection

  Serial.println("Setting volume to 5");
  myMP3TheaterSoundboard.volume(5);
  Serial.println("DFPlayer ready");
}

void loop()
{
  StopButonState = digitalRead(StopButon);
  T1ButonState = digitalRead(T1Buton);
  T2ButonState = digitalRead(T2Buton);
  // check if the pushbutton is pressed. If it is, the buttonState is HIGH:
  if (StopButonState == HIGH) {
    // stop playback:
    myMP3TheaterSoundboard.pause();
    Serial.println("DFPlayer paused by stop button");
  } else {
    // do nothing
  }
  if (T1ButonState == HIGH) {
    // Start T1 playback:
    myMP3TheaterSoundboard.play(1);
    Serial.println("DFPlayer start T1");
  } else {
    // do nothing
  }
  if (T2ButonState == HIGH) {
    // Start T2 playback:
    myMP3TheaterSoundboard.play(2);
    Serial.println("DFPlayer start T2");
  } else {
    // do nothing
  }

  // read the value from the volume slider:
  VolumeSlider = analogRead(VolumeSliderInputPin) / 34;
  Serial.println("volumeSliser: ");
  Serial.println(VolumeSlider);
  myMP3TheaterSoundboard.volume(VolumeSlider);
  delay(200);
}
