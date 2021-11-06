#include "config.h"

#define POWER_RELAY 13  //set to proper pin
#define RESET_RELAY 13   //set to proper pin 

// set up the feed
AdafruitIO_Feed *poweron  = io.feed("power");
AdafruitIO_Feed *reset    = io.feed("reset");
AdafruitIO_Feed *shut     = io.feed("shutdown");

void setup() {

  pinMode(POWER_RELAY, OUTPUT);
  pinMode(RESET_RELAY, OUTPUT);

  IPAddress ip = WiFi.localIP();
  
  // start the serial connection
  Serial.begin(115200);

  // wait for serial monitor to open
  while (! Serial);

  // connect to io.adafruit.com
  Serial.print("Connecting to Adafruit IO");
  io.connect();

  poweron->onMessage(PowerMessageHandler);
  reset->onMessage(ResetMessageHandler);
  shut->onMessage(ShutMessageHandler);

  // wait for a connection
  while (io.status() < AIO_CONNECTED) {
    Serial.print(".");
    delay(500);
  }

  // we are connected
  Serial.println();
  Serial.println(io.statusText());

  // get intial values
  //poweron->get();
  //reset->get();
  //shut->get();

  // Reset Interface
  shut->save(0);
  delay(500);
  
  reset->save("Done");
  delay(500);
  
  poweron->save("OFF");
}

void loop() {
  io.run();
}

void ShutMessageHandler(AdafruitIO_Data *data)
{
  Serial.print("Shut <- ");
  //Serial.print(data->value());
  //Serial.print("\n");

  if (data->toBool() == true)
  {
    Serial.println("1");

    digitalWrite(POWER_RELAY, HIGH);   // turn the Relay ON
    delay(5000);                       // wait for a  5 second to shutdown
    digitalWrite(POWER_RELAY, LOW);    // turn the Relay OFf
  
    shut->save(0);                    // reset the UI element on interface
    Serial.println("Shut <- 0");
    delay(1000);
  }
  else {
    Serial.println("0");
  }

}//ShutMessageHandler

void PowerMessageHandler(AdafruitIO_Data *data)
{
  Serial.print("Power <- ");
  //  Serial.print(data->value());
  //  Serial.print("\n");

  if (data->toString() == "ON")
  {
    Serial.println("ON");

    digitalWrite(POWER_RELAY, HIGH);   // turn the Relay ON
    delay(1000);                       // wait for a second
    digitalWrite(POWER_RELAY, LOW);    // turn the Relay OFf
   
    poweron->save("OFF");                 // reset the UI element on interface
    Serial.println("Power <- OFF");
    delay(1000);
  }
  else {
    Serial.println("OFF");
  }

}//PowerMessageHandler

void ResetMessageHandler(AdafruitIO_Data *data)
{
  Serial.print("Reset <- ");
  //  Serial.print(data->value());
  //  Serial.print("\n");

  if (data->toString() == "Reset")
  {
    Serial.println("Reset");

    digitalWrite(RESET_RELAY, HIGH);   // turn the Relay ON
    delay(1000);                       // wait for a second
    digitalWrite(RESET_RELAY, LOW);    // turn the Relay OFf
    
    reset->save("Done");
    Serial.println("Reset <- Done");
    delay(1000);
  }
  else {
    Serial.println("Done");
  }

}//ResetMessageHandler
