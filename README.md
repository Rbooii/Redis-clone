# Redis Clone

A small in-memory key-value store built from scratch in C, inspired by Redis. This is a learning project focused on understanding how a database like Redis actually works under the hood: storage, hashing, networking, and the event loop that ties it all together.

## Why this exists

I wanted to go past "using" Redis and actually build something that behaves like it. The goal isn't to replace Redis, it's to understand the pieces: how key-value storage works with a hash map, how TCP sockets accept and read client connections, and how a single-threaded event loop can handle many clients without spawning a thread per connection (the same core idea Redis itself uses).

The project started in C++ as a way to practice OOP while learning networking, but the guide this project follows ([build-your-own.org/redis](https://build-your-own.org/redis/)) is written in C, and translating every chapter added overhead that got in the way of actually learning the material. So the project moved to plain C to follow the guide more directly. The original C++ version (a `Store` class with set/get/del, plus early raw socket experiments) is kept in `legacy_cpp/` for reference.

## Current state

Working on the request-response protocol chapter: reading and writing length-prefixed messages over a TCP connection (a 4-byte length header followed by the payload), so the server always knows exactly how many bytes to read instead of guessing.

Before this, a basic blocking TCP server was built that accepts one connection at a time using raw sockets (`socket`, `bind`, `listen`, `accept`).

The `legacy_cpp` files aren't compiled by default, but they're still tracked by CMake so the editor's language server doesn't flag them with errors.

## Requirements

- A C compiler with C11 support or newer. `clang` works fine.
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

If you want to recompile the old C++ prototype for reference:

```bash
cmake --build build --target legacy-server-test
```

## Roadmap

- [x] In-memory key-value store with set/get/del (originally C++, later reimplemented in C)
- [ ] TCP server using raw sockets
- [ ] Request-response protocol (length-prefixed messages)
- [ ] Event loop for handling multiple clients
- [ ] Basic command protocol (parsing text commands from clients and rewrite c++ -> c)
- [ ] TTL / key expiry
- [ ] Simple persistence (snapshot to disk)
