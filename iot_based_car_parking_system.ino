#include<ESP8266WiFi.h> 
#include<ESP8266WebServer.h> 
#include<SPI.h> 
#include<Wire.h> 
#include<Adafruit_GFX.h> 
#include<Adafruit_SSD1306.h> 
#include <Servo.h>  
  
//--------------------------------------------------------------------- 
  
ESP8266WebServer server; 
Adafruit_SSD1306 display(128,64, &Wire,-1); 
const char username[]="CLINICS"; 
const char password[]="9874563120"; 
Servo myservo1, myservo2; 
int ir_s1 = 14;//connect an ir sensor to pin D5 of nodemcu 
int ir_s2 = 2;//connect an ir sensor to pin D4 of nodemcu 
int Total = 5; 
int Space; 
int ocp; 
int flag1 = 0; 
int flag2 = 0; 
  
//--------------------------------------------------------------------- 
  
void setup()  
{ 
int i; 
  
//buzzer is connected to D8 pin of nodemcu 
pinMode(15,OUTPUT); 
  
for(i=0;i<2;i++) 
{ 
digitalWrite(15,HIGH); 
delay(50); 
digitalWrite(15,LOW); 
delay(50); 
} 
  
Serial.begin(9600); 
WiFi.softAP(username,password); 
IPAddress MyIP=WiFi.softAPIP(); 
Serial.print(MyIP); 
server.begin(); 
  
pinMode(ir_s1, INPUT); 
pinMode(ir_s2, INPUT); 
myservo1.attach(12);//connect a servo to D6 pin of nodemcu 
myservo1.write(100); 
myservo2.attach(0);//connect a servo to D3 
myservo2.write(100); 
  
//connect the SCK and SDA pins of OLED to D1 and D2 pins of nodemcu respectively 
display.begin(SSD1306_SWITCHCAPVCC, 0x3C); 
  
display.clearDisplay(); 
  
display.setTextSize(3); 
display.setTextColor(WHITE);  
display.setCursor (2,25); 
display.println("WELCOME"); 
display.display(); 
delay(3000); 
display.clearDisplay(); 
  
display.setTextSize(2); 
display.setTextColor(WHITE);  
display.setCursor (46,5); 
display.println("CAR"); 
display.setCursor(24,25); 
display.println("PARKING"); 
display.setCursor (17,45); 
display.println(" SYSTEM "); 
display.display(); 
delay (2000); 
display.clearDisplay(); 
   
Space = Total; 
  
server.on("/page",page); 
} 
  
//--------------------------------------------------------------------- 
  
void loop() 
{  
   
server.handleClient(); 
  
if(digitalRead (ir_s1) == LOW ) 
{ 
if(Space>0) 
{ 
  myservo1.write(0);  
delay(3000); 
myservo1.write(100); 
  Space = Space-1; 
} 
else 
{ 
  display.clearDisplay();  
display.setTextSize(1); 
display.setTextColor(BLACK,WHITE); 
display.setCursor (22,33); 
display.println("Sorry Slots not");   
display.setCursor(33,41); 
display.println("Available");  
display.display(); 
delay (10000); 
} 
  
} 
  
if(digitalRead (ir_s2) == LOW ) 
{ 
  
myservo2.write(0);  
delay(3000); 
myservo2.write(100); 
if(Space<Total) 
{ 
Space = Space+1; 
} 
} 
  
  
display.clearDisplay(); 
display.setTextSize(1); 
display.setTextColor(WHITE); 
display.setCursor (0,0); 
display.println("TOTAL SPACE:"); 
display.println(Total); 
display.setCursor (0,17); 
display.println("AVAILABLE SPACE:"); 
display.println(Space); 
display.display(); 
delay(2000); 
  
ocp=Total-Space; 
  
} 
  
//--------------------------------------------------------------------- 
  
void page() 
{ 
   
String mycode=String("No. of parking slots:")+ int(Total)+"<br>"+String("No. of parking slots 
available:")+ int(Space)+"<br>"+String("No. of parking slots occupied:")+ int(ocp)+"<br>"; 
mycode= "<html><head><title>nodemcu</title></head><body><body bgcolor=\"white\"><font 
size=\"7\" color=\"orange\"><center><u><b><h1>PARKING SLOTS</h1></b></u></font><b><font 
size=\"6\" color=\"violet\">"+mycode+"</center></font><meta http-equiv=\"refresh\" 
content=\"5\"></body></html>"; 
server.send(200,"text/html",mycode); 
  
} 
//---------------------------------------------------------------------
