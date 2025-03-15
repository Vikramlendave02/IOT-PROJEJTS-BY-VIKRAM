#include <ESP8266WiFi.h>  // Include the ESP8266 WiFi library

// Replace with your network credentials
const char* ssid = "IOT";
const char* password = "IOT12345";

// Set web server port number to 80
WiFiServer server(80);

// Variable to store the HTTP request
String header;

// Auxiliary variables to store the current output state
String output5State = "off";
String output4State = "on";

// Assign output variables to GPIO pins
const int output5 = D5;
const int output4 = D4;

// Current time and previous time for handling timeouts
unsigned long currentTime = millis();
unsigned long previousTime = 0;

// Timeout time in milliseconds (2 seconds)
const long timeoutTime = 2000;

void setup() {
  Serial.begin(9600);  // Initialize serial communication

  // Initialize GPIO pins as outputs
  pinMode(output5, OUTPUT);
  pinMode(output4, OUTPUT);

  // Set initial states for GPIOs
  digitalWrite(output5, LOW);
  digitalWrite(output4, HIGH);

  // Connect to Wi-Fi network
  Serial.print("Connecting to ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);

  // Wait until connected to Wi-Fi
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  // Print local IP address and start web server
  Serial.println("");
  Serial.println("WiFi connected.");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
  server.begin();
}

void loop() {
  WiFiClient client = server.available();  // Listen for incoming clients

  if (client) {  // If a new client connects
    Serial.println("New Client.");
    String currentLine = "";  // String to hold incoming data
    currentTime = millis();
    previousTime = currentTime;

    // Read data while client is connected and within timeout period
    while (client.connected() && currentTime - previousTime <= timeoutTime) {
      currentTime = millis();

      if (client.available()) {  // If there's data to read from the client
        char c = client.read();  // Read a byte
        Serial.write(c);  // Print the byte to the serial monitor
        header += c;  // Append the byte to the HTTP header

        if (c == '\n') {  // If a newline is encountered
          if (currentLine.length() == 0) {  // If the current line is empty (end of request)
            // Send HTTP response header
            client.println("HTTP/1.1 200 OK");
            client.println("Content-type:text/html");
            client.println("Connection: close");
            client.println();

            // Control GPIO pins based on the HTTP request
            if (header.indexOf("GET /5/on") >= 0) {
              Serial.println("GPIO 5 on");
              output5State = "on";
              digitalWrite(output5, HIGH);
            } else if (header.indexOf("GET /5/off") >= 0) {
              Serial.println("GPIO 5 off");
              output5State = "off";
              digitalWrite(output5, LOW);
            } else if (header.indexOf("GET /4/on") >= 0) {
              Serial.println("GPIO 4 on");
              output4State = "on";
              digitalWrite(output4, HIGH);
            } else if (header.indexOf("GET /4/off") >= 0) {
              Serial.println("GPIO 4 off");
              output4State = "off";
              digitalWrite(output4, LOW);
            }

            // Send the HTML page with updated GPIO states
            client.println("<!DOCTYPE html>");
            client.println("<html>");
            client.println("<head>");
            client.println("<meta name=\"viewport\" content=\"width=device-width, initial-scale=1\">");
            client.println("<link href=\"https://fonts.googleapis.com/css2?family=Poppins:wght@300;500&display=swap\" rel=\"stylesheet\">");
            client.println("<title>ESP8266 Web Control</title>");
            client.println("<style>");
            client.println("body {");
            client.println("  font-family: 'Poppins', sans-serif;");
            client.println("  text-align: center;");
            client.println("  margin: 0;");
            client.println("  padding: 0;");
            client.println("  background: linear-gradient(45deg, #4facfe, #00f2fe);");
            client.println("  color: white;");
            client.println("}");
            client.println("h1 {");
            client.println("  margin: 15px 0;");
            client.println("}");
            client.println(".button {");
            client.println("  border: none;");
            client.println("  border-radius: 8px;");
            client.println("  color: white;");
            client.println("  padding: 16px 40px;");
            client.println("  font-size: 20px;");
            client.println("  cursor: pointer;");
            client.println("  box-shadow: 0 6px 12px rgba(0, 0, 0, 0.2);");
            client.println("  transition: transform 0.2s, box-shadow 0.2s;");
            client.println("  margin: 5px;");
            client.println("}");
            client.println(".button:hover {");
            client.println("  transform: scale(1.1);");
            client.println("  box-shadow: 0 8px 16px rgba(0, 0, 0, 0.3);");
            client.println("}");
            client.println(".button.on {");
            client.println("  background-color: red;");
            client.println("}");
            client.println(".button.off {");
            client.println("  background-color: green;");
            client.println("}");
            client.println(".container {");
            client.println("  padding: 20px;");
            client.println("  max-width: 600px;");
            client.println("  margin: auto;");
            client.println("}");
            client.println("p {");
            client.println("  margin: 15px 0;");
            client.println("}");
            client.println("footer {");
            client.println("  margin-top: 40px;");
            client.println("  font-size: 14px;");
            client.println("  color: #f0f0f0;");
            client.println("}");
            client.println("footer a {");
            client.println("  color: #ff5722;");
            client.println("  text-decoration: none;");
            client.println("}");
            client.println("</style>");
            client.println("</head>");
            client.println("<body>");
            client.println("<h1>ESP8266 Web Control</h1>");
            client.println("<div class=\"container\">");

            // Display current state and control buttons for GPIO 5
            client.println("<p>GPIO 5 - State: " + output5State + "</p>");
            if (output5State == "off") {
              client.println("<p><a href=\"/5/on\"><button class=\"button off\">Turn ON</button></a></p>");
            } else {
              client.println("<p><a href=\"/5/off\"><button class=\"button on\">Turn OFF</button></a></p>");
            }

            // Display current state and control buttons for GPIO 4
            client.println("<p>GPIO 4 - State: " + output4State + "</p>");
            if (output4State == "off") {
              client.println("<p><a href=\"/4/on\"><button class=\"button off\">Turn ON</button></a></p>");
            } else {
              client.println("<p><a href=\"/4/off\"><button class=\"button on\">Turn OFF</button></a></p>");
            }

            client.println("</div>");
            client.println("<footer>");
            client.println("<p>Designed and Developed by <a href=\"https://www.example.com\" target=\"_blank\">Vikram</a></p>");
            client.println("</footer>");
            client.println("</body>");
            client.println("</html>");

            client.println();  // End the response
            break;  // End the loop when done
          } else {
            currentLine = "";  // Clear current line if newline is not empty
          }
        } else if (c != '\r') {  // Ignore carriage return
          currentLine += c;  // Append character to current line
        }
      }
    }

    // Clear the header for next request
    header = "";

    // Close the client connection
    client.stop();
    Serial.println("Client disconnected.");
    Serial.println("");
  }
}
