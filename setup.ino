void setup() {
  EEPROM.begin(40);//текущее значени 30 макс колличество переменных 512
  Serial.begin(115200);
  readmem();
  pinMode(PinDL,OUTPUT);
  pinMode(PinNL,OUTPUT);
  pinMode(PinUH,OUTPUT);
  pinMode(PinDH,OUTPUT);
  pinMode(PinH,OUTPUT);
  Wire.begin(4, 5); // определяем пины L2C (D2,D1)
  myHumidity.begin();
  RTC.begin();//  Активация часов 
  
  //RTC.adjust(DateTime(2017, 3, 24, 17, 23, 0)); //устанавливаем часы 
  
  Serial.println("");
  //Запускаем файловую систему
  Serial.println("Start 4-FS");
  FS_init();
  Serial.println("Step7-FileConfig");
  loadConfig();
  Serial.println("Start 1-WIFI");
  //Запускаем WIFI
  WIFIinit();
  //Настраиваем и запускаем SSDP интерфейс
  Serial.println("Start 3-SSDP");
  SSDP_init();
  //Настраиваем и запускаем HTTP интерфейс
  Serial.println("Start 2-WebServer");
  HTTP_init();
}


void readmem(){
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
  NUWMinLevel = EEPROM.read(20); //минимальное значение УФ для включения лампы ночного света 
  NUWMaxLevel = EEPROM.read(21); //минимальное значение УФ для вкылючения лампы ночного света 
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
