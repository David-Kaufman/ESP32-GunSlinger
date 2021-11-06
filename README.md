# ESP32 GunSlinger
My take on making a smart remote switch which can be connected to any WIFI network to turn devices ON or OFF through the internet.

Based on the [ESPRESSIF ESP32 MCU](https://www.espressif.com/en/products/socs/esp32) the [Adafruit HUZZAH32 – ESP32 Feather Board](https://www.adafruit.com/product/3405)
listens to incoming messages to switch its GPIO pins ON or OFF. The pins can be connected to any external device, in this case my PC, to remotely turn it ON or OFF and even RESET it.

The [Adafruit IO](https://io.adafruit.com/) service handles the incoming messages to the device from:
- A Web UI which acts as a dashboard.
- MQTT messages from a phone app.

![Dashboard Web_UI](https://user-images.githubusercontent.com/61059109/140608841-5fffc5b0-7618-409e-b784-a5a5388757a1.png)
