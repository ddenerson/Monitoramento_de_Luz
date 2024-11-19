# Sistema de Monitoramento de Luz

### Introdução
Este projeto utiliza um sensor LDR (fotoresistor) para monitorar a intensidade de luz e acionar um buzzer quando os níveis de luminosidade estiverem acima de um determinado limite. O sistema também publica os dados de luminosidade e status de brilho em tempo real usando o protocolo MQTT.


### Funcionalidades
- Monitoramento da intensidade de luz com sensor LDR.
- Acionamento do buzzer em caso de luminosidade excessiva.
- Comunicação em tempo real via protocolo MQTT com um broker.
- Dados publicados em tópicos MQTT para visualização remota.


### Requisitos:
- Placa de desenvolvimento: NodeMCU ESP8266.
- Sensor LDR.
- Buzzer.
- Broker MQTT


### Instruções de Uso:
1. Conecte o NodeMCU à sua rede Wi-Fi.
2. Configure o broker MQTT no código.
3. Carregue o código no NodeMCU e execute.
  

### Código e Documentação:
**1. Inclusão das Bibliotecas**
#include <ESP8266WiFi.h>               // Biblioteca para conectar o ESP8266 à rede Wi-Fi
#include <Adafruit_MQTT.h>              // Biblioteca para comunicação com o broker MQTT
#include <Adafruit_MQTT_Client.h>       // Cliente MQTT para o ESP8266

Essas bibliotecas são necessárias para a comunicação com o Wi-Fi e o broker MQTT. O ESP8266 será usado para se conectar a uma rede sem fio e enviar dados para um broker MQTT

**2. Definição de Variáveis e Parâmetros**
const char* wifiSSID = "";            // SSID da rede Wi-Fi
const char* wifiPassword = "";        // Senha da rede Wi-Fi

const char* mqttServer = "";          // Endereço do servidor MQTT
const int mqttPort = 1883;            // Porta do servidor MQTT
const char* mqttUser = "";            // Usuário para autenticação MQTT
const char* mqttKey = "";             // Senha ou chave para autenticação MQTT

const int sensorPin = A0;             // Pino analógico onde o sensor de luminosidade está conectado
const int buzzerPin = D3;             // Pino digital para controlar o buzzer
const int brightnessLimit = 600;      // Limite de luminosidade para ativar o buzzer
const int normalLimit = 1000;         // Limite superior de luminosidade para "Nível Normal"

Aqui são configuradas as credenciais de Wi-Fi e as informações de conexão do MQTT (endereço, porta, usuário e chave). Também são definidos os pinos para o sensor de luminosidade (que usa leitura analógica) e o buzzer, além dos limites de luminosidade.

**3. Objetos MQTT**
WiFiClient wifiClient;                  // Cliente Wi-Fi para comunicação com o servidor MQTT
Adafruit_MQTT_Client mqtt(&wifiClient, mqttServer, mqttPort, mqttUser, mqttKey); // Cliente MQTT

char lightDataFeedName[50];            // Nome do feed para enviar os dados de luminosidade
char lightStatusFeedName[50];          // Nome do feed para enviar o status de brilho (normal ou alto)

Adafruit_MQTT_Publish lightLevelFeed(&mqtt, lightDataFeedName);   // Publica o valor da luminosidade
Adafruit_MQTT_Publish lightStatusFeed(&mqtt, lightStatusFeedName); // Publica o status do nível de brilho

Aqui são criados objetos para o cliente Wi-Fi e o cliente MQTT. As variáveis lightDataFeedName e lightStatusFeedName definem os tópicos para onde os dados serão enviados no broker MQTT.

**4. Flags e Controle de Tempo**
bool excessiveBrightness = false;         // Flag para indicar se o brilho está excessivo
unsigned long previousLightMillis = 0;    // Armazena o tempo da última vez que o status foi atualizado
const long intervalLightNormal = 5000;    // Intervalo de tempo entre os envios do status "Nível Normal" (5 segundos)

A variável excessiveBrightness é usada para rastrear se o brilho está acima do limite. A variável previousLightMillis é usada para controlar o intervalo de tempo para o envio do status de luminosidade normal.

**5. Funções de Inicialização**
- Conexão Wi-Fi:
void initializeWiFi() {
    Serial.println("Tentando conectar à rede Wi-Fi...");
    WiFi.begin(wifiSSID, wifiPassword);
    while (WiFi.status() != WL_CONNECTED) {
        delay(500);
        Serial.print("-");
    }
    Serial.println("Conectado à rede Wi-Fi!");
}

A função initializeWiFi() tenta conectar o ESP8266 à rede Wi-Fi usando o SSID e a senha definidos. Enquanto não houver conexão, o código exibe um sinal "-" no console.

- Conexão MQTT:
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

A função initializeMQTT() tenta conectar o cliente MQTT ao servidor. Caso a conexão falhe, ele tentará novamente após um atraso de 5 segundos.

- Função Setup:
void setup() {
    Serial.begin(115200);         // Inicializa a comunicação serial
    pinMode(buzzerPin, OUTPUT);   // Define o pino do buzzer como saída
    digitalWrite(buzzerPin, LOW); // Garante que o buzzer começa desligado

    snprintf(lightDataFeedName, sizeof(lightDataFeedName), "%s/feeds/light-data", mqttUser);  // Configura o nome do feed de dados
    snprintf(lightStatusFeedName, sizeof(lightStatusFeedName), "%s/feeds/light-status", mqttUser);  // Configura o nome do feed de status

    initializeWiFi();   // Conecta à rede Wi-Fi
    initializeMQTT();   // Conecta ao servidor MQTT
}

