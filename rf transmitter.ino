#include <SoftwareSerial.h>
#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>

RF24 radio(7, 8); // CE, CSN
SoftwareSerial ArduinoUno(3,2);
const byte address1[6] = "00001";
const byte address2[6] = "00002";
void setup(){
	 radio.begin();
  radio.openWritingPipe(address1);
  radio.setPALevel(RF24_PA_MAX);
  radio.stopListening();
  Serial.begin(9600);
	ArduinoUno.begin(4800);

}

void loop(){
	radio.openWritingPipe(address1);
  radio.stopListening();
  char text[32] = "0";
	while(ArduinoUno.available()>0){
	int val = ArduinoUno.parseInt();
	//if(ArduinoUno.read()== 'c'){
  char text[32];
  itoa(val, text, 10);
  Serial.println(text);
  radio.write(&text, sizeof(text));
  delay(500);
}

delay(500);
	
  
  radio.openReadingPipe(0, address2);
  radio.startListening();
  delay(50);
  if (radio.available()) {
    radio.read(&text, sizeof(text));
    
    ArduinoUno.write(text);
}
  
Serial.println(text);
delay(500);
}
