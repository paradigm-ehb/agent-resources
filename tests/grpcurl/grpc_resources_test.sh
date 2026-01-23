# Linux System Resources Gathering Tool

A low-level **Linux system resource gathering tool** written in **C99**, developed for **paradigm-ehb**.

This tool collects detailed system information by directly interfacing with the Linux kernel through:

- Virtual filesystem interfaces such as **`/proc`** and **`/sys`**
- Select **syscalls** where file-based introspection is insufficient

The design prioritizes **performance**, **predictability**, and **explicit memory control**, avoiding unnecessary abstractions and dependencies.

---

## Features

- Reads CPU, memory, disk, and process information via:
  - `/proc`
  - `/sys`
- Executes syscalls for system data not exposed through virtual files
- Explicit memory management using **arena allocators**
- Suitable for:
  - system agents
  - monitoring daemons
  - resource snapshots
  - low-overhead telemetry
- Package manager wrapping for dnf ( more package managers are on the todo list for this project)

---

## Stack

- **C99** 
- **C++20** ( requiered for libdn5 )
---

## Platform Support

- **Linux only**
- Requires:
  - `/proc` filesystem
  - `/sys` filesystem
- Tested on modern Linux distributions

---

## Build Requirements

- C compiler with C99 support (e.g. `gcc`, `clang`)
- Linux system headers
- Optional (depending on build targets):
  - `libdnf5` (DNF-based systems)

---

## Credits & References

The implementation and design are informed by the following resources:

### Memory Allocation & Arenas
- Ginger Bill — *Memory Allocation Strategies*  
  https://www.gingerbill.org/series/memory-allocation-strategies/
- Ryan Fleury — *Untangling Lifetimes: The Arena Allocator*  
  https://www.rfleury.com/p/untangling-lifetimes-the-arena-allocator
- Epic Games — *raddebugger arena implementation*  
  https://github.com/EpicGamesExt/raddebugger/blob/master/src/base/base_arena.c

### Talks & Articles
- Ginger Bill — *Practical Data-Oriented Design* (YouTube)  
  https://www.youtube.com/watch?v=jgiMagdjA1s
- GeeksForGeeks — *Inline functions in C*  
  https://www.geeksforgeeks.org/c/inline-function-in-c/

### System Libraries & APIs
- libdnf5 documentation  
  https://libdnf.readthedocs.io/en/dnf-5-devel/tutorial/install-build-deps.html
- libalpm manual  
  https://man.archlinux.org/man/libalpm.3#Topics

---

## License

See the `LICENSE` file for details.
