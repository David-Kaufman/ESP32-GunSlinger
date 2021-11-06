#include "config.h"
#include <ESP32Ping.h>

/*  ESP32Ping: https://github.com/marian-craciunescu/ESP32Ping
 *  Download and extract to Documents -> Arduino -> libraries  
 */

const IPAddress remote_ip(192, 168, 1, 100);

void ConnectAIO() {

  // connect to io.adafruit.com
  Serial.print("Connecting to Adafruit IO");
  io.connect();

  // wait for a connection
  while (io.status() < AIO_CONNECTED) {
    Serial.print(".");
    delay(500);
  }

  // we are connected
  Serial.println();
  Serial.println(io.statusText());

}

void CheckState(){

  remote_ip.toString();
  
  IPAddress ip = WiFi.localIP();
//  String ipstring = WiFi.localIP().toString();
  long rssi = WiFi.RSSI();
  String StatusString = "";
  
  Serial.print("IP Address: ");
  Serial.println(ip);
  
  Serial.print("Signal Strength: ");
  Serial.println(rssi);

//  StatusString += "Checking State... \n";
//  StatusString += "ESP IP Address: ";
//  StatusString += ipstring;
//  StatusString += "\n";
//  StatusString += "ESP Signal Strength: ";
//  StatusString += rssi;
//  StatusString += "\n";

  Serial.print(StatusString);
  
  /* Remember to allow incoming Ping via firewall.
     Go to Firewall Settings -> Inbound Rules 
     Find the rule titled: "File and Printer Sharing(Echo Request -ICMPv4-In)"
     Make sure the "Profile" column says "Private, Public" and NOT "Domain"
     Right click on the rule and choose "Enable Rule"
  */
  Serial.print("Pinging: ");
  Serial.println(remote_ip);
  
  if(Ping.ping(remote_ip)) {
    Serial.println("Success!!");
  }
  else {
    Serial.println("Error :(");
  }
  
}

void setup() {

   // start the serial connection
  Serial.begin(115200);

  // wait for serial monitor to open
  while (! Serial);

  ConnectAIO();

 
}

void loop() {
   CheckState();
   delay(2000);
}
