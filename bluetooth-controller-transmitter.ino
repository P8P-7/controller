#include <Nextion.h>

#include <SoftwareSerial.h> //Serial library

static const char *WAARDE[] = {
    "n1.val=","n2.val=","n3.val=","n4.val="
};

enum CONTROLS {
  JSLX,
  JSLY,
  JSRX,
  JSRY,
  BTN1,
  BTN2,
  BTN3,
  BTN4,
  POS1,
  POS2,
  POS3,
  POS4
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
int SetDeadzone;
int Deadzone;
int CurrentPage = 0;
String Waarde;

// Buttons toevoegen---------------------
NexNumber n0 = NexNumber(0, 1, "n0");
NexButton b0 = NexButton(0, 3, "b0");
NexButton b1 = NexButton(0, 5, "b1");   
NexButton b4 = NexButton(1, 3, "b4");
NexButton b5 = NexButton(1, 4, "b5");
NexButton b7 = NexButton(4, 1, "b7");
NexButton b8 = NexButton(4, 2, "b8");
NexButton b9 = NexButton(4, 3, "b9");
NexButton b10 = NexButton(4, 4, "b10");

// Pagina's toevoegen--------------------
NexPage page0 = NexPage(0, 0, "page0");
NexPage page1 = NexPage(1, 0, "page1");
NexPage page2 = NexPage(2, 0, "page2");
NexPage page3 = NexPage(3, 0, "page3");
NexPage page4 = NexPage(4, 0, "page4");

//Luisterlijst---------------------------
NexTouch *nex_listen_list[] = 
{   &b1,
    &b0,
    &b4,
    &b5,
    &page0,
    &page1,
    &page2,
    &page3,
    &page4,
    NULL };

void einderegel() {
  Serial.write(0xff);
  Serial.write(0xff);
  Serial.write(0xff);}

void b4PushCallback(void *ptr) {  
  Deadzone++;
  einderegel();
  Serial.print("n0.val=");
  Serial.print(Deadzone);
  einderegel();}

void b5PushCallback(void *ptr) {  
  if(Deadzone>0) {
  Deadzone--;
  einderegel();
  Serial.print("n0.val=");
  Serial.print(Deadzone);}
  einderegel();}
  
void sendDisplay(String Waarde, int Beun) {
  if(CurrentPage == 1) {
  einderegel();
  Serial.print(Waarde);
  Serial.print(Beun);
  einderegel();}}

void sendMessage(CONTROLS control, int state) {
  if(CurrentPage != 1) {
  bt.write("{");
  bt.print(control);
  bt.write(":");
  bt.print(state);
  bt.write("}");}}

void page0PushCallback(void *ptr)  {
  CurrentPage = 0;}

void page1PushCallback(void *ptr)  {
  SetDeadzone = 1;
  CurrentPage = 1;}

void page2PushCallback(void *ptr)  {
  CurrentPage = 2;}

void page3PushCallback(void *ptr)  {
  CurrentPage = 3;}

void page4PushCallback(void *ptr)  {
  CurrentPage = 4;}

void setup() {
  Serial.begin(9600);
  bt.begin(9600);
  b4.attachPush(b4PushCallback);
  b5.attachPush(b5PushCallback);
  page0.attachPush(page0PushCallback);
  page1.attachPush(page1PushCallback);
  page2.attachPush(page2PushCallback); 
  page3.attachPush(page2PushCallback); 
  page4.attachPush(page2PushCallback); 
};

void loop() {
nexLoop(nex_listen_list);

if(SetDeadzone == 1) {
  einderegel();
  Serial.print("n0.val=");
  Serial.print(Deadzone);
  einderegel(); 
  SetDeadzone = 0;
}
  
// JoyStickLeftx---------------------------------------------------------
JoyStickLx = analogRead(A5)-511;                     // Joystick uitlezen
if(JoyStickLx < Deadzone and JoyStickLx > -Deadzone) {
  if(x1 != 0) {
    x1 = 0;
    sendMessage(JSLX, x1);
    sendDisplay(WAARDE[0], x1);}}
else if(JoyStickLx > (511 - Deadzone)) {
  if(x1 != 511) {
    x1 = 511;
    sendMessage(JSLX, x1);
    sendDisplay(WAARDE[0], x1);}}
else if(JoyStickLx < (Deadzone - 511)) {
  if(x1 != -511) {
    x1 = -511;
    sendMessage(JSLX, x1);
    sendDisplay(WAARDE[0], x1);}}
else if(abs(JoyStickLx - x1) > 2) {
  x1 = JoyStickLx;
  sendMessage(JSLX, JoyStickLx);
  sendDisplay(WAARDE[0], JoyStickLx);}
 
// JoyStickLefty---------------------------------------------------------
JoyStickLy = analogRead(A4)-511;                     // Joystick uitlezen
if(JoyStickLy < Deadzone and JoyStickLy > -Deadzone) {
  if(y1 != 0) {
    y1 = 0;
    sendMessage(JSLY, y1);
    sendDisplay(WAARDE[1], y1);}}
else if(JoyStickLy > (511 - Deadzone)) {
  if(y1 != 511) {
    y1 = 511;
    sendMessage(JSLY, y1);
    sendDisplay(WAARDE[1], y1);}}
else if(JoyStickLy < (Deadzone - 511)) {
  if(y1 != -511) {
    y1 = -511;
    sendMessage(JSLY, y1);
    sendDisplay(WAARDE[1], y1);}}
else if(abs(JoyStickLy - y1) > 2) {
  y1 = JoyStickLy;
  sendMessage(JSLY, JoyStickLy);
  sendDisplay(WAARDE[1], JoyStickLy);}
  
// JoyStickRightx---------------------------------------------------------
JoyStickRx = analogRead(A3)-511;                      // Joystick uitlezen
if(JoyStickRx < Deadzone and JoyStickRx > -Deadzone) {
  if(x2 != 0) {
    x2 = 0;
    sendMessage(JSRX, x2);
    sendDisplay(WAARDE[2], x2);}}
else if(JoyStickRx > (511 - Deadzone)) {
  if(x2 != 511){
    x2 = 511;
    sendMessage(JSRX, x2);
    sendDisplay(WAARDE[2], x2);}}
else if(JoyStickRx < (Deadzone - 511)) {
  if(x2 != -511) {
    x2 = -511;
    sendMessage(JSRX, x2);
    sendDisplay(WAARDE[2], x2);}}
else if(abs(JoyStickRx - x2) > 2) {
  x2 = JoyStickRx;
  sendMessage(JSRX, JoyStickRx);
  sendDisplay(WAARDE[2], x2);}

// JoyStickRighty---------------------------------------------------------
JoyStickRy = analogRead(A2)-511;                      // Joystick uitlezen
if(JoyStickRy < Deadzone and JoyStickRy > -Deadzone) {
  if(y2 != 0){
    y2  =0;
    sendMessage(JSRY, y2);
    sendDisplay(WAARDE[3], y2);}}
else if(JoyStickRy > (511 - Deadzone)) {
  if(y2!=511){
    y2=511;
    sendMessage(JSRY, y2);
    sendDisplay(WAARDE[3], y2);}}
else if(JoyStickRy < (Deadzone - 511)) {
  if(y2 != -511) {
    y2 = -511;
    sendMessage(JSRY, y2);
    sendDisplay(WAARDE[3], y2);}}
else if(abs(JoyStickRy - y2) > 2) {
  y2 = JoyStickRy;
  sendMessage(JSRY, JoyStickRy);
  sendDisplay(WAARDE[3], y2);}  
delay(100);
};
