/*
 * Ping-pong example of ESP-NOW. All nodes can use this sketch.
 * 
 * Based on the code by Rui Santos, https://RandomNerdTutorials.com/esp-now-esp8266-nodemcu-arduino-ide/
 */

#include <ESP8266WiFi.h>
#include <espnow.h>

// this does the broadcast, if you want to send only to particular receiver, enter mac address here
uint8_t broadcastAddress[] = {0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF};

unsigned long counter = 0;
unsigned long lastTime = 0;  
unsigned long timerDelay = 2000;  // send readings timer

// Callback when data is sent
void OnDataSent(uint8_t *mac_addr, uint8_t sendStatus) {
  Serial.print("Last Packet Send Status: ");
  if (sendStatus == 0){
    Serial.println("Delivery success");
  }
  else{
    Serial.print("Delivery fail, status: ");
    Serial.println(sendStatus);
  }
}

// when we receive the counter, we increment it
void OnDataRecv(uint8_t * mac, uint8_t *incomingData, uint8_t len) {
  Serial.print("Received packet from ");
  char msg[50];
  sprintf(msg, "%02x:%02x:%02x:%02x:%02x:%02x", mac[0], mac[1], mac[2], mac[3], mac[4], mac[5]);
  Serial.println(msg);
  memcpy(&counter, incomingData, sizeof(counter));
  Serial.print("counter: ");
  Serial.println(counter);
  counter++;
}
 
void setup() {
  // Init Serial Monitor
  Serial.begin(115200);
 
  // Set device as a Wi-Fi Station
  WiFi.mode(WIFI_STA);

  // Init ESP-NOW
  if (esp_now_init() != 0) {
    Serial.println("Error initializing ESP-NOW");
    return;
  }

  // we want to both send and receive
  esp_now_set_self_role(ESP_NOW_ROLE_COMBO);
  esp_now_register_send_cb(OnDataSent);
  esp_now_register_recv_cb(OnDataRecv);

  // Register peer
  esp_now_add_peer(broadcastAddress, ESP_NOW_ROLE_SLAVE, 13, NULL, 0);
}
 
void loop() {
  if ((millis() - lastTime) > timerDelay) {

    // Send message via ESP-NOW
    esp_now_send(NULL, (uint8_t *) &counter, sizeof(counter));

    lastTime = millis();
  }
}
