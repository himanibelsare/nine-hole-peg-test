#include <esp_now.h>
#include <WiFi.h>
#include "pegs.h"

//pegs - 32-36 and 39 (these can't be used for ultrasonic or led's)
//any sensors - 16-19, 21-23
//led's - 16-19, 21-23

//1,4,7 pegs and 1, 2, 4, 5, 7, 8 leds here

int pegPin[9] = {36, -1, -1, 39, -1, -1, 34, -1, -1};
int ledPin[9] = {16, 19, -1, 17, 21, -1, 18, 22, -1};
int level = 0;
float voltage;

// REPLACE WITH THE RECEIVER'S MAC Address
uint8_t broadcastAddress[] = {0xEC, 0x94, 0xCB, 0x6D, 0x06, 0x20};

esp_data esp3 = {3, 0, 0.0};
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
  pinMode(pegPin[3], INPUT);
  pinMode(pegPin[6], INPUT);

  pinMode(ledPin[0], OUTPUT);
  pinMode(ledPin[1], OUTPUT);
  pinMode(ledPin[3], OUTPUT);
  pinMode(ledPin[4], OUTPUT);
  pinMode(ledPin[6], OUTPUT);
  pinMode(ledPin[7], OUTPUT);
}

void loop() {
  if (espnow.level == 0) delay(1000);
  else if (espnow.level == 1) {
    level = 1;

    while (espnow.start != 3) { delay(500); }

    //connection 10
    esp3.voltage = led_pin_same_board(pegPin[0], ledPin[0]);
    esp3.peg_in = 10;
    esp_err_t result = esp_now_send(broadcastAddress, (uint8_t *) &esp3, sizeof(esp_data));
    
    //connection 11
    digitalWrite(ledPin[1], HIGH);
    espnow.start = 4;
    while (espnow.start != 3) { delay(250); }
    digitalWrite(ledPin[1], LOW);
    espnow.start = 4;

    //connection 12
    while (espnow.start != 3) { delay(500); }

    //connection 13
    esp3.voltage = led_pin_same_board(pegPin[3], ledPin[3]);
    esp3.peg_in = 13;
    result = esp_now_send(broadcastAddress, (uint8_t *) &esp3, sizeof(esp_data));

    //connection 14
    digitalWrite(ledPin[4], HIGH);
    espnow.start = 4;
    while (espnow.start != 3) { delay(250); }
    digitalWrite(ledPin[4], LOW);
    espnow.start = 4;

    //connection 15
    while (espnow.start != 3) { delay(500); }

    //connection 16
    esp3.voltage = led_pin_same_board(pegPin[6], ledPin[6]);
    esp3.peg_in = 16;
    result = esp_now_send(broadcastAddress, (uint8_t *) &esp3, sizeof(esp_data));

    //connection 17
    digitalWrite(ledPin[7], HIGH);
    espnow.start = 4;
    while (espnow.start != 3) { delay(250); }
    digitalWrite(ledPin[7], LOW);
    espnow.start = 4;

    //connection 18

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
