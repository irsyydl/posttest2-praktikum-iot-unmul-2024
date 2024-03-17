#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>

const int buzzer = D1;
const int led = D2;
const int plusButton = D5;
const int minusButton = D6;
const int powerButton = D4;

int ledBrightness = 0;
int buzzerTone = 100;
bool isLedLow = false;
bool isPowerOn = true;

const char* ssid = "HUJANPAGIII";
const char* password = "aaaassss";

ESP8266WebServer server(80);
String webpage;

void handleRoot() {
  String ledStatus = String(ledBrightness);
  String buzzerStatus = String(buzzerTone);
  String componentStatus;
  if(isPowerOn == true){
    componentStatus = "ON";
  } else {
    componentStatus = "OFF";
  }
  webpage = "<html><head><title>Monitoring Values</title></head><body>";
  webpage += "<h1>Monitoring Values</h1>";
  webpage += "<p>LED Brightness: " + ledStatus + "</p>";
  webpage += "<p>Buzzer Tone: " + buzzerStatus + "</p>";
  webpage += "<p>Component Status: " + componentStatus + "</p>";
  webpage += "</body></html>";
  server.send(200, "text/html", webpage);
}

void setup() {
  Serial.begin(115200);
  delay(10);

  pinMode(buzzer, OUTPUT);
  pinMode(led, OUTPUT);
  pinMode(plusButton, INPUT_PULLUP);
  pinMode(minusButton, INPUT_PULLUP);
  pinMode(powerButton, INPUT_PULLUP);

  // Connect WiFi
  Serial.println();
  Serial.print("Configuring access point...");

  // Setup WiFi
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid,password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  
  // Print status Connect 
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());

  server.on("/", handleRoot);
  server.begin();
}

void loop() {
  server.handleClient();

  if (digitalRead(plusButton) == LOW) {
    if (isPowerOn) {
      ledBrightness += 3;
      buzzerTone += 10;
      if (buzzerTone > 2500) {
        buzzerTone = 2500;
      }
      if (ledBrightness > 249) {
        ledBrightness = 249;
      }
      tone(buzzer, buzzerTone);
      delay(200);
      analogWrite(led, ledBrightness);
      delay(200);
    }
  }

  if (digitalRead(minusButton) == LOW) {
    if (isPowerOn) {
      ledBrightness -= 3;
      buzzerTone -= 10;
      if (buzzerTone < 100) {
        buzzerTone = 100;
      }
      if (ledBrightness < 0) {
        ledBrightness = 0;
      }
      delay(200);
      analogWrite(led, ledBrightness);
      delay(200);
    }
  }

  if (digitalRead(powerButton) == LOW) {
    if (isPowerOn) {
      digitalWrite(led, LOW);
      noTone(buzzer);
    } else {
      tone(buzzer, buzzerTone);
      analogWrite(led, ledBrightness);
    }
    isPowerOn = !isPowerOn;
    delay(200);
  }
}
