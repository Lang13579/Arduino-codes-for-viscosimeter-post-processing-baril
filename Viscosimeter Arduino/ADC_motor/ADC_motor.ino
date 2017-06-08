#include <SPI.h>
#include <TimerOne.h>
#include "ads12xx.h"

//#define Q_UITE 1
//#define W_HITE 2

//////////
char whichmotor = '0';
int interval = 6000;
int spd = 10;
int i = 0;
int x = LOW;

//////////
long Filter_value;
#define FILTER_N 12
long filter_buf[FILTER_N+1];
long Filter(long data) {
  int i;
  long filter_sum = 0;
  filter_buf[FILTER_N] = data;
  for(i = 0; i < FILTER_N; i++) {
    filter_buf[i] = filter_buf[i + 1]; 
    Serial.println(filter_buf[i]);//filter_sum);
    filter_sum += filter_buf[i];
//    Serial.print(", "); 
  }
  Serial.println("\tEnd.");
  return (long)(filter_sum / FILTER_N);
}

//////////
long crash[FILTER_N+1];
static int c = 0;
void Crash(long data) {
  int i;
  int j = 0;
  crash[i] = data;
  for (i = 0; i < FILTER_N; i++) {
    if (crash[i] > 1120) {
      j++;
    }
  }
  if (j > 8) {
    c = 1;
  }
}

//////////
int  START = 26;
int  CS = 7;
int  DRDY = 1;
ads12xx ADS;

int pin = A0;  //down
int pina = A2;  //up
int pin1 = A3;  //cw
int pin2 = A4;  //ccw
int iHome = A5;

void setup() {
  Serial.begin(115200);
  
  while (!Serial) {
    
  }
//  Serial.println("Serial online");
  pinMode(20, OUTPUT);
  digitalWrite(20, HIGH); //Force other Chip into CS high
  ADS.begin(CS, START, DRDY);  //initialize ADS as object of the ads12xx class

  ADS.Reset();
   ADS.SetRegisterValue(2,38);
  ADS.SetRegisterValue(3,83);   //15Hz Sampling frequency(3 for 2.5Hz, 35 for 10Hz, 83 for 30Hz, 99 for 50Hz,130 for 100Hz,146 for 500Hz)

  pinMode(pin,OUTPUT);
  pinMode(pina,OUTPUT);
  pinMode(pin1,OUTPUT); 

  pinMode(pin2,OUTPUT);
  pinMode(iHome,INPUT);
  digitalWrite(iHome, HIGH);

//  Serial.println("Commands for motor:");
//  Serial.println("'u' to make motor1 up");
//  Serial.println("'i' to make motor1 down");
//  Serial.println("'o' to make motor2 CW");
//  Serial.println("'p' to make motor2 CCW");
//  Serial.println("'a' to go up 2 steps");  modification by SHI Kunkun
//  Serial.println("'5' for the speed");
//  Serial.println("'m' to stop motor");
//  Serial.println("Commands for testing:");
//  Serial.println("'r' to read Register");
//  Serial.println("'j' before 'f'");
//  Serial.println("'f' to be in place");
//  Serial.println("'w' to write Register");
//  Serial.println("'s' to set Register");////////
//  Serial.println("'d' to SDATA");
//  Serial.println("'x' for SDATAC");
//  Serial.println("'h' for this help");
//  delay(100);

  Timer1.initialize(1000000);
  Timer1.attachInterrupt(motor);

}

void motor() {
//  Serial.println(whichmotor);
  int iHome = analogRead(A5);
//  if (x == LOW) {
//    x = HIGH;
//  }
//  else {
//    x = LOW;
//  }
//  digitalWrite(pin1,x);

//  while (check == 'y' || check == 'm') {
//  if (Serial.available()) {
//    check = Serial.read();
//  }

  switch (whichmotor) {
    case '1':                     //up
      if (iHome > 100) {
        digitalWrite(pin,LOW);
        digitalWrite(pin1,LOW);
        digitalWrite(pin2,LOW);
        if (x == LOW) {
          x = HIGH;
        }
        else {
          x = LOW;
        }
        digitalWrite(pina,x);
        i--;
        iHome = analogRead(A5);
      }
      else {
        i = 0;
      }
      break;
    case '2':                   //down
      if (i < 17000){                     //9375,8125,8248,8245,16490
        digitalWrite(pina,LOW);
        digitalWrite(pin1,LOW);
        digitalWrite(pin2,LOW);
        if (x == LOW) {
          x = HIGH;
        }
        else {
          x = LOW;
        }
        digitalWrite(pin,x);
        i++;
      }
      break;
    case '3':                  //cw
      digitalWrite(pin,LOW);
      digitalWrite(pina,LOW);
      digitalWrite(pin2,LOW);
      if (x == LOW) {
        x = HIGH;
      }
      else {
        x = LOW;
      }
      digitalWrite(pin1,x);
      break;
    case '4':                 //ccw
      digitalWrite(pin,LOW);
      digitalWrite(pina,LOW);
      digitalWrite(pin1,LOW);
      if (x == LOW) {
        x = HIGH;
      }
      else {
        x = LOW;
      }
      digitalWrite(pin2,x);
      break;

    default:
      ///////
      digitalWrite(pin,LOW);
      digitalWrite(pin1,LOW);
      digitalWrite(pin2,LOW);
      digitalWrite(pina,LOW);
      break;
    }
//  }
}

