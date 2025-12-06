#include <Wire.h>
#include <Adafruit_ADS1X15.h>
#include <ESP8266WiFi.h>
#include <Firebase_ESP_Client.h>
#include <NTPClient.h>
#include <WiFiUdp.h>

// Enable serial debugging
#define DEBUG true

// WiFi credentials
#define WIFI_SSID "5.0"
#define WIFI_PASSWORD "ishan002"

// Firebase credentials
#define API_KEY "AIzaSyCn697xTLvtC7Y2tQEvaTrWPwIB6bP3tjE"
#define DATABASE_URL "https://nexus4-39b9c-default-rtdb.firebaseio.com/"
#define USER_EMAIL "ishanjijigeorge@ec.ajce.in"
#define USER_PASSWORD "MYpassword@1234"

// Firebase objects
FirebaseData fbdo;
FirebaseAuth auth;
FirebaseConfig config;

// ADS1115
Adafruit_ADS1115 ads;

// Time
WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP, "pool.ntp.org", 19800, 60000); // IST offset

// Tracking
float totalEnergy = 0; // Wh
float billAmount = 0;
unsigned long lastUploadTime = 0;

// Constants
const float pricePerKWh = 7.0;
const float voltageRMS = 230.0;
const float currentScale = 100.0;
const float offsetVoltage = 0.5;
const float mVperStep = 0.1875F;

void setup() {
  Serial.begin(115200);
  
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  if (DEBUG) Serial.print("Connecting to WiFi");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    if (DEBUG) Serial.print(".");
  }
  if (DEBUG) Serial.println("\nConnected!");

  if (!ads.begin()) {
    if (DEBUG) Serial.println("ADS1115 init failed!");
    while (1);
  }
  ads.setGain(GAIN_ONE);

  timeClient.begin();

  config.api_key = API_KEY;
  auth.user.email = USER_EMAIL;
  auth.user.password = USER_PASSWORD;
  config.database_url = DATABASE_URL;
  
  Firebase.begin(&config, &auth);
  Firebase.reconnectWiFi(true);

  if (DEBUG) Serial.println("System initialized");
}

String ICACHE_FLASH_ATTR getFormattedDateTime() {
  timeClient.update();
  time_t epochTime = timeClient.getEpochTime();
  struct tm *ti = localtime(&epochTime);
  char buffer[25];
  sprintf(buffer, "%04d-%02d-%02d %02d:%02d:%02d",
          ti->tm_year + 1900, ti->tm_mon + 1, ti->tm_mday,
          ti->tm_hour, ti->tm_min, ti->tm_sec);
  return String(buffer);
}

void loop() {
  int16_t adcValue = ads.readADC_SingleEnded(0);
  float voltage = adcValue * mVperStep / 1000.0;
  float current = abs((voltage - offsetVoltage) * currentScale);
  static float filteredCurrent = 0;
  filteredCurrent = 0.9 * filteredCurrent + 0.1 * current;

  float power = filteredCurrent * voltageRMS;
  float energy = power / 3600.0;
  totalEnergy += energy;
  billAmount = (totalEnergy / 1000.0) * pricePerKWh;

  if (DEBUG) {
    Serial.printf("Voltage: %.4f V\n", voltage);
    Serial.printf("Current: %.2f A\n", filteredCurrent);
    Serial.printf("Power: %.2f W\n", power);
    Serial.printf("Energy: %.4f kWh\n", totalEnergy / 1000.0);
    Serial.printf("Bill: ₹%.2f\n", billAmount);
  }

  if (millis() - lastUploadTime >= 10000) {
    lastUploadTime = millis();
    
    String path = "/SmartMeter/";
    String timestamp = getFormattedDateTime();

    FirebaseJson json;
    json.set("energy", totalEnergy / 1000.0);
    json.set("current", filteredCurrent);
    json.set("power", power);
    json.set("bill", billAmount);
    json.set("timestamp", timestamp);

    if (Firebase.RTDB.updateNode(&fbdo, path.c_str(), &json)) {
      if (DEBUG) Serial.println("Data uploaded to Firebase");
    } else {
      if (DEBUG) Serial.println("Firebase upload failed: " + fbdo.errorReason());
    }
  }

  delay(1000);
}
