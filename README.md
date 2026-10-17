
![Logo](https://i.imgur.com/Zg37VpH.png)
# SpecSeek v5. All in One Specification Tool by xhcisucks

A soloution to finding everything there is to know about a computers specifications. all in one elegant location. **a great way to share your device specifications support and features**. not sure if your PC supports some functionality? SpecSeek will relay that to you.

## Contributing
Contributions in any way are always welcome, if its porting to your own operating system or making a PR for me to review, I welcome all sorts of contributions and intergration. when porting I would appreciate it if your build script pulls directly from this repo to keep your build of specseek up to date as its still in active development.

**When contributing please try follow the naming standard used already, and do not add repeat functionality. please keep a simple codebase as a priority.**
## Build & Run from Source
you can build all binaries for all platforms by running
```sh
make all
```
or you can make for a specific platfom by running make with your platform (elf for linux/unix)
```sh
make {win/mac/elf}
```
for example, to build 32 and 64 bit for windows
```sh
make win
```
if you want a specific platfom and bit width you run make with your platform and then either 32 or 64 after
```sh
make {win/mac/elf}{32/64}
```
for example
```sh
make win64
```
to build a 64 bit windows binary

## Create your own version of SpecSeek
the backend of specseek that parses all the information is libspecseek, you can use this to create your own client for some obscure operating system or just use it as a library to identify CPU support. It is a powerful tool! and its free [check it out here](github.com/Mellurboo/libspecseek)

Made with Love by xhcisucks (Isabella)
