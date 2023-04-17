#include <esp_now.h>
#include <WiFi.h>
#include "pegs.h"

int levelPegPin[3] = {35, 34, 36};
int levelLedPin[3] = {19, 21, 23};
int trigPin1;
int echoPin1;
int trigPin2;
int echoPin2;

// REPLACE WITH THE RECEIVER'S MAC Address
uint8_t broadcastAddress1[] = {0xEC, 0x94, 0xCB, 0x6D, 0x06, 0x20};
uint8_t broadcastAddress2[] = {0xEC, 0x94, 0xCB, 0x6D, 0x06, 0x20};
uint8_t broadcastAddress3[] = {0xEC, 0x94, 0xCB, 0x6D, 0x06, 0x20};
uint8_t broadcastAddress4[] = {0xEC, 0x94, 0xCB, 0x6D, 0x06, 0x20};

esp_data espData;

esp_data esp1 = {0, 0, 0.0};
esp_data esp2 = {0, 0, 0.0};
esp_data esp3 = {0, 0, 0.0};
esp_data esp4 = {0, 0, 0.0};
//esp_data esps[4];
espnow_data espnow = {0, 0, 0, 0};

esp_now_peer_info_t peerInfo;

void OnDataRecv(const uint8_t * mac_addr, const uint8_t *incomingData, int len) {
  char macStr[18];
  Serial.print("Packet received from: ");
  snprintf(macStr, sizeof(macStr), "%02x:%02x:%02x:%02x:%02x:%02x",
           mac_addr[0], mac_addr[1], mac_addr[2], mac_addr[3], mac_addr[4], mac_addr[5]);
  Serial.println(macStr);
  memcpy(&espData, incomingData, sizeof(espData));
  Serial.printf("Board ID %u: %u bytes\n", espData.id, len);
  Serial.println();
}

void setup() {
  Serial.begin(115200);

  WiFi.mode(WIFI_STA);

  if (esp_now_init() != ESP_OK) {
    Serial.println("Error initializing ESP-NOW");
    return;
  }

  esp_now_register_send_cb(OnDataSent);

  esp_now_peer_info_t peerInfo;
  peerInfo.channel = 0;  
  peerInfo.encrypt = false;
  
  memcpy(peerInfo.peer_addr, broadcastAddress1, 6);
  if (esp_now_add_peer(&peerInfo) != ESP_OK){
    Serial.println("Failed to add peer 1");
    return;
  }
  
  memcpy(peerInfo.peer_addr, broadcastAddress2, 6);
  if (esp_now_add_peer(&peerInfo) != ESP_OK){
    Serial.println("Failed to add peer 2");
    return;
  }
  
  memcpy(peerInfo.peer_addr, broadcastAddress3, 6);
  if (esp_now_add_peer(&peerInfo) != ESP_OK){
    Serial.println("Failed to add peer 3");
    return;
  }
  
  memcpy(peerInfo.peer_addr, broadcastAddress4, 6);
  if (esp_now_add_peer(&peerInfo) != ESP_OK){
    Serial.println("Failed to add peer 4");
    return;
  }

  esp_now_register_recv_cb(OnDataRecv);

  //initializing pins
  pinMode(trigPin1, OUTPUT);
  pinMode(trigPin2, OUTPUT);
  pinMode(echoPin1, INPUT);
  pinMode(echoPin2, INPUT);
  pinMode(levelPegPin[0], INPUT);
  pinMode(levelPegPin[1], INPUT);
  pinMode(levelPegPin[2], INPUT);
  pinMode(levelLedPin[0], OUTPUT);
  pinMode(levelLedPin[1], OUTPUT);
  pinMode(levelLedPin[2], OUTPUT);
}

