> This project has been created as part of the 42 curriculum by hmnasfa, aammisse, mouerchi.

## Description

This project consists of implementing a fully functional IRC server in C++98. Its objective is to develop an understanding of network programming, TCP sockets, and event-driven I/O while following the IRC protocol defined by the RFC specifications.

The server supports multiple simultaneous clients, channel management, operator privileges, and the core IRC commands required by the project subject.

## Build

Compile the project using:

```bash
make
```

## Usage

Run the server with:

```bash
./ircserv <port> <password>
```

Example:

```bash
./ircserv 6667 secret
```

## Connecting to the Server

You can connect using **netcat** or **telnet** or any IRC client:

```bash
nc -C localhost <port>
```

```bash
telnet localhost <port>
```

## Supported Commands

- PASS
- NICK
- USER
- JOIN
- PRIVMSG
- INVITE
- TOPIC
- MODE
- KICK
- QUIT

## Channel Modes

`-/+` => Add or Remove a channel mode.
 
`i` => Invite-only channel.
`t` => Restrict topic changes to channel operators.
`k` => Set or remove a channel password.
`o` => Grant or remove operator privileges.
`l` => Set or remove the channel user limit.

## References

- RFC 1459 — Internet Relay Chat Protocol
- RFC 2812 — Internet Relay Chat: Client Protocol
- ircdocs.horse — Modern IRC protocol documentation and implementation guide

## AI Usage

AI was used as a learning and documentation assistant to:

- Understand network programming concepts.
- Clarify the behavior of socket-related system calls (`socket`, `bind`, `listen`, `accept`, `recv`, `send`, `epoll`, etc.).
- Explain RFC specifications and IRC protocol behavior.
- Assist with debugging and interpreting error codes.
- Reduced friction by implementing function descriptions, allowing teammates to understand the logic at a glance.