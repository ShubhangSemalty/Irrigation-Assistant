#include <RTClib.h> // Date and time functions using a DS1307 RTC connected via I2C and Wire lib
#include <Wire.h>
#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>
RF24 radio(7, 8); // CE, CSN

const byte address1[6] = "00001";
const byte address2[6] = "00002";

unsigned long oldTime;
byte sensorInterrupt = 0;  // 0 = digital pin 2
byte sensorPin       = 2;
float calibrationFactor = 4.5;

volatile byte pulseCount;  
float flowRate;
unsigned int flowMilliLitres;
unsigned long totalMilliLitres;


int valve = 2;
int v1 = 3;
int v2 = 4;
int value=0;
const int hygrometer = A0;
RTC_DS1307 rtc;
void setup () 
{ pinMode(v1,OUTPUT);
  pinMode(v2,OUTPUT);
  
  Serial.begin(9600);
  digitalWrite(v1,1);
  digitalWrite(v2,1);
  //Serial.begin(9600);
  pinMode(sensorPin, INPUT);
  digitalWrite(sensorPin, HIGH);

  pulseCount        = 0;
  flowRate          = 0.0;
  flowMilliLitres   = 0;
  totalMilliLitres  = 0;
  oldTime           = 0;

  attachInterrupt(sensorInterrupt, pulseCounter, FALLING);
  radio.begin();
  radio.openReadingPipe(0, address1);
  delay(100);
  radio.setPALevel(RF24_PA_MAX);
  radio.startListening();
  
  
  if (! rtc.begin()) 
  {
    Serial.println("Couldn't find RTC");
    while (1);
  }

  if (! rtc.isrunning()) 
  {
    Serial.println("Wait for the pump to start!");
  }
  
    rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));//auto update from computer time
    //rtc.adjust(DateTime(2019, 2, 17, 0, 36, 0));// to set the time manualy 
  
}

void loop () 
{ radio.openReadingPipe(0, address1);
  delay(100);
  radio.setPALevel(RF24_PA_MAX);
  radio.startListening();
    
  //Serial.print("Select The Field to be irrigated 1 or 2\n");
    char time_from_user[32] = "";
    if (radio.available()) {
    radio.read(&time_from_user, sizeof(time_from_user));
  }
    Serial.print("Time received  ");
    Serial.println(time_from_user);
    DateTime now = rtc.now();
    delay(3000);
    String Hour = String(now.hour(),DEC);
    String Minute = String(now.minute(),DEC);
    if(Minute.toInt()<10)
    {
      Minute = "0"+Minute;
      //Serial.print(Minute);
      }
    if(Hour.toInt()<10)
    {
      Hour = "0"+Hour;
      }  
    String DateTimeString= Hour+Minute;
    Serial.print(DateTimeString+"\n");
    if(DateTimeString==time_from_user)
    { if(valve == 1)
      { value = analogRead(hygrometer);   //Read analog value 
        value = constrain(value,400,1023);  //Keep the ranges!
        value = map(value,400,1023,100,0);  //Map value : 400 will be 100 and 1023 will be 0
        while(1)
        { if(value<30)
          { value = analogRead(hygrometer);   //Read analog value 
            value = constrain(value,400,1023);  //Keep the ranges!
            value = map(value,400,1023,100,0);  //Map value : 400 will be 100 and 1023 will be 0
            Serial.println("Irrigating The Field");
            Serial.print("Soil humidity: ");
            Serial.println(value);
            digitalWrite(v1,1);
            digitalWrite(v2,0);
            if((millis() - oldTime) > 1000)    // Only process counters once per second
  { 
    detachInterrupt(sensorInterrupt);
        
    flowRate = ((1000.0 / (millis() - oldTime)) * pulseCount) / calibrationFactor;
    
    oldTime = millis();
    
    flowMilliLitres = (flowRate / 60) * 1000;
    totalMilliLitres += flowMilliLitres;
      
    unsigned int frac;
    Serial.print("Flow rate: ");
    Serial.print(int(flowRate));  // Print the integer part of the variable
    Serial.print("L/min");
    Serial.print("\t");       // Print tab space
    Serial.print("Output Liquid Quantity: ");        
    Serial.print(totalMilliLitres);
    Serial.println("mL"); 
    Serial.print("\t");       // Print tab space
  Serial.print(totalMilliLitres/1000);
  Serial.print("L");
    
    pulseCount = 0;
    
    attachInterrupt(sensorInterrupt, pulseCounter, FALLING);
  }
            delay(1000);
          } 
          else
          {
              Serial.println("Stopped Irrigating The Field");
              Serial.print("Soil humidity: ");
              Serial.println(value);
              delay(50);
              radio.openWritingPipe(address2);
              radio.stopListening();
              const char text[] = "1";
              radio.write(&text, sizeof(text));
              delay(5000);
              digitalWrite(v1,1);
              digitalWrite(v2,1);
              break;
          }
        }
        delay(500);
      }
      else if(valve == 2)
      
      { value = analogRead(hygrometer);   //Read analog value 
        value = constrain(value,400,1023);  //Keep the ranges!
        value = map(value,400,1023,100,0);  //Map value : 400 will be 100 and 1023 will be 0
            
      while(1)
        { if(value<34)
          { value = analogRead(hygrometer);   //Read analog value 
            value = constrain(value,400,1023);  //Keep the ranges!
            value = map(value,400,1023,100,0);  //Map value : 400 will be 100 and 1023 will be 0*/
            Serial.println("Irrigating The Field-2");
            Serial.print("Soil humidity: ");
            Serial.println(value);
            digitalWrite(v2,1);
            digitalWrite(v1,0);
            delay(1000);
          } 
          else
          {
              Serial.println("Stopped Irrigating The Field");
              Serial.print("Soil humidity: ");
              Serial.println(value);
              delay(5000);
              digitalWrite(v1,1);
              digitalWrite(v2,1);
              break;
              delay(5000);
              break;
          }
        } 
      }
    }    
}
void pulseCounter()
{
  pulseCount++;
} 
    
