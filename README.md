# Sistema de Monitoramento de Luz

### Introdução:
Este projeto utiliza um sensor LDR (fotoresistor) para monitorar a intensidade de luz e acionar o buzzer quando os níveis de luminosidade estiverem acima do limite determinado. O sistema publica os dados de luminosidade e status de brilho em tempo real usando o protocolo MQTT.


### Funcionalidades:
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
  

### Documentação:
**1. Inclusão das Bibliotecas:** 
- As bibliotecas fazem a comunicação com o Wi-Fi e o broker MQTT.
- O ESP8266 é utilizado para se conectar a uma rede sem fio e enviar dados para um broker MQTT.

***No Código:***
- ESP8266WiFi.h: é a biblioteca que permite a conexão do microcontrolador ESP8266 com redes Wi-Fi.
- Adafruit_MQTT.h e Adafruit_MQTT_Client.h: são as bibliotecas que implementam o protocolo MQTT para comunicação entre o microcontrolador e um servidor MQTT.


**2. Definição de Variáveis e Parâmetros:**
- Configuração das credenciais de Wi-Fi e as informações de conexão do MQTT (endereço, porta, usuário e chave).  
- São definidos os pinos para o sensor de luminosidade, o buzzer e os limites de luminosidade.
  
***No Código:***
- wifiSSID e wifiPassword: armazenam as credenciais da rede Wi-Fi.
- mqttServer: endereço do servidor MQTT ao qual o ESP8266 se conectará.
- mqttPort: porta do servidor MQTT (geralmente 1883 para conexões sem criptografia).
- mqttUser e mqttKey: credenciais para autenticação no servidor MQTT.
- sensorPin: pino analógico A0, onde está conectado o sensor de luz (LDR).
- buzzerPin: pino D3, onde está conectado o buzzer (atuador sonoro).
- brightnessLimit: limite de luminosidade para acionar o buzzer em níveis elevados de luz.
- normalLimit: limite de luminosidade para normalizar a condição do sistema, desativando o buzzer.


**3. Objetos MQTT:**
- São criados objetos para o cliente Wi-Fi e o cliente MQTT.
- As variáveis lightDataFeedName e lightStatusFeedName definem os tópicos para onde os dados serão enviados no broker MQTT.

***No Código:***
- WiFiClient wifiClient: responsável pela conexão com a rede Wi-Fi.
- Adafruit_MQTT_Client mqtt: representa o cliente MQTT que gerencia a comunicação com o servidor MQTT, utilizando as credenciais passadas.
- lightDataFeedName e lightStatusFeedName: variáveis para armazenar os tópicos onde os dados serão publicados no servidor MQTT.
- lightLevelFeed: objeto MQTT para publicar os valores do sensor de luminosidade.
- lightStatusFeed: objeto MQTT para publicar o status da luminosidade (normal ou alta).


**4. Flags e Controle de Tempo:**
- A variável excessiveBrightness é usada para rastrear se o brilho está acima do limite.
- A variável previousLightMillis é usada para controlar o intervalo de tempo para o envio do status de luminosidade normal.

***No Código:***
- excessiveBrightness: variável de controle para indicar se a luminosidade está acima do limite e o buzzer foi ativado.
- previousLightMillis: armazena o tempo da última vez que o status da luz foi publicado.
- intervalLightNormal: intervalo para verificar o status de luz normal e publicar novamente.
  

**5. Funções de Inicialização:**
- Conexão Wi-Fi:
A função initializeWiFi() tenta conectar o ESP8266 à rede Wi-Fi usando o SSID e a senha definidos. Enquanto não houver conexão, o código exibe um sinal "-" no console.

- Conexão MQTT:
A função initializeMQTT() tenta conectar o cliente MQTT ao servidor. Caso a conexão falhe, ele tentará novamente após um atraso de 5 segundos.

**6. Função Setup:**
- Setup(), o código inicializa a comunicação serial e os pinos para o buzzer.
- Os nomes dos feeds MQTT são configurados para incluir o nome de usuário e as funções de inicialização Wi-Fi e MQTT são chamadas.

***No Código:***
- Serial.begin(115200): inicializa a comunicação serial a 115200 bps para monitoramento do projeto.
- pinMode(buzzerPin, OUTPUT): configura o pino do buzzer como saída.
- digitalWrite(buzzerPin, LOW): desativa o buzzer.
- snprintf(): cria os tópicos de publicação com base no nome de usuário MQTT.


