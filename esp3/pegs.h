//0 if none, 1-9 for board 1, 10-18 for board 2
typedef struct esp_data {
  int id;
  int peg_in;
  float voltage;
//  int led_on;
} esp_data;

typedef struct espnow_data {
  int peg_in;
//  int led_on;
  int level;
  int start;
  int complete;
} espnow_data;

void OnDataSent(const uint8_t *mac_addr, esp_now_send_status_t status) {
  char macStr[18];
  Serial.print("Packet to: ");
  // Copies the sender mac address to a string
  snprintf(macStr, sizeof(macStr), "%02x:%02x:%02x:%02x:%02x:%02x",
           mac_addr[0], mac_addr[1], mac_addr[2], mac_addr[3], mac_addr[4], mac_addr[5]);
  Serial.print(macStr);
  Serial.print(" send status:\t");
  Serial.println(status == ESP_NOW_SEND_SUCCESS ? "Delivery Success" : "Delivery Fail");
}

float find_voltage(int pegPin) {
  int pegState = analogRead(pegPin);
  float voltage = float(pegState) * (3.3 / 4095);

  return voltage;
}

float led_pin_same_board(int pegPin, int ledPin) {
    digitalWrite(ledPin, HIGH);

    float voltage = find_voltage(pegPin);
    while (voltage < 3) {
      voltage = find_voltage(pegPin);
      delay(500);
    }

    digitalWrite(ledPin, LOW);

    return voltage;
}
