#include <Nextion.h>

#include <SoftwareSerial.h> //Serial library

static const char *WAARDE[] = {
    "n0.val=","n1.val=","n2.val=","n3.val=","n4.val=","n8.val=","j0.val=","j1.val=","j2.val="
};

enum CONTROLS {                           // Verzonden controls
  JSLX,
  JSLY,
  JSRX,
  JSRY,
  BTN1,
  BTN2,
  BTN3,
  BTN4,
};

enum CONFIG {                             // Verzonden modes
  MODE,
  SENSITIVITY
};

enum STATUS : unsigned char {
  CONNECTED,
  INVALID_INPUT,
  BATTERY,
};

SoftwareSerial bt (5,6);                  //RX, TX (Switched on the Bluetooth - RX -> TX | TX -> RX)

int JoyStickLx;
int JoyStickLy;
int JoyStickRx;
int JoyStickRy;
int x1;
int y1;
int x2;
int y2;
int SetDeadzone;
int Deadzone = 20;
int CurrentPage = 0;
int Sensitivity = 100;
int btSensitivity = 255;
int var1, var2, var3, var4;
int Batterij, BatterijDisp;
int x;
char c;
String Waarde;


// Buttons toevoegen---------------------
NexNumber n5 = NexNumber(1, 16, "n5");
NexButton b0 = NexButton(0, 3, "b0");
NexButton b1 = NexButton(0, 5, "b1");   
NexButton b4 = NexButton(1, 3, "b4");
NexButton b5 = NexButton(1, 4, "b5");
NexButton b7 = NexButton(4, 1, "b7");
NexButton b8 = NexButton(4, 2, "b8");
NexButton b9 = NexButton(4, 3, "b9");
NexButton b10 = NexButton(4, 4, "b10");
NexButton b11 = NexButton(1, 16, "b11");
NexButton b12 = NexButton(1, 17, "b12");
NexButton b13 = NexButton(3, 2, "b13");
NexButton b14 = NexButton(3, 3, "b14");
NexButton b15 = NexButton(3, 4, "b15");
NexButton b16 = NexButton(3, 5, "b16");
NexButton b17 = NexButton(4, 1, "b17");
NexButton b18 = NexButton(4, 2, "b18");
NexButton b19 = NexButton(4, 3, "b19");
NexButton b20 = NexButton(4, 4, "b20");

// Pagina's toevoegen--------------------
NexPage page0 = NexPage(0, 0, "page0");
NexPage page1 = NexPage(1, 0, "page1");
NexPage page2 = NexPage(2, 0, "page2");
NexPage page3 = NexPage(3, 0, "page3");
NexPage page4 = NexPage(4, 0, "page4");
NexPage page5 = NexPage(5, 0, "page5");

//Luisterlijst---------------------------
NexTouch *nex_listen_list[] = 
{   &b1,
    &b0,
    &b4,
    &b5,
    &b7,
    &b8,
    &b9,
    &b10,
    &b11,
    &b12,
    &b13,
    &b14,
    &b15,
    &b16,
    &b17,
    &b18,
    &b19,
    &b20,
    &page0,
    &page1,
    &page2,
    &page3,
    &page4,
    NULL };

void einderegel() {                                 // Einde bericht display
  Serial.write(0xff);
  Serial.write(0xff);
  Serial.write(0xff);}

void b4PushCallback(void *ptr) {                    // Uitvoer voor knop b4 van display (Deadzone instellen)
  Deadzone++;
  sendDisplay(WAARDE[0], Deadzone);}

void b5PushCallback(void *ptr) {                    // Uitvoer voor knop b5 van display (Deadzone instellen)
  if(Deadzone>0) {
  Deadzone--;
  sendDisplay(WAARDE[0], Deadzone);}}
  
void sendDisplay(String Waarde, int Beun) {         // Verstuurd analoge waardes naar display
  //if(CurrentPage == 1) {
  einderegel();
  Serial.print(Waarde);
  Serial.print(Beun);
  einderegel();}//}

void sendControl(CONTROLS control, int state) {     // Verstuurd CONTROLS via bluetooth
  if(CurrentPage != 1) {
  bt.write("{0;");
  bt.print(control);
  bt.write(":");
  bt.print(state);
  bt.write("}");}}

void sendConfig(CONFIG conf, int state) {           // Verstuurd CONFIG via bluetooth
  bt.write("{1;");
  bt.print(conf);
  bt.write(":");
  bt.print(state);
  bt.write("}");}

void sendStatusRequest() {                          // Verstuurd foutcode via bluetooth
  bt.write("{2;0:0}");}

void page0PushCallback(void *ptr)  {                // Init actie op pagina 0
  sendDisplay(WAARDE[6], BatterijDisp);
  CurrentPage = 0;}

