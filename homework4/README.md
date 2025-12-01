Multi-Client Chat System
========================

Proyecto de un sistema de chat cliente-servidor usando IPC en Linux (sockets UNIX + message queues).
Permite múltiples clientes conectados al mismo tiempo, soporte de mensajes privados, broadcast,
lista de usuarios y comandos administrativos.

------------------------------------------------------------
Características del Servidor (chat_server.c)
------------------------------------------------------------
- Acepta múltiples clientes concurrentes.
- Mantiene una lista de usuarios activos.
- Soporta mensajes públicos y privados.
- Detecta desconexiones y limpia recursos.
- Comandos disponibles:
  /list
  /stats
  /kick <usuario>
  /broadcast <mensaje>
  /shutdown

------------------------------------------------------------
Características del Cliente (chat_client.c)
------------------------------------------------------------
- Conecta al servidor mediante UNIX Domain Sockets.
- Solicita nombre de usuario.
- Envía mensajes públicos y privados.
- Permite ver usuarios conectados.
- Comandos disponibles:
  /help
  /users
  /msg <usuario> <mensaje>
  /quit

------------------------------------------------------------
##IPC Implementado
------------------------------------------------------------
1. UNIX Domain Sockets
   - Conexión principal cliente ⇾ servidor.

2. POSIX Message Queues
   - Manejo interno de mensajes estructurados.

------------------------------------------------------------
##Estructura del Proyecto
------------------------------------------------------------
```
homework4/
├── src/
│   ├── chat_server.c
│   ├── chat_client.c
│   ├── ipc_sockets.c
│   ├── ipc_mq.c
│   ├── message.c
│   └── utils.c
├── include/
│   ├── chat.h
│   ├── message.h
│   └── ipc.h
├── tests/
│   ├── test_server.sh
│   ├── test_client.sh
│   ├── test_stress.sh
│   └── test_disconnect.sh
├── docs/
│   ├── PROTOCOL.md
│   ├── ARCHITECTURE.md
│   └── TESTING.md
├── Makefile
└── README.txt
```
------------------------------------------------------------
##Compilación
------------------------------------------------------------
Ejecutar:
```
make
```
Para limpiar y recompilar:
```
make clean
```
