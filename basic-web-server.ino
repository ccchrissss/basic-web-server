/*  
  Rui Santos & Sara Santos - Random Nerd Tutorials
  https://RandomNerdTutorials.com/esp32-web-server-beginners-guide/
  Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files.
  The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.
*/

#include <secrets.h>
#include <WiFi.h>
#include <WebServer.h>
#include <TFT_eSPI.h>

TFT_eSPI tft = TFT_eSPI();

// Replace with your network credentials
// const char* ssid = "REPLACE_WITH_YOUR_SSID";
// const char* password = "REPLACE_WITH_YOUR_PASSWORD";

// Assign output variables to GPIO pins
// const int output26 = 26;
// const int output27 = 27;
// String output26State = "off";
// String output27State = "off";

String krustyKrabState = "closed";

// Create a web server object
WebServer server(80);

// function to handle changing state to "open" for the Krusty Krab
void handleKrustyKrabOpen() {
  krustyKrabState = "open";
  handleRoot();
};

// function to handle changing state to "closed" for the Krusty Krab
void handleKrustyKrabClosed() {
  krustyKrabState = "closed";
  handleRoot();
};

// Function to handle turning GPIO 26 on
// void handleGPIO26On() {
//   output26State = "on";
//   digitalWrite(output26, HIGH);
//   handleRoot();
// }

// Function to handle turning GPIO 26 off
// void handleGPIO26Off() {
//   output26State = "off";
//   digitalWrite(output26, LOW);
//   handleRoot();
// }

// Function to handle turning GPIO 27 on
// void handleGPIO27On() {
//   output27State = "on";
//   digitalWrite(output27, HIGH);
//   handleRoot();
// }

// Function to handle turning GPIO 27 off
// void handleGPIO27Off() {
//   output27State = "off";
//   digitalWrite(output27, LOW);
//   handleRoot();
// }

// Function to handle the root URL and show the current states
void handleRoot() {
  String html = "<!DOCTYPE html><html><head><meta name=\"viewport\" content=\"width=device-width, initial-scale=1\">";
  html += "<link rel=\"icon\" href=\"data:,\">";
  html += "<style>html { font-family: Helvetica; display: inline-block; margin: 0px auto; text-align: center;}";
  html += ".button { background-color: #4CAF50; border: none; color: white; padding: 16px 40px; text-decoration: none; font-size: 30px; margin: 2px; cursor: pointer; }";
  html += ".button2 { background-color: #555555; }";
  html += ".button3 { background-color: #b80000; border: none; color: white; padding: 16px 40px; text-decoration: none; font-size: 30px; margin: 2px; cursor: pointer; }";
  html += ".image { max-width: 50vw; }</style></head>";
  html += "<body><h1>ESP32 Web Server</h1>";
  html += "<p>hey it's ya boi</p>";
  html += "<p><a href=\"/\"><button class=\"button2\">Return to Homepage</button></a></p>";

  // Display Krusty Krab controls
  html += "<h3>The Krusty Krab is kurrently " + krustyKrabState + "</h3>";
  if (krustyKrabState == "closed") {
    html += "<p>Spongebob me Boi! It's time to feed these scallywags breakfast! When the sea potato hashbrowns are prepared, click the sign to open the Krusty Krab</p><p><a href=\"/krustyKrab/open\"><button class=\"button\">OPEN</button></a></p>";
  } else {
    html += "<p>Spongebob me Boi! Poor Mr Squidward needs to go home and rest before he loses a tentacle! After ye've swabbed the poopdeck, click the sign to close the Krusty Krab</p><p><a href=\"/krustyKrab/closed\"><button class=\"button3\">CLOSED</button></a></p>";
  }

  html += "<img class=\"image\" src=\"https://upload.wikimedia.org/wikipedia/commons/2/25/The_Krusty_Krab.png?utm_source=en.wikipedia.org&utm_campaign=imageinfo&utm_content=thumbnail_unscaled\" />";

  // } else {
  //   html += "<p><a href=\"/krustyKrab/closed\"><button class=\"button button2\">CLOSED</button></a></p>";
  // }

  // Display GPIO 26 controls
  // html += "<p>GPIO 26 - State " + output26State + "</p>";
  // if (output26State == "off") {
  //   html += "<p><a href=\"/26/on\"><button class=\"button\">ON</button></a></p>";
  // } else {
  //   html += "<p><a href=\"/26/off\"><button class=\"button button2\">OFF</button></a></p>";
  // }

  // Display GPIO 27 controls
  // html += "<p>GPIO 27 - State " + output27State + "</p>";
  // if (output27State == "off") {
  //   html += "<p><a href=\"/27/on\"><button class=\"button\">ON</button></a></p>";
  // } else {
  //   html += "<p><a href=\"/27/off\"><button class=\"button button2\">OFF</button></a></p>";
  // }

  html += "</body></html>";
  server.send(200, "text/html", html);
}

void setup() {
  // Initialize Serial Output
  Serial.begin(115200);


  // Initialize Display Output
  pinMode(21, OUTPUT);
  digitalWrite(21, HIGH); // backlight ON

  tft.init();
  tft.setRotation(1);
  tft.fillScreen(TFT_BLACK);

  tft.setTextColor(TFT_WHITE);
  tft.setCursor(20, 120);
  tft.setTextSize(2);


  // Initialize the output variables as outputs
  // pinMode(output26, OUTPUT);
  // pinMode(output27, OUTPUT);
  // Set outputs to LOW
  // digitalWrite(output26, LOW);
  // digitalWrite(output27, LOW);

  // Connect to Wi-Fi network
  Serial.print("Connecting to ");
  Serial.println(WIFI_SSID);
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");

  }
  Serial.println("");
  Serial.println("WiFi connected.");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());

  tft.println("WiFi connected.");
  tft.println("IP address: ");
  tft.println(WiFi.localIP());

  // Set up the web server to handle different routes
  server.on("/", handleRoot);
  server.on("/krustyKrab/open", handleKrustyKrabOpen);
  server.on("/krustyKrab/closed", handleKrustyKrabClosed);
  // server.on("/26/on", handleGPIO26On);
  // server.on("/26/off", handleGPIO26Off);
  // server.on("/27/on", handleGPIO27On);
  // server.on("/27/off", handleGPIO27Off);

  // Start the web server
  server.begin();
  Serial.println("HTTP server started");
}

void loop() {
  // Handle incoming client requests
  server.handleClient();
}