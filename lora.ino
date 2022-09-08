void set_lora() {
//initialize Serial Monitor
Serial2.println("LoRa Sender");
LoRa.setSPIFrequency(2E6);
pinMode(SS, OUTPUT);
digitalWrite(SS, HIGH);
//setup LoRa transceiver module
  SPI.setMISO(MISO);
  SPI.setMOSI(MOSI);
  SPI.setSCLK(SCK);
  SPI.setSSEL(SS);
LoRa.setPins(SS, RST, DI0);

// replace the LoRa.begin(---E-) argument with your location's frequency
// 433E6 for Asia
// 866E6 for Europe
// 915E6 for North America

//LoRa.setFrequency(915E6);
while (!LoRa.begin(915E6)) {
Serial2.println("Lora Failed");
delay(500);
}
// Change sync word (0xF3) to match the receiver
// The sync word assures you don't get LoRa messages from other LoRa transceivers
// ranges from 0-0xFF
// LoRa.setSyncWord(0xF3);
Serial2.println("LoRa Initializing OK!");
}

void loop_lora() {
  Serial2.print("Sending packet: ");
  Serial2.println(counter);
  // send packet
  LoRa.beginPacket();
  LoRa.print("hello");
  LoRa.print(counter);
  LoRa.endPacket();
  counter++;
  delay(1000);
}
