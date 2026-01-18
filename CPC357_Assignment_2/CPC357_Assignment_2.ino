#include <WiFi.h>
#include <PubSubClient.h>
#include <DHT.h>
#include <Wire.h> 
#include <LiquidCrystal_I2C.h>

// ================= USER CONFIGURATION =================
const char* ssid        = "ROSHOT_2.4GHz@unifi"; 
const char* password    = "Pjumi2039#"; 

// MQTT Broker Settings
const char* mqtt_server = "34.31.6.207"; 
const int mqtt_port     = 1883;
const char* mqtt_topic  = "cpc357/smartfan/data";

// ================= PIN MAPPING =================
#define DHTPIN      5       // D5
#define DHTTYPE     DHT11
#define MQ2PIN      A0      // A0
#define RELAY_PIN   6       // D6 (Fan)
#define I2C_SDA     42      // SDA
#define I2C_SCL     41      // SCL

// ================= TIMER SETTINGS =================
unsigned long lastMsg = 0;
const long interval = 60000; // 60,000 milliseconds = 1 Minute

// Initialize Objects
DHT dht(DHTPIN, DHTTYPE);
LiquidCrystal_I2C lcd(0x27, 16, 2); 
WiFiClient espClient;
PubSubClient client(espClient);

void setup() {
  Serial.begin(115200);
  
  // 1. Setup Pins
  pinMode(RELAY_PIN, OUTPUT);
  pinMode(MQ2PIN, INPUT);
  digitalWrite(RELAY_PIN, LOW); // Fan OFF initially

  // 2. Setup LCD
  Wire.begin(I2C_SDA, I2C_SCL);
  lcd.init();
  lcd.backlight();
  lcd.setCursor(0, 0);
  lcd.print("System Booting..");

  // 3. Setup Sensors
  dht.begin();

  // 4. Connect to WiFi
  setup_wifi();

  // 5. Setup MQTT
  client.setServer(mqtt_server, mqtt_port);
}

void setup_wifi() {
  delay(10);
  lcd.setCursor(0, 1);
  lcd.print("WiFi Connecting");
  Serial.print("Connecting to "); Serial.println(ssid);
  
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\nWiFi Connected!");
  lcd.clear();
  lcd.setCursor(0, 0); lcd.print("WiFi OK!");
}

void reconnect() {
  // Loop until we're reconnected
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    String clientId = "FeatherS3-" + String(random(0xffff), HEX);
    
    if (client.connect(clientId.c_str())) {
      Serial.println("connected");
      lcd.clear();
      lcd.setCursor(0, 0); lcd.print("MQTT Connected");
      delay(1000); // Short delay to show message
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5s");
      delay(5000);
    }
  }
}

void loop() {
  // 1. Keep MQTT Alive (Crucial!)
  if (!client.connected()) {
    reconnect();
  }
  client.loop();

  // 2. Check if 1 Minute has passed
  unsigned long now = millis();
  if (now - lastMsg > interval) {
    lastMsg = now; // Reset timer

    // --- READ SENSORS ---
    float h = dht.readHumidity();
    float t = dht.readTemperature();
    int mq2_raw = analogRead(MQ2PIN);
    float mq2_volts = mq2_raw * (3.3 / 4095.0); 

    if (isnan(h) || isnan(t)) {
      Serial.println("DHT Read Failed");
      t = 0.0; 
    }

    // --- SAFETY LOGIC ---
    String status = "SAFE";
    if (mq2_volts > 1.5 || t > 32.0) {
      status = "DANGER";
      digitalWrite(RELAY_PIN, HIGH); // FAN ON
    } else {
      status = "SAFE";
      digitalWrite(RELAY_PIN, LOW); // FAN OFF
    }

    // --- LCD UPDATE (Temperature & Gas) ---
    lcd.clear();
    
    // Row 0: Temperature
    lcd.setCursor(0, 0);
    lcd.print("Temp: " + String(t, 1) + " C");

    // Row 1: Gas Level
    lcd.setCursor(0, 1);
    lcd.print("Gas : " + String(mq2_volts, 2) + " m");

    // --- PUBLISH TO MQTT ---
    String json = "{";
    json += "\"temp\":" + String(t) + ",";
    json += "\"gas\":" + String(mq2_volts) + ",";
    json += "\"fan\":\"" + String((status == "DANGER" ? "ON" : "OFF")) + "\",";
    json += "\"status\":\"" + status + "\"";
    json += "}";

    char msg[256];
    json.toCharArray(msg, 256);
    
    client.publish(mqtt_topic, msg);
    Serial.print("1 Min Update Sent: "); Serial.println(msg);
  }
}