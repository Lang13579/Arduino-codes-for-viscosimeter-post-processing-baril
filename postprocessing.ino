// Please inspect Notes_Credits tab for references to this work
#pragma GCC diagnostic ignored "-Wwrite-strings"

#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <PgmSpace.h>

int pina = D1;//PWM
int pine = D2;//Direction
int pin1 = D3;//Rotation
int pin2 = D5;//UV
//char cin;

const char web_site[] PROGMEM = R"=====(
  <!DOCTYPE html>
<head>
<style>
#header {
    color:#006284;
    text-align:center;
    padding:5px;
    font-family:arial;
}
#nav { 
    font-family:arial;
    color:#0089A7;
    width:375px;
    height:500px;
    float:left;
    text-align:center;
    padding:0px;        
}
#section {
    font-family:arial;
    color:#0089A7;
    width:375px;
    heirht:500px;
    float:left;
    text-align:center;
    padding:0px;     
}
#footer {
    color:white;
    clear:both;
    text-align:center;
    padding:5px;     
}
</style>
</head>

<body>
<body bgcolor=#D9CD90>

<div id="header">
<h1>Hey guys, this is the controlling page for our post processing barrel</h1>
<br>
<br>
</div>

<div id="nav">

</div>

<div id="nav">
<h2>Dry up first</h2>
<br>
<a href=UP><button>UP</button></a>&nbsp;<a href=DOWN><button>DOWN</button></a>  
<br>
<br>
<form action='dry'>Duration(s): <input type='text' name='duration1'>
<br>
<br>
Speed: 
<select name='state1'>
<option value='0'>0</option>
<option value='1'>1</option>
<option value='2'>2</option>
<option value='3'>3</option>
<option value='4'>4</option>
<option value='5'>5</option>
</select>&nbsp;
<input type='submit' value='ROTATE'>
<input type='submit' value='STOP'> 
</form>
</div>


<div id="section">
<h2>UV exposure later</h2>
<br>
<form action='uv'>UV light desity: 
<select name='desity'>
<option value='0'>0</option>
<option value='1'>1</option>
<option value='2'>2</option>
<option value='3'>3</option>
<option value='4'>4</option>
</select>&nbsp;
<br>
<br>
Duration(s): <input type='text' name='duration2'>
<br>
<br>


<input type='submit' value='START'>
<input type='submit' value='STOP'> 
</form>
</div>


<div id="footer">
<img src='http://kast3d.io.img.800cdn.com/kast3d.io_files/Kast-3D-logo.png'width='200'height='80'>
</div>

</body>

  </html>
)=====";

//Speed: 
//<select name='state2'>
//<option value='0'>0</option>
//<option value='1'>1</option>
//<option value='2'>2</option>
//<option value='3'>3</option>
//<option value='4'>4</option>
//<option value='5'>5</option>
//<option value='6'>6</option>
//<option value='7'>7</option>
//<option value='8'>8</option>
//<option value='9'>9</option>
//</select>&nbsp;

 //\"DOWN\"

