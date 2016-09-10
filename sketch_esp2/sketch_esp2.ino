#include <ESP8266WiFi.h>

WiFiServer server(80);
//////////////////////
// WiFi Definitions //
//////////////////////
const char WiFiAPPSK[] = "PaulTest";

/////////////////////
// Pin Definitions //
/////////////////////
const int LED_PIN = 13;

void setupWiFi()
{
  WiFi.mode(WIFI_AP);

  uint8_t mac[WL_MAC_ADDR_LENGTH];
  WiFi.softAPmacAddress(mac);
  String macID = String(mac[WL_MAC_ADDR_LENGTH - 2], HEX) +
                 String(mac[WL_MAC_ADDR_LENGTH - 1], HEX);
  macID.toUpperCase();
  String AP_NameString = "ESP8266_AP" + macID;

  char AP_NameChar[AP_NameString.length() + 1];
  memset(AP_NameChar, 0, AP_NameString.length() + 1);

  for (int i=0; i<AP_NameString.length(); i++)
    AP_NameChar[i] = AP_NameString.charAt(i);

  WiFi.softAP(AP_NameChar, WiFiAPPSK);
}

void initHardware()
{
  Serial.begin(115200);
  pinMode(LED_PIN, OUTPUT);
  digitalWrite(LED_PIN, LOW);
}
 
void setup() {
  initHardware();
  setupWiFi();
  server.begin();

}

void loop() 
{
  // Check if a client has connected
  WiFiClient client = server.available();
  if (!client) {
    return;
  }

 // Read the first line of the request
  String request = client.readStringUntil('\r');
  Serial.println(request);
  client.flush();
 
  // Match the request
 
  int value = LOW;
  if (request.indexOf("/LED=ON") != -1)  {
    digitalWrite(LED_PIN, 1);
    value = HIGH;
  }
  if (request.indexOf("/LED=OFF") != -1)  {
    digitalWrite(LED_PIN, 0);
    value = LOW;
  }
  
  // Return the response
  client.println("HTTP/1.1 200 OK");
  client.println("Content-Type: text/html");
  client.println(""); //  do not forget this one
  client.println("<!DOCTYPE HTML>");
  client.println("<html>");
  client.println("<br><br>");
  client.println("<br><br>");

  client.println("<font size=\"24\">Test Web Page</font>"); 

  client.println("<br><br>");
  client.println("<a style=\"font-size:20px\" href=\"/LED=ON\"\"><button  style=\"font-size:20px;height:200px;width:200px\">Turn On </button></a>");
  client.println("<a style=\"font-size:20px\" href=\"/LED=OFF\"\"><button style=\"font-size:20px;height:200px;width:200px\">Turn Off </button></a><br />");
  client.println("<br><br>");
  client.println("<br><br>");

  client.println("<font size=\"24\">Led pin is now:</font>"); 
  
  if(value == HIGH) {
    client.print("<font size=\"24\">On</font>");
  } else {
    client.print("<font size=\"24\">Off</font>");
  }

  client.println("</html>");
 
  delay(1);
  Serial.println("Client disonnected");
  Serial.println("");

}


