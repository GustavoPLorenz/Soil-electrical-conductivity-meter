#include <DHT.h>
#include <SoftwareSerial.h>

float Temp;
float Humi;
int Read;
float Voltage;
float Resistance;
float Conductance;
int Count = 0;

DHT Dht(8, DHT11);
SoftwareSerial Bluetooth(10, 11);

void setup() {
  pinMode(13, OUTPUT);
  pinMode(12, OUTPUT);
  pinMode(9, INPUT_PULLUP);
  pinMode(8, INPUT);
  pinMode(7, OUTPUT);
  pinMode(A0, INPUT);

  Dht.begin();
  Bluetooth.begin(9600);
  
  Bluetooth.println("Connected!");
  Bluetooth.println("===================");
  Bluetooth.println("Soil electrical conductivity meter");
  Bluetooth.println("===================");
}

void loop() {
  if(digitalRead(9) == HIGH){

    digitalWrite(7, HIGH);

    Temp = Dht.readTemperature();
    Humi = Dht.readHumidity();
    Read = analogRead(A0);
    Voltage = Read * (5.0 / 1023.0);

    digitalWrite(7, LOW);
       
    if(!isnan(Temp) && !isnan(Humi) && Read >= 5 && Read <= 1018){

      Count++;
      
      digitalWrite(13, HIGH);
      tone(12, 2000);
      delay(300);
      noTone(12);
      delay(300);
      tone(12, 2000);
      delay(300);
      noTone(12);
      digitalWrite(13, LOW);

      Resistance = 10000.0 * (5.0 / Voltage - 1.0);
      Conductance = 1.0 / Resistance;

      Bluetooth.print("INPUT ");
      Bluetooth.print(Count);
      Bluetooth.println(":");
      
      Bluetooth.println("");

      Bluetooth.print("Temperature ");
      Bluetooth.print(Temp);
      Bluetooth.println("°C");

      Bluetooth.println("");

      Bluetooth.print("Humidity ");
      Bluetooth.print(Humi);
      Bluetooth.println("%");

      Bluetooth.println("");

      Bluetooth.print("ADC Read ");
      Bluetooth.println(Read);

      Bluetooth.println("");

      Bluetooth.print("Voltage ");
      Bluetooth.print(Voltage);
      Bluetooth.println(" V");

      Bluetooth.println("");

      Bluetooth.print("Resistance ");
      Bluetooth.print(Resistance/1000);
      Bluetooth.println(" kOhm");

      Bluetooth.println("");

      Bluetooth.print("Conductance ");
      Bluetooth.print(Conductance*1000);
      Bluetooth.println(" mS");

      Bluetooth.println("");
      Bluetooth.println("===================");
    }

    else{
      Bluetooth.println("ERROR");
      if(isnan(Temp) || isnan(Humi)){Bluetooth.println("DHT11 read failed!");}
      if(Read <= 5){Bluetooth.println("Signal below the lane!");}
      if(Read >= 1018){Bluetooth.println("Signal above the lane!");}
    }
  }
}
