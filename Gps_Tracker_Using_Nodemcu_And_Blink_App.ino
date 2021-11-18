
//Prateek
//wwww.prateeks.in
//https://www.youtube.com/c/JustDoElectronics/videos

#include <SoftwareSerial.h>
#define BLYNK_PRINT Serial
#include <TinyGPS++.h>
#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>
#include <LiquidCrystal_I2C.h>
LiquidCrystal_I2C lcd(0x27, 16, 2);

const int RXPin = 12, TXPin = 14;
const uint32_t GPSBaud = 9600;
SoftwareSerial gps_module(RXPin, TXPin);

TinyGPSPlus gps;
WidgetMap myMap(V0);

BlynkTimer timer;

float gps_speed;
float no_of_satellites;
String satellite_orientation;

char auth[] = "GL7svcQiOVqFwTZS43svvI7ah33i06p4";
char ssid[] = "justdoelectronics";
char pass[] = "123456789";

unsigned int move_index = 1;


void setup()
{
  lcd.begin();
  lcd.backlight();
  lcd.setCursor(0, 0);
  lcd.print("  Welcome To");
  lcd.setCursor(0, 1);
  lcd.print("JustDoElectronic");
  lcd.clear();
  Serial.begin(115200);
  Serial.println();
  gps_module.begin(GPSBaud);
  Blynk.begin(auth, ssid, pass);
  timer.setInterval(5000L, checkGPS);
}

void checkGPS() {
  if (gps.charsProcessed() < 10)
  {
    Serial.println(F("No GPS detected: check wiring."));
    Blynk.virtualWrite(V4, "GPS ERROR");
  }
}

void loop()
{
  while (gps_module.available() > 0)
  {
    if (gps.encode(gps_module.read()))
      displayInfo();
  }

  Blynk.run();
  timer.run();
}

void displayInfo()
{
  if (gps.location.isValid())
  {
    float latitude = (gps.location.lat());
    float longitude = (gps.location.lng());

    Serial.print("LAT:  ");
    Serial.println(latitude, 6);
    Serial.print("LONG: ");
    Serial.println(longitude, 6);

    lcd.setCursor(0, 0);
    lcd.print("LAT:");
    lcd.setCursor(6, 0);
    lcd.print(latitude, 6);
    lcd.setCursor(0, 1);
    lcd.print("LONG:");
    lcd.setCursor(6, 1);
    lcd.print(gps.location.lng());

    //Prateek
    //wwww.prateeks.in
    //https://www.youtube.com/c/JustDoElectronics/videos
    Blynk.virtualWrite(V1, String(latitude, 6));
    Blynk.virtualWrite(V2, String(longitude, 6));
    myMap.location(move_index, latitude, longitude, "GPS_Location");
    gps_speed = gps.speed.kmph();
    Blynk.virtualWrite(V3, gps_speed);
    no_of_satellites = gps.satellites.value();
    Blynk.virtualWrite(V4, no_of_satellites);
    satellite_orientation = TinyGPSPlus::cardinal(gps.course.value());
    Blynk.virtualWrite(V5, satellite_orientation);
  }
  Serial.println();
}
