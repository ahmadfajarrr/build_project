  void set_sim800l(){
  // Initialize a SoftwareSerial
  SoftwareSerial* serial = new SoftwareSerial(SIM800_RX_PIN, SIM800_TX_PIN);
  serial->begin(9600);
  delay(1000);
   
  // Initialize SIM800L driver with an internal buffer of 200 bytes and a reception buffer of 512 bytes, debug disabled
  sim800l = new SIM800L((Stream *)serial, SIM800_RST_PIN, 200, 512);

  // Equivalent line with the debug enabled on the Serial
  // sim800l = new SIM800L((Stream *)serial, SIM800_RST_PIN, 200, 512, (Stream *)&Serial);

  // Setup module for GPRS communication
  setupModule();
  }
  void setupModule() {
    // Wait until the module is ready to accept AT commands
  while(!sim800l->isReady()) {
    Serial2.println(F("Problem to initialize AT command, retry in 1 sec"));
    delay(1000);
  }
  
  Serial2.println(F("Setup Complete!"));

  // Wait for the GSM signal
  uint8_t signal = sim800l->getSignal();
  while(signal <= 0) {
    delay(1000);
    signal = sim800l->getSignal();
  }
  Serial2.print(F("Signal OK (strenght: "));
  Serial2.print(signal);
  Serial2.println(F(")"));
  delay(1000);

  // Wait for operator network registration (national or roaming network)
  NetworkRegistration network = sim800l->getRegistrationStatus();
  while(network != REGISTERED_HOME && network != REGISTERED_ROAMING) {
    delay(1000);
    network = sim800l->getRegistrationStatus();
  }
  Serial2.println(F("Network registration OK"));
  delay(1000);

  // Setup APN for GPRS configuration
  bool success = sim800l->setupGPRS(APN);
  while(!success) {
    success = sim800l->setupGPRS(APN);
    delay(5000);
  }
  Serial2.println(F("GPRS config OK"));
  
}
void loop_sim800l(){
    // Establish GPRS connectivity (5 trials)
  bool connected = false;
  for(uint8_t i = 0; i < 5 && !connected; i++) {
    delay(1000);
    connected = sim800l->connectGPRS();
  }

  // Check if connected, if not reset the module and setup the config again
  if(connected) {
    Serial2.print(F("GPRS connected with IP "));
    Serial2.println(sim800l->getIP());
  } else {
    Serial2.println(F("GPRS not connected !"));
    Serial2.println(F("Reset the module."));
    sim800l->reset();
    setupModule();
    return;
  }

  Serial2.println(F("Start HTTP POST..."));
  loop_dfrobot();

  // Do HTTP POST communication with 10s for the timeout (read and write)
  uint16_t rc = sim800l->doPost(URL, CONTENT_TYPE, PAYLOAD, 10000, 10000);
   if(rc == 200) {
    // Success, output the data received on the serial
    Serial2.print(F("HTTP POST successful ("));
    Serial2.print(sim800l->getDataSizeReceived());
    Serial2.println(F(" bytes)"));
    Serial2.print(F("Received : "));
    Serial2.println(sim800l->getDataReceived());
  } else {
    // Failed...
    Serial2.print(F("HTTP POST error "));
    Serial2.println(rc);
  }

  delay(1000);

//  // Close GPRS connectivity (5 trials)
//  bool disconnected = sim800l->disconnectGPRS();
//  for(uint8_t i = 0; i < 5 && !connected; i++) {
//    delay(1000);
//    disconnected = sim800l->disconnectGPRS();
//  }
//  
//  if(disconnected) {
//    Serial2.println(F("GPRS disconnected !"));
//  } else {
//    Serial2.println(F("GPRS still connected !"));
//  }
//
//  // Go into low power mode
//  bool lowPowerMode = sim800l->setPowerMode(MINIMUM);
//  if(lowPowerMode) {
//    Serial2.println(F("Module in low power mode"));
//  } else {
//    Serial2.println(F("Failed to switch module to low power mode"));
//  }

  // End of program... wait...
  while(1);
 
}
