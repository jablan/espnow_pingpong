# espnow_pingpong

## Sketch to demonstrate peer-to-peer communication of ESP8266 devices using ESP-Now protocol

Each device broadcasts to `FF:FF:FF:FF:FF:FF` and listens to all other devices (using `ESP_NOW_ROLE_COMBO` mode). The payload is just a counter, which is incremented in every listen handler (with more than 2 devices this probably introduces a race condition, but that's convenient as that's something that one should face in the real usage too).

### Usage

Flash the same sketch on multiple ESP8266 devices, open up their serial and see the packets ping-ponging between them.

### Problems with Wifi settings

First I had problems getting this to run, but it seems to be due to WiFi settings which get saved on ESP8266 even when the ROM is flashed. Use `esptool erase_flash` command line utility or `Tools->Erase Flash->"Sketch + Wifi settings` in Arduino IDE to ensure Wifi settings are erased.