void SSDP_init(void) {
  // SSDP дескриптор
  HTTP.on("/description.xml", HTTP_GET, []() {
    SSDP.schema(HTTP.client());
  });
  //Если версия  2.0.0 закаментируйте следующую строчку
  SSDP.setDeviceType("upnp:rootdevice");
  SSDP.setSchemaURL("description.xml");
  SSDP.setHTTPPort(80);
  SSDP.setName(SSDP_Name);
  SSDP.setSerialNumber("001788102201");
  SSDP.setURL("/");
  SSDP.setModelName("SSDP-TERRARIUM");
  SSDP.setModelNumber("000000000001");
  SSDP.setModelURL("http://");
  SSDP.setManufacturer("MY SALF");
  SSDP.setManufacturerURL("http://www.arduinoide.ru");
  SSDP.begin();
}
