void setup() {
  EEPROM.begin(40);//текущее значени 30 макс колличество переменных 512
  Serial.begin(115200);
  readmem();
  if (ManualMod){
    pinMode(PinDL,OUTPUT);
    pinMode(PinNL,OUTPUT);
    pinMode(PinUH,OUTPUT);
    pinMode(PinDH,OUTPUT);
    pinMode(PinH,OUTPUT);
  }
  else {
    DLState = EEPROM.read(22);
    NLState = EEPROM.read(23);
    UHState = EEPROM.read(24);
    DHState = EEPROM.read(25);
    HState  = EEPROM.read(27);
  }
  Wire.begin(4, 5); // определяем пины L2C (D2,D1)
  myHumidity.begin();
  RTC.begin();//  Активация часов 
  
  //RTC.adjust(DateTime(2017, 3, 24, 17, 23, 0)); //устанавливаем часы 
  
  Serial.println("");
  
  Serial.println("Start 4-FS");
  FS_init();//Запускаем файловую систему
  Serial.println("Step7-FileConfig");
  loadConfig();
  Serial.println("Start 1-WIFI");
  WIFIinit();//Запускаем WIFI
  Serial.println("Start 3-SSDP");
  SSDP_init();//Настраиваем и запускаем SSDP интерфейс
  Serial.println("Start 2-WebServer");
  HTTP_init();//Настраиваем и запускаем HTTP интерфейс

}

