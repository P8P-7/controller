#include <SoftwareSerial.h> //Serial library

static const char *CONTROLS[] = {
    "JoyStickLx","JoyStickLy","JoyStickRx","JoyStickRy","button1","button2","button3","button4"
};


SoftwareSerial bt (5,6);  //RX, TX (Switched on the Bluetooth - RX -> TX | TX -> RX)

int JoyStickLx;
int JoyStickLy;
int JoyStickRx;
int JoyStickRy;
int x1;
int y1;
int x2;
int y2;
int deadzone = 20;

void sendMessage(String control, int state){
  bt.write("{");
  bt.print(control);
  bt.write(":");
  bt.print(state);
  bt.write("}");
};

void setup() {
  Serial.begin(9600);
  bt.begin(9600);
};

void loop() {
  
// JoyStickLeftx---------------------------------------------------------
JoyStickLx = analogRead(A5)-511;                     // Joystick uitlezen
if(JoyStickLx < deadzone and JoyStickLx > -deadzone){
  if(x1!=0){
    x1=0;
    sendMessage("JSLX", x1);}}
else if(JoyStickLx > (511-deadzone)){
  if(x1!=511){
    x1=511;
    sendMessage("JSLX", x1);}}
else if(JoyStickLx < (deadzone-511)){
  if(x1!=-511){
    x1=-511;
    sendMessage("JSLX", x1);}}
else if(abs(JoyStickLx - x1) > 2){
  x1 = JoyStickLx;
  sendMessage("JSLX", JoyStickLx);}
 
// JoyStickLefty---------------------------------------------------------
JoyStickLy = analogRead(A4)-511;                     // Joystick uitlezen
if(JoyStickLy < deadzone and JoyStickLy > -deadzone){
  if(y1!=0){
    y1=0;
    sendMessage("JSLY", y1);}}
else if(JoyStickLy > (511-deadzone)){
  if(y1!=511){
    y1=511;
    sendMessage("JSLY", y1);}}
else if(JoyStickLy < (deadzone-511)){
  if(y1!=-511){
    y1=-511;
    sendMessage("JSLY", y1);}}
else if(abs(JoyStickLy - y1) > 2){
  y1 = JoyStickLy;
  sendMessage("JSLY", JoyStickLy);}
  
// JoyStickRightx---------------------------------------------------------
JoyStickRx = analogRead(A3)-511;                      // Joystick uitlezen
if(JoyStickRx < deadzone and JoyStickRx > -deadzone){
  if(x2!=0){
    x2=0;
    sendMessage("JSRX", x2);}}
else if(JoyStickRx > (511-deadzone)){
  if(x2!=511){
    x2=511;
    sendMessage("JSRX", x2);}}
else if(JoyStickRx < (deadzone-511)){
  if(x2!=-511){
    x2=-511;
    sendMessage("JSRX", x2);}}
else if(abs(JoyStickRx - x2) > 2){
  x2 = JoyStickRx;
  sendMessage("JSRX", JoyStickRx);}

// JoyStickRighty---------------------------------------------------------
JoyStickRy = analogRead(A2)-511;                      // Joystick uitlezen
if(JoyStickRy < deadzone and JoyStickRy > -deadzone){
  if(y2!=0){
    y2=0;
    sendMessage("JSRY", y2);}}
else if(JoyStickRy > (511-deadzone)){
  if(y2!=511){
    y2=511;
    sendMessage("JSRY", y2);}}
else if(JoyStickRy < (deadzone-511)){
  if(y2!=-511){
    y2=-511;
    sendMessage("JSRY", y2);}}
else if(abs(JoyStickRy - y2) > 2){
  y2 = JoyStickRy;
  sendMessage("JSRY", JoyStickRy);}  
delay(10);

Serial.print("n0.val=");
Serial.print(x1);
Serial.write(0xff);
Serial.write(0xff);
Serial.write(0xff);

};
