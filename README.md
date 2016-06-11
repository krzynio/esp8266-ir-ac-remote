# esp8266-ir-ac-remote
ESP8266 / Arduino Air Conditioner Remote

Air Conditioner Remote using ESP-8266 and some libraries:

## Web server:

https://github.com/marcoschwartz/aREST

## Brilliant Air Conditioner Arduino Library:

https://github.com/ToniA/arduino-heatpumpir

An Arduino library to control a Panasonic, Midea, Carrier, Fujitsu or Mitsubishi heat pump/split unit air conditioner.

## Schematic

![ESP Infrared Remote](http://fritzing.org/media/fritzing-repo/projects/e/esp8266-ir-transmitter/images/IR%20Transmitter_bb.png)

## Example usage - REST call

```
curl http://ir/ac?params=power=1&temp=23&mode=3&fan=2&vert=0&horiz=0
```

This code sends only integer values. Parameters are defined here:

https://github.com/ToniA/arduino-heatpumpir/blob/master/HeatpumpIR.h

## Example usage - MQTT message

You can send MQTT message to subscribed queue - message format is like the above in REST example

```
mosquitto_pub -h 192.168.51.185 -t "home/air" -m "power=1&temp=23&mode=3&fan=2&vert=0&horiz=0"
```