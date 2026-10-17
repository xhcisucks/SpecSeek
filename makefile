TARGET      = specseek
BIN_DIR     = bin
VENDOR_DIR  = libspecseek

#host platform
ifeq ($(OS),Windows_NT)
HOST       := win
else
UNAME_S    := $(shell uname -s)
ifeq ($(UNAME_S),Darwin)
HOST       := mac
else
HOST       := elf
endif
endif

#t target platform
PLATFORM   ?= $(HOST)
BITS       ?= 64

NAME        = $(TARGET)_$(PLATFORM)_$(BITS)
OBJ_DIR     = $(BIN_DIR)/obj/$(PLATFORM)_$(BITS)
LIB_BIN_DIR = bin/$(PLATFORM)_$(BITS)
ARCH        = -m$(BITS)

ifeq ($(PLATFORM),win)
EXE        := .exe
ifneq ($(HOST),win)

ifeq ($(BITS),32)
CC         := i686-w64-mingw32-gcc
AR         := i686-w64-mingw32-ar
else
CC         := x86_64-w64-mingw32-gcc
AR         := x86_64-w64-mingw32-ar
endif
else

CC         := gcc
endif

else ifeq ($(PLATFORM),mac)
EXE        :=
ifeq ($(BITS),32)
$(error macOS can't run 32 bit binaries, use BITS=64)
endif
ifeq ($(HOST),mac)
# you can build ON apple silicon but you cant run on apple silicon as
# specseek relies on CPUID which ARM doesn't implement
# rosetta can emulate it though

CC         := clang -arch x86_64
else
# cross compiling, zig ships the macOS headers/libs so no SDK needed
CC         := zig cc -target x86_64-macos -fno-sanitize=undefined
AR         := zig ar
endif

else ifeq ($(PLATFORM),elf)
EXE        :=
ifeq ($(HOST),elf)
# 32 bit needs gcc-multilib installed too
CC         := gcc
else

ifeq ($(BITS),32)
CC         := zig cc -target x86-linux-gnu -fno-sanitize=undefined
else
CC         := zig cc -target x86_64-linux-gnu -fno-sanitize=undefined
endif
AR         := zig ar
endif

else
$(error unknown PLATFORM '$(PLATFORM)', use win, mac or elf)
endif

# Building on windows
ifeq ($(OS)-$(MSYSTEM),Windows_NT-)
MKDIR       = if not exist "$(subst /,\,$(1:/=))" mkdir "$(subst /,\,$(1:/=))"
RMDIR       = if exist "$(subst /,\,$1)" rmdir /s /q "$(subst /,\,$1)"
RUN         = $(subst /,\,$1)
else
MKDIR       = mkdir -p $1
RMDIR       = rm -rf $1
RUN         = ./$1
endif

rwildcard   = $(foreach d,$(wildcard $(1:=/*)),$(call rwildcard,$d,$2) $(filter $(subst *,%,$2),$d))

C_FLAGS     = -Wall -Wextra -Werror -Wno-unused-parameter $(ARCH) \
              -Iinclude \
              -I$(VENDOR_DIR)

LD_FLAGS    = $(ARCH) -L$(VENDOR_DIR)/$(LIB_BIN_DIR) -lspecseek

SRCS        = $(call rwildcard,src,*.c)
OBJS        = $(patsubst src/%.c,$(OBJ_DIR)/%.o,$(SRCS))

HAVE_ZIG   := $(shell zig version 2>$(if $(filter win,$(HOST)),NUL,/dev/null))
ifeq ($(HOST),mac)
ALL_TARGETS := mac64 win32 win64 $(if $(HAVE_ZIG),elf32 elf64)
else ifeq ($(HOST),elf)
ALL_TARGETS := elf32 elf64 win32 win64 $(if $(HAVE_ZIG),mac64)
else
ALL_TARGETS := win32 win64
endif

all: $(ALL_TARGETS)

build: lib $(OBJS)
	@$(call MKDIR,$(BIN_DIR))
	$(CC) $(OBJS) $(LD_FLAGS) -o $(BIN_DIR)/$(NAME)$(EXE)

$(OBJ_DIR)/%.o: src/%.c
	@$(call MKDIR,$(dir $@))
	$(CC) $(C_FLAGS) -c $< -o $@

lib:
	$(MAKE) -C $(VENDOR_DIR) build CC="$(CC) $(ARCH)" AR="$(AR)" LIB_BIN_DIR=$(LIB_BIN_DIR)

win32 win64 mac64 elf32 elf64:
	$(MAKE) build PLATFORM=$(patsubst %32,%,$(patsubst %64,%,$@)) BITS=$(patsubst win%,%,$(patsubst mac%,%,$(patsubst elf%,%,$@)))

win: win32 win64
mac: mac64
elf: elf32 elf64

run: build
	$(call RUN,$(BIN_DIR)/$(NAME)$(EXE))

clean:
	@$(call RMDIR,bin)
	$(MAKE) -C $(VENDOR_DIR) clean

.PHONY: all build clean lib run win mac elf win32 win64 mac64 elf32 elf64
