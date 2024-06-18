#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>

// WiFi credentials
const char* ssid = "your_SSID"; // Replace with WiFi SSID
const char* password = "your_PASSWORD"; // Replace with WiFi password

// Server details
const char* serverUrl = ""; // server address

void setup() {
  Serial.begin(115200);  
  delay(10); 

  // Connection to WiFi
  Serial.println("Connecting to WiFi..."); 
  WiFi.begin(ssid, password); 

  while (WiFi.status() != WL_CONNECTED) { // Waiting for the connection to be established
    delay(500); 
    Serial.print("."); // Printing dots while connecting
  }
  Serial.println("WiFi connected"); // Successful connection
  Serial.print("IP Address: ");
  Serial.println(WiFi.localIP()); // Print the assigned IP address
}

void loop() {
  if (WiFi.status() == WL_CONNECTED) { // Check if WiFi is connected
    HTTPClient http; // Creating an HTTPClient object to handle the HTTP communication between the ESP8266 module and the server.

    // Sending data to the server
    http.begin(serverUrl); 
    http.addHeader("Content-Type", "application/json"); // Specifing content type as JSON

    // Create a JSON payload
    String payload = "{\"sensor\": \"drone\", \"data\": \"test\"}"; 
    int httpResponseCode = http.POST(payload);
    
    if (httpResponseCode > 0) { // Request success checking.....
      String response = http.getString(); 
      Serial.println(httpResponseCode); 
      Serial.println(response); 
    } else {
      Serial.print("Error on sending POST: "); // ERROR
      Serial.println(httpResponseCode); 
    }

    http.end(); // HTTP end*----*
  } else {
    Serial.println("WiFi not connected"); // WiFi NOT CONNECTED
  }

  if (Serial.available()) { // Read from serial data
    String command = Serial.readStringUntil('\n'); 
    Serial.print("Received command: ");
    Serial.println(command); // Printing the received command

   //Telemetry data:typically includes various parameters and measurements related to the drone's operational status, environmental conditions, and mission-specific data.
    String telemetryData = processCommand(command);
    sendTelemetry(telemetryData); 
  }

  delay(1000); 
}

String processCommand(String command) {
  // Command processing for telemetry
  if (command == "STATUS") {
    return "FINE"; 
  }
  return "UNKNOWN"; 
}

void sendTelemetry(String data) {
  Serial.print("Sending telemetry: ");
  Serial.println(data); // Printing the telemetry data

  // Example: Sending telemetry data over WiFi
  if (WiFi.status() == WL_CONNECTED) { // Check if WiFi is connected
    HTTPClient http; // Create an HTTPClient object

    // Sending telemetry data
    http.begin(serverUrl); // Connect to the server
    http.addHeader("Content-Type", "application/json"); // Specify content type as JSON

    // Create JSON payload
    String payload = "{\"telemetry\": \"" + data + "\"}"; // Create telemetry JSON payload
    int httpResponseCode = http.POST(payload); // Send HTTP POST request with the payload

    if (httpResponseCode > 0) { // Check if the request was successful
      String response = http.getString(); // Get the response from the server
      Serial.println(httpResponseCode); // Print the response code
      Serial.println(response); // Print the server response
    } else {
      Serial.print("Error on sending POST: "); // Print error message
      Serial.println(httpResponseCode); // Print the response code
    }

    http.end(); // End the HTTP connection
  } else {
    Serial.println("WiFi not connected"); // Print message if WiFi is not connected
  }
}
