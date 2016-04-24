CC = gcc

BLINKLED = blinkled
SRCBLINKLED = blinkLed.c
OBJBLINKLED = $(SRCBLINKLED:.c=.o)

READSENSOR = readsensor
SRCREADSENSOR = readSensor.c
OBJREADSENSOR = $(SRCREADSENSOR:.c=.o)

READGASSENSOR = readgassensor
SRCREADGASSENSOR = readGasSensor.c
OBJREADGASSENSOR = $(SRCREADGASSENSOR:.c=.o)

CFLAGS = -Wall -Wextra -std=gnu11

LDFLAGS = -lmraa


all:	$(BLINKLED) $(READSENSOR) $(READGASSENSOR)

$(BLINKLED):	$(OBJBLINKLED)
		$(CC) -o $@ $^ $(LDFLAGS)

$(READSENSOR):	$(OBJREADSENSOR)
		$(CC) -o $@ $^ $(LDFLAGS)

$(READGASSENSOR): $(OBJREADGASSENSOR)
		$(CC) -o $@ $^ $(LDFLAGS)

%.o: %.c
	$(CC) -o $@ -c $< $(CFLAGS)

.PHONY:	all clean mrproper

clean:
		rm -rf $(OBJBLINKLED) $(OBJREADSENSOR) $(OBJREADGASSENSOR)

mrproper:	clean
		rm -f $(BLINKLED) $(READSENSOR) $(READGASSENSOR)

re:	mrproper all

