# c-weather-broadcast

## Description
This project implements a simple UDP broadcast system in C.
The server periodically broadcasts weather information, and clients receive it.

## Technologies
- C
- UDP sockets
- Broadcast
- POSIX networking

## How it works
- The server sends weather data every 5 seconds using UDP broadcast.
- Clients listen on the specified port and receive weather updates.

## Purpose
Educational project to practice socket programming and network communication.
