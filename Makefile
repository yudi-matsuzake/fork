GCC=gcc
GCC_FLAGS=-Wall -Wextra -ansi -O3
BIN_PATH=bin
BIN=$(BIN_PATH)/fork

ZSH_COMPLETITION=zsh/_fork
ZSH_COMPLETITION_INSTALL=/usr/share/zsh/site-functions

SRC_PATH=src
SRC=$(SRC_PATH)/fork.c

INSTALL_PATH=/usr/bin

$(BIN): $(SRC)
	@mkdir -p $(BIN_PATH)
	$(GCC) $(SRC) -o $(BIN) $(GCC_FLAGS)

install:
	cp $(BIN) $(INSTALL_PATH)
	cp $(ZSH_COMPLETITION) $(ZSH_COMPLETITION_INSTALL)
