/*
 WiFi Web Server LED Blink

 A simple web server that lets you blink an LED via the web.
 This sketch will print the IP address of your WiFi Shield (once connected)
 to the Serial monitor. From there, you can open that address in a web browser
 to turn on and off the LED on pin 2.

 If the IP address of your shield is yourAddress:
 http://yourAddress/H turns the LED on
 http://yourAddress/L turns it off

 This example is written for a network using WPA encryption. For
 WEP or WPA, change the Wifi.begin() call accordingly.

 Circuit:
 * WiFi shield attached
 * LED attached to pin 5

 created for arduino 25 Nov 2012
 by Tom Igoe

ported for sparkfun esp32 
31.01.2017 by Jan Hendrik Berlin
 
 */

#include <WiFi.h>

const char* ssid     = "GVT-0404";
const char* password = "netnet0161884200";

WiFiServer server(80);

void setup()
{
    Serial.println("Setup");
    Serial.begin(115200);
    pinMode(2, OUTPUT);      // set the LED pin mode

    delay(10);

    // We start by connecting to a WiFi network

    Serial.println();
    Serial.println();
    Serial.print("Connecting to ");
    Serial.println(ssid);

    WiFi.begin(ssid, password);

    while (WiFi.status() != WL_CONNECTED) {
        delay(500);
        Serial.print(".");
    }

    Serial.println("");
    Serial.println("WiFi connected.");
    Serial.println("IP address: ");
    Serial.println(WiFi.localIP());
    
    server.begin();

}

int value = 0;
int contagem = 0;

void atualizaCliente(WiFiClient client, int contagem)
{
    Serial.println("New Client.");           // print a message out the serial port
    String currentLine = "";                // make a String to hold incoming data from the client
    while (client.connected()) {            // loop while the client's connected
      if (client.available()) {             // if there's bytes to read from the client,
        char c = client.read();             // read a byte, then
        Serial.write(c);                    // print it out the serial monitor
        if (c == '\n') {                    // if the byte is a newline character

          // if the current line is blank, you got two newline characters in a row.
          // that's the end of the client HTTP request, so send a response:
          if (currentLine.length() == 0) {
            String sHTML;
            sHTML  = "<!doctype html>";
            sHTML += "<html>";
            sHTML += "<head>";
            sHTML += "</head>";
            sHTML += "<body>"; 
            sHTML += "Clique <a href=\"/C\">aqui</a> ver contator.<br>";
            sHTML += "</body>";
            sHTML += "</html>"; 
            client.print(sHTML);
            client.println();
            break;
          } else {
            currentLine = "";
          }
        } else if (c != '\r') {
          currentLine += c;   
        }

        if (currentLine.endsWith("GET /C")) {
            String sHTML;
            sHTML  = "<!doctype html>";
            sHTML += "<html>";
            sHTML += "<head>";
            sHTML += "<meta http-equiv=\"refresh\" content=\"1\">";
            sHTML += "</head>";
            sHTML += "<body>"; 
            sHTML += "Clique <a href=\"/C\">aqui</a> ver contator.<br>";
            sHTML += "Contador está em ";
            sHTML += contagem;
            sHTML += "<br>";
            sHTML += "</body>";
            sHTML += "</html>"; 
            client.print(sHTML);
            client.println();
        }
    }
  }
}

void loop(){
  while(contagem>=0){
    WiFiClient client = server.available();   // listen for incoming clients  
    if(client) {                             // if you get a client,
      atualizaCliente(client, contagem);
      contagem++;
      break;
    }
  }
}
