CC = gcc

BLINKLED = blinkled
SRCBLINKLED = blinkLed.c
OBJBLINKLED = $(SRCBLINKLED:.c=.o)

READSENSOR = readsensor
SRCREADSENSOR = readSensor.c
OBJREADSENSOR = $(SRCREADSENSOR:.c=.o)

CFLAGS = -Wall -Wextra -ansi -pedantic -D _DEFAULT_SOURCE

LDFLAGS = -lmraa


all:	$(BLINKLED) $(READSENSOR)

$(BLINKLED):	$(OBJBLINKLED)
		$(CC) -o $@ $^ $(LDFLAGS)

$(READSENSOR):	$(OBJREADSENSOR)
		$(CC) -o $@ $^ $(LDFLAGS)

%.o: %.c
	$(CC) -o $@ -c $< $(CFLAGS)

.PHONY:	all clean mrproper

clean:
		rm -rf $(OBJBLINKLED) $(OBJREADSENSOR)

mrproper:	clean
		rm -f $(BLINKLED) $(READSENSOR)

re:	mrproper all
