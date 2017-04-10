void HTTP_init(void) {
  HTTP.on("/configs.json", handle_ConfigJSON); // формирование configs.json страницы для передачи данных в web интерфейс
  HTTP.on("/state.json", handle_StateJSON); 
  HTTP.on("/data.json", handle_DataJSON); 
  HTTP.on("/set", handle_Set_SetJSON); 
  HTTP.on("/setstate", handle_Set_StateJSON); 
  HTTP.on("/setmode", handle_Set_modeJSON);
  HTTP.on("/settime", handle_time);  
  // API для устройства
  HTTP.on("/ssdp", handle_Set_Ssdp);            // Установить имя SSDP устройства по запросу вида /ssdp?ssdp=proba
  HTTP.on("/ssid", handle_Set_Ssid);            // Установить имя и пароль роутера по запросу вида /ssid?ssid=home2&password=12345678
  HTTP.on("/ssidap", handle_Set_Ssidap);        // Установить имя и пароль для точки доступа по запросу вида /ssidap?ssidAP=home1&passwordAP=8765439
  HTTP.on("/TimeZone", handle_time_zone);       // Установка времянной зоны по запросу вида http://192.168.0.101/TimeZone?timezone=3
  HTTP.on("/restart", handle_Restart);          // Перезагрузка модуля по запросу вида /restart?device=ok
  // Запускаем HTTP сервер
  HTTP.begin();
}
// Функции API-Set
// Установка SSDP имени по запросу вида http://192.168.0.101/ssdp?ssdp=proba

void handle_Set_StateJSON() {         
  String button = HTTP.arg("button");
  if (button=="DLState"){
      DLState = !digitalRead(PinDL); 
      digitalWrite(PinDL,DLState); 
      if (DLState) button = "OFF";
      else button = "ON";
      HTTP.send(200, "text/plain", button);
      }
   if (button=="NLState"){
      NLState = !digitalRead(PinNL); 
      digitalWrite(PinNL,NLState); 
      if (NLState) button = "OFF";
      else button = "ON";
      HTTP.send(200, "text/plain", button);
   } 
   if (button=="UHState"){
      UHState = !digitalRead(PinUH); 
      digitalWrite(PinUH,UHState); 
      if (UHState) button = "OFF";
      else button = "ON";
      HTTP.send(200, "text/plain", button);
   } 
      if (button=="DHState"){
      DHState = !digitalRead(PinDH); 
      digitalWrite(PinDH,DHState); 
      if (DHState) button = "OFF";
      else button = "On";
      HTTP.send(200, "text/plain", button);
   } 
} 

void handle_Set_modeJSON() {   
      ManualMod =  HTTP.arg("ManualMod").toInt();
      EEPROM.write(0, ManualMod);
      EEPROM.commit();
      HTTP.send(200, "text/plain", "OK");            // отправляем ответ о выполнении
}

void handle_Set_SetJSON() {           //
    String button = HTTP.arg("button");
    if (button=="commit"){
      readmem();
      HTTP.send(200, "text/plain", "OK");
    }
    else{
      byte data =  HTTP.arg("data").toInt();
      byte eeprom =  HTTP.arg("eeprom").toInt();
      EEPROM.write(eeprom, data);
      EEPROM.commit();
      HTTP.send(200, "text/plain", "OK");            // отправляем ответ о выполнении
    }
}

void handle_time() {
 int  Y = HTTP.arg("Year").toInt();  
 byte M = HTTP.arg("Month").toInt();  
 byte d = HTTP.arg("Day").toInt();  
 byte h = HTTP.arg("Hour").toInt();  
 byte m = HTTP.arg("Minute").toInt();  
 setTime(Y,M,d,h,m);
 HTTP.send(200, "text/plain", "OK"); // отправляем ответ о выполнении
}

void handle_Set_Ssdp() {
  SSDP_Name = HTTP.arg("ssdp"); // Получаем значение ssdp из запроса сохраняем в глобальной переменной
  saveConfig();                 // Функция сохранения данных во Flash пока пустая
  HTTP.send(200, "text/plain", "OK"); // отправляем ответ о выполнении
}

// Установка параметров для подключения к внешней AP по запросу вида http://192.168.0.101/ssid?ssid=home2&password=12345678
void handle_Set_Ssid() {
  _ssid = HTTP.arg("ssid");            // Получаем значение ssid из запроса сохраняем в глобальной переменной
  _password = HTTP.arg("password");    // Получаем значение password из запроса сохраняем в глобальной переменной
  saveConfig();                        // Функция сохранения данных во Flash пока пустая
  HTTP.send(200, "text/plain", "OK");   // отправляем ответ о выполнении
}

//Установка параметров внутренней точки доступа по запросу вида http://192.168.0.101/ssidap?ssidAP=home1&passwordAP=8765439 
void handle_Set_Ssidap() {              //
  _ssidAP = HTTP.arg("ssidAP");         // Получаем значение ssidAP из запроса сохраняем в глобальной переменной
  _passwordAP = HTTP.arg("passwordAP"); // Получаем значение passwordAP из запроса сохраняем в глобальной переменной
  saveConfig();                         // Функция сохранения данных во Flash пока пустая
  HTTP.send(200, "text/plain", "OK");   // отправляем ответ о выполнении
}