void page1PushCallback(void *ptr)  {                // Init actie op pagina 1
  CurrentPage = 1;
  sendDisplay(WAARDE[7], Sensitivity);
  sendDisplay(WAARDE[6], BatterijDisp);
  sendDisplay(WAARDE[0], Deadzone);
  }
    
void page2PushCallback(void *ptr)  {                // Init actie op pagina 2
  sendDisplay(WAARDE[6], BatterijDisp);
  CurrentPage = 2;}

void page3PushCallback(void *ptr)  {                // Init actie op pagina 3
  sendDisplay(WAARDE[6], BatterijDisp);
  CurrentPage = 3;}

void page4PushCallback(void *ptr)  {                // Init actie op pagina 4
  sendDisplay(WAARDE[6], BatterijDisp);
  CurrentPage = 4;}

void b11PushCallback(void *ptr) {                   // Actie bij push b11
  if(Sensitivity <= 90){
    Sensitivity = Sensitivity + 10;
    btSensitivity = btSensitivity + 13;
    sendDisplay(WAARDE[7], Sensitivity);
    sendConfig(SENSITIVITY, btSensitivity);}
}

void b12PushCallback(void *ptr) {                   // Actie bij push b12
  if(Sensitivity >= 10){
    Sensitivity = Sensitivity - 10;
    btSensitivity = btSensitivity - 13;
    sendDisplay(WAARDE[7], Sensitivity);
    sendConfig(SENSITIVITY, btSensitivity);}
}

void b13PushCallback(void *ptr) {                   // Actie bij push b13
  sendConfig(MODE, 0);
}

void b14PushCallback(void *ptr) {                   // Actie bij push b14
  sendConfig(MODE, 1);
}

void b15PushCallback(void *ptr) {                   // Actie bij push b15
  sendConfig(MODE, 2);
}

void b16PushCallback(void *ptr) {                   // Actie bij push b16
  sendConfig(MODE, 3);
}

void b17PushCallback(void *ptr) {                   // Actie bij push b17
  // Preset Ramp
}

void b18PushCallback(void *ptr) {                   // Actie bij push b18
  // Preset Flat
}

void b19PushCallback(void *ptr) {                   // Actie bij push b19
  // Preset Up
}

void b20PushCallback(void *ptr) {                   // Actie bij push b20
  // Preset Down
}

void receiveMessage() {                             // Ontvangen bericht verwerken vanaf bluetooth
  String string;
  int type, value;
  
  while(bt.available()) {
    string += (char)bt.read();
  }
  
  int start = string.indexOf('{');
  int seperator = string.indexOf(':');
  int last = string.indexOf('}');
  
  if((start >= 0) && (start < seperator) && (seperator < last)){
    type = string.substring(start + 1, seperator).toInt();
    value = string.substring(seperator + 1, last).toInt();  
  }
  else {
      sendStatusRequest();
      Serial.write("error");}

  if(type == 2) {                                   // Verstuurd bericht naar display
   sendDisplay(WAARDE[8],value);
  }
}

void setup() {
  pinMode(8, INPUT);
  pinMode(9, INPUT);
  pinMode(10, INPUT);
  pinMode(11, INPUT);
  Serial.begin(9600);
  bt.begin(9600);
  delay(2000);
  bt.print("AT+NAMEGOLIATH");
  delay(2000);
  bt.print("AT+BAUD6");
  
// Displayfuncties aanmaken--------------------------------------------  
  b4.attachPush(b4PushCallback);
  b5.attachPush(b5PushCallback);
  b11.attachPush(b11PushCallback);
  b12.attachPush(b12PushCallback);
  b13.attachPush(b13PushCallback);
  b14.attachPush(b14PushCallback);
  b15.attachPush(b15PushCallback);
  b16.attachPush(b16PushCallback);
  page0.attachPush(page0PushCallback);
  page1.attachPush(page1PushCallback);
  page2.attachPush(page2PushCallback); 
  page3.attachPush(page2PushCallback); 
  page4.attachPush(page2PushCallback);
};