void loop() {
  if (find_voltage(levelPegPin[0]) > 3) {
    espnow.level = 1;
    digitalWrite(levelLedPin[0], HIGH);

    espnow.complete = 0;

    //connection 1
    espnow.start = 1;
    esp_err_t result = esp_now_send(broadcastAddress1, (uint8_t *) &espnow, sizeof(espnow_data));
    if (result != ESP_OK) {
      Serial.println("Error sending the data 1");
    }
    while (espData.id != 1 || espData.peg_in != 1) { delay(500); }
    espnow.peg_in = 1;
    
    //connection 2
    espnow.start = 2;
    result = esp_now_send(broadcastAddress2, (uint8_t *) &espnow, sizeof(espnow_data));
    if (result != ESP_OK) {
      Serial.println("Error sending the data 2");
    }
    espnow.start = 1;
    while (espData.id != 2 || espData.peg_in != 2) { delay(500); }
    espnow.peg_in = 2;

    result = esp_now_send(broadcastAddress1, (uint8_t *) &espnow, sizeof(espnow_data));
    if (result != ESP_OK) {
      Serial.println("Error sending the data 3");
    }

    //connection 3
    espnow.start = 2;
    result = esp_now_send(broadcastAddress2, (uint8_t *) &espnow, sizeof(espnow_data));
    if (result != ESP_OK) {
      Serial.println("Error sending the data 4");
    }
    while (espData.id != 2 || espData.peg_in != 3) { delay(500); }
    espnow.peg_in = 3;

    //connection 4
    espnow.start = 1;
    result = esp_now_send(broadcastAddress1, (uint8_t *) &espnow, sizeof(espnow_data));
    if (result != ESP_OK) {
      Serial.println("Error sending the data 5");
    }
    while (espData.id != 1 || espData.peg_in != 4) { delay(500); }
    espnow.peg_in = 4;

    //connection 5
    espnow.start = 2;
    result = esp_now_send(broadcastAddress2, (uint8_t *) &espnow, sizeof(espnow_data));
    if (result != ESP_OK) {
      Serial.println("Error sending the data 6");
    }
    espnow.start = 1;
    while (espData.id != 2 || espData.peg_in != 5) { delay(500); }
    espnow.peg_in = 5;

    result = esp_now_send(broadcastAddress1, (uint8_t *) &espnow, sizeof(espnow_data));
    if (result != ESP_OK) {
      Serial.println("Error sending the data 7");
    }

    //connection 6
    espnow.start = 2;
    result = esp_now_send(broadcastAddress2, (uint8_t *) &espnow, sizeof(espnow_data));
    if (result != ESP_OK) {
      Serial.println("Error sending the data 8");
    }
    while (espData.id != 2 || espData.peg_in != 6) { delay(500); }
    espnow.peg_in = 6;

    //connection 7
    espnow.start = 1;
    result = esp_now_send(broadcastAddress1, (uint8_t *) &espnow, sizeof(espnow_data));
    if (result != ESP_OK) {
      Serial.println("Error sending the data 9");
    }
    while (espData.id != 1 || espData.peg_in != 7) { delay(500); }
    espnow.peg_in = 7;

    //connection 8
    espnow.start = 2;
    result = esp_now_send(broadcastAddress2, (uint8_t *) &espnow, sizeof(espnow_data));
    if (result != ESP_OK) {
      Serial.println("Error sending the data 10");
    }
    espnow.start = 1;
    while (espData.id != 2 || espData.peg_in != 8) { delay(500); }
    espnow.peg_in = 8;

    result = esp_now_send(broadcastAddress1, (uint8_t *) &espnow, sizeof(espnow_data));
    if (result != ESP_OK) {
      Serial.println("Error sending the data 11");
    }

    //connection 9
    espnow.start = 2;
    result = esp_now_send(broadcastAddress2, (uint8_t *) &espnow, sizeof(espnow_data));
    if (result != ESP_OK) {
      Serial.println("Error sending the data 12");
    }
    while (espData.id != 2 || espData.peg_in != 9) { delay(500); }
    espnow.peg_in = 9;


    

    //connection 10
    espnow.start = 3;
    result = esp_now_send(broadcastAddress3, (uint8_t *) &espnow, sizeof(espnow_data));
    while (espData.id != 3 || espData.peg_in != 10) { delay(500); }
    espnow.peg_in = 10;

    //connection 11
    espnow.start = 4;
    result = esp_now_send(broadcastAddress4, (uint8_t *) &espnow, sizeof(espnow_data));
    espnow.start = 3;
    while (espData.id != 4 || espData.peg_in != 11) { delay(500); }
    espnow.peg_in = 11;
    result = esp_now_send(broadcastAddress3, (uint8_t *) &espnow, sizeof(espnow_data));

    //connection 12
    espnow.start = 4;
    result = esp_now_send(broadcastAddress4, (uint8_t *) &espnow, sizeof(espnow_data));
    while (espData.id != 4 || espData.peg_in != 12) { delay(500); }
    espnow.peg_in = 12;

    //connection 13
    espnow.start = 3;
    result = esp_now_send(broadcastAddress3, (uint8_t *) &espnow, sizeof(espnow_data));
    while (espData.id != 3 || espData.peg_in != 13) { delay(500); }
    espnow.peg_in = 13;

    //connection 14
    espnow.start = 4;
    result = esp_now_send(broadcastAddress4, (uint8_t *) &espnow, sizeof(espnow_data));
    espnow.start = 3;
    while (espData.id != 4 || espData.peg_in != 14) { delay(500); }
    espnow.peg_in = 14;
    result = esp_now_send(broadcastAddress3, (uint8_t *) &espnow, sizeof(espnow_data));

    //connection 15
    espnow.start = 4;
    result = esp_now_send(broadcastAddress4, (uint8_t *) &espnow, sizeof(espnow_data));
    while (espData.id != 4 || espData.peg_in != 15) { delay(500); }
    espnow.peg_in = 15;

    //connection 16
    espnow.start = 5;
    result = esp_now_send(broadcastAddress3, (uint8_t *) &espnow, sizeof(espnow_data));
    while (espData.id != 3 || espData.peg_in != 16) { delay(500); }
    espnow.peg_in = 16;

    //connection 17
    espnow.start = 4;
    result = esp_now_send(broadcastAddress4, (uint8_t *) &espnow, sizeof(espnow_data));
    espnow.start = 3;
    while (espData.id != 4 || espData.peg_in != 17) { delay(500); }
    espnow.peg_in = 17;
    result = esp_now_send(broadcastAddress3, (uint8_t *) &espnow, sizeof(espnow_data));

    //connection 18
    espnow.start = 4;
    result = esp_now_send(broadcastAddress4, (uint8_t *) &espnow, sizeof(espnow_data));
    while (espData.id != 4 || espData.peg_in != 18) { delay(500); }
    espnow.peg_in = 18;

    espnow.start = 0;
    Serial.println("LEVEL 1 COMPLETE!!!!!");
    espnow.complete = 1;
    result = esp_now_send(broadcastAddress1, (uint8_t *) &espnow, sizeof(espnow_data));
    result = esp_now_send(broadcastAddress2, (uint8_t *) &espnow, sizeof(espnow_data));
    result = esp_now_send(broadcastAddress3, (uint8_t *) &espnow, sizeof(espnow_data));
    result = esp_now_send(broadcastAddress4, (uint8_t *) &espnow, sizeof(espnow_data));
    espnow.complete = 0;
    
    digitalWrite(levelLedPin[0], LOW);
    espnow.level = 0;
  }
  else if (find_voltage(levelPegPin[1]) > 3) {
    espnow.level = 2;
    digitalWrite(levelLedPin[1], HIGH);

    digitalWrite(levelLedPin[1], LOW);
    espnow.level = 0;
  }
  else if (find_voltage(levelPegPin[2]) > 3) {
    espnow.level = 3;
    digitalWrite(levelLedPin[2], HIGH);

    digitalWrite(levelLedPin[2], LOW);
    espnow.level = 0;
  }
  else {
    espnow.level = 0;
  }
  delay(1000);
}
