#include <esp_now.h>
#include <WiFi.h>

typedef struct {
  bool obstaculoDetectado; 
} Dados;

Dados dadosRecebidos;

void dadosRecebidosCallback(const esp_now_recv_info* info, const uint8_t* dados, int tamanho) {
  if (tamanho == sizeof(Dados)) {
    memcpy(&dadosRecebidos, dados, tamanho);
    Serial.print("Obstáculo detectado: ");
    if(dadosRecebidos.obstaculoDetectado){
    digitalWrite(2,HIGH);
    } else {
      digitalWrite(2,LOW);
    }
    Serial.println(dadosRecebidos.obstaculoDetectado ? "Sim" : "Não");
  } else {
    Serial.println("Dados recebidos com tamanho incorreto");
  }
}

void setup() {
  pinMode(2,OUTPUT);
  digitalWrite(2,LOW);
  Serial.begin(115200);
  WiFi.mode(WIFI_STA);
  Serial.print("Endereço MAC do Receptor: ");
  Serial.println(WiFi.macAddress());

}

void loop() {
   if (esp_now_init() != ESP_OK) {
    Serial.println("Erro ao inicializar ESP-NOW");
    ESP.restart();
  }

  esp_now_register_recv_cb(dadosRecebidosCallback);

}
