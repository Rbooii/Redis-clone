# Redis Clone

A small in-memory key-value store built from scratch in C++, inspired by Redis. This is a learning project focused on understanding how a database like Redis actually works under the hood: storage, hashing, networking, and the event loop that ties it all together.

## Why this exists

I wanted to go past "using" Redis and actually build something that behaves like it. The goal isn't to replace Redis, it's to understand the pieces: how key-value storage works with a hash map, how TCP sockets accept and read client connections, and how a single-threaded event loop can handle many clients without spawning a thread per connection (the same core idea Redis itself uses).

## Current state

Right now the project has a working `Store` class that supports:

- `set(key, value)` to store a value under a key
- `get(key)` to retrieve it, returning `std::optional<std::string>` so a missing key is never confused with an empty string
- `del(key)` to remove a key, returning whether it actually existed

Networking hasn't been added yet. That's the next step: wrapping raw sockets in a `Server` class and building a small event loop so multiple clients can connect and issue commands.

## Requirements

- A C++ compiler with C++17 support or newer (this project targets C++20). `clang++` works fine.
- CMake 3.20 or newer

On macOS, both can be installed with Homebrew:

```bash
brew install cmake
```

## Building and running

```bash
cmake -B build
cmake --build build
./build/redis-clone
```

- `cmake -B build` generates the build files. You only need to rerun this if `CMakeLists.txt` changes.
- `cmake --build build` compiles the project. Run this every time you change the code.
- `./build/redis-clone` runs the compiled binary.

## Roadmap

- [x] In-memory `Store` class with set/get/del
- [ ] TCP server using raw sockets
- [ ] Event loop for handling multiple clients
- [ ] Basic command protocol (parsing text commands from clients)
- [ ] TTL / key expiry
- [ ] Simple persistence (snapshot to disk)

## Notes

This project follows along loosely with [build-your-own.org/redis](https://build-your-own.org/redis/), adapted from C into a class-based C++ design.