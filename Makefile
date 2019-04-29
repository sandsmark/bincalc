CFILES=$(wildcard *.c)
LDFLAGS+=-lgmp -fsanitize=undefined
CFLAGS+=-g -fsanitize=undefined
OBJECTS=$(patsubst %.c, %.o, $(CFILES))
EXECUTABLE=bincalc

all: $(EXECUTABLE)

$(EXECUTABLE): $(OBJECTS)
	gcc -o $@ $^ $(LDFLAGS)

clean:
	rm -f $(EXECUTABLE) $(OBJECTS)
