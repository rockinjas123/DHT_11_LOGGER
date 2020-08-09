//including libraries

#include<DHT.h>  // for temp dht22 sensor
#include<SPI.h>  // for SD card module
#include<SD.h>    // for SD card
#include<RTClib.h>  // for real time clock

//defining the dht sensor
#define DHTTYPE DHT22 
#define DHTPIN 2  // data for dht22 sen

DHT dht(DHTPIN,DHTTYPE); // initializing the dht sensor

const int chipSelect = 4; // cs for the sd card module


File Temp; // Creating file for the data

RTC_DS1307 rtc; // now declaring the real time clock

void setup() {
  
  dht.begin();  //initialize the DHT sensor
  Serial.begin(9600); //initialize Serial monitor

  // setup for the real time clock
  while(!Serial); 
    if(! rtc.begin()) {
      Serial.println("Couldn't detect RTC");
      while (1);
    }
    else {
      rtc.adjust(DateTime(F(__DATE__), F(__TIME__))); // setting the RTC to the date & time this sketch was compiled
    }
    if(! rtc.isrunning()) {
      Serial.println("RTC is Not running!");
    }
    
  // setup for the SD card
  Serial.print("Initializing SD card.....");

  if(!SD.begin(chipSelect)) {
    Serial.println("initialization failed!");
    return;
  }
  Serial.println("initialization completed.");
    
  
  Temp=SD.open("DATA.txt", FILE_WRITE);  //opening file

 // writing to file
  if (Temp) {
    Serial.println("File opened ");
    // print the headings for our data
    Temp.println("Date,Time,Temperature ºC,Humidity %");
  }
  Temp.close();
}

void loggingTime() {
  DateTime now = rtc.now();
  Temp = SD.open("DATA.txt", FILE_WRITE);
  if (Temp) {
    Temp.print(now.year(), DEC);
    Temp.print('/');
    Temp.print(now.month(), DEC);
    Temp.print('/');
    Temp.print(now.day(), DEC);
    Temp.print(',');
    Temp.print(now.hour(), DEC);
    Temp.print(':');
    Temp.print(now.minute(), DEC);
    Temp.print(':');
    Temp.print(now.second(), DEC);
    Temp.print(",");
  }
  Serial.print(now.year(), DEC);
  Serial.print('/');
  Serial.print(now.month(), DEC);
  Serial.print('/');
  Serial.println(now.day(), DEC);
  Serial.print(now.hour(), DEC);
  Serial.print(':');
  Serial.print(now.minute(), DEC);
  Serial.print(':');
  Serial.println(now.second(), DEC);
  Temp.close();
  delay(1000);  
}

void loggingTemperature() {
  // Read temperature as Celsius and humidity in %
  float t = dht.readTemperature();
  float h = dht.readHumidity();
  // Check if any reading failed (to try again).
  
  if  (isnan(t)|| isnan(h)) {
    Serial.println("Failed to read from DHT sensor!");
    return;
  }

  Serial.print("Temperature: "); 
  Serial.print(t);
  Serial.println(" *C"); 
  Serial.print("Humidity: "); 
  Serial.print(h);
  Serial.println(" %"); 
  
  Temp = SD.open("DATA.txt", FILE_WRITE);
  if (Temp) {
    Serial.println("open with success");
    Temp.print(t);
    Temp.println(",");
  }
  Temp.close();
}

void loop() {
  loggingTime();
  loggingTemperature();
  delay(5000);
}
