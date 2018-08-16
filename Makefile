CC=gcc
CFLAGS=-I. -lssl -lcrypto -g -std=gnu99

BINARIES=md2 md4 md5 mdBrute

all: clean mdTools

.PHONY: mdTools
mdTools:
	cd mdTools && make all

.PHONY: install
install: $(BINARIES)
	cp $^ /usr/bin/

.PHONY: uninstall
uninstall: $(BINARIES)
	cd /usr/bin && rm -rf $^

.PHONY: clean
clean:
	rm -rf *.o
