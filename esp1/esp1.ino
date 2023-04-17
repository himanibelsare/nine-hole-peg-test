#include <esp_now.h>
#include <WiFi.h>
#include "pegs.h"

//1,4,7 pegs and 1, 2, 4, 5, 7, 8 leds here

int pegPin[9] = {36, -1, -1, 35, -1, -1, 39, -1, -1};
int ledPin[9] = {16, 19, -1, 17, 21, -1, 18, 22, -1};
int level = 0;
float voltage;

// REPLACE WITH THE RECEIVER'S MAC Address
uint8_t broadcastAddress[] = {0xEC, 0x94, 0xCB, 0x6D, 0x06, 0x20};

esp_data esp1 = {1, 0, 0.0};
espnow_data espnow = {0, 0, 1};

esp_now_peer_info_t peerInfo;

void OnDataRecv(const uint8_t * mac, const uint8_t *incomingData, int len) {
  memcpy(&espnow, incomingData, sizeof(espnow));
  Serial.print("Bytes received: ");
  Serial.println(len);
  Serial.print("peg: ");
  Serial.println(espnow.peg_in);
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

  memcpy(peerInfo.peer_addr, broadcastAddress, 6);
  if (esp_now_add_peer(&peerInfo) != ESP_OK){
    Serial.println("Failed to add peer");
    return;
  }

  esp_now_register_recv_cb(OnDataRecv);

  //initializing pins
  pinMode(pegPin[0], INPUT);
//  pinMode(pegPin[1], INPUT);
//  pinMode(pegPin[2], INPUT);
  pinMode(pegPin[3], INPUT);
//  pinMode(pegPin[4], INPUT);
//  pinMode(pegPin[5], INPUT);
  pinMode(pegPin[6], INPUT);
//  pinMode(pegPin[7], INPUT);
//  pinMode(pegPin[8], INPUT);

  pinMode(ledPin[0], OUTPUT);
  pinMode(ledPin[1], OUTPUT);
//  pinMode(ledPin[2], OUTPUT);
  pinMode(ledPin[3], OUTPUT);
  pinMode(ledPin[4], OUTPUT);
//  pinMode(ledPin[5], OUTPUT);
  pinMode(ledPin[6], OUTPUT);
  pinMode(ledPin[7], OUTPUT);
//  pinMode(ledPin[8], OUTPUT);
}

void loop() {
  if (espnow.level == 0) delay(1000);
  else if (espnow.level == 1) {
    level = 1;

    //connection 1
    esp1.voltage = led_pin_same_board(pegPin[0], ledPin[0]);

    esp1.peg_in = 1;
    esp_err_t result = esp_now_send(broadcastAddress, (uint8_t *) &esp1, sizeof(esp_data));
    if (result != ESP_OK) {
      Serial.println("Error sending the data 1");
    }
    
    //connection 2
    digitalWrite(ledPin[1], HIGH);
    espnow.start = 2;
    while (espnow.start != 1) { delay(250); }
    digitalWrite(ledPin[1], LOW);
    espnow.start = 2;

    //connection 3
    while (espnow.start != 1) { delay(500); }

    //connection 4
    esp1.voltage = led_pin_same_board(pegPin[3], ledPin[3]);

    esp1.peg_in = 4;
    result = esp_now_send(broadcastAddress, (uint8_t *) &esp1, sizeof(esp_data));
    if (result != ESP_OK) {
      Serial.println("Error sending the data 4");
    }

    //connection 5
    digitalWrite(ledPin[4], HIGH);
    espnow.start = 2;
    while (espnow.start != 1) { delay(250); }
    digitalWrite(ledPin[4], LOW);
    espnow.start = 2;

    //connection 6
    while (espnow.start != 1) { delay(500); }

    //connection 7
    esp1.voltage = led_pin_same_board(pegPin[6], ledPin[6]);

    esp1.peg_in = 7;
    result = esp_now_send(broadcastAddress, (uint8_t *) &esp1, sizeof(esp_data));
    if (result != ESP_OK) {
      Serial.println("Error sending the data 4");
    }

    //connection 8
    digitalWrite(ledPin[7], HIGH);
    espnow.start = 2;
    while (espnow.start != 1) { delay(250); }
    digitalWrite(ledPin[7], LOW);
    espnow.start = 2;

    //connection 9

    while(espnow.complete != 1) { delay(500); }
    digitalWrite(ledPin[0], HIGH);
    digitalWrite(ledPin[1], HIGH);
    digitalWrite(ledPin[3], HIGH);
    digitalWrite(ledPin[4], HIGH);
    digitalWrite(ledPin[6], HIGH);
    digitalWrite(ledPin[7], HIGH);
    delay(4000);
    digitalWrite(ledPin[0], LOW);
    digitalWrite(ledPin[1], LOW);
    digitalWrite(ledPin[3], LOW);
    digitalWrite(ledPin[4], LOW);
    digitalWrite(ledPin[6], LOW);
    digitalWrite(ledPin[7], LOW);
    
    level = 0;
  }
  else if (espnow.level == 2) {
    level = 2;

    level = 0;
  }
  else if (espnow.level == 3) {
    level = 3;

    level = 0;
  }
  delay(1000);
}
