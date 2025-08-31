//Arduino nano:

void setup() {
  Serial.begin(9600);
pinMode(9, INPUT);
pinMode(10, INPUT);
pinMode(7, OUTPUT);
pinMode(6, OUTPUT);
}

void loop() {
   int c = digitalRead(9);
   int b = digitalRead(10);
 if(c == 1){
  digitalWrite(7, HIGH);
 }
  if(c == 0){
  digitalWrite(7, LOW);
 }
   if(b == 1){
  digitalWrite(6, HIGH);
 }
   if(b == 0){
  digitalWrite(6, LOW);
 }
Serial.println(b);
}

EASY_VR SHIELD(Arduino Uno):

#include <Servo.h>
#include <EasyVR.h>
 
 
EasyVR easyvr(Serial);

enum
{
    GROUP_MAIN = 1,
};
 
enum
{
    G1_ON = 0,
    G1_OFF = 1,
  
};

void setup(void)
{
    Serial.begin(9600);
    pinMode(7, OUTPUT);
 
    while(!easyvr.detect()) delay(1000);
 
    easyvr.setTimeout(5);
 
    easyvr.setLanguage(EasyVR::ENGLISH);
  
}
 
void action(int8_t group, int8_t idx)
{
    switch (group)
    {
    case GROUP_MAIN:
        switch (idx)
        {
        case G1_ON:
           digitalWrite(7, HIGH);
            break;
        case G1_OFF:
            digitalWrite(7, LOW);
            break;
        }
        break;
    }
}
 
 
void loop(void)
{
    int8_t idx;

    easyvr.recognizeCommand(GROUP_MAIN);
 
    while(!easyvr.hasFinished());

    delay(500);
    idx = easyvr.getCommand();
 
    if(idx >= 0)
    {
        action(GROUP_MAIN, idx);
    }
    else
    {
 }
}


//ESP32:

#ifdef ESP32
  #include <WiFi.h>
#else
  #include <ESP8266WiFi.h>
#endif
#include <WiFiClientSecure.h>
#include <UniversalTelegramBot.h>
#include <ArduinoJson.h>
 
// Замените учетными данными вашей сети
const char* ssid = "xxxxxxxxxx";
const char* password = "xxxxxxx";
 
#define BOTtoken "xxxxxx"
 
#define CHAT_ID "xxxxxx"
 
#ifdef ESP8266
  X509List cert(TELEGRAM_CERTIFICATE_ROOT);
#endif
 
WiFiClientSecure client;
UniversalTelegramBot bot(BOTtoken, client);
 
int botRequestDelay = 1000;
unsigned long lastTimeBotRan;
 
const int ledPin = 5;
bool ledState = LOW;
 
void handleNewMessages(int numNewMessages) {
  Serial.println("handleNewMessages");
  Serial.println(String(numNewMessages));
 
  for (int i=0; i<numNewMessages; i++) {
    // Проверяем ID чата, с котрого пришел запрос
    String chat_id = String(bot.messages[i].chat_id);
    if (chat_id != CHAT_ID){
      bot.sendMessage(chat_id, "Unauthorized user", "");
      continue;
    }
    
    String text = bot.messages[i].text;
 
    String from_name = bot.messages[i].from_name;
 
    if (text == "/start") {
      String welcome = "Добропожаловать, " + from_name + ".\n";
      welcome += "Use the following commands to control your outputs.\n\n";
      welcome += "/led_on - включить светидиод \n";
      welcome += "/led_off - выключить светидиод \n";
      welcome += "/state - проверить состояние светидиода \n";
      bot.sendMessage(chat_id, welcome, "");
    }
 
    if (text == "/led_on") {
      bot.sendMessage(chat_id, "Включён светидиод", "");
      ledState = HIGH;
      digitalWrite(ledPin, ledState);
    }
    
    if (text == "/led_off") {
      bot.sendMessage(chat_id, "Выключен светидиод", "");
      ledState = LOW;
      digitalWrite(ledPin, ledState);
    }
    
    if (text == "/state") {
      if (digitalRead(5) == 1){
        bot.sendMessage(chat_id, "Светидиод на данный момент включён", "");
      }
      else{
        bot.sendMessage(chat_id, "Светидиод на данный момент выключён", "");
      }
    }
  }
}
 
