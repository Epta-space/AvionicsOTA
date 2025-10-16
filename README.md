# EPTA\_OTA - Firmware de Atualização Over-the-Air para ESP32

## 🎯 Sobre o Projeto

**EPTA\_OTA** é um firmware robusto para a plataforma ESP32, projetado para facilitar atualizações de software remotamente através de Wi-Fi, uma técnica conhecida como **Over-The-Air (OTA)**.

O projeto resolve um problema crucial em dispositivos embarcados: a necessidade de atualizar o firmware sem acesso físico. Isso é ideal para equipamentos instalados em locais de difícil acesso ou para gerenciar uma frota de dispositivos de forma centralizada.

Além da funcionalidade de OTA, o firmware utiliza o FreeRTOS para criar duas tarefas distintas que rodam em *cores* separados do processador, garantindo que o processo de atualização não interfira na aplicação principal.

### ✨ Funcionalidades Principais

  * 📡 **Atualização Over-the-Air (OTA):** Atualize o firmware do ESP32 via Wi-Fi, sem a necessidade de conexão por cabo. O serviço OTA aguarda por um novo firmware na porta `3232`.
  * ⚙️ **Operação Dual-Core:** O projeto utiliza o FreeRTOS para rodar a lógica de OTA e a aplicação principal em *cores* diferentes do ESP32, garantindo alta disponibilidade e performance.
  * 📄 **Logging Remoto com Syslog:** Todas as operações, desde a conexão Wi-Fi até o progresso da atualização, são enviadas para um servidor Syslog. Isso permite monitorar e depurar o dispositivo remotamente.
  * 🛰️ **Task de Aviônica Simulado:** Como exemplo de aplicação principal, uma tarefa de "aviônica" envia dados de telemetria (altitude e velocidade) para o servidor Syslog a cada 2 segundos.
  * 🌐 **Configuração de Rede Estática:** O dispositivo é configurado com um endereço IP fixo (`192.168.1.123`) para facilitar o acesso e a automação na rede local.

## 🛠️ Tecnologias e Ferramentas

Este projeto foi construído com as seguintes tecnologias:

  * **Hardware:** Placa de desenvolvimento genérica ESP32 (`esp32dev`).
  * **Framework:** **Arduino** sobre o ESP-IDF.
  * **Sistema Operacional:** **FreeRTOS** para gerenciamento de tarefas.
  * **Linguagem:** C++.
  * **Build System:** **PlatformIO**.
  * **Bibliotecas:**
      * `WiFi` e `ArduinoOTA` para a conectividade e atualização.
      * `Syslog` para logging remoto.
  * **Scripts de Automação:** Python 3 para monitoramento de logs.

## 🚀 Como Começar

Siga os passos abaixo para compilar, enviar o firmware e testar a funcionalidade de OTA.

### Pré-requisitos

  * **Hardware:** Uma placa de desenvolvimento baseada no ESP32.
  * **Software:**
      * [Visual Studio Code](https://code.visualstudio.com/) com a extensão [PlatformIO IDE](https://platformio.org/install/ide?install=vscode).
      * [Python 3.8+](https://www.python.org/downloads/) para executar o script de monitoramento.
      * Um servidor Syslog (o projeto já inclui um script para isso).

### ⚙️ Configuração do Ambiente

1.  **Clone o repositório:**

    ```bash
    git clone git@github.com:Epta-space/AvionicsOTA.git
    cd AvionicsOTA
    ```

2.  **Configure suas credenciais de rede:**
    Abra o arquivo `src/main.cpp` e altere as seguintes variáveis com os dados da sua rede Wi-Fi e do IP da máquina onde o script de log irá rodar:

    ```cpp
    // Em src/main.cpp
    const char* WIFI_SSID     = "SUA_REDE_WIFI";
    const char* WIFI_PASSWORD = "SUA_SENHA_WIFI";
    const char* SYSLOG_SERVER = "IP_DA_SUA_MAQUINA"; // Ex: "192.168.1.8"
    ```

### 📦 Compilação e Primeiro Upload

1.  **Conecte o ESP32** ao seu computador via USB.
2.  Abra o projeto no VS Code com PlatformIO.
3.  Compile e faça o upload pela primeira vez usando o cabo USB. Você pode fazer isso pela interface do PlatformIO (`PlatformIO: Upload`) ou pelo terminal:
    ```bash
    pio run --target upload
    ```

### 🛰️ Monitoramento e Atualização OTA

Com o firmware inicial gravado, você pode monitorar o dispositivo e enviar novas atualizações pela rede.

1.  **Monitore os logs:**
    Em um terminal, execute o servidor Syslog para receber as mensagens de telemetria e status do ESP32:

    ```bash
    python3 scripts/watch_logs.py
    ```

    Você deverá ver mensagens de conexão e os dados da "aviônica" chegando a cada 2 segundos.

2.  **Envie uma atualização OTA:**
    Faça uma alteração no código (por exemplo, mude a frequência de envio de telemetria no `vTaskDelay` da `taskAvionica`).
    Para enviar o novo firmware pela rede, execute o seguinte comando no terminal do PlatformIO:

    ```bash
    pio run --target upload --upload-port 192.168.1.123
    ```

    O PlatformIO irá compilar o projeto e enviar o binário diretamente para o IP do ESP32. Acompanhe o progresso da atualização no terminal onde o `watch_logs.py` está rodando\!

## 📂 Estrutura do Projeto

```
EPTA_OTA/
├── .pio/            # Arquivos de build do PlatformIO (ignorado)
├── build/           # Arquivos de build do CMake (ignorado)
├── include/         # Headers (se necessário)
├── lib/             # Bibliotecas locais do projeto
├── scripts/         # Scripts de automação
│   └── watch_logs.py# Servidor Syslog para visualizar logs remotamente
├── src/             # Código-fonte principal
│   └── main.cpp     # Lógica principal, setup do Wi-Fi, OTA e tasks
├── .gitignore       # Arquivos e pastas a serem ignorados pelo Git
├── CMakeLists.txt   # Arquivo de configuração do projeto para CMake/ESP-IDF
└── platformio.ini   # Arquivo de configuração do PlatformIO
```

## 🤝 Como Contribuir

Contribuições são o que tornam a comunidade de código aberto um lugar incrível para aprender, inspirar e criar. Qualquer contribuição que você fizer será **muito apreciada**.

1.  Faça um *Fork* do projeto
2.  Crie uma *Branch* para sua feature (`git checkout -b feature/AmazingFeature`)
3.  Faça o *Commit* de suas alterações (`git commit -m 'Add some AmazingFeature'`)
4.  Faça o *Push* para a Branch (`git push origin feature/AmazingFeature`)
5.  Abra um *Pull Request*

## 📜 Licença

Distribuído sob a licença MIT. Veja `LICENSE` para mais informações.