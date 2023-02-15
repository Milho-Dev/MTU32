#include <WiFi.h>
#include <WiFiClient.h>
#include <WebServer.h>
#include <Adafruit_Sensor.h>
#include <DHT.h>

// Replace with your network credentials
const char* ssid = "A70";
const char* password = "MilhoNerfado";

// Create a DHT sensor object
DHT dht(2, DHT22);

// Define LED pin
const int ledPin = 2;

// Create a WebServer object
WebServer server(80);

// Store webpage data
String webpage = "<html>\
<head>\
  <title>ESP32 Sensor Monitor</title>\
</head>\
<body>\
  <h1>ESP32 Sensor Monitor</h1>\
  <p>Temperature: %TEMPERATURE%</p>\
  <p>Humidity: %HUMIDITY%</p>\
  <form method='get'>\
    <button name='LED' value='ON'>Turn LED ON</button>\
    <button name='LED' value='OFF'>Turn LED OFF</button>\
  </form>\
</body>\
</html>";

// Replace placeholders in webpage with actual sensor data
String replacePlaceholders(float temperature, float humidity) {
  String page = webpage;
  page.replace("%TEMPERATURE%", String(temperature));
  page.replace("%HUMIDITY%", String(humidity));
  return page;
}

// Handle root request
void handleRoot() {
  // Read sensor data
  float temperature = dht.readTemperature();
  float humidity = dht.readHumidity();
  
  // Generate response
  String response = replacePlaceholders(temperature, humidity);
  
  // Send response
  server.send(200, "text/html", response);
}

// Handle LED request
void handleLED() {
  String LEDValue = server.arg("LED");
  
  if (LEDValue == "ON") {
    digitalWrite(ledPin, HIGH);
  } else {
    digitalWrite(ledPin, LOW);
  }
  
  // Redirect back to root
  server.sendHeader("Location", "/");
  server.send(302, "text/plain", "");
}

void setup() {
  // Initialize serial communication
  Serial.begin(115200);

  // Initialize LED pin
  pinMode(ledPin, OUTPUT);
  digitalWrite(ledPin, LOW);
  
  // Initialize DHT sensor
  dht.begin();
  
  // Connect to Wi-Fi network
  Serial.print("Connecting to ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi...");
  }
  Serial.println("Connected to WiFi");

  // Start web server
  server.on("/", handleRoot);
  server.on("/led", handleLED);
  server.begin();
  Serial.println("Web server started");
}

void loop() {
  // Handle web server requests
  server.handleClient();
  
  // Other code to run
}
