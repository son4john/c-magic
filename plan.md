# Building a Small OS in C: Learning Plan

## Goal and decisions

The goal is to learn C by building a small operating system that boots, runs programs from disk and gives you a shell prompt.

- **Target: 64-bit RISC-V on QEMU's `virt` machine.** The development Mac is Apple Silicon, so x86 would run under slower emulation and bring decades of legacy boot steps. RISC-V is simple, and QEMU's firmware (OpenSBI) handles early boot, so you reach C almost immediately. The best beginner material also targets it.
- **You write the code.** The goal is learning C, so Claude explains concepts, reviews your work and helps debug rather than writing it all for you.
- **C comes first.** A kernel has no `printf`, no `malloc` and often no error message when it crashes, so it is a hard place to learn the basics. Phase 0 runs on your Mac, where you have proper tools.

| Phase | Focus | Covers | Runs on |
| --- | --- | --- | --- |
| 0 | C foundations | Strings, malloc, a small shell | Your Mac |
| 1 | Kernel basics | Boots, prints, reports crashes | QEMU on 64-bit RISC-V |
| 2 | Memory and time | Pages, timer, virtual memory | QEMU on 64-bit RISC-V |
| 3 | Processes | Threads, user mode, syscalls | QEMU on 64-bit RISC-V |
| 4 | A real system | Disk, files, your shell | QEMU on 64-bit RISC-V |

Each phase builds on the one before it, and only Phase 0 runs directly on your Mac.

## Phase 0: C foundations on your Mac

Build these as normal programs; several come back later as kernel code. If you already know pointers and `malloc`, skip to Phase 1.

| Exercise | What you learn |
| --- | --- |
| `strlen`, `memcpy`, `strcmp` | Pointers, arrays, strings |
| Your own `printf` | Varargs, number formatting |
| Linked list and hash table | Structs, heap memory, `void *` |
| Your own `malloc`/`free` | Memory layout, alignment, casting |
| Small shell using `fork`/`exec` | How programs use an OS |

Compile with `-Wall -Wextra -fsanitize=address,undefined`, which catches many memory bugs that a kernel would hide.

## Phase 1: Kernel basics

This phase ends with a kernel that prints text and reports its own crashes.

1. **Boot:** a little assembly sets up the stack and jumps to `kernel_main()`. You also write a linker script.
2. **Hello world:** write characters to the UART, a device controlled through memory addresses. This teaches `volatile` and bitwise operations.
3. **Kernel `printf`:** port your Phase 0 version, which becomes your main debugging tool.
4. **Traps and panic:** catch exceptions and print the cause and address instead of hanging silently.

## Phase 2: Memory and time

This phase gives the kernel managed memory and a regular clock tick. Virtual memory is usually the hardest step in the whole plan.

5. **Page allocator:** hand out 4 KB pages from a free list.
6. **Timer interrupts:** get a regular tick.
7. **Virtual memory:** build page tables with Sv39 and turn on the MMU.
8. **Kernel `malloc`:** port your allocator on top of the page allocator.

## Phase 3: Processes

This phase ends with user programs running in their own address spaces and calling into the kernel.

9. **Context switching:** run two kernel threads, using a small amount of assembly.
10. **Scheduler:** round-robin, driven by the timer.
11. **User mode:** run a program in its own address space.
12. **System calls:** start with `write`, `exit` and `getpid`.

## Phase 4: A real system

The finish line is booting your OS, getting a prompt and running programs from disk. After that, you could try pipes, multiple CPUs or networking.

13. **Disk driver:** virtio block device.
14. **File system:** a simple one, with `open`, `read`, `write` and `close`.
15. **Program loading:** an ELF loader, then `fork`, `exec` and `wait`.
16. **Your shell:** port the Phase 0 shell.

## Repo layout and tools

This layout is still to be created:

```
c-magic/
├── exercises/    # Phase 0
├── kernel/       # kernel C and assembly, linker script
├── user/         # user programs, including the shell
├── Makefile      # `make run`, `make debug`
└── docs/notes.md # what you learned at each step
```

Apple's clang covers Phase 0. As of the last check, none of the kernel tools were installed, and this adds them for Phase 1:

```sh
brew install qemu riscv64-elf-gcc riscv64-elf-binutils riscv64-elf-gdb
```

QEMU and the GDB debugger are the most useful pair here: you can step through your kernel one instruction at a time.

## Resources

These are recommended from memory and were not looked up for this plan.

| Use | Resource |
| --- | --- |
| Learning C | *Beej's Guide to C Programming* (free), or *The C Programming Language* (K&R) |
| Early kernel steps | *Operating System in 1000 Lines*, which also targets RISC-V |
| Reference OS | MIT's *xv6-riscv* and its book; read it after trying each piece yourself, not before |
| Theory | *Operating Systems: Three Easy Pieces* (free) |