//String form =                                             // String form to sent to the client-browser
////  "<script type='text/javascript'>"
////  "var c=100"
////  "var t"
////  "function timedCount(){"
////  "document.getElementById('txt').value=c"
////  "c=c-1"
////  "t=setTimeout('timedCount()',1000)}"
////  "function stopCount(){"
////  "c=0"
////  "setTimeout('document.getElementById('txt').value=0',0)"
////  "clearTimeout(t)}"
////  "</script>"
//  /////////
//  "<p>"
////  "<body background='http://kast3d.io.img.800cdn.com/kast3d.io_files/Kast-3D-logo.png''width='10'height='4'>"
//  "<body bgcolor=#D9CD90>"//986DB2 B28FCE
//  "<center>"
//  "<br>"
//  "<br>"
//  "<br>"
//  "<h1 style='font-family:arial;color:#0089A7;'>Hey guys, this is the controlling page for our post processing barrel.</h1>"
//  "<br>"
////  "<h3 style='font-family:arial;color:#33A6B8'>'up', 'down','r+level(0-9)'<h3>"
//  "<p style='font-family:arial;color:#33A6B8'>TRY TO CONTROL IT"
//  "<p><a style='font-family:arial;color:#0089A7' href=\"UP\"><button>UP</button></a>&nbsp;<a style='font-family:arial;color:#0089A7' href=\"DOWN\"><button>DOWN</button></a></p>"
//  
//  "<form action='msg'><p style='font-family:arial;color:#0089A7'>Duration(s) :<input type='text' name='duration'></p> "  //id='txt'
////  "</form>"
//  "<p style='font-family:arial;color:#0089A7'>Speed :"
//  "<select name='state'>" 
//    "<option value='0'>0</option>"
//    "<option value='1'>1</option>"
//    "<option value='2'>2</option>"
//    "<option value='3'>3</option>"
//    "<option value='4'>4</option>"
//    "<option value='5'>5</option>"
//    "<option value='6'>6</option>"
//    "<option value='7'>7</option>"
//    "<option value='8'>8</option>"
//    "<option value='9'>9</option>"
//  "</select>&nbsp;"
//  
////  "<p style='font-family:arial;color:#0089A7'><a href=\"ROTATE\"><button>ROTATE</button></a>&nbsp;<a href=\"STOP\"><button>STOP</button></a></p>"
//
////  "<a style='font-family:arial;color:#0089A7' href=\"ROTATE\"><button>ROTATE</button></a>"
//  "<input  style='font-family:arial;color:#0089A7' type='submit' value='ROTATE'> "// onClick='timedCount()'
//  "<input  style='font-family:arial;color:#0089A7' type='submit' value='STOP'> "// onClick='stopCount()'
//  "</form>"
////  "<p style='font-family:arial;color:#0089A7'><a href=\"STOP\"><button>STOP</button></a></p>"
//  "<br>"
//  "<img src='http://kast3d.io.img.800cdn.com/kast3d.io_files/Kast-3D-logo.png'width='200'height='80'>"
//  
//  "</center>";


#define SSID "TP-LINK_EED772"  //"KAST"  "fangsong"                                 // insert your SSID
#define PASS "1FEED772"        //"39115355"  //"grandpotato380"                                   // insert your password

// Instantiate server class                               // See tab Network for #include
ESP8266WebServer server(80);                              // HTTP server will listen at port 80

char flag = '0';
int state1 = 0;
int tim1 = 0;
int spd1;
int state2 = 0;
int spd2;
int light = 0;
int desity = 0;
unsigned long pM = 0;

