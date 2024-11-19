# Sistema de Monitoramento de Luz

### Introdução
Este projeto utiliza um sensor LDR (fotoresistor) para monitorar a intensidade de luz e acionar um buzzer quando os níveis de luminosidade estiverem acima de um determinado limite. O sistema também publica os dados de luminosidade e status de brilho em tempo real usando o protocolo MQTT.


### Funcionalidades
- Monitoramento da intensidade de luz com sensor LDR.
- Acionamento do buzzer em caso de luminosidade excessiva.
- Comunicação em tempo real via protocolo MQTT com um broker.
- Dados publicados em tópicos MQTT para visualização remota.


## Requisitos:
- Placa de desenvolvimento: NodeMCU ESP8266.
- Sensor LDR.
- Buzzer.
- Broker MQTT


## Instruções de Uso:
1. Conecte o NodeMCU à sua rede Wi-Fi.
2. Configure o broker MQTT no código.
3. Carregue o código no NodeMCU e execute.
  

### Código e Documentação


## Hardware Utilizado:
- **Plataforma de desenvolvimento:** NodeMCU ESP8266.
- **Sensor de Luz:** LDR (fotoresistor).
- **Atuador:** Buzzer.
- **Outros componentes:**
  - Protoboard.
  - Fios de conexão.
  - Fonte de alimentação para o NodeMCU.


## Diagrama de Circuito:
-![image](https://github.com/user-attachments/assets/99541bc3-649f-4726-a22d-2098c91ffc88)


## Comunicação MQTT:
- O projeto utiliza o protocolo MQTT para enviar dados de luminosidade e status de brilho.
- **Tópicos utilizados:**
  - `light-data`: Para enviar os valores de luminosidade do sensor.
  - `light-status`: Para enviar o status de brilho (Nível Alto ou Nível Normal).


## Configuração do Broker MQTT:
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
