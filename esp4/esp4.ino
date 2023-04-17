#include <esp_now.h>
#include <WiFi.h>
#include "pegs.h"

//pegs - 32-36 and 39 (these can't be used for ultrasonic or led's)
//any sensors - 16-19, 21-23
//led's - 16-19, 21-23

int pegPin[9] = {-1, 32, 36, -1, 35, 39, -1, 33, 34};
int ledPin[9] = {-1, -1, 23, -1, -1, 22, -1, -1, 21};
int level = 0;
float voltage;

// REPLACE WITH THE RECEIVER'S MAC Address
uint8_t broadcastAddress[] = {0xEC, 0x94, 0xCB, 0x6D, 0x06, 0x20};

esp_data esp4 = {4, 0, 0.0};
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
  pinMode(pegPin[1], INPUT);
  pinMode(pegPin[2], INPUT);
  pinMode(pegPin[4], INPUT);
  pinMode(pegPin[5], INPUT);
  pinMode(pegPin[7], INPUT);
  pinMode(pegPin[8], INPUT);

  pinMode(ledPin[2], OUTPUT);
  pinMode(ledPin[5], OUTPUT);
  pinMode(ledPin[8], OUTPUT);
}

void loop() {
  if (espnow.level == 0) delay(1000);
  else if (espnow.level == 1) {
    level = 1;

    //connection 10
    while (espnow.start != 4) { delay(500); }
    
    //connection 11
    esp4.voltage = find_voltage(pegPin[1]);
    while (voltage < 3) {
      esp4.voltage = find_voltage(pegPin[1]);
      delay(500);
    }
    esp4.peg_in = 11;
    esp_err_t result = esp_now_send(broadcastAddress, (uint8_t *) &esp4, sizeof(esp_data));
    espnow.start = 3;

    //connection 12
    while (espnow.start != 4) { delay(250); }
    esp4.voltage = led_pin_same_board(pegPin[2], ledPin[2]);

    esp4.peg_in = 12;
    result = esp_now_send(broadcastAddress, (uint8_t *) &esp4, sizeof(esp_data));

    //connection 13
    while (espnow.start != 4) { delay(500); }

    //connection 14
    esp4.voltage = find_voltage(pegPin[4]);
    while (voltage < 3) {
      esp4.voltage = find_voltage(pegPin[4]);
      delay(500);
    }
    esp4.peg_in = 14;
    result = esp_now_send(broadcastAddress, (uint8_t *) &esp4, sizeof(esp_data));
    espnow.start = 3;

    //connection 15
    while (espnow.start != 4) { delay(250); }
    esp4.voltage = led_pin_same_board(pegPin[5], ledPin[5]);

    esp4.peg_in = 6;
    result = esp_now_send(broadcastAddress, (uint8_t *) &esp4, sizeof(esp_data));
    
    //connection 16
    while (espnow.start != 4) { delay(500); }

    //connection 17
    esp4.voltage = find_voltage(pegPin[7]);
    while (voltage < 3) {
      esp4.voltage = find_voltage(pegPin[7]);
      delay(500);
    }
    esp4.peg_in = 17;
    result = esp_now_send(broadcastAddress, (uint8_t *) &esp4, sizeof(esp_data));
    espnow.start = 3;

    //connection 9
    while (espnow.start != 4) { delay(250); }
    esp4.voltage = led_pin_same_board(pegPin[8], ledPin[8]);

    esp4.peg_in = 9;
    result = esp_now_send(broadcastAddress, (uint8_t *) &esp4, sizeof(esp_data));
    espnow.start = 0;

    while(espnow.complete != 1) { delay(500); }
    digitalWrite(ledPin[2], HIGH);
    digitalWrite(ledPin[5], HIGH);
    digitalWrite(ledPin[8], HIGH);
    delay(4000);
    digitalWrite(ledPin[2], LOW);
    digitalWrite(ledPin[5], LOW);
    digitalWrite(ledPin[8], LOW);

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
