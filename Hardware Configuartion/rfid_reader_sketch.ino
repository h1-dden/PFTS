#include <Firebase_ESP_Client.h>
#include <WiFi.h>
#include <MFRC522.h>
#include <SPI.h>
#include <NTPClient.h>
#include <WiFiUdp.h>
#include "addons/RTDBHelper.h"
// Define RFID pins
#define RST_PIN 0
#define SS_PIN 5

// Define Firebase project credentials
#define API_KEY "AIzaSyDUJeHLwpI-bHN67u9dfz3k2tbB1t6Ol7o"
#define DATABASE_URL "https://sign-in-d1306-default-rtdb.firebaseio.com/"

//Define Firebase Data object
FirebaseData firebaseData;
FirebaseAuth auth;
FirebaseConfig config;

// Define NTP Client to get time
WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP, "pool.ntp.org");

// Variable to save current epoch time
unsigned long epochTime; 


//Declare variables
bool signupOK = false;
int timestamp;

// Initialize RFID reader
MFRC522 rfid(SS_PIN, RST_PIN);

// Function that gets current epoch time
unsigned long getTime() {
  timeClient.update();
  unsigned long now = timeClient.getEpochTime();
  return now;
}

void setup() {
  pinMode(LED_BUILTIN, OUTPUT);
  Serial.begin(9600);

  // Initialize RFID reader
  SPI.begin();
  rfid.PCD_Init();

  // Connect to WiFi
  WiFi.begin("JioFiber-JqsTf", "home@1234");
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi...");
  }
  Serial.println("Connected to WiFi");


config.api_key = API_KEY;
config.database_url = DATABASE_URL;

Serial.println("Connecting to Firebase....");

if (Firebase.signUp(&config, &auth, "", "")){
    Serial.println("Firebase connection complete");
    signupOK = true;
  }
  else{
    Serial.printf("%s\n", config.signer.signupError.message.c_str());
  }

  Firebase.begin(&config, &auth);
  Firebase.reconnectWiFi(true);
}

void loop() {
  // Scan for RFID tags
  if (rfid.PICC_IsNewCardPresent() && rfid.PICC_ReadCardSerial()) {
    // Get RFID tag ID
    String tagID = "";
    for (byte i = 0; i < rfid.uid.size; i++) {
      tagID.concat(String(rfid.uid.uidByte[i] < 0x10 ? "0" : ""));
      tagID.concat(String(rfid.uid.uidByte[i], HEX));
    }
    Serial.println(tagID);
    digitalWrite(LED_BUILTIN, HIGH);  // turn the LED on (HIGH is the voltage level)
    delay(500);                      // wait for a half second
    digitalWrite(LED_BUILTIN, LOW); 
  
     float latitude=42.5050;
     float longitude=32.5050;
  
    if (Firebase.RTDB.getString(&firebaseData,"/Personnel data/"+tagID)) 
    {
      // Update the latitude and longitude values for the RFID tag in the Firebase Realtime Database
      Firebase.RTDB.setFloat(&firebaseData, "/Personnel data/" + tagID + "/latitude", latitude);
      Firebase.RTDB.setFloat(&firebaseData, "/Personnel data/" + tagID + "/longitude", longitude);
      epochTime = getTime();
      Serial.print("Epoch Time: ");
      Serial.println(epochTime);
      Firebase.RTDB.setInt(&firebaseData, "/Personnel data/" + tagID + "/timestamp", epochTime);
      Serial.println("Latitude and longitude updated in Firebase for Tag ID: " + tagID);
    } 
    else 
    {
      Serial.println("RFID tag " + tagID + " not found in Firebase");
    }
  // Reset RFID reader
  rfid.PICC_HaltA();
  rfid.PCD_StopCrypto1();
  }
}