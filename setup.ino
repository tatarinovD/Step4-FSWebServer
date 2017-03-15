void setup() {
  EEPROM.begin(40);//текущее значени 30 макс колличество переменных 512
  Serial.begin(115200);
  ManualMod = EEPROM.read(0); // режим работы контроллера 0- авто 1- ручной 
  PinDL = EEPROM.read(1); // пин подключения реле лампы дневного света 
  Serial.println(PinDL);
  PinNL = EEPROM.read(2); // пин подключения реле лампы ночного света  
  Serial.println(PinNL);
  PinUH = EEPROM.read(3); // пин подключения реле верхнего подогрева   
  PinDH = EEPROM.read(4); // пин подключения реле нижнего подогрева 
  PinH  = EEPROM.read(5); // пин подключения реле увлажнителья воздуха 
  PinDT = EEPROM.read(6); // пин подключения цифрового датчика темпиратуры DS18B20 
  DMinTemp = EEPROM.read(7);  //дневной минимум темпиратуры
  DMaxTemp = EEPROM.read(8);  //дневной максимум темпиратуры
  NMinTemp = EEPROM.read(9);  //ночной минимум темпиратуры
  NMaxTemp = EEPROM.read(10);  //ночной максимум темпиратуры
  HMinTemp = EEPROM.read(11);  //минимальное значение влажности 
  HMaxTemp = EEPROM.read(12);  //максимальное значение влажности 
  DDHMinTemp = EEPROM.read(13); //дневной минимум температуры нижнего подогрева
  DDHMaxTemp = EEPROM.read(14); //дневной максимум температуры нижнего подогрева
  NDHMinTemp = EEPROM.read(15); //ночной минимум температуры нижнего подогрева
  NDHMaxTemp = EEPROM.read(16); //ночной максимум температуры нижнего подогрева
  UWMinLevel = EEPROM.read(17); //минимальное значение УФ излучения от лампы дневного света 
  BeginDay = EEPROM.read(18); //начало светового дня 
  EndDay = EEPROM.read(19); //окончание светового дня 
  NUWMinLevel = EEPROM.read(20); //минимальное значение УФ для включения лампы ночного света 
  NUWMaxLevel = EEPROM.read(21); //минимальное значение УФ для вкылючения лампы ночного света 
  pinMode(PinDL,OUTPUT);
  pinMode(PinNL,OUTPUT);
  
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
