# IRC Server

A lightweight IRC (Internet Relay Chat) server implementation written in C++ using epoll for efficient event handling.

## Overview

This is an IRC server that handles multiple client connections simultaneously using Linux epoll for non-blocking I/O. The server manages client authentication, command processing, and message routing between clients.

## Features

- **Non-blocking I/O**: Uses epoll for efficient handling of multiple concurrent connections
- **Client Authentication**: Implements IRC password-based authentication with PASS, NICK, and USER commands
- **Command Handling**: Extensible command system with support for IRC protocol commands
- **Channel Support**: Allows clients to join channels and exchange messages
- **Dynamic Client Management**: Add/remove clients dynamically, track connection state

## Project Structure

```
IRC-Server/
├── Makefile                 # Build configuration
├── README.md               # This file
├── includes/               # Header files
│   ├── Client.hpp         # Client class definition
│   ├── Command.hpp        # Command base class
│   ├── CommandManager.hpp # Command dispatch system
│   └── Server.hpp         # Server class definition
└── src/
    ├── main.cpp               # Entry point
    ├── Server.cpp             # Server implementation
    ├── Client.cpp             # Client implementation
    ├── Command.cpp            # Command base class implementation
    ├── CommandManager.cpp     # Command dispatch logic
    └── Command/               # IRC command implementations
        ├── executeCommand.cpp # Command execution dispatcher
        ├── Nick.cpp          # NICK command (set nickname)
        ├── Pass.cpp          # PASS command (authenticate)
        ├── Privmsg.cpp       # PRIVMSG command (send messages)
        ├── Quit.cpp          # QUIT command (disconnect)
        └── User.cpp          # USER command (set user info)
```

## Building and Running

### Prerequisites

- C++ compiler with C++98 or later support (gcc/clang)
- Linux system with epoll support
- Make build system

### Compilation

```bash
make              # Compile the project
make clean        # Remove object files
make fclean       # Remove all build artifacts
make re           # Clean rebuild
```

### Running the Server

```bash
./ircserv <port> <password>
```

**Parameters:**
- `<port>`: TCP port number for the server to listen on
- `<password>`: Password required for client authentication

**Example:**
```bash
./ircserv 6667 mypassword
```

The server will start listening for incoming connections on the specified port.

## Architecture

### Core Components

#### Server (Server.hpp/Server.cpp)
- Manages the listening socket and epoll event loop
- Maintains a map of connected clients indexed by file descriptor
- Handles client connection and disconnection
- Distributes events to connected clients

#### Client (Client.hpp/Client.cpp)
- Represents a connected IRC client
- Tracks authentication state:
  - `AwaitPass`: Waiting for password authentication
  - `AwaitNickUser`: Waiting for NICK and USER commands
  - `Registered`: Fully authenticated and ready for commands
- Manages client data (nickname, username, hostname, etc.)
- Buffers incoming data for command parsing

#### Command System (Command.hpp/Command.cpp)
- Abstract base class for all IRC commands
- Base interface for command execution

#### CommandManager (CommandManager.hpp/CommandManager.cpp)
- Dispatches incoming commands to appropriate handlers
- Maintains channel membership information
- Coordinates between command handlers and the server

### Authentication Flow

1. Client connects → `AwaitPass` state
2. Client sends `PASS <password>` → Verified against server password
3. Client sends `NICK <nickname>` and `USER <username> ...`
4. Upon receiving both → Transitions to `Registered` state
5. Client can now execute IRC commands

## Implemented Commands

- **PASS**: Authenticate with server password
- **NICK**: Set or change client nickname
- **USER**: Provide user information during registration
- **PRIVMSG**: Send private messages to clients or channels
- **QUIT**: Disconnect gracefully from the server

## Design Notes

- **Include Guards**: Each header file uses unique include guard macros (e.g., `SERVER_HPP`, `CLIENT_HPP`, `COMMAND_HPP`, `COMMANDMANAGER_HPP`) to prevent header conflicts
- **Command Dispatch**: Centralized in CommandManager; Server maintains client and socket state
- **Channel Management**: Implemented within CommandManager with internal channel membership tracking
- **Event Handling**: Non-blocking epoll loop allows handling thousands of concurrent connections

## Future Enhancements

- MODE command for user and channel mode changes
- WHO/WHOIS commands for user information queries
- Additional channel operations (KICK, BAN, etc.)
- SSL/TLS support for encrypted connections
- More comprehensive IRC protocol compliance

## Dependencies

- `<vector>`, `<map>`, `<string>`: Standard library containers
- `<netinet/in.h>`: Network socket structures
- `<sys/epoll.h>`: Epoll event handling
- `<unistd.h>`: Unix standard functions
- `<fcntl.h>`: File control operations

## License

This project is provided as-is for educational and development purposes.
