#Sample Makefile

CFLAGS=-g -O0 -Wall -Werror -fno-stack-protector

all:	check

default: check
	
clean-ckpt:
	rm -rf myckpt

clean: clean-ckpt
	rm -rf restart hello ckpt ckpt.o libckpt.so

hello.o: hello.c
	gcc ${CFLAGS} -c -fpic -o hello.o hello.c

hello:	hello.o
	gcc ${CFLAGS} -g -o hello hello.o

helper.o: helper.c helper.h
	gcc ${CFLAGS} -c -fpic -o helper.o helper.c

ckpt.o: ckpt.c helper.h
	gcc ${CFLAGS} -c -fpic -o ckpt.o ckpt.c

libckpt.so: ckpt.o helper.o
	gcc ${CFLAGS} -shared -o libckpt.so ckpt.o helper.o

restart: restart.c helper.h helper.o
	gcc ${CFLAGS} -g -static -Wl,-Ttext-segment=5000000 -Wl,-Tdata=5100000 -Wl,-Tbss=5200000 -o restart restart.c helper.o

res: 	restart
	./restart myckpt

gdb:
	gdb --args ./restart myckpt

check:	clean-ckpt libckpt.so hello restart
	(sleep 3 && kill -12 `pgrep -n hello` && sleep 2 && pkill -9 hello) & 
	LD_PRELOAD=`pwd`/libckpt.so ./hello || true
	(sleep 2 ) &
	make res

dist:
	dir=`basename $$PWD`; cd ..; tar cvf $$dir.tar ./$$dir; gzip $$dir.tar
