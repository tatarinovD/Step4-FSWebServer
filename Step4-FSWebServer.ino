/*
   Шаг №7
   Запись и чтение параметров конфигурации в файл

*/
#include <EEPROM.h>  
#include <ESP8266WiFi.h>        //Содержится в пакете. Видео с уроком http://esp8266-arduinoide.ru/step1-wifi
#include <ESP8266WebServer.h>   //Содержится в пакете. Видео с уроком http://esp8266-arduinoide.ru/step2-webserver
#include <ESP8266SSDP.h>        //Содержится в пакете. Видео с уроком http://esp8266-arduinoide.ru/step3-ssdp
#include <FS.h>                 //Содержится в пакете. Видео с уроком http://esp8266-arduinoide.ru/step4-fswebserver
//                    ПЕРЕДАЧА ДАННЫХ НА WEB СТРАНИЦУ. Видео с уроком http://esp8266-arduinoide.ru/step5-datapages/
//                    ПЕРЕДАЧА ДАННЫХ С WEB СТРАНИЦЫ.  Видео с уроком http://esp8266-arduinoide.ru/step6-datasend/
#include <ArduinoJson.h>        //Установить из менеджера библиотек.
IPAddress apIP(192, 168, 4, 1);

// Web интерфейс для устройства
ESP8266WebServer HTTP(80);
// Для файловой системы
File fsUploadFile;
// Определяем переменные wifi
String _ssid     = "d-net.kiev.ua"; // Для хранения SSID
String _password = "098765432154321"; // Для хранения пароля сети
String _ssidAP = "WiFi";   // SSID AP точки доступа
String _passwordAP = ""; // пароль точки доступа
String SSDP_Name = "SendToWeb"; // Имя SSDP
int timezone = 3;               // часовой пояс GTM
String jsonConfig = "{}";
String jsonSettings = "{}";
String jsonState = "{}";
String jsonData = "{}";

byte ManualMod;
byte PreManualMod;
byte PinDL;
byte PinNL;
byte PinUH;
byte PinDH;
byte PinH;
byte PinDT;
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
byte BeginDay;
byte EndDay;
byte NUWMinLevel;
byte NUWMaxLevel;
byte DLState;
byte NLState;
byte UHState;
byte DHState;
byte HState;

void loop() {
  HTTP.handleClient();
  delay(1);
}



