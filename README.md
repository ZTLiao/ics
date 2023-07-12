# 南京大学`操作系统基础`练习题
# ICS2020 Programming Assignment

This project is the programming assignment of the class ICS(Introduction to Computer System)
in Department of Computer Science and Technology, Nanjing University.

For the guide of this programming assignment,
refer to http://nju-ics.gitbooks.io/ics2020-programming-assignment/content/

To initialize, run
```bash
bash init.sh subproject-name
```
See `init.sh` for more details.

The following subprojects/components are included. Some of them are not fully implemented.
* [NEMU](https://github.com/NJU-ProjectN/nemu)
* [Abstract-Machine](https://github.com/NJU-ProjectN/abstract-machine)
* [Nanos-lite](https://github.com/NJU-ProjectN/nanos-lite)
* [Navy-apps](https://github.com/NJU-ProjectN/navy-apps)

nemu
make ISA=x86 run

fceux-am
make ARCH=x86-nemu run mainargs=mario
make ARCH=native run mainargs=mario

am-tests
make ARCH=native run mainargs=p

cpu-tests
make ARCH=x86-nemu ALL=dummry

difftest
make ISA=x86 run DIFF=qemu NEMUARGS="--port=8888"
qemu-system-i386 -S -gdb tcp::8888 -nographic