No setup(), o código inicializa a comunicação serial e os pinos para o buzzer. Os nomes dos feeds MQTT são configurados para incluir o nome de usuário. Em seguida, as funções de inicialização Wi-Fi e MQTT são chamadas.

- Função Loop:
void loop() {
    if (!mqtt.connected()) {
        initializeMQTT();  // Reconnecta ao MQTT se a conexão cair
    }

    int lightReading = analogRead(sensorPin);   // Lê o valor do sensor de luminosidade
    int reversedReading = 1000 - lightReading;  // Inverte o valor da leitura

    Serial.printf("Leitura do sensor: %d, Valor invertido: %d\n", lightReading, reversedReading); // Exibe os valores no console

    if (!lightLevelFeed.publish(reversedReading)) {
        Serial.println("Falha ao enviar o valor do sensor.");
    } else {
        Serial.println("Valor do sensor enviado com sucesso.");
    }

    // Verifica o nível de luminosidade e ativa o buzzer se necessário
    if (lightReading < brightnessLimit && !excessiveBrightness) {
        excessiveBrightness = true;
        if (!lightStatusFeed.publish("Nível Alto")) {
            Serial.println("Erro ao enviar status de brilho excessivo.");
        } else {
            Serial.println("Status de brilho excessivo enviado.");
        }
        digitalWrite(buzzerPin, HIGH);  // Ativa o buzzer
        Serial.println("Buzzer ativado.");
    } else if (lightReading > normalLimit && excessiveBrightness) {
        excessiveBrightness = false;
        digitalWrite(buzzerPin, LOW);  // Desativa o buzzer
        Serial.println("Buzzer desativado.");
    }

    // Envia o status de luminosidade normal a cada intervalo de 5 segundos
    unsigned long currentMillis = millis();
    if (!excessiveBrightness && (currentMillis - previousLightMillis >= intervalLightNormal)) {
        previousLightMillis = currentMillis;
        if (!lightStatusFeed.publish("Nível Normal")) {
            Serial.println("Erro ao enviar status de brilho normal.");
        } else {
            Serial.println("Status de brilho normal enviado.");
        }
    }

    mqtt.processPackets(10);  // Processa pacotes MQTT recebidos
    delay(5000);               // Atraso de 5 segundos antes da próxima iteração
}

O loop() executa as tarefas principais de leitura do sensor de luminosidade, envio de dados via MQTT, controle do buzzer e verificação de status. Ele também se reconecta automaticamente ao servidor MQTT se a conexão for perdida. O status do brilho (alto ou normal) é enviado periodicamente.


### Hardware Utilizado:
- **Plataforma de desenvolvimento:** NodeMCU ESP8266.
- **Sensor de Luz:** LDR (fotoresistor).
- **Atuador:** Buzzer.
- **Outros componentes:**
  - Protoboard.
  - Fios de conexão.
  - Fonte de alimentação para o NodeMCU.


## Diagrama de Circuito:
-![image](https://github.com/user-attachments/assets/99541bc3-649f-4726-a22d-2098c91ffc88)


### Comunicação MQTT:
- O projeto utiliza o protocolo MQTT para enviar dados de luminosidade e status de brilho.
- **Tópicos utilizados:**
  - `light-data`: Para enviar os valores de luminosidade do sensor.
  - `light-status`: Para enviar o status de brilho (Nível Alto ou Nível Normal).


### Configuração do Broker MQTT:
- O broker MQTT pode ser configurado para receber dados em tempo real.


### Interfaces e Protocolos de Comunicação
Protocolo/Interface: MQTT (Message Queuing Telemetry Transport).
Função: Comunicação de dados.
Detalhes: Utilizado para enviar os dados do sensor de luminosidade e o status de brilho. O ESP8266 publica mensagens em tópicos MQTT que podem ser lidas por outros dispositivos ou sistemas.

Protocolo/Interface: TCP/IP (Transmission Control Protocol/Internet Protocol).
Função: Comunicação via rede.
Detalhes: Utilizado para estabelecer a conexão do ESP8266 com a rede Wi-Fi e, em seguida, com o broker MQTT. Permite a troca de mensagens via internet entre o dispositivo e o broker MQTT.

Protocolo/Interface: Wi-Fi.
Função: Interface de rede local.
Detalhes:O ESP8266 utiliza o Wi-Fi como interface para acessar a internet e se comunicar com o broker MQTT via TCP/IP.


### Protocolo MQTT e Comunicação TCP/IP
O ESP8266 utiliza o protocolo MQTT para se comunicar com um broker MQTT e enviar os dados do sensor de luminosidade e o status (alto ou normal) da luz. O código que você forneceu usa a biblioteca Adafruit_MQTT para implementar a comunicação MQTT no ESP8266. No código, o dispositivo se conecta a um broker MQTT e publica dados em tópicos específicos para monitoramento remoto.

O ESP8266 se conecta à rede Wi-Fi usando o protocolo TCP/IP, estabelecendo uma comunicação através da rede.
Uma vez conectado ao Wi-Fi, o ESP8266 utiliza o protocolo TCP/IP para se conectar ao broker MQTT e enviar as mensagens.

Uma vez conectado, as mensagens MQTT são transmitidas entre o ESP8266 e o broker MQTT via TCP/IP, permitindo que o dispositivo envie os dados do sensor e receba comandos ou informações de outros dispositivos (caso configurado).
