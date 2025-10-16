#include <Arduino.h>
#include <WiFi.h>
#include <ArduinoOTA.h>
#include <Syslog.h>
#include <WiFiUdp.h>

// ===========================
// CONFIGURAÇÕES DE REDE
// ===========================
const char* WIFI_SSID     = "Manga_Mznet_2G";
const char* WIFI_PASSWORD = "pedromaria";
IPAddress localIP(192, 168, 1, 123);
IPAddress gateway(192, 168, 1, 1);
IPAddress subnet(255, 255, 255, 0);
IPAddress primaryDNS(8, 8, 8, 8);

// ===========================
// CONFIGURAÇÕES DE SYSLOG
// ===========================
WiFiUDP udpClient;
const char* SYSLOG_SERVER = "192.168.1.8";  // <-- IP da sua máquina
const int SYSLOG_PORT = 5514;                  // Porta que o script Python escuta
Syslog syslog(udpClient, SYSLOG_SERVER, SYSLOG_PORT, "esp32", "esp32_ota_dualtask", LOG_KERN);

// ===========================
// PROTÓTIPOS DAS TASKS
// ===========================
void taskOTA(void *parameter);
void taskAvionica(void *parameter);

// ===========================
// FUNÇÃO DE INICIALIZAÇÃO
// ===========================
void setup() {
  Serial.begin(115200);
  delay(1000);

  // --- Conexão Wi-Fi ---
  WiFi.mode(WIFI_STA);
  WiFi.config(localIP, gateway, subnet, primaryDNS);
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  syslog.log(LOG_INFO, "Conectando ao Wi-Fi...");

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  syslog.logf(LOG_INFO, "Conectado! IP: %s", WiFi.localIP().toString().c_str());

  // --- Configuração OTA ---
  ArduinoOTA.onStart([]() {
    syslog.log(LOG_INFO, "OTA iniciado!");
  });
  ArduinoOTA.onEnd([]() {
    syslog.log(LOG_INFO, "OTA finalizado com sucesso!");
  });
  ArduinoOTA.onProgress([](unsigned int progress, unsigned int total) {
    int percent = (progress * 100) / total;
    syslog.logf(LOG_INFO, "Progresso OTA: %d%%", percent);
  });
  ArduinoOTA.onError([](ota_error_t error) {
    syslog.logf(LOG_ERR, "Erro OTA [%u]", error);
  });
  ArduinoOTA.begin();

  syslog.log(LOG_INFO, "Serviço OTA iniciado e aguardando na porta 3232");

  // --- Criação das tasks paralelas ---
  xTaskCreatePinnedToCore(
    taskOTA,            // Função da task
    "TaskOTA",          // Nome
    4096,               // Stack size
    NULL,               // Parâmetro
    1,                  // Prioridade
    NULL,               // Handle
    0                   // Core 0 (mantemos OTA no core de rede)
  );

  xTaskCreatePinnedToCore(
    taskAvionica,       // Função da task
    "TaskAvionica",     // Nome
    8192,               // Stack size
    NULL,               // Parâmetro
    1,                  // Prioridade
    NULL,               // Handle
    1                   // Core 1 (deixa livre o outro core)
  );
}

// ===========================
// LOOP PRINCIPAL (vazio)
// ===========================
void loop() {
  // tudo roda em tasks
}

// ===========================
// TASK 1: OTA
// ===========================
void taskOTA(void *parameter) {
  for (;;) {
    ArduinoOTA.handle();
    vTaskDelay(12 / portTICK_PERIOD_MS);
  }
}

// ===========================
// TASK 2: AVIÔNICA
// ===========================
void taskAvionica(void *parameter) {
  syslog.log(LOG_INFO, "Task de aviônica iniciada!");

  for (;;) {
    // === SEU CÓDIGO AQUI ===
    // Exemplo de telemetria fictícia:
    float altitude = random(100, 1000) / 10.0;
    float velocidade = random(0, 100) / 10.0;

    syslog.logf(LOG_INFO, "Alt: %.1f m | Vel: %.1f m/s", altitude, velocidade);

    vTaskDelay(2000 / portTICK_PERIOD_MS);  // Envia a cada 2 segundos
  }
}
