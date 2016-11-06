CFILES=$(wildcard *.c)
OBJECTS=$(patsubst %.c, %.o, $(CFILES))
CFLAGS+=-std=c99 -Wall -Wextra -pedantic -pie -fPIE -fstack-check -fstack-protector-all -O2 -g -I/usr/include/postgresql -D_POSIX_C_SOURCE=200809L -D_FORTIFY_SOURCE=2
LDFLAGS+=-lpcre -lpthread -lcurl -lpq -lc -L/lib/x86_64-linux-gnu -z relro
EXECUTABLE=bincalc

all: $(EXECUTABLE)

$(EXECUTABLE): $(OBJECTS)
	gcc -o $@ $^ $(LDFLAGS)

clean:
	rm -f cbot $(OBJECTS)
