void HTTP_init(void) {

  HTTP.on("/configs.json", handle_ConfigJSON); // формирование configs.json страницы для передачи данных в web интерфейс
  // API для устройства
  HTTP.on("/ssdp", handle_Set_Ssdp);            // Установить имя SSDP устройства по запросу вида /ssdp?ssdp=proba
  HTTP.on("/ssid", handle_Set_Ssid);            // Установить имя и пароль роутера по запросу вида /ssid?ssid=home2&password=12345678
  HTTP.on("/ssidap", handle_Set_Ssidap);        // Установить имя и пароль для точки доступа по запросу вида /ssidap?ssidAP=home1&passwordAP=8765439
  HTTP.on("/TimeZone", handle_time_zone);       // Установка времянной зоны по запросу вида http://192.168.0.101/TimeZone?timezone=3
  HTTP.on("/restart", handle_Restart);          // Перезагрузка модуля по запросу вида /restart?device=ok
  HTTP.on("/swdlight", handle_swdlight);        // Перезагрузка модуля по запросу вида /swdlight?swdlight=ok
  HTTP.on("/swnlight", handle_swnlight);        // Перезагрузка модуля по запросу вида /swnlight?swnlight=ok
  HTTP.on("/sdlightpin", handle_set_DlightPin); // Установить пин дневного света /sdlightpin?DlightPin=14
  HTTP.on("/snlightpin", handle_set_NlightPin); // Установить пин ночного света  /snlightpin?NlightPin=12
  
  // Запускаем HTTP сервер
  HTTP.begin();
}
// Функции API-Set
// Установка SSDP имени по запросу вида http://192.168.0.101/ssdp?ssdp=proba
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

void handle_swdlight() {
  String swdlight = HTTP.arg("swdlight");          // Получаем значение device из запроса
  if (swdlight == "on") {    
      byte i = digitalRead(PinDlight); 
      digitalWrite(PinDlight,!i); 
      if (i) swdlight = "выключено";
      else swdlight = "включено";
      }
    HTTP.send(200, "text/plain", swdlight);
   }
  void handle_swnlight() {
  String swnlight = HTTP.arg("swnlight");          // Получаем значение device из запроса
  if (swnlight == "on") {    
      byte i = digitalRead(PinDlight); 
      digitalWrite(PinDlight,!i); 
      if (i) swnlight = "выключено";
      else swnlight = "включено";
      }
    HTTP.send(200, "text/plain", swnlight);
   }

void handle_set_DlightPin() {           //
  PinDlight = HTTP.arg("DlightPin").toInt();  // Получаем значение ssidAP из запроса сохраняем в глобальной переменной
  saveConfig();                         // Функция сохранения данных во Flash пока пустая
  HTTP.send(200, "text/plain", "OK");   // отправляем ответ о выполнении
}
void handle_set_NlightPin() {           //
  PinNlight = HTTP.arg("NlightPin").toInt();  // Получаем значение ssidAP из запроса сохраняем в глобальной переменной
  saveConfig();                         // Функция сохранения данных во Flash пока пустая
  HTTP.send(200, "text/plain", "OK");   // отправляем ответ о выполнении
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
  json += "\"DlightState\":\"";
  if (digitalRead(PinDlight)) json += "включено";
  else json += "выключено";
  json += "\"NlightState\":\"";
  if (digitalRead(PinNlight)) json += "включено";
  else json += "выключено";
  json += "\",\"PinDlight\":\"";
  json += PinDlight;
  json += "\",\"PinNlight\":\"";
  json += PinNlight;
  json += "\"}";
  HTTP.send(200, "text/json", json);
}