void setup(void) 
{
  //ESP.wdtDisable();                                     // used to debug, disable wachdog timer,
  Serial.begin(115200);                                   // full speed to monitor

  pinMode(pina, OUTPUT);
  pinMode(pine, OUTPUT);
  pinMode(pin1, OUTPUT);
  pinMode(pin2, OUTPUT);
  digitalWrite(pina, LOW);
  digitalWrite(pine, LOW);
  analogWrite(pin1, 0);
  analogWrite(pin2, 0);
  
  WiFi.begin(SSID, PASS);                                 // Connect to WiFi network

  while (WiFi.status() != WL_CONNECTED) {                 // Wait for connection
    delay(500);
    Serial.print(".");
  }

  // Set up the endpoints for HTTP server,  Endpoints can be written as inline functions:
  server.on("/", []()
  {
    server.send(200, "text/html", web_site);//form
  });

  server.on("/UP", [](){
    server.send(200, "text/html", web_site);//form
    flag = '1';
    tim1 = 5000;
    pM = millis();
    Serial.println(pM);
//      for(int i = 0;i < 10;i++){   //&&sp == 1
//        if(1) {    
//        server.on("/STOP",[](){
//          sp = 0;
//        });
    Serial.println("ready to up");
//        digitalWrite(pine,HIGH);
//        digitalWrite(pina,HIGH);
//        delay(1000);
//        digitalWrite(pina,LOW);
//        }
//      }
  });
  
  server.on("/DOWN", [](){
    server.send(200, "text/html", web_site);//form
    flag = '2';
    tim1 = 5000;
    pM = millis();
    Serial.println("ready to down");
  }); 

//  server.on("/msg1",[](){
//    server.send(200, "text/html", web_site);//
//    flag = '3';
//    String msg1 = server.arg("duration");//
//    tim = 1000*server.arg("duration").toInt();//
    
//    String decodedMsg1 = msg1;
//    int tim = 0;
//    char t;
//    unsigned int lengte = decodedMsg.length();
//    for(int i = 0: i < lengte; i++) {
//      t = decodedMsg1[lengte - i];
//      int im = t - '0';
//      tim = tim + im*pow(10,lengte - i);
//    }
//    Serial.println(tim);//

//  });//
  
  server.on("/dry", [](){ //handle_msg{
    server.send(200, "text/html", web_site);//form
    pM = millis();
    flag = '3';
    String msg1 = server.arg("duration1");
    tim1 = 1000*server.arg("duration1").toInt();
    String msg = server.arg("state1");
    state1 = server.arg("state1").toInt();
//      String decodedMsg = msg;
//      Serial.println(decodedMsg);                           // print original string to monitor
//      unsigned int lengte = decodedMsg.length();              // length of received message
//      char s = decodedMsg[0];
//      int sp = s - '0';
//      int spd;
    spd1 = map(state1, 0, 5, 0, 511);//(state1, 0, 9, 0, 1023); 
    Serial.println(tim1); 
    Serial.println(spd1);
  });                          // And as regular external functions:

  server.on("/uv",[](){
    server.send(200, "text/html", web_site);//form
    pM = millis();
    flag = '4';
    String msg2 = server.arg("duration2");
    tim1 = 1000*server.arg("duration2").toInt();
//    String msg3 = server.arg("state2");
//    state2 = server.arg("state2").toInt();
//    spd2 = map(state2, 0, 9, 0, 1023); 
    spd2 = 80;
    light = server.arg("desity").toInt();
    desity = map(light, 0, 4, 0, 1023);
    Serial.println(desity);
    Serial.println(tim1); 
    Serial.println(spd2);
    Serial.println(pM);
  });

//  server.on("/ROTATE", [](){
//    flag = '3';
//    pM = millis();
//    Serial.println("rotate");
//  });

  server.on("/STOP", [](){
    flag = '0';
  });
  
  server.begin();                                         // Start the server

  Serial.print("SSID : ");                                // prints SSID in monitor
  Serial.println(SSID);                                   // to monitor

  char result[16];

  Serial.println("WebServer ready:   ");
  Serial.println(WiFi.localIP());                         // Serial monitor prints localIP
  Serial.println(); 
  Serial.println(result);
  server.begin();
}


void motor() {
  switch (flag) {
    case '1':                        //up
      digitalWrite(pine,HIGH);
      digitalWrite(pina,HIGH);
      break;
    case '2':                        //down
      digitalWrite(pine,LOW);
      digitalWrite(pina,HIGH);
      break;
    case '3':                        //dry
      analogWrite(pin1,spd1);
      break;
    case '4':                        //uv
      //////
      analogWrite(pin1,spd2);
      analogWrite(pin2,desity);
      break;
    case '0':                        //stop
      digitalWrite(pina,LOW);
      analogWrite(pin1,0);
      analogWrite(pin2,0);
      break;
  }
}

void loop(void) {
  server.handleClient();                                  // checks for incoming messages
  unsigned long cM = millis();
//  motor();
  if (cM - pM < tim1) {
    if (flag == '4') {
      if ((cM - pM) % 8000 < 500) {
        motor();
      }
      else {
        analogWrite(pin1,0);
        analogWrite(pin2,desity);
      }
    }
    else {
      motor();
    }
  }
  else {
//  if (cM - pM > tim1) {
    digitalWrite(pina,LOW);
    analogWrite(pin1,0);
    analogWrite(pin2,0);
    ////////
    if (flag == '1') {
      digitalWrite(pine,LOW);
      digitalWrite(pina,HIGH);
      delay(300);
      digitalWrite(pina,LOW);
      Serial.println(flag);
    }
    flag = '0';
//    Serial.println(flag);
  }
}


