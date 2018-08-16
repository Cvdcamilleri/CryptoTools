CC=gcc
CFLAGS=-I. -lssl -lcrypto -g -std=gnu99
OBJ_mdBrute = mdBrute.o
OBJ_md2 = md2.o
OBJ_md4 = md4.o
OBJ_md5 = md5.o

BINARIES = mdBrute md2 md4 md5
OUTDIR = ../

all: clean mdBrute md2 md4 md5 finish
all-noclean: mdBrute md2 md4 md5 finish

%.o: %.c
	$(CC) -c -o $@ $< $(CFLAGS)

mdBrute: $(OBJ_mdBrute)
	$(CC) -o $@ $^ $(CFLAGS)

md2: $(OBJ_md2)
	$(CC) -o $@ $^ $(CFLAGS)

md4: $(OBJ_md4)
	$(CC) -o $@ $^ $(CFLAGS)

md5: $(OBJ_md5)
	$(CC) -o $@ $^ $(CFLAGS)

.PHONY: finish
finish: $(BINARIES)
	cp -f $^ $(OUTDIR)

.PHONY: clean
clean:
	rm -rf *.o
	rm -rf $(BINARIES)
