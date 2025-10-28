// Setup instructions serial: https://forum.arduino.cc/t/arduino-mega-2560-serial-communication-to-esp8266-nodemcu/1057321/11

// ESP8266 SoftwareSerial
// https://circuits4you.com/2016/12/14/software-serial-esp8266/

#include <SoftwareSerial.h>

#include <Pinger.h> // ESP8266-ping library by Alessio Leoncini
#include <ESP8266WiFi.h>
extern "C"
{
  #include <lwip/icmp.h> // needed for icmp packet definitions, used by ESP8266-ping
}
#include <WiFiClient.h> // used to make the HTTP get request


// pins Rx GPIO14 (D5) and Tx GPIO 12 (D6)
SoftwareSerial swSer(14, 12);  


Pinger pinger;

// SSID the ESP8266 connects to for internet access
const char* ssid = "MSO ASUS Zenfone 10"; // WiFi network name
const char* password = "spejdernet"; // WiFi network password

const char* server = "api.restful-api.dev";
const int port = 80;

void wifiSetup(){
  // Documentation: https://arduino-esp8266.readthedocs.io/en/latest/esp8266wifi/generic-class.html

  WiFi.begin(ssid, password); 
  WiFi.mode(WIFI_STA);

  // Connecting to Wifi Network
  Serial.println();
  Serial.print("Connecting to WiFi");

  unsigned long timeout = millis();
  while (WiFi.status() != WL_CONNECTED){
    if (millis() - timeout > 10000) {
      Serial.println(">>> WiFi Connection timed out");
      return;
    }
    delay(500);
    Serial.print(".");
  }

  Serial.println("success!");
  Serial.print("Local IP Address is: ");
  Serial.println(WiFi.localIP());
}

void pingerSetup(){ // code from ESP8266-ping library by Alessio Leoncini
  pinger.OnReceive([](const PingerResponse& response)
  {
    if (response.ReceivedResponse)
    {
      Serial.printf(
        "Reply from %s: bytes=%d time=%lums TTL=%d\n",
        response.DestIPAddress.toString().c_str(),
        response.EchoMessageSize - sizeof(struct icmp_echo_hdr),
        response.ResponseTime,
        response.TimeToLive);
    }
    else
    {
      Serial.printf("Request timed out.\n");
    }

    // Return true to continue the ping sequence.
    // If current event returns false, the ping sequence is interrupted.
    return true;
  });
  
  pinger.OnEnd([](const PingerResponse& response)
  {
    // Evaluate lost packet percentage
    float loss = 100;
    if(response.TotalReceivedResponses > 0)
    {
      loss = (response.TotalSentRequests - response.TotalReceivedResponses) * 100 / response.TotalSentRequests;
    }
    
    // Print packet trip data
    Serial.printf(
      "Ping statistics for %s:\n",
      response.DestIPAddress.toString().c_str());
    Serial.printf(
      "    Packets: Sent = %lu, Received = %lu, Lost = %lu (%.2f%% loss),\n",
      response.TotalSentRequests,
      response.TotalReceivedResponses,
      response.TotalSentRequests - response.TotalReceivedResponses,
      loss);

    // Print time information
    if(response.TotalReceivedResponses > 0)
    {
      Serial.printf("Approximate round trip times in milli-seconds:\n");
      Serial.printf(
        "    Minimum = %lums, Maximum = %lums, Average = %.2fms\n",
        response.MinResponseTime,
        response.MaxResponseTime,
        response.AvgResponseTime);
    }
    
    // Print host data
    Serial.printf("Destination host data:\n");
    Serial.printf(
      "    IP address: %s\n",
      response.DestIPAddress.toString().c_str());
    if(response.DestMacAddress != nullptr)
    {
      Serial.printf(
        "    MAC address: " MACSTR "\n",
        MAC2STR(response.DestMacAddress->addr));
    }
    if(response.DestHostname != "")
    {
      Serial.printf(
        "    DNS name: %s\n",
        response.DestHostname.c_str());
    }

    return true;
  });
}

void pingGoogle(){
   // Ping google.com
  Serial.printf("\n\nPinging google.com\n");
  if(pinger.Ping("google.com") == false)
  {
    Serial.println("Error during ping command.");
  }
}

void exampleAPIrequest(){ // code from https://github.com/esp8266/Arduino/blob/master/libraries/ESP8266WiFi/examples/WiFiClient/WiFiClient.ino and https://www.techatomfusion.com/blogs/beginners-guide-calling-api-esp8266
  // Make HTTP request
  Serial.print("Connecting to ");
  Serial.println(server);
  WiFiClient client;
  if (!client.connect(server, port)) {
    Serial.println("Connection failed");
    return;
  }

  // Send HTTP request
  client.print("GET /objects HTTP/1.1\r\n");
  client.print("Host: ");
  client.print(server);
  client.print("\r\n\r\n");
  delay(10);

  // wait for data to be available
  unsigned long timeout = millis();
  while (client.available() == 0) {
    if (millis() - timeout > 5000) {
      Serial.println(">>> Client Timeout !");
      client.stop();
      //delay(60000);
      return;
    }
  }

  // Read response
  while (client.available()) {
    String line = client.readStringUntil('\r');
    Serial.print(line);
  }

  // Close the connection
  Serial.println("closing connection");
  client.stop();
  Serial.println("Example API call finished");
}

void POST(){ 

  //server = ;
  String path = "/objects";
  String payload = "{\"name\": \"myTestObject\", \"data\": { \"Text\": \"Hello World!\" }}\r\n"; 

  Serial.println("-------");
  Serial.println(payload);
  Serial.println("-------");
  
  
  // Make HTTP POST request
  Serial.print("Connecting to ");
  Serial.println(server);
  WiFiClient client;
  if (!client.connect(server, port)) {
    Serial.println("Connection failed");
    return;
  }

  // Send HTTP POST request
  client.print("POST ");
  client.print(path);
  client.print(" HTTP/1.1\r\n");
  client.print("Host: ");
  client.print(server);
  client.print("\r\n");
  client.print("Content-Type:application/json\r\n");
  client.print("Content-Length:");
  client.print(payload.length());
  //client.print("Accept:text/plain\r\n");
  client.print("\r\n");
  client.print("\r\n");

  client.print(payload);
  client.print("\r\n");


  delay(10);

  // wait for data to be available
  unsigned long timeout = millis();
  while (client.available() == 0) {
    if (millis() - timeout > 5000) {
      Serial.println(">>> Client Timeout !");
      client.stop();
      //delay(60000);
      return;
    }
  }

  // Read response
  while (client.available()) {
    String line = client.readStringUntil('\r');
    Serial.print(line);
  }

  // Close the connection
  Serial.println("closing connection");
  client.stop();
  Serial.println("Example API call finished");
}

void setup() {
  Serial.begin(9600);   //Initialize hardware serial with baudrate of 9600
  swSer.begin(9600);    //Initialize software serial with baudrate of 9600
  delay(1000);

  wifiSetup();
  pingerSetup();
  delay(500);
  Serial.println("\n################################################################################\n");
  //exampleAPIrequest();
  POST();
  Serial.println("\n################################################################################");
  //pingGoogle();

  Serial.println("\n################################################################################\n");

}


void loop() {
  while (swSer.available() > 0) {  //wait for data at software serial
    Serial.write(swSer.read()); //Send data recived from software serial to hardware serial    
  }
  while (Serial.available() > 0) { //wait for data at hardware serial
    swSer.write(Serial.read());     //send data recived from hardware serial to software serial
  }
}