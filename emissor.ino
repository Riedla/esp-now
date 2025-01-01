#include <esp_now.h>
#include <WiFi.h>

#define SENSOR_PIN 2

uint8_t macReceptor[] = {0x08, 0x3A, 0xF2, 0x2C, 0xC3, 0x2C};

typedef struct {
  bool obstaculoDetectado; 
} Dados;

Dados dados;

void envioConfirmado(const uint8_t *mac, esp_now_send_status_t status) {
  Serial.print("Status do envio: ");
  Serial.println(status == ESP_NOW_SEND_SUCCESS ? "Sucesso" : "Falha");
}

void setup() {
  Serial.begin(115200);
  WiFi.mode(WIFI_STA);
  Serial.print("Endereço MAC do Transmissor: ");
  Serial.println(WiFi.macAddress());

  pinMode(SENSOR_PIN, INPUT);

  if (esp_now_init() != ESP_OK) {
    Serial.println("Erro ao inicializar ESP-NOW");
    ESP.restart();
  }

  esp_now_register_send_cb(envioConfirmado);

  esp_now_peer_info_t peerInfo = {};
  memcpy(peerInfo.peer_addr, macReceptor, 6);
  peerInfo.channel = 0;
  peerInfo.encrypt = false;

  if (esp_now_add_peer(&peerInfo) != ESP_OK) {
    Serial.println("Erro ao adicionar peer");
    ESP.restart();
  }
}

void loop() {
  dados.obstaculoDetectado = digitalRead(SENSOR_PIN) == LOW;

  esp_err_t resultado = esp_now_send(macReceptor, (uint8_t *)&dados, sizeof(dados));
  if (resultado == ESP_OK) {
    Serial.println("Dados enviados com sucesso");
  } else {
    Serial.println("Erro ao enviar dados");
  }
  delay(500);
}