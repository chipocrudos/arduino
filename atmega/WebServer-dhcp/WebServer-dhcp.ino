/*
  Web Server

References:
  https://startingelectronics.org/tutorials/arduino/ethernet-shield-web-server-tutorial/SD-card-web-server/
  http://fabianoallex.blogspot.com.br/2015/06/arduino-ethernet-shield-sdcard-arquivos.html
  http://arduinobasics.blogspot.com.au/2015/11/get-arduino-data-over-internet-using.html
*/

#include <SPI.h>
#include <Ethernet.h>
#include <SD.h>

// Enter a MAC address and IP address for your controller below.
// The IP address will be dependent on your local network:
byte mac[] = {
  0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED
};
IPAddress ip(10, 10, 10, 80);
EthernetServer server(80);
File webFile;
String requestClient = "index.htm";

void setup() {
  // Open serial communications and wait for port to open:
  Serial.begin(9600);
  while (!Serial) {
    ; // wait for serial port to connect. Needed for Leonardo only
  }

  // initialize SD card
  Serial.println("Initializing SD card...");
  if (!SD.begin(4)) {
      Serial.println("ERROR - SD card initialization failed!");
      return;    // init failed
  }
  Serial.println("SUCCESS - SD card initialized.");
  // check for index.htm file
  /*
  if (!SD.exists("index.htm")) {
      Serial.println("ERROR - Can't find index.htm file!");
      return;  // can't find index file
  }
  Serial.println("SUCCESS - Found index.htm file.");
  */
  
  Serial.println("connecting...");
  // start the Ethernet connection:
  if (Ethernet.begin(mac) == 0) {
    Serial.println("Failed to configure Ethernet using DHCP");
    // no point in carrying on, so do nothing forevermore:
    // try to congifure using IP address instead of DHCP:
    Ethernet.begin(mac, ip);
  }
  delay(500);

  // start the Ethernet connection and the server:
  server.begin();
  Serial.print("server is at ");
  Serial.println(Ethernet.localIP());
}


void loop() {
  // listen for incoming clients
    EthernetClient client = server.available();
  if (client) {

    String readString = "";
    
    //Serial.println("new client");
    // an http request ends with a blank line
    boolean currentLineIsBlank = true;
    while (client.connected()) {
      if (client.available()) {
        char c = client.read();

        if (readString.length() < 80) { readString += c; }

        if (c == '\n')
        {
          if (readString.indexOf("GET") != -1)
          {
              int indexReq1 = readString.indexOf("GET");
              int indexReq2 = readString.indexOf("HTTP");
              requestClient = readString.substring(indexReq1+4, indexReq2-1);
          }
          else
          {
            requestClient = "error404.htm";
          }

          if(requestClient.indexOf(".htm")>0)
          {
            client.println("HTTP/1.1 200 OK");
            client.println("Content-Type: text/html");
            //client.println("Connection: keep-alive");
          }
          else if(requestClient.indexOf(".js")>0)
          {
            client.println("HTTP/1.1 200 OK");
            client.println("Content-Type: text/javascript");
            //client.println("Connection: keep-alive");
          }
          else if(requestClient.indexOf(".css")>0)
          {
            client.println("HTTP/1.1 200 OK");
            client.println("Content-Type: text/css");
            //client.println("Connection: keep-alive");
          }
          else if(requestClient.indexOf("/") !=-1 && requestClient.length() < 2)
          {
            client.println("HTTP/1.1 200 OK");
            client.println("Content-Type: text/html");
            //client.println("Connection: keep-alive");
            requestClient = "index.htm";
          }/*
          else if(requestClient.indexOf(".pdf")>0)
          {
            client.println("HTTP/1.1 200 OK");
            client.println("Content-Type: application/download");
            //client.println("Connection: keep-alive");
          }*/
          else if(requestClient.indexOf(".png")>0)
          {
            //client.println("HTTP/1.1 200 OK");
            //client.println("Content-Type: file/png");
            //client.println("Connection: close");
          }
          else if(requestClient.indexOf(".jpg")>0)
          {
            //client.println("HTTP/1.1 200 OK");
            //client.println("Content-Type: file/jpg");
            //client.println("Connection: close");
          }
          else if(requestClient.indexOf(".ico")>0)
          {
            //client.println("HTTP/1.1 200 OK");
            //client.println("Content-Type: image/x-icon");
            //client.println("Connection: keep-alive");
          }
          else if(requestClient.indexOf("?")>0)
          {
            Serial.println("devolver json");
            jsonResponse(client);
            break;
          }

          webFile = SD.open(requestClient);
          if (webFile)
          {
            Serial.print("Devolviendo archivo SD CARD: ");
            Serial.println(requestClient);
            while(webFile.available()) {
              client.write(webFile.read()); 
            }
            webFile.close();
          }
          else
          {
            Serial.print("Erro SD CARD: ");
            Serial.println(requestClient);
          }
          break;

        }
        else if (c != '\r')
        {
          // you've gotten a character on the current line
          currentLineIsBlank = false;
        }
      }
    }

    // give the web browser time to receive the data
    delay(1);
    // close the connection:
    client.stop();
    //Serial.println("client disconnected");
  }
}

void jsonResponse(EthernetClient clnt)
{
    int valRead;
    clnt.println("Content-Type: application/json");
    clnt.println("Connection: keep-alive");
    clnt.println();

    clnt.println("{status:");
    clnt.println("analog:{");
    for (int count = 0; count <= 5; count++) { // A1 to A5
        valRead = analogRead(count);
        clnt.print("analog");
        clnt.print(count);
        clnt.print(":");
        clnt.print(valRead);
        clnt.println(",");
    }
    clnt.println("}");
    clnt.println("}");
}