**7. Função Loop:**
- Loop() executa as tarefas principais de leitura do sensor de luminosidade, envio de dados via MQTT, controle do buzzer e verificação de status. Se reconecta automaticamente ao servidor MQTT se a conexão for perdida.
- O status do brilho (alto ou normal) é enviado periodicamente.

***No Código:***
- mqtt.connected(): verifica se o ESP8266 está conectado ao servidor MQTT. Caso contrário, tenta reconectar.
- analogRead(sensorPin): lê o valor analógico do sensor de luminosidade.
- reversedReading = 1000 - lightReading: inverte o valor da leitura para corrigir o comportamento do sensor de luminosidade.
- lightLevelFeed.publish(reversedReading): publica a leitura do sensor no tópico MQTT.
- lightReading < brightnessLimit: se a leitura de luz estiver abaixo do limite de brilho excessivo, ativa o buzzer e publica o status "Nível Alto".
- lightReading > normalLimit: se a leitura de luz estiver acima do limite normal e o buzzer estiver ativado, desativa o buzzer e publica "Nível Normal".
- millis(): usa o tempo atual para garantir que o status de "Nível Normal" seja publicado a cada 5 segundos.
- mqtt.processPackets(10): processa pacotes de comunicação MQTT.
- delay(5000): aguarda 5 segundos antes de fazer a próxima leitura.


### Hardware Utilizado:
- **Plataforma de desenvolvimento:** NodeMCU ESP8266.
- **Sensor de Luz:** LDR (fotoresistor).
- **Atuador:** Buzzer.
- **Outros componentes:**
  - Protoboard.
  - Fios de conexão.
  - Fonte de alimentação para o NodeMCU.


### Fluxograma:
-![image](https://github.com/user-attachments/assets/99541bc3-649f-4726-a22d-2098c91ffc88)


### Circuito:
![image](https://github.com/user-attachments/assets/d048202c-cf6d-4ede-b352-0ed409661a4b)


### Comunicação MQTT:
- O projeto utiliza o protocolo MQTT para enviar dados de luminosidade e status de brilho.
- **Tópicos utilizados:**
  - `light-data`: Para enviar os valores de luminosidade do sensor.
  - `light-status`: Para enviar o status de brilho (Nível Alto ou Nível Normal).


### Configuração do Broker MQTT:
- O broker MQTT pode ser configurado para receber dados em tempo real.


### Interfaces e Protocolos de Comunicação:
Protocolo/Interface: MQTT (Message Queuing Telemetry Transport).
Função: Comunicação de dados.
Detalhes: Utilizado para enviar os dados do sensor de luminosidade e o status de brilho. O ESP8266 publica mensagens em tópicos MQTT que podem ser lidas por outros dispositivos ou sistemas.

Protocolo/Interface: TCP/IP (Transmission Control Protocol/Internet Protocol).
Função: Comunicação via rede.
Detalhes: Utilizado para estabelecer a conexão do ESP8266 com a rede Wi-Fi e, em seguida, com o broker MQTT. Permite a troca de mensagens via internet entre o dispositivo e o broker MQTT.

Protocolo/Interface: Wi-Fi.
Função: Interface de rede local.
Detalhes:O ESP8266 utiliza o Wi-Fi como interface para acessar a internet e se comunicar com o broker MQTT via TCP/IP.


### Protocolo MQTT e Comunicação TCP/IP:
O ESP8266 utiliza o protocolo MQTT para se comunicar com um broker MQTT e enviar os dados do sensor de luminosidade e o status (alto ou normal) da luz. O código que você forneceu usa a biblioteca Adafruit_MQTT para implementar a comunicação MQTT no ESP8266. No código, o dispositivo se conecta a um broker MQTT e publica dados em tópicos específicos para monitoramento remoto.

O ESP8266 se conecta à rede Wi-Fi usando o protocolo TCP/IP, estabelecendo uma comunicação através da rede.
Uma vez conectado ao Wi-Fi, o ESP8266 utiliza o protocolo TCP/IP para se conectar ao broker MQTT e enviar as mensagens.

Uma vez conectado, as mensagens MQTT são transmitidas entre o ESP8266 e o broker MQTT via TCP/IP, permitindo que o dispositivo envie os dados do sensor e receba comandos ou informações de outros dispositivos (caso configurado).


## Artigo:
[A4 - Objetos.pdf](https://github.com/user-attachments/files/17822949/A4.-.Objetos.pdf)


## Link do vídeo:
https://www.youtube.com/watch?v=GvMV1MD0YHQ&feature=youtu.be
