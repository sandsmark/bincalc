CFILES=$(wildcard *.c)
LDFLAGS+=-lgmp
OBJECTS=$(patsubst %.c, %.o, $(CFILES))
EXECUTABLE=bincalc

all: $(EXECUTABLE)

$(EXECUTABLE): $(OBJECTS)
	gcc -o $@ $^ $(LDFLAGS)

clean:
	rm -f $(EXECUTABLE) $(OBJECTS)
