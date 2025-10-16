#!/usr/bin/env python3
import socket
from datetime import datetime

# =================== CONFIGURAÇÕES ===================
UDP_IP = "0.0.0.0"   # Escuta todas as interfaces
UDP_PORT = 5514       # Porta padrão Syslog (use a mesma do ESP32)

# =====================================================
print(f"🟢 Servidor Syslog escutando em {UDP_IP}:{UDP_PORT} ...")

# Cria o socket UDP
sock = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
sock.bind((UDP_IP, UDP_PORT))

try:
    while True:
        data, addr = sock.recvfrom(1024)
        message = data.decode(errors="ignore").strip()
        timestamp = datetime.now().strftime("%Y-%m-%d %H:%M:%S")

        print(f"[{timestamp}] {addr[0]}: {message}")

except KeyboardInterrupt:
    print("\n🟡 Encerrando servidor Syslog.")
finally:
    sock.close()
