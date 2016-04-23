CC = gcc

BLINKLED = blinkled
SRCBLINKLED = blinkLed.c
OBJBLINKLED = $(SRCBLINKLED:.c=.o)


CFLAGS = -Wall -Wextra -ansi -pedantic -D _BSD_SOURCE

LDFLAGS = -lmraa


all:	$(BLINKLED)

$(BLINKLED):	$(OBJBLINKLED)
		$(CC) -o $@ $^ $(LDFLAGS)



%.o: %.c
	$(CC) -o $@ -c $< $(CFLAGS)

.PHONY:	all clean mrproper

clean:
		rm -rf $(OBJBLINKLED)

mrproper:	clean
		rm -f $(BLINKLED)

re:	mrproper all
