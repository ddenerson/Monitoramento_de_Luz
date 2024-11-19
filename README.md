# Sistema de Monitoramento de Luz

### Introdução
Este projeto utiliza um sensor LDR (fotoresistor) para monitorar a intensidade de luz e acionar o buzzer quando os níveis de luminosidade estiverem acima do limite determinado. O sistema publica os dados de luminosidade e status de brilho em tempo real usando o protocolo MQTT.



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
As bibliotecas são necessárias para a comunicação com o Wi-Fi e o broker MQTT. O ESP8266 será usado para se conectar a uma rede sem fio e enviar dados para um broker MQTT

**2. Definição de Variáveis e Parâmetros**
Configuradas as credenciais de Wi-Fi e as informações de conexão do MQTT (endereço, porta, usuário e chave). Também são definidos os pinos para o sensor de luminosidade (que usa leitura analógica) e o buzzer, além dos limites de luminosidade.

**3. Objetos MQTT**
Aqui são criados objetos para o cliente Wi-Fi e o cliente MQTT. As variáveis lightDataFeedName e lightStatusFeedName definem os tópicos para onde os dados serão enviados no broker MQTT.

**4. Flags e Controle de Tempo**
A variável excessiveBrightness é usada para rastrear se o brilho está acima do limite. A variável previousLightMillis é usada para controlar o intervalo de tempo para o envio do status de luminosidade normal.

**5. Funções de Inicialização**
- Conexão Wi-Fi:
A função initializeWiFi() tenta conectar o ESP8266 à rede Wi-Fi usando o SSID e a senha definidos. Enquanto não houver conexão, o código exibe um sinal "-" no console.

- Conexão MQTT:
A função initializeMQTT() tenta conectar o cliente MQTT ao servidor. Caso a conexão falhe, ele tentará novamente após um atraso de 5 segundos.

**6. Função Setup:**
No setup(), o código inicializa a comunicação serial e os pinos para o buzzer. Os nomes dos feeds MQTT são configurados para incluir o nome de usuário. Em seguida, as funções de inicialização Wi-Fi e MQTT são chamadas.

**7. Função Loop:**
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
