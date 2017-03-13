void HTTP_init(void) {
  HTTP.on("/configs.json", handle_ConfigJSON); // формирование configs.json страницы для передачи данных в web интерфейс
  HTTP.on("/restart", handle_Restart); // Перезагрузка модуля по запросу вида http://192.168.0.101/restart?device=ok
  // Запускаем HTTP сервер
  HTTP.begin();
}
// Перезагрузка модуля по запросу вида http://192.168.0.101/restart?device=ok
void handle_Restart() {
  int j =HTTP.arg("PinRele").toInt();
   switch (j) {
    case 1:
      if (restart == "on"){ 
        byte i = digitalRead(PinRele1); 
        digitalWrite(PinRele1,!i); 
        if (i) restart = "выключено";
        else restart = "включено";
      }
      break;
    case 2:
      if (restart == "on"){ 
        byte i = digitalRead(PinRele2); 
        digitalWrite(PinRele1,!i); 
        if (i) restart = "выключено";
        else restart = "включено";
      } 
      break;
    default: 
    break;
  }
  HTTP.send(200, "text/plain", restart);
 
}

void handle_ConfigJSON() {
  String json = "{";  // Формировать строку для отправки в браузер json формат
  //{"PinState":"1"}
  json += "\"PinStste1\":\"";
  if (digitalRead(PinRele1)) json += "включено";
  else json += "выключено";
  json += "\"PinStste2\":\"";
  if (digitalRead(PinRele2)) json += "включено";
  else json += "выключено";
  json += "\",\"PinRele1\":\"";
  json += PinRele1;
  json += "\",\"PinRele2\":\"";
  json += PinRele2;
  json += "\"}";
  HTTP.send(200, "text/json", json);
}


