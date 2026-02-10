TARGETS:=udp

OBJS:=udp_lib/udp.o main.o

CFLAGS+=-I./

udp: $(OBJS)
	$(CC) $(CFLAGS) $^ -o $@

all: $(TARGETS)

clean:
	rm -f $(TARGETS) $(OBJS)

default: clean all

.PHONY: clean all
