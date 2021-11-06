This project is my take on making a smart remote switch which can be connected to any wifi network to turn devices ON or OFF through the internet.

This project uses the [Adafruit HUZZAH32 – ESP32 Feather Board](https://www.adafruit.com/product/3405) which is based on the [ESPRESSIF ESP32 MCU](https://www.espressif.com/en/products/socs/esp32) to listen to incoming messages and switch its GPIO pins on or off.

The [Adafruit IO](https://io.adafruit.com/) service handles the incoming messages to the device from:
- A Web UI which acts as a dashboard.
- MQTT messages from a phone app.

The GPIO pins can be connected to any external device to remotely turn it ON or OFF and even RESET it.

![Dashboard](./MQTT_Final_ Web_UI)


