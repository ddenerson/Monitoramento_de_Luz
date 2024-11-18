#include <ESP8266WiFi.h>
#include <Adafruit_MQTT.h>
#include <Adafruit_MQTT_Client.h>

const char* wifiSSID = ;
const char* wifiPassword = ;

const char* mqttServer = ;
const int mqttPort = ;
const char* mqttUser = ;
const char* mqttKey = ;

const int sensorPin = A0;
const int buzzerPin = D3;
const int brightnessLimit = 600;
const int normalLimit = 1000;

WiFiClient wifiClient;
Adafruit_MQTT_Client mqtt(&wifiClient, mqttServer, mqttPort, mqttUser, mqttKey);

char lightDataFeedName[50];
char lightStatusFeedName[50];

Adafruit_MQTT_Publish lightLevelFeed(&mqtt, lightDataFeedName);
Adafruit_MQTT_Publish lightStatusFeed(&mqtt, lightStatusFeedName);

bool excessiveBrightness = false;
unsigned long previousLightMillis = 0;
const long intervalLightNormal = 5000;

void initializeWiFi() {
    Serial.println("Tentando conectar à rede Wi-Fi...");
    WiFi.begin(wifiSSID, wifiPassword);
    while (WiFi.status() != WL_CONNECTED) {
        delay(500);
        Serial.print("-");
    }
    Serial.println("Conectado à rede Wi-Fi!");
}

void initializeMQTT() {
    while (!mqtt.connected()) {
        Serial.print("Tentando conectar ao servidor MQTT...");
        int mqttReturnCode = mqtt.connect();
        if (mqttReturnCode == 0) {
            Serial.println("Conexão bem-sucedida ao servidor MQTT!");
        } else {
            Serial.printf("Erro na conexão MQTT: %s\n", mqtt.connectErrorString(mqttReturnCode));
            delay(5000);
        }
    }
}

void setup() {
    Serial.begin(115200);
    pinMode(buzzerPin, OUTPUT);
    digitalWrite(buzzerPin, LOW);
    
    snprintf(lightDataFeedName, sizeof(lightDataFeedName), "%s/feeds/light-data", mqttUser);
    snprintf(lightStatusFeedName, sizeof(lightStatusFeedName), "%s/feeds/light-status", mqttUser);

    initializeWiFi();
    initializeMQTT();
}

void loop() {
    if (!mqtt.connected()) {
        initializeMQTT();
    }

    int lightReading = analogRead(sensorPin);
    int reversedReading = 1000 - lightReading;

    Serial.printf("Leitura do sensor: %d, Valor invertido: %d\n", lightReading, reversedReading);

    if (!lightLevelFeed.publish(reversedReading)) {
        Serial.println("Falha ao enviar o valor do sensor.");
    } else {
        Serial.println("Valor do sensor enviado com sucesso.");
    }

    if (lightReading < brightnessLimit && !excessiveBrightness) {
        excessiveBrightness = true;
        if (!lightStatusFeed.publish("Nível Alto")) {
            Serial.println("Erro ao enviar status de brilho excessivo.");
        } else {
            Serial.println("Status de brilho excessivo enviado.");
        }
        digitalWrite(buzzerPin, HIGH);
        Serial.println("Buzzer ativado.");

    } else if (lightReading > normalLimit && excessiveBrightness) {
        excessiveBrightness = false;
        digitalWrite(buzzerPin, LOW);
        Serial.println("Buzzer desativado.");
    }

    unsigned long currentMillis = millis();
    if (!excessiveBrightness && (currentMillis - previousLightMillis >= intervalLightNormal)) {
        previousLightMillis = currentMillis;
        if (!lightStatusFeed.publish("Nível Normal")) {
            Serial.println("Erro ao enviar status de brilho normal.");
        } else {
            Serial.println("Status de brilho normal enviado.");
        }
    }

    mqtt.processPackets(10);
    delay(5000);
}