
#include <EEPROM.h>
#include <ESP8266WiFi.h>        //Содержится в пакете. Видео с уроком http://esp8266-arduinoide.ru/step1-wifi
#include <ESP8266WebServer.h>   //Содержится в пакете. Видео с уроком http://esp8266-arduinoide.ru/step2-webserver
#include <ESP8266SSDP.h>        //Содержится в пакете. Видео с уроком http://esp8266-arduinoide.ru/step3-ssdp
#include <FS.h>                 //Содержится в пакете. Видео с уроком http://esp8266-arduinoide.ru/step4-fswebserver
#include "Adafruit_VEML6070.h"
#include <Wire.h>
#include "RTClib.h"
#include "HTU21D.h"
#include <OneWire.h>
#include "BH1750FVI.h" 

BH1750FVI LightSensor;
RTC_DS1307 RTC; //работат с реалтайм часами
Adafruit_VEML6070 uv = Adafruit_VEML6070(); //обьявляем раобту с датчиком ультрафиолета
HTU21D myHumidity; //обьект работы с датчиком темпиратуры и влажности
#include <ArduinoJson.h>        //Установить из менеджера библиотек.
IPAddress apIP(192, 168, 4, 1);

// Web интерфейс для устройства
ESP8266WebServer HTTP(80);
// Для файловой системы
File fsUploadFile;
// Определяем переменные wifi
long previousMillis = 5000; // частота опроса датчиков
long interval = 5000; // интервал опроса часов
String _ssid     = "d-net.kiev.ua"; // Для хранения SSID
String _password = "098765432154321"; // Для хранения пароля сети
String _ssidAP = "TERRARIUM";   // SSID AP точки доступа
String _passwordAP = ""; // пароль точки доступа
String SSDP_Name = "TERRARIUM"; // Имя SSDP
int timezone = 3;               // часовой пояс GTM
String jsonConfig = "{}";
String jsonSettings = "{}";
String jsonState = "{}";
String jsonData = "{}";
float Humd ;
float Temp ;
float tempDH;
DateTime now;

byte ManualMod;
byte PreManualMod;
byte PinDL = 14;
byte PinNL = 12;
byte PinUH = 13;
byte PinDH = 2;
//byte PinH = 9;
byte PinDT = 0;
byte DMinTemp;
byte DMaxTemp;
byte NMinTemp;
byte NMaxTemp;
byte HMinTemp;
byte HMaxTemp;
byte DDHMinTemp;
byte DDHMaxTemp;
byte NDHMinTemp;
byte NDHMaxTemp;
byte UWMinLevel;
byte UWLevel;
byte BeginDay;
byte EndDay;
byte NUWMinLevel;
byte NUWMaxLevel;

int LUXMinLevel;
int LUXMaxLevel;

byte DLState;
byte NLState;
byte UHState;
byte DHState;
int lux;
//byte HState;
OneWire ds(PinDT); // на пине 0 (нужен резистор 4.7 КОм)

void loop() {
  HTTP.handleClient();
  delay(1);
  unsigned long currentMillis = millis();
  if (currentMillis - previousMillis > interval) {
    previousMillis = currentMillis;
    UWLevel = uv.readUV();
    Humd = myHumidity.readHumidity();
    Temp = myHumidity.readTemperature();
    tempDH = currentDigTemp(); 
    lux = LightSensor.getAmbientLight();
    now = RTC.now(); //считываение параметров часов
    auto_mod();
  }
    
}




