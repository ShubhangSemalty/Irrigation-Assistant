#include <SoftwareSerial.h>
#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>

RF24 radio(7, 8); // CE, CSN
SoftwareSerial ArduinoUno(3,2);
const byte address[6] = "00001";
void setup(){
	 radio.begin();
  radio.openWritingPipe(address);
  radio.setPALevel(RF24_PA_MAX);
  radio.stopListening();
  Serial.begin(9600);
	ArduinoUno.begin(4800);

}

void loop(){
	
	while(ArduinoUno.available()>0){
	int val = ArduinoUno.parseInt();
	//if(ArduinoUno.read()== 'c'){
  char text[32];
  itoa(val, text, 10);
  Serial.println(text);
  radio.write(&text, sizeof(text));
  delay(1000);//}
}
delay(1500);
}
