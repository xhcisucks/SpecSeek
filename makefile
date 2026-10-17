TARGET      = specseek
BIN_DIR     = bin
OBJ_DIR     = $(BIN_DIR)/obj

VENDOR_DIR  = libspecseek
LIB_BIN_DIR = bin/elf

ifeq ($(OS),Windows_NT)
CC         := gcc
EXE        := .exe
else
UNAME_S    := $(shell uname -s)
UNAME_M    := $(shell uname -m)

# you can build ON apple silicon but you cant run on apple silicon as
# specseek relies on CPUID which ARM doesn't implement
ifeq ($(UNAME_S)-$(UNAME_M),Darwin-arm64)
CC         := clang -arch x86_64
else
CC         := gcc
endif
EXE        :=
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

C_FLAGS     = -Wall -Wextra -Werror -Wno-unused-parameter -m64 \
              -Iinclude \
              -I$(VENDOR_DIR)

LD_FLAGS    = -m64 -L$(VENDOR_DIR)/$(LIB_BIN_DIR) -lspecseek

SRCS        = $(call rwildcard,src,*.c)
OBJS        = $(patsubst src/%.c,$(OBJ_DIR)/%.o,$(SRCS))

all: $(TARGET)

$(TARGET): lib $(OBJS)
	@$(call MKDIR,$(BIN_DIR))
	$(CC) $(OBJS) $(LD_FLAGS) -o $(BIN_DIR)/$(TARGET)$(EXE)

$(OBJ_DIR)/%.o: src/%.c
	@$(call MKDIR,$(dir $@))
	$(CC) $(C_FLAGS) -c $< -o $@

lib:
	$(MAKE) -C $(VENDOR_DIR) build CC="$(CC)" AR="$(AR)" LIB_BIN_DIR=$(LIB_BIN_DIR)

# you will need mingw-w64 for this
windows:
	$(MAKE) CC=x86_64-w64-mingw32-gcc AR=x86_64-w64-mingw32-ar EXE=.exe \
	        BIN_DIR=bin/win LIB_BIN_DIR=bin/win

build: $(TARGET)

run: $(TARGET)
	$(call RUN,$(BIN_DIR)/$(TARGET)$(EXE))

clean:
	@$(call RMDIR,bin)
	$(MAKE) -C $(VENDOR_DIR) clean

.PHONY: all build clean lib run windows
