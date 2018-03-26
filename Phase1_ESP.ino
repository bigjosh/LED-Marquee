#include <ESP8266WiFi.h>

const char* ssid = "********";
const char* password = "********";
const char* host = "********";
const int httpPort = 80;

String str = "";

void setup() {
  Serial.begin(115200);
  delay(10);

  // We start by connecting to a WiFi network
  Serial.print("Connecting to wifi network: ");
  Serial.println(ssid);
  
  /* Explicitly set the ESP8266 to be a WiFi-client, otherwise, it by default,
     would try to act as both a client and an access-point and could cause
     network-issues with other WiFi-devices on the WiFi-network. */
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);

  // print a '.' and loop until it connects
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("WiFi connected");  
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());
}

void loop() {
  delay(5000);  // wait a little so we're not spamming the host with requests
  str = "";

  Serial.print("Connecting to host: ");
  Serial.println(host);
  
  // Use WiFiClient class to create TCP connections
  WiFiClient client;
  if (!client.connect(host, httpPort)) {
    // if it cant connect, kick out of loop() and try again
    Serial.println("connection failed");
    return;
  }
  
  // We now create a URI for the request
  String url = "/marquee/data.txt";
  Serial.print("Requesting URL: ");
  Serial.println(url);
  
  // This will send the request to the server
  client.print(String("GET ") + url + " HTTP/1.1\r\n" +
               "Host: " + host + "\r\n" + 
               "Connection: close\r\n\r\n");

  // if nothing is available for a few seconds, then timeout the connection, try again
  unsigned long timeout = millis();
  while (client.available() == 0) {
    if (millis() - timeout > 5000) {
      Serial.println(">>> Client Timeout !");
      client.stop();
      return;
    }
  }
  
  // Read the reply from server, store message data for later
  while(client.available()){
    str = client.readString();
    Serial.println("BEGIN");
    Serial.println(str);
    Serial.println("END");
    
//    String line = client.readStringUntil('Z');
//    if (line.substring(0,4) == "HTTP") {
//      Serial.print("Skipped headers");
//    }
//    else {
//      Serial.print(line);
//      str += line;
//    }
  }
  
  // Read from the UNO. If we get a '~' then send the custom message as a response
  while(Serial.available()) {
    if (Serial.readString() == "~") {
      Serial.write(str);
    }
  }

  
}