//////////
uint32_t read_data() {
  uint32_t data = ADS.GetConversion();
  int timer1 = micros();
  if (long minus = data >> 23 == 1) {
    data = 16777216 - data;
  }
  return data; 

#ifdef ADS1256
        //  double voltage = (4.9986 / 8388608)*data;
        //  Serial.println(voltage);
#endif
}
int initial = 0;
long sum = 0;
int in = 0;
int rd1;

void loop() {
//  Serial.println(interval);
  if (Serial.available()) {
    char cin = Serial.read();
    char check = 'y';
    uint8_t cmd;
    uint8_t cin1;
//    int initial = 0;
//    int sum = 0;
//    int in = 0;

    switch (cin) {
      case 'u':                          //up
       if (check == 'y') {
        if (Serial.available()) {
          check = Serial.read();
        }
//        Serial.println("What speed up?(rpm)");
        while (!Serial.available());
        spd = Serial.parseInt();
        interval = 60000/spd; 
        whichmotor = '1';
       }
        break;
        
      case 'i':                          //down
       if (check == 'y') {
          if (Serial.available()) {
            check = Serial.read();
//            break;
          }
//          Serial.println("What speed down?(rpm)");
            while (!Serial.available()); // Wait for inputting speed value
            spd = Serial.parseInt();
            interval = 60000/spd;
            whichmotor = '2';
//            Serial.println(spd); 
          }
        break;
        
      case 'o':                          //CW
       if (check == 'y') {     
        if (Serial.available()) {
          check = Serial.read();
        }
//        Serial.println("What speed CW?(rpm)");
        while (!Serial.available());
        spd = Serial.parseInt();
        interval = 60000/spd;
        whichmotor = '3'; 
//        Serial.println(spd);
       }
        break;
        
      case 'p':                          //CWW
       if (check == 'y' ) {
        if (Serial.available()) {
          check = Serial.read();
        }
//        Serial.println("What speed CWW?(rpm)");
        while (!Serial.available());
        spd = Serial.parseInt();
        interval = 60000/spd;
        whichmotor = '4';
       }
        break;
/* What I added*/
           case 'a':                          // Adjust
       if (check == 'y' ) {
        if (Serial.available()) {
          check = Serial.read();
        }
//        Serial.println("What speed CWW?(rpm)");
         for (int j = 0; j < 2; j++) {   // artificially go up 2 steps to compensate
            digitalWrite(pina,HIGH);
            delay(1);
            digitalWrite(pina,LOW);
            delay(1);
            i--;
          }
       }
        break;
      
      case '5':
        Serial.println(spd);
        break;
        
      case 'm':
        whichmotor = '0';
        spd = 10;
        interval = 6000;
        break;
        
      case 'j':
         for (int o = 0; o < 8; o++) {
          in = read_data();
          sum += in;
          //delay(10);
          //Serial.println(sum);
          //Serial.println(in);
        }
        initial = sum / 8;
        //Serial.println(initial);
        sum = 0;
          break;
          
      case 'f':                           //find proper position
          spd = 10;
          interval = 6000;
          rd1 = read_data();
          while (rd1 - initial < 200 && check=='y') { 
            if (Serial.available()) {
            check = Serial.read();
          }
            rd1 = read_data();
//            spd = 10;
//            interval = 6000;  
            whichmotor = '2';   //Down
            i++;
          }
          
          whichmotor = '0';               // Stop the motor firstly
          for (int j = 0; j < 10; j++) {   // Then go up 8 steps to compensate
            digitalWrite(pina,HIGH);
            delay(1);
            digitalWrite(pina,LOW);
            delay(1);
            i--;
          }
          
//          Serial.print(rd);
//          Serial.print('  ');
//          Serial.println(rd1);
        
        break;
        
      case 'r':
        Serial.println("Which Register to read?");
        while (!Serial.available());
        Serial.print("Register Value for: ");
        cin1 = Serial.parseInt();
        Serial.println(cin1);
        Serial.println(ADS.GetRegisterValue(cin1));
        break;
        
      case 'w':
        Serial.println("Which Register to write?");
        while (!Serial.available());
        cin1 = Serial.parseInt();
        Serial.println("Which Value to write?");
        while (!Serial.available());
        ADS.SetRegisterValue(cin1, Serial.parseInt());
        break;
        
      case 's':
        ADS.SetRegisterValue(1,50); //Define which pinout to read
        ADS.SetRegisterValue(2,39); //Amplification factor
        ADS.SetRegisterValue(3,3); 
//        Serial.println("set successfully");
        break;
        
      case 'x':
//        Serial.println("Stop SDATAC");
        check = 'x';//ADS.Reset();
        break;
        
      case 'd':
        while (check != 'x') {
          if (Serial.available()) {
            check = Serial.read();
          }
          
          Serial.println(read_data());
//        uint32_t data = ADS.GetConversion();
//        int timer1 = micros();
////        for(int i; i < )ADS.GetConversion();  //LPF
//        if (long minus = data >> 23 == 1) {
////          long data = 
//          data = 16777216 - data;
////          delay(10);
//        }
//
////       else
//        Serial.println(data); 
////        Serial.print(' ');//
////        int data() {
////          return data;
////        }
////        int Filter_value = Filter(data);//
////        Serial.println(Filter_value);//
////        Serial.println(filter_sum/FILTER_N);        
////       
//#ifdef ADS1256
//        //  double voltage = (4.9986 / 8388608)*data;
//        //  Serial.println(voltage);
//#endif
        }
       break;
  
      default:
       break;
    }
  }
//  noInterrupts();
//  Serial.println(i);
  Timer1.setPeriod(interval);
//  interrupts();
}
