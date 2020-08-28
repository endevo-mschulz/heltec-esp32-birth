#include <Arduino.h>
#include <U8x8lib.h>
#include <WiFi.h>
#include <AsyncTCP.h>
#include <ESPAsyncWebServer.h>
#include <DNSServer.h>
 
#define OLED_SCL 15   // GPIO 15
#define OLED_SDA  4   // GPIO  4
#define OLED_RST 16   // GPIO 16

long previousMillis = 0;
long interval = 1000;

const char *ssid = "SSID";
String apIP = "";
//const char *password = "1234";

U8X8_SSD1306_128X64_NONAME_SW_I2C u8x8(/* clock=*/ OLED_SCL, /* data=*/ OLED_SDA, /* reset=*/ OLED_RST);
AsyncWebServer server(80);

const char index_html[] PROGMEM = R"rawliteral(
<!DOCTYPE HTML><html><head>
  <title>Mazzens ESP32 Lora</title>
  <meta http-equiv="content-type" content="text/html; charset=utf-8">
  </head><body>
  <h1>Happy Birthday, Digger!!</h1>
  <h2>Wünschen dir Alle</h2>
  <p>P.S.: keine Lust zu stylen:/</p>
  <p>Lass es dir gut gehen</p>
  </body></html>)rawliteral";

void printDisplay(String str1, String str2, String str3, String str4){
    u8x8.clear();
    const char* thisstr1 = str1.c_str();
    const char* thisstr2 = str2.c_str();
    const char* thisstr3 = str3.c_str();
    const char* thisstr4 = str4.c_str();
    u8x8.setFont(u8x8_font_chroma48medium8_r);
    u8x8.drawString(0,0, thisstr1);
    u8x8.drawString(0,1,thisstr2);
    u8x8.drawString(0,3,thisstr3);
    u8x8.drawString(0,4,thisstr4);
    delay(2000);
}

void printDSPstartup(){
  printDisplay("Irgendwas", "Access Point!", ssid, "IP: " + apIP);
}

void setup() {
  Serial.begin(9600);
  Serial.println();
  Serial.print("Configuring access point...");
  /* You can remove the password parameter if you want the AP to be open. */
  //WiFi.softAP(ssid, password);
  WiFi.softAP(ssid);
  
  IPAddress myIP = WiFi.softAPIP();
  apIP = myIP.toString();
  Serial.print("AP IP address: ");
  u8x8.begin();
  u8x8.setPowerSave(0);
  
  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send_P(200, "text/html", index_html);
    printDisplay("Client connected", "Have fun", "", "");
  });
  server.begin();

  printDSPstartup();
}

void loop() {
  unsigned long currentMillis = millis();
  if(currentMillis - previousMillis > interval) {
    previousMillis = currentMillis;
    Serial.println("Timer elapsed");
    //Do fancy stuff here on the Birthday! :-D
  }
}