// Установка параметров времянной зоны по запросу вида http://192.168.0.101/TimeZone?timezone=3
void handle_time_zone() {               
  timezone = HTTP.arg("timezone").toInt(); // Получаем значение timezone из запроса конвертируем в int сохраняем в глобальной переменной
  saveConfig();
  HTTP.send(200, "text/plain", "OK");
}

// Перезагрузка модуля по запросу вида http://192.168.0.101/restart?device=ok
void handle_Restart() {
  String restart = HTTP.arg("device");          // Получаем значение device из запроса
  if (restart == "ok") {                         // Если значение равно Ок
    HTTP.send(200, "text / plain", "Reset OK"); // Oтправляем ответ Reset OK
    ESP.restart();                                // перезагружаем модуль
  }
  else {                                        // иначе
    HTTP.send(200, "text / plain", "No Reset"); // Oтправляем ответ No Reset
  }
}
void handle_ConfigJSON() {
  String json = "{";  // Формировать строку для отправки в браузер json формат
  //{"SSDP":"SSDP-test","ssid":"home","password":"i12345678","ssidAP":"WiFi","passwordAP":"","ip":"192.168.0.101"}
  // Имя SSDP
  json += "\"SSDP\":\"";
  json += SSDP_Name;
  // Имя сети
  json += "\",\"ssid\":\"";
  json += _ssid;
  // Пароль сети
  json += "\",\"password\":\"";
  json += _password;
  // Имя точки доступа
  json += "\",\"ssidAP\":\"";
  json += _ssidAP;
  // Пароль точки доступа
  json += "\",\"passwordAP\":\"";
  json += _passwordAP;
  // Времянная зона
  json += "\",\"timezone\":\"";
  json += timezone;
  // IP устройства
  json += "\",\"ip\":\"";
  json += WiFi.localIP().toString();
  json += "\"}";
  HTTP.send(200, "text/json", json);
}

void handle_StateJSON() {
  String json = "{";  // Формировать строку для отправки в браузер json формат
  json += "\"ManualMod\":\"";
  if (ManualMod) json += "Ручное управление";
  else json += "авто. управление";
  json += "\",\"DLState\":\"";
  if (digitalRead(PinDL)) json += "OFF";
  else json += "ON";
  json += "\",\"NLState\":\"";
  if (digitalRead(PinNL)) json += "OFF";
  else json += "ON";
  json += "\",\"UHState\":\"";
  if (digitalRead(PinUH)) json += "OFF";
  else json += "ON";
  json += "\",\"DHState\":\"";
  if (digitalRead(PinDH)) json += "OFF";
  else json += "ON"; 
  json += "\",\"UWLevel\":\"";
  json += lux ;
  json += "/" ;
  json += UWLevel;
  json += "\",\"Humd\":\"";
  json += Humd;
  json += "\",\"Temp\":\"";
  json += Temp;
  json += "\",\"TempDH\":\"";
  json += tempDH;
  json += "\",\"Year\":\"";
  json += now.year();
  json += "\",\"Month\":\"";
  json += now.month();
  json += "\",\"Day\":\"";
  json += now.day();
  json += "\",\"Hour\":\"";
  json += now.hour();
  json += "\",\"Minute\":\"";
  json += now.minute();
  json += "\"}";
  HTTP.send(200, "text/json", json);
}
void handle_DataJSON() {
  String json = "{";  // Формировать строку для отправки в браузер json формат
  json += "\"DMinTemp\":\"";
  json += DMinTemp;
  json += "\",\"DMaxTemp\":\"";
  json += DMaxTemp;
  json += "\",\"NMinTemp\":\"";
  json += NMinTemp;
  json += "\",\"NMaxTemp\":\"";
  json += NMaxTemp;
  json += "\",\"HMinTemp\":\"";
  json += HMinTemp;
  json += "\",\"HMaxTemp\":\"";
  json += HMaxTemp;
  json += "\",\"DDHMinTemp\":\"";
  json += DDHMinTemp;
  json += "\",\"DMaxTemp\":\"";
  json += DMaxTemp;
  json += "\",\"DDHMinTemp\":\"";
  json += DDHMinTemp;
  json += "\",\"DDHMaxTemp\":\"";
  json += DDHMaxTemp;
  json += "\",\"NDHMinTemp\":\"";
  json += NDHMinTemp;
  json += "\",\"NDHMinTemp\":\"";
  json += NDHMinTemp;
  json += "\",\"NDHMaxTemp\":\"";
  json += NDHMaxTemp;
  json += "\",\"UWMinLevel\":\"";
  json += UWMinLevel;
  json += "\",\"NUWMinLevel\":\"";
  json += NUWMinLevel;
  json += "\",\"NUWMaxLevel\":\"";
  json += NUWMaxLevel;
  json += "\",\"LUXMinLevel\":\"";
  json += LUXMinLevel;
  json += "\",\"LUXMaxLevel\":\"";
  json += LUXMaxLevel;
  json += "\",\"BeginDay\":\"";
  json += BeginDay;
  json += "\",\"EndDay\":\"";
  json += EndDay;
  json += "\",\"Year\":\"";
  json += now.year();
  json += "\",\"Month\":\"";
  json += now.month();
  json += "\",\"Day\":\"";
  json += now.day();
  json += "\",\"Hour\":\"";
  json += now.hour();
  json += "\",\"Minute\":\"";
  json += now.minute();
  json += "\"}";
  HTTP.send(200, "text/json", json);
}

