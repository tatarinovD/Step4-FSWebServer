
void auto_mod() {  
      DLState = digitalRead(PinDL);
      NLState = digitalRead(PinNL);
      UHState = digitalRead(PinUH);
      DHState = digitalRead(PinDH);
    if (ManualMod){
      if(now.hour() >= BeginDay and  now.hour() < EndDay) { // --------------------------день а свет не включен
        if(now.hour() >= 12 and  now.hour() < 15) {
           if (DLState and UWLevel < 5) {
             Serial.println("UF LAMP TURN ON by UWLevel and TIME");
             digitalWrite(PinDL,0);//включаем дневной свет 
           }             
           else if (!DLState and UWLevel > 5){
             Serial.println("UF LAMP TURN OFF by UWLevel");
             digitalWrite(PinDL,1);//включаем дневной свет 
           }
        }  
        else { 
          if (!DLState){
            Serial.println("UF LAMP TURN OFF by TIME");
            digitalWrite(PinDL,1);
          }
        }
        if (!NLState) {
          digitalWrite(PinNL,1); //включаем ночной свет если он включен
          Serial.println("NIGHT LIGHT TURN OF by TIME");
        }
        if (Temp <= DMinTemp and UHState) {
          digitalWrite(PinUH,0); //включаем подогрев на дневном минимуме
          Serial.println("TURN ON UP HITING BY DAY TEMP MIN ");
          Serial.println(digitalRead(PinUH));
        }
        if (Temp >= DMaxTemp and !UHState) {
          digitalWrite(PinUH,1); //отключаем дневной подогрев по максимуму
          Serial.println("TURN OFF UP HITING BY DAY TEMP MAX");
        }
        if (tempDH < DDHMinTemp and DHState) {
          digitalWrite(PinDH,0); //включаем коврик по минимуму
          Serial.println("TURN ON DOWN HITING BY DAY TEMP MIN");
        }
        if (tempDH >= DDHMaxTemp and !DHState) {
          digitalWrite(PinDH,1); //отключаем коврик по достижению максимума 
          Serial.println("TURN OFF DOWN HITING BY DAY TEMP MAX");
        }
      }
      else {
        if (!DLState) {
          digitalWrite(PinDL,1);
          Serial.println("NIGHT LIGHT TURN ON by TIME");          
        }
        if (NLState and lux < LUXMinLevel) {
          digitalWrite(PinNL,0);//включаем ночной свет по LUX датчику
          Serial.println("NIGHT LIGHT TURN ON by LUXMinLevel");   
        }
        if (!NLState and lux >= LUXMaxLevel) {
          digitalWrite(PinNL,1);//выключаем ночной свет по УФ датчику
          Serial.println("NIGHT LIGHT TURN OFF by NUWMaxLevel");   
        }
        if (Temp <= NMinTemp and UHState) {
          digitalWrite(PinUH,0); //включаем подогрев на дневном минимуме
          Serial.println("TURN ON UP HITING BY NIGHT TEMP MIN");   
        }
        if (Temp >= NMaxTemp and UHState ==0) {
          digitalWrite(PinUH,1); //отключаем дневной подогрев по максимуму
          Serial.println("TURN OFF UP HITING BY NIGHT TEMP MAX");  
        }
        if (tempDH < DDHMinTemp and DHState){
          digitalWrite(PinDH,0); //включаем коврик по минимуму
          Serial.println("TURN ON DOWN HITING BY NIGHT TEMP MIN");  
        }
        if (tempDH >= DDHMaxTemp and DHState ==0){
          digitalWrite(PinDH,1); //отключаем коврик по достижению максимума 
          Serial.println("TURN OFF DOWN HITING BY NIGHT TEMP MAX");
        }
      }
   }
   else {
      Serial.println("manual mode");
      if (DLState != EEPROM.read(22)) { 
        Serial.println("EEPROM WRITE 22");
        EEPROM.write(22, DLState);
        EEPROM.commit();}
      if (NLState != EEPROM.read(23)){
        Serial.println("EEPROM WRITE 23");
        EEPROM.write(23, NLState);
        EEPROM.commit(); 
      }
      if (UHState != EEPROM.read(24)){
        Serial.println("EEPROM WRITE 24");
        EEPROM.write(24, UHState);
        EEPROM.commit(); 
      }
      if (DHState != EEPROM.read(25)){
        Serial.println("EEPROM WRITE 25");
        EEPROM.write(25, DHState);
        EEPROM.commit(); 
      }
   }
}    
void readmem(){
  Serial.println("read mem ");
  ManualMod = EEPROM.read(0); // режим работы контроллера 0- авто 1- ручной 
  DMinTemp = EEPROM.read(7);  //дневной минимум температуры
  DMaxTemp = EEPROM.read(8);  //дневной максимум температуры
  NMinTemp = EEPROM.read(9);  //ночной минимум температуры
  NMaxTemp = EEPROM.read(10);  //ночной максимум темпиратуры
  HMinTemp = EEPROM.read(11);  //минимальное значение влажности 
  HMaxTemp = EEPROM.read(12);  //максимальное значение влажности 
  DDHMinTemp = EEPROM.read(13); //дневной минимум температуры нижнего подогрева
  DDHMaxTemp = EEPROM.read(14); //дневной максимум температуры нижнего подогрева
  NDHMinTemp = EEPROM.read(15); //ночной минимум температуры нижнего подогрева
  NDHMaxTemp = EEPROM.read(16); //ночной максимум температуры нижнего подогрева
  UWMinLevel = EEPROM.read(17); //минимальное значение УФ излучения от лампы дневного света 
  BeginDay  = EEPROM.read(18); //начало светового дня 
  EndDay    = EEPROM.read(19); //окончание светового дня 
  NUWMinLevel = EEPROM.read(20); //минимальное значение УФ 
  NUWMaxLevel = EEPROM.read(21); //минимальное значение УФ 
  LUXMinLevel = EEPROM.read(26); //минимальное значение LUX для включения лампы ночного света 
  LUXMaxLevel = EEPROM.read(27); //минимальное значение LUX для вкылючения лампы ночного света 
}

float currentDigTemp(){
  byte i;
  byte present = 0;
  byte type_s = 0;
  byte data[12];
  byte addr[8];
  ds.search(addr);
    if ( !ds.search(addr)) {
      ds.reset_search();
    }   
   // the first ROM byte indicates which chip
  ds.reset();
  ds.select(addr);
  ds.write(0x44, 1);        // start conversion, with parasite power on at the end
  present = ds.reset();
  ds.select(addr);    
  ds.write(0xBE);         // Read Scratchpad
  for ( i = 0; i < 9; i++) {           // we need 9 bytes
    data[i] = ds.read();
  }
  int16_t raw = (data[1] << 8) | data[0];
  if (type_s) {
    raw = raw << 3; // 9 bit resolution default
    if (data[7] == 0x10) {
      // "count remain" gives full 12 bit resolution
      raw = (raw & 0xFFF0) + 12 - data[6];
    }
  } else {
      byte cfg = (data[4] & 0x60);
      // at lower res, the low bits are undefined, so let's zero them
      if (cfg == 0x00) raw = raw & ~7;  // 9 bit resolution, 93.75 ms
      else if (cfg == 0x20) raw = raw & ~3; // 10 bit res, 187.5 ms
      else if (cfg == 0x40) raw = raw & ~1; // 11 bit res, 375 ms
    }
  return  ((float)raw / 16.0);
}

void setTime(int Y,byte M,byte d,byte h,byte m){
  RTC.adjust(DateTime(Y, M, d, h, m, 0));
}