void setup() {
  Serial.begin(115200);
 
  #ifdef ESP8266
    configTime(0, 0, "pool.ntp.org"); 
    client.setTrustAnchors(&cert);   
  #endif
 
  pinMode(ledPin, OUTPUT);
  digitalWrite(ledPin, ledState);
  
 
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  #ifdef ESP32
    client.setCACert(TELEGRAM_CERTIFICATE_ROOT); 
  #endif
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi..");
  }
  
  Serial.println(WiFi.localIP());
}
 
void loop() {
  if (millis() > lastTimeBotRan + botRequestDelay)  {
    int numNewMessages = bot.getUpdates(bot.last_message_received + 1);
 
    while(numNewMessages) {
      Serial.println("got response");
      handleNewMessages(numNewMessages);
      numNewMessages = bot.getUpdates(bot.last_message_received + 1);
    }
    lastTimeBotRan = millis();
  }
}


//Arduino Mega:

#include "Wire.h"                            
#include "LiquidCrystal_I2C.h"        
LiquidCrystal_I2C LCD(0x27, 20, 2);  


byte SEC = 0;
byte MIN = 21;
byte HOUR = 17;
byte MIN_A = 15;
byte HOUR_A = 17;
unsigned long timer;


boolean button1WasUp = true;
boolean button2WasUp = true;
boolean button1IsUp;
boolean button2IsUp;

boolean alarm = true;
byte c1, c2, c3, w, i;


byte bell [8] = {
  0b11000,
  0b11110,
  0b01100,
  0b01000,
  0b00000,
  0b00000,
  0b00000,
  0b00000,
};

void setup() {
  pinMode(9, INPUT_PULLUP);  
  pinMode(8, INPUT_PULLUP);
  pinMode(5, OUTPUT);               
  LCD.init();                                     
  LCD.backlight();                          
  LCD.createChar(1, bell);
  timer = millis();
}