void loop() {
// modes vleugels
// mode0 = voor appart
// mode1 = achter appart

  nexLoop(nex_listen_list);                         // Luisteren of er data van display komt

  if (bt.available()) {
    receiveMessage();
  }
  
  // ButtonControl-------------------------------------------------------
  if(digitalRead(8) != var1) {                      // BTN1 wordt doorgestuurd via bt
      var1 = digitalRead(8);
      sendControl(BTN1, digitalRead(8));}
  
  if(digitalRead(9) != var2) {                      // BTN2 wordt doorgestuurd via bt
      var2 = digitalRead(9);
      sendControl(BTN2, digitalRead(9));}
  
  if(digitalRead(10) != var3) {                     // BTN3 wordt doorgestuurd via bt
      var3 = digitalRead(10);
      sendControl(BTN3, digitalRead(10));}
  
  if(digitalRead(11) != var4) {                     // BTN4 wordt doorgestuurd via bt
      var4 = digitalRead(11);
      sendControl(BTN4, digitalRead(11));}
    
  // BatterijSpanning------------------------------------------------------
  // 150 tot 464 (analog - 512)
  // -4 tot 310 (analog - 666) 6.6V - 9.6V
  
  if(abs((analogRead(A4)-666) - Batterij) > 20) {   // Batterijspanning inlezen
    Batterij = (analogRead(A4)-666);
    BatterijDisp = Batterij * 100 / 310;
    sendDisplay(WAARDE[6], BatterijDisp);}
    
  // JoyStickLeftx---------------------------------------------------------
  JoyStickLx = analogRead(A0)-511;                     // Joystick JSLX uitlezen
  if((JoyStickLx < Deadzone) and (JoyStickLx > -Deadzone)) {
    if(x1 != 0) {
      x1 = 0;
      sendControl(JSLX, x1);
      sendDisplay(WAARDE[1], x1);}}
  else if(JoyStickLx > (511 - Deadzone)) {
    if(x1 != 511) {
      x1 = 511;
      sendControl(JSLX, x1);
      sendDisplay(WAARDE[1], x1);}}
  else if(JoyStickLx < (Deadzone - 511)) {
    if(x1 != -511) {
      x1 = -511;
      sendControl(JSLX, x1);
      sendDisplay(WAARDE[1], x1);}}
  else if(abs(JoyStickLx - x1) > 5) {
    x1 = JoyStickLx;
    sendControl(JSLX, x1);
    sendDisplay(WAARDE[1], x1);}
   
  // JoyStickLefty---------------------------------------------------------
  JoyStickLy = analogRead(A1)-511;                     // Joystick JSLY uitlezen
  if((JoyStickLy < Deadzone) and (JoyStickLy > -Deadzone)) {
    if(y1 != 0) {
      y1 = 0;
      sendControl(JSLY, y1);
      sendDisplay(WAARDE[2], y1);}}
  else if(JoyStickLy > (511 - Deadzone)) {
    if(y1 != 511) {
      y1 = 511;
      sendControl(JSLY, y1);
      sendDisplay(WAARDE[2], y1);}}
  else if(JoyStickLy < (Deadzone - 511)) {
    if(y1 != -511) {
      y1 = -511;
      sendControl(JSLY, y1);
      sendDisplay(WAARDE[2], y1);}}
  else if(abs(JoyStickLy - y1) > 5) {
    y1 = JoyStickLy;
    sendControl(JSLY, y1);
    sendDisplay(WAARDE[2], y1);}
    
  // JoyStickRightx---------------------------------------------------------
  JoyStickRx = analogRead(A2)-511;                      // Joystick JSRX uitlezen
  if((JoyStickRx < Deadzone) and (JoyStickRx > -Deadzone)) {
    if(x2 != 0) {
      x2 = 0;
      sendControl(JSRX, x2);
      sendDisplay(WAARDE[3], x2);}}
  else if(JoyStickRx > (511 - Deadzone)) {
    if(x2 != 511){
      x2 = 511;
      sendControl(JSRX, x2);
      sendDisplay(WAARDE[3], x2);}}
  else if(JoyStickRx < (Deadzone - 511)) {
    if(x2 != -511) {
      x2 = -511;
      sendControl(JSRX, x2);
      sendDisplay(WAARDE[3], x2);}}
  else if(abs(JoyStickRx - x2) > 5) {
    x2 = JoyStickRx;
    sendControl(JSRX, x2);
    sendDisplay(WAARDE[3], x2);}
  
  // JoyStickRighty---------------------------------------------------------
  JoyStickRy = analogRead(A3)-511;                      // Joystick JSRY uitlezen
  if((JoyStickRy < Deadzone) and (JoyStickRy > -Deadzone)) {
    if(y2 != 0){
      y2  = 0;
      sendControl(JSRY, y2);
      sendDisplay(WAARDE[4], y2);}}
  else if(JoyStickRy > (511 - Deadzone)) {
    if(y2 != 511){
      y2 = 511;
      sendControl(JSRY, y2);
      sendDisplay(WAARDE[4], y2);}}
  else if(JoyStickRy < (Deadzone - 511)) {
    if(y2 != -511) {
      y2 = -511;
      sendControl(JSRY, y2);
      sendDisplay(WAARDE[4], y2);}}
  else if(abs(JoyStickRy - y2) > 5) {
    y2 = JoyStickRy;
    sendControl(JSRY, y2);
    sendDisplay(WAARDE[4], y2);}  
  delay(10);
};
