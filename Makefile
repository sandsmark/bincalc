LDFLAGS+=-lgmp
EXECUTABLE=bincalc
all: $(EXECUTABLE)

clean:
	rm -f $(EXECUTABLE)
