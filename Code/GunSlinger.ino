#include "config.h"
#include <ESP32Ping.h>

/*  ESP32Ping: https://github.com/marian-craciunescu/ESP32Ping
    Download and extract to Documents -> Arduino -> libraries
*/

#define POWER_RELAY 26  // A0  
#define RESET_RELAY 25  // A1 

// Assuming PC has a static ip 192.168.1.100
const IPAddress PC(192, 168, 1, 100);

const int DELAY           = 1000;   // General delay      1 sec
const int RESET_DELAY     = 40000;  // Reset takes        40 sec
const int POWER_DELAY     = 30000;  // Power on takes     30 sec
const int SHUTDOWN_DELAY  = 10000;  // Shutdown takes     10 sec
const int RELAY_DELAY     = 2000;   // Hold the relay ON  1 Sec

// Feed setup
AdafruitIO_Feed *powerFeed  = io.feed("power");
AdafruitIO_Feed *statusFeed = io.feed("status");
AdafruitIO_Feed *ledFeed    = io.feed("led");

/* Current state of the PC
 * 0 - shutdown
 * 1 - power on */
int state = 0;

void ConnectAIO() {

  // connect to io.adafruit.com
  Serial.print("Connecting to Adafruit IO");
  io.connect();

  // wait for a connection
  while (io.status() < AIO_CONNECTED) {
    Serial.print(".");
    delay(DELAY / 2);
  }

  // we are connected
  Serial.println();
  Serial.println(io.statusText());

}//ConnectAIO

void UpdateState() {
  /* Remember to allow incoming Ping via firewall.
    Go to Firewall Settings -> Inbound Rules
    Find the rule titled: "File and Printer Sharing(Echo Request -ICMPv4-In)"
    Make sure the "Profile" column says "Private, Public" and NOT "Domain"
    Right click on the rule and choose "Enable Rule"
  */
  IPAddress ip = WiFi.localIP();
  long rssi = WiFi.RSSI();
  
  Serial.println("Checking State...");
  statusFeed->save("Checking State...");
  
  Serial.print("ESP IP Address: ");
  Serial.println(ip);
  
  Serial.print("Signal Strength: ");
  Serial.println(rssi);
  
  Serial.print("Pinging PC @");
  Serial.println(PC);
  
  if (Ping.ping(PC)) {
    state = 1;
    
    Serial.println("Ping OK!!");
    Serial.println("State: ON");
    Serial.println("------------------------------");
    
    statusFeed->save("State: ON");
    delay(DELAY / 2);
    ledFeed->save(state);
    delay(DELAY);
  }
  else {
    state = 0;

    Serial.println("Ping Fail!!");
    Serial.println("State: OFF");
    Serial.println("------------------------------");
    
    statusFeed->save("State: OFF");
    delay(DELAY / 2);
    ledFeed->save(state);
    delay(DELAY);
  }
}//UpdateState

void Power() {
  Serial.println("Powering ON...");

  Serial.println("Sending -> Power ON...");
  statusFeed->save("Powering ON... ~(" + String(POWER_DELAY / 1000) + "Sec)");
  
  digitalWrite(POWER_RELAY, HIGH);    // Turn relay ON
  delay(RELAY_DELAY);                 // Hold for 1 sec
  digitalWrite(POWER_RELAY, LOW);     // Turn relay OFF

  delay(POWER_DELAY);                 // Wait for the computer to boot
  
  Serial.println("Done!");
  Serial.println("Sending -> Done...");
  statusFeed->save("Done!");
  delay(DELAY);

}//Power

void ShutDown() {
  Serial.println("Shutting Down...");

  Serial.println("Sending -> Shutting Down...");
  statusFeed->save("Shutting Down... ~(" + String(SHUTDOWN_DELAY / 1000) + "Sec)");

  digitalWrite(POWER_RELAY, HIGH);    // turn the Relay ON
  delay(RELAY_DELAY);                 // wait for a second
  digitalWrite(POWER_RELAY, LOW);     // turn the Relay OFF

  delay(SHUTDOWN_DELAY);              // Wait for the computer to shutdown
  
  Serial.println("Done!");
  Serial.println("Sending -> Done...");
  statusFeed->save("Done!");
  delay(DELAY);

}//ShutDown

void Reset() {
  Serial.println("Resetting...");

  Serial.println("Sending -> Resetting...");
  statusFeed->save("Resetting...~(" + String(RESET_DELAY / 1000) + "Sec)");

  digitalWrite(RESET_RELAY, HIGH);    // turn the Relay ON
  delay(RELAY_DELAY);                 // wait for a second
  digitalWrite(RESET_RELAY, LOW);     // turn the Relay OFf

  delay(RESET_DELAY);                 // Wait for the computer to reboot
    
  Serial.println("Done!");
  Serial.println("Sending -> Done...");
  statusFeed->save("Done!");
  delay(DELAY);

}//Reset

void setup() {

  pinMode(POWER_RELAY, OUTPUT);
  pinMode(RESET_RELAY, OUTPUT);

  // start the serial connection
  Serial.begin(115200);

  // wait for serial monitor to open
  while (! Serial);

  ConnectAIO();

  powerFeed->onMessage(OperationHandler);

  UpdateState();
  
}//setup

void loop() {
  io.run();
  //delay(5000);
}

void OperationHandler(AdafruitIO_Data *data) {

  char operation = *data->value();
  
  Serial.print("Received <- ");
  Serial.println(data->value());
  
  switch (operation) {
    case 'P':
      if (state == 0) {
        // PC is OFF 
        Power();            // Turn it on   
        UpdateState();      // Check if it's on & update feed
      }
      else {
        // PC is ON
        ShutDown();         // Turn it off  
        UpdateState();      // Check if it's on & update feed
      }
      break;

    case 'R':
      Reset();              // Reset PC
      UpdateState();        // Check if it's on & update feed
      break;

   case 'C':
     UpdateState();         // A manual request to update state    
   break;

  }//switch

}//OperationHandler
