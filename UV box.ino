#include <SPI.h>
#include <Esplora.h>
#include <TFT.h>

//int pin = 10;  //11;  //OUT_B
int pine = 11;
char spdprintout[5];
char spdprintout1[5];
char iExopDesity = 255;//0;
int ili = 1;

void setup() {
  Serial.begin(9600);
//  pinMode(pin,OUTPUT);
  pinMode(pine,OUTPUT);
//  while (!Serial) {
//    
//  }
//  Serial.println("Serial online");
////  pinMode(20, OUTPUT);
////  digitalWrite(20, HIGH); //Force other Chip into CS high
////
////  delay(10);
//
//  Serial.println("'s' to start");
  EsploraTFT.begin();
  EsploraTFT.background(0,0,0);
  EsploraTFT.stroke(255,50,255);
  EsploraTFT.setTextSize(1.8);
  EsploraTFT.text("SWITCH",0,0);
  EsploraTFT.text("left: 3 min",0,20);
  EsploraTFT.text("right: 2 min",0,40);
  EsploraTFT.text("up: 0-1 min",0,60);
  EsploraTFT.stroke(0,255,255);  
  EsploraTFT.setTextSize(1.8);
  EsploraTFT.text("Light intensity",70,10);
  EsploraTFT.text("SWITCH down: 1",70,30);
  EsploraTFT.text("JOY up: 1/4",70,50);
  EsploraTFT.text("JOY down: 1/2",70,70);
  EsploraTFT.setTextSize(3);

}

int t = 0;
int sc[] = {
  127,
  255,
  383,
  511,
  639,
  767,
  895,
  1023,
  0
};

int ilast1 = 0;
int ilast2 = 0;
int ilast3 = 0;
int ilast4 = 0;
int state = 0;
int i1 = 0;
int i2 = 0;
int i3 = 0;
int i4 = 0;
int iprint = 1;

void frequence() {
  int freq = Esplora.readJoystickY();
  int fCtrl = Esplora.readButton(SWITCH_DOWN);
//  if (fCtrl == LOW) {
    if (freq > 350) {
      i4 = i4 + 1;
    }
    if (i4 > ilast4) {
      iExopDesity = 127;
      ili = 2;
//      EsploraTFT.stroke(255, 255, 255);
//      EsploraTFT.text("1/2", 70, 80);
//      delay(100);
//      EsploraTFT.stroke(0, 0, 0);
//      EsploraTFT.text("1/2", 70, 80);
    }
    ilast4 = i4;
    if (freq < -350) {
      i4 = i4 - 1;
    }
    if (i4 < ilast4) {
      iExopDesity = 63;
      ili = 4;
//      EsploraTFT.stroke(255, 255, 255);
//      EsploraTFT.text("1/4", 70, 80);
//      delay(100);
//      EsploraTFT.stroke(0, 0, 0);
//      EsploraTFT.text("1/4", 70, 80);
    }
    ilast4 = i4;
    if (fCtrl == LOW) {  //if(freq < 350 && freq > -350) {
      iExopDesity = 255;
      ili = 1;
//      EsploraTFT.stroke(255, 255, 255);
//      EsploraTFT.text("1", 70, 80);
//      delay(100);
//      EsploraTFT.stroke(0, 0, 0);
//      EsploraTFT.text("1", 70, 80);
    }
//  }
}

