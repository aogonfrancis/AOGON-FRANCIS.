#include <WiFi.h>

// Replace with your network credentials
const char* ssid = "eduroam2";
const char* password = "FRANCIS@2024";
// GPIO assignments
#define LED_SITTING  9
#define LED_BED      7
#define LED_SECURITY 3

WiFiServer server(80);

//String getHTML(bool s1, bool s2, bool s3) is a custom function that returns the full HTML code (as a String)for 
//the ESP32's web dashboard, dynamically showing the state of the three lights.

String getHTML(bool s1, bool s2, bool s3) {
  String html = "<!DOCTYPE html><html><head><meta name='viewport' content='width=device-width, initial-scale=1'>";
  html += "<title> SMART LIGHTS - DASHBOARD </title>";
  html += "<style>body{font-family:sans-serif;text-align:center;background:#f7f7f7;}button{width:100px;height:40px;margin:10px;font-size:16px;}</style></head><body>";
  html += "<h2> AOGON FRANCIS - SMART LIGHTS SYSTEM </h2>";

  html += "<p><b>SITTING ROOM</b><br>";
  html += s1 ? "<a href='?sitting=off'><button style='background:red;'>OFF</button></a>" :
               "<a href='?sitting=on'><button style='background:green;'>ON</button></a>";
  html += "</p>";

  html += "<p><b>BED ROOM</b><br>";
  html += s2 ? "<a href='?bed=off'><button style='background:red;'>OFF</button></a>" :
               "<a href='?bed=on'><button style='background:green;'>ON</button></a>";
  html += "</p>";

  html += "<p><b>SECURITY</b><br>";
  html += s3 ? "<a href='?sec=off'><button style='background:red;'>OFF</button></a>" :
               "<a href='?sec=on'><button style='background:green;'>ON</button></a>";
  html += "</p>";

  html += "<footer><hr><p>&copy; 2025 AOGON FRANCIS SMART LIGHTS - FROM THE BIT-CREATORS</p></footer>";
  html += "</body></html>";
  return html;
}
//Serial.begin(115200);Starts the serial communication between the ESP32 and your computer-115200 is the baud rate (speed of communication).
void setup() {
  Serial.begin(115200);

  // Set pin modes
  pinMode(LED_SITTING, OUTPUT);
  pinMode(LED_BED, OUTPUT);
  pinMode(LED_SECURITY, OUTPUT);

  // Turn off all initially
  digitalWrite(LED_SITTING, LOW);
  digitalWrite(LED_BED, LOW);
  digitalWrite(LED_SECURITY, LOW);

  // Connect to WiFi
  WiFi.begin(ssid, password);
  Serial.print("Connecting to WiFi...");
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000); Serial.print(".");
  }
  Serial.println("\nWiFi connected");
  Serial.println("IP Address: " + WiFi.localIP().toString());

  server.begin();
}

void loop() {
  WiFiClient client = server.accept();
  if (client) {
    Serial.println("Client Connected");
    String req = client.readStringUntil('\r');
    Serial.println(req);
    client.clear();

    // Handle controls
    if (req.indexOf("sitting=on") != -1) digitalWrite(LED_SITTING, HIGH);
    if (req.indexOf("sitting=off") != -1) digitalWrite(LED_SITTING, LOW);
    if (req.indexOf("bed=on") != -1) digitalWrite(LED_BED, HIGH);
    if (req.indexOf("bed=off") != -1) digitalWrite(LED_BED, LOW);
    if (req.indexOf("sec=on") != -1) digitalWrite(LED_SECURITY, HIGH);
    if (req.indexOf("sec=off") != -1) digitalWrite(LED_SECURITY, LOW);

    // Send response
    client.println("HTTP/1.1 200 OK");
    client.println("Content-Type: text/html");
    client.println();
    client.println(getHTML(
      digitalRead(LED_SITTING),
      digitalRead(LED_BED),
      digitalRead(LED_SECURITY)
    ));
    delay(1);
    client.stop();
  }
}