void loop() {
  
  button1IsUp = digitalRead(9);
  button2IsUp = digitalRead(8);


  if (button1WasUp && !button1IsUp) {
    delay(10);
    button1IsUp = digitalRead(9);
    if (!button1IsUp) {
      SEC = 0; w = 1;
    }
  }
  button1WasUp = button1IsUp;


  if (button2WasUp && !button2IsUp) {
    delay(10);
    button2IsUp = digitalRead(8);
    if (!button2IsUp) {
      alarm = false;
    }
  }
  button2WasUp = button2IsUp;

  
  if (millis() - timer > 1000) {
    timer = millis();
    SEC++;
    // если прошло 59 секунд, прибавляем 1 минуту
    if (SEC > 59) { SEC = 0; MIN++; }
    // если прошло 59 минут, прибавляем 1 час
    if (MIN > 59) { MIN = 0; HOUR++; }
    if (HOUR > 23) { HOUR = 0; }
    // включение будильника в течении 1 минуты
    if (alarm == true && HOUR == HOUR_A && MIN == MIN_A && i < 60) { tone(5, 50); i++; }
    if (i >= 60) { noTone(5); alarm == false; i = 0; }

    if (HOUR < 10) { c1 = 5; }
    if (HOUR >= 10) { c1 = 4; }
    if (MIN < 10) { c2 = 8; }
    if (MIN >= 10) { c2 = 7; }
    if (SEC < 10) { c3 = 11; }
    if (SEC >= 10) { c3 = 10; }

    
    LCD.setCursor(4, 0);
    LCD.print("0");
    LCD.setCursor(c1, 0);
    LCD.print(HOUR);
    LCD.print(":");
    LCD.setCursor(7, 0);
    LCD.print("0");
    LCD.setCursor(c2, 0);
    LCD.print(MIN);
    LCD.print(":");
    LCD.setCursor(10, 0);
    LCD.print("0");
    LCD.setCursor(c3, 0);
    LCD.print(SEC);
    if (alarm == true) { LCD.print(char(1)); }
    if (alarm == false) { LCD.print(" "); }
    noTone(5);
  }

  while (w == 1) {

    if (HOUR > 23) { HOUR = 0; }
    if (HOUR < 10) { c1 = 5; }
    if (HOUR >= 10) { c1 = 4; }
    if (MIN < 10) { c2 = 8; }
    if (MIN >= 10) { c2 = 7; }

    LCD.setCursor(4, 0);
    LCD.print("0");
    LCD.setCursor(c1, 0);
    LCD.print(HOUR);
    LCD.print(":");
    LCD.setCursor(7, 0);
    LCD.print("0");
    LCD.setCursor(c2, 0);
    LCD.print(MIN);
    LCD.print(":");
    LCD.setCursor(10, 0);
    LCD.print("00");
    if (alarm == true) { LCD.print(char(1)); }
    if (alarm == false) { LCD.print(" "); }

    LCD.setCursor(2, 1);
    LCD.print("HOUR SETTING");

    button1IsUp = digitalRead(9);
    button2IsUp = digitalRead(8);

   
    if (button1WasUp && !button1IsUp) {
      delay(10);
      button1IsUp = digitalRead(9);
      if (!button1IsUp) {
        LCD.clear(); w = 2;
      }
    }
    button1WasUp = button1IsUp;

    
    if (button2WasUp && !button2IsUp) {
      delay(10);
      button2IsUp = digitalRead(8);
      if (!button2IsUp) {
        HOUR++;
      }
    }
    button2WasUp = button2IsUp;

  }

  while (w == 2) {

    if (MIN > 59) { MIN = 0;}
    if (HOUR < 10) { c1 = 5; }
    if (HOUR >= 10) { c1 = 4; }
    if (MIN < 10) { c2 = 8; }
    if (MIN >= 10) { c2 = 7; }

    LCD.setCursor(4, 0);
    LCD.print("0");
    LCD.setCursor(c1, 0);
    LCD.print(HOUR);
    LCD.print(":");
    LCD.setCursor(7, 0);
    LCD.print("0");
    LCD.setCursor(c2, 0);
    LCD.print(MIN);
    LCD.print(":");
    LCD.setCursor(10, 0);
    LCD.print("00");
    if (alarm == true) { LCD.print(char(1)); }
    if (alarm == false) { LCD.print(" "); }

    LCD.setCursor(1, 1);
    LCD.print("MINUTE SETTING");

    button1IsUp = digitalRead(9);
    button2IsUp = digitalRead(8);

    
    if (button1WasUp && !button1IsUp) {
      delay(10);
      button1IsUp = digitalRead(9);
      if (!button1IsUp) {
        LCD.clear(); w = 3;
      }
    }
    button1WasUp = button1IsUp;

    if (button2WasUp && !button2IsUp) {
      delay(10);
      button2IsUp = digitalRead(8);
      if (!button2IsUp) {
        MIN++;
      }
    }
    button2WasUp = button2IsUp;

  }

  while (w == 3) {

    if (MIN_A > 59) { MIN_A = 0; HOUR_A++; }
    if (HOUR_A > 23) { HOUR_A = 0; }
    if (HOUR_A < 10) { c1 = 5; }
    if (HOUR_A >= 10) { c1 = 4; }
    if (MIN_A < 10) { c2 = 8; }
    if (MIN_A >= 10) { c2 = 7; }

    LCD.setCursor(4, 0);
    LCD.print("0");
    LCD.setCursor(c1, 0);
    LCD.print(HOUR_A);
    LCD.print(":");
    LCD.setCursor(7, 0);
    LCD.print("0");
    LCD.setCursor(c2, 0);
    LCD.print(MIN_A);
    LCD.print(":");
    LCD.setCursor(10, 0);
    LCD.print("00");
    if (alarm == true) { LCD.print(char(1)); LCD.setCursor(4, 1); LCD.print("ALARM ON "); }
    if (alarm == false) { LCD.print(" "); LCD.setCursor(4, 1); LCD.print("ALARM OFF"); }
 
    button1IsUp = digitalRead(9);
    button2IsUp = digitalRead(8);

    if (button1WasUp && !button1IsUp) {
      delay(10);
      button1IsUp = digitalRead(9);
      if (!button1IsUp) {
        LCD.clear(); w = 4;
      }
    }
    button1WasUp = button1IsUp;

  
    if (button2WasUp && !button2IsUp) {
      delay(10);
      button2IsUp = digitalRead(8);
      if (!button2IsUp) {
        alarm = !alarm;
      }
    }
    button2WasUp = button2IsUp;

  }

  while (w == 4) {

    if (MIN_A > 59) { MIN_A = 0; HOUR_A++; }
    if (HOUR_A > 23) { HOUR_A = 0; }
    if (HOUR_A < 10) { c1 = 5; }
    if (HOUR_A >= 10) { c1 = 4; }
    if (MIN_A < 10) { c2 = 8; }
    if (MIN_A >= 10) { c2 = 7; }

    LCD.setCursor(4, 0);
    LCD.print("0");
    LCD.setCursor(c1, 0);
    LCD.print(HOUR_A);
    LCD.print(":");
    LCD.setCursor(7, 0);
    LCD.print("0");
    LCD.setCursor(c2, 0);
    LCD.print(MIN_A);
    LCD.print(":");
    LCD.setCursor(10, 0);
    LCD.print("00");
    if (alarm == true) { LCD.print(char(1)); }
    if (alarm == false) { LCD.print(" "); }

    LCD.setCursor(1, 1);
    LCD.print("ALARM SET HOUR");

    button1IsUp = digitalRead(9);
    button2IsUp = digitalRead(8);

   
    if (button1WasUp && !button1IsUp) {
      delay(10);
      button1IsUp = digitalRead(9);
      if (!button1IsUp) {
        LCD.clear(); w = 5;
      }
    }
    button1WasUp = button1IsUp;

    
    if (button2WasUp && !button2IsUp) {
      delay(10);
      button2IsUp = digitalRead(8);
      if (!button2IsUp) {
        HOUR_A++;
      }
    }
    button2WasUp = button2IsUp;
  }

  while (w == 5) {

    if (MIN_A > 59) { MIN_A = 0; HOUR_A++; }
    if (HOUR_A > 23) { HOUR_A = 0; }
    if (HOUR_A < 10) { c1 = 5; }
    if (HOUR_A >= 10) { c1 = 4; }
    if (MIN_A < 10) { c2 = 8; }
    if (MIN_A >= 10) { c2 = 7; }

    LCD.setCursor(4, 0);
    LCD.print("0");
    LCD.setCursor(c1, 0);
    LCD.print(HOUR_A);
    LCD.print(":");
    LCD.setCursor(7, 0);
    LCD.print("0");
    LCD.setCursor(c2, 0);
    LCD.print(MIN_A);
    LCD.print(":");
    LCD.setCursor(10, 0);
    LCD.print("00");
    if (alarm == true) { LCD.print(char(1)); }
    if (alarm == false) { LCD.print(" "); }

    LCD.setCursor(0, 1);
    LCD.print("ALARM SET MINUTE");

    button1IsUp = digitalRead(9);
    button2IsUp = digitalRead(8);

   
    if (button1WasUp && !button1IsUp) {
      delay(10);
      button1IsUp = digitalRead(9);
      if (!button1IsUp) {
        LCD.clear(); w = 0;
      }
    }
    button1WasUp = button1IsUp;

    if (button2WasUp && !button2IsUp) {
      delay(10);
      button2IsUp = digitalRead(8);
      if (!button2IsUp) {
        MIN_A++;
      }
    }
    button2WasUp = button2IsUp;

  }

}


//MUSIC SHIELD(Arduino Uno):

#include <SD.h>
#include <SPI.h>
#include <arduino.h>
#include <MusicPlayer.h>

void setup() {
    Serial.begin(9600);
    player.begin(); //will initialize the hardware and set default mode to be normal.
    player.setPlayMode(PM_NORMAL_PLAY); //set mode to play shuffle
    player.scanAndPlayAll(); //If the current playlist is empty,it will add all the songs in the root directory to the playlist.
}
void loop() {
    player.play();
}