void loop() {
  int slider = Esplora.readSlider();
//  int freq = Esplora.readJoystickY();
  byte spe = 8;
  sc[spe] = map(slider, 0, 1023,  60,  0);
  int v1 = Esplora.readButton(SWITCH_UP);
  int v2 = Esplora.readButton(SWITCH_LEFT);
  int v3 = Esplora.readButton(SWITCH_RIGHT);

  long counter = 0;
  char printout[4];

  frequence();
  
    
//  String spd = String(sc[spe]);
//  spd.toCharArray(spdprintout,5);
//  EsploraTFT.stroke(255, 255, 255);
//  EsploraTFT.text(spdprintout, 0, 30);
//  delay(1000);
//  EsploraTFT.stroke(0, 0, 0);
//  EsploraTFT.text(spdprintout, 0, 30);
  
//  int slider = Esplora.readSlider();
//  byte spe = 8;
//  sc[spe] = map(slider, 0, 1023,  60,  0);
//  int val = Esplora.readButton(SWITCH_UP);
  if (v1 == LOW) {
    i1 = i1 + 1;
  }
  if (i1 != ilast1) {
    state += i1;
//    digitalWrite(pin,HIGH);
    Esplora.writeRGB(100,20,0);
    analogWrite(pine,iExopDesity);//HIGH);
    delay(sc[spe]*500);
    delay(sc[spe]*500);
//    digitalWrite(pin,LOW);
    Esplora.writeRGB(0,0,0);
    digitalWrite(pine,LOW);
    Esplora.tone(500);
    delay(500);
    Esplora.noTone();
    ////
//    Serial.println(sc[spe]);
//    counter = micros();
//    String elapsedTime = String(counter/1000);
//    elapsedTime.toCharArray(spdprintout,5);
//    EsploraTFT.stroke(255, 255, 255);
//    EsploraTFT.text(printout, 0, 90);
//    delay(100);
//    EsploraTFT.stroke(0, 0, 0);
//    EsploraTFT.text(spdprintout, 0, 90);
  }
  ilast1 = i1;

  if (v2 == LOW) {
    i2 = i2 + 1;
  }
  if (i2 != ilast2) {
    state += i2;
//    digitalWrite(pin,HIGH);
    Esplora.writeRGB(100,20,0);
    analogWrite(pine,iExopDesity);//HIGH);
    delay(30*1000);
    delay(30*1000);
    delay(30*1000);
    delay(30*1000);
    delay(30*1000);
    delay(30*1000);
//    digitalWrite(pin,LOW);
    Esplora.writeRGB(0,0,0);
    digitalWrite(pine,LOW);    
    Esplora.tone(500);
    delay(500);
    Esplora.noTone();
  }
  ilast2 = i2;

  if (v3 == LOW) {
    i3 = i3 + 1;
  }
  if (i3 != ilast3) {
    state += i3;
//    digitalWrite(pin,HIGH);
    Esplora.writeRGB(100,20,0);
    analogWrite(pine,iExopDesity);//HIGH);
    delay(30*1000);
    delay(30*1000);
    delay(30*1000);
    delay(30*1000);
//    digitalWrite(pin,LOW);
    Esplora.writeRGB(0,0,0);
    digitalWrite(pine,LOW);
    Esplora.tone(500);
    delay(500);
    Esplora.noTone();
  }
  ilast3 = i3;
//  if (Serial.available()) {
//    char cin = Serial.read();
//    char  check = 'y';
//    switch (cin) {
//      case 's':
//      Serial.println("How long?(s)");
//      while (!Serial.available());
//      t = Serial.parseInt();
//      digitalWrite(pin, HIGH);   
//      delay(t*1000);                       // wait for a second
//      digitalWrite(pin, LOW); 
//      Esplora.tone(500);
//      delay(500);
//      Esplora.noTone();   
//      break;
//    }
//    
//  }
  String spd = String(sc[spe]);
  spd.toCharArray(spdprintout,5);
  ////
  String light = String(ili);
  light.toCharArray(spdprintout1,5);

  EsploraTFT.stroke(255, 255, 255);
  EsploraTFT.text(spdprintout, 0, 80);
  ////
  EsploraTFT.stroke(255, 255, 255);
  EsploraTFT.text("1/", 70, 90);//80
  EsploraTFT.stroke(255, 255, 255);
  EsploraTFT.text(spdprintout1, 100, 90);
  
//  EsploraTFT.text(spdprintout1, 70, 80);
  delay(100);
  EsploraTFT.stroke(0, 0, 0);
  EsploraTFT.text(spdprintout, 0, 80);
  ////
  EsploraTFT.stroke(0, 0, 0);
  EsploraTFT.text("1/", 70, 90);
  EsploraTFT.stroke(0, 0, 0);
  EsploraTFT.text(spdprintout1, 100, 90);
//  EsploraTFT.text(spdprintout1, 70, 80);
  ///iExopDesity
//  String light = String(iExopDesity);
//  light.toCharArray(spdprintout1,5);
//  EsploraTFT.stroke(255, 255, 255);
//  EsploraTFT.text(spdprintout1, 70, 80);
//  delay(100);
//  EsploraTFT.stroke(0, 0, 0);
//  EsploraTFT.text(spdprintout1, 70, 80);


}
