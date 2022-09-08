CC     = g++
CFLAGS = -Wall -O3
LFLAGS = 

PROG = escalona 
OBJS = utils.o vision.o conflict.o

.PHONY: limpa faxina clean purge all

all: escalona

%.o: %.cpp %.hpp
	$(CC) -c $(CFLAGS) $<

$(PROG) : % :  $(OBJS) %.o
	$(CC) -o $@ $^ $(LFLAGS)

faxina purge:
	@rm -f *~ *.bak

limpa clean:   faxina
	@rm -f *.o core a.out
	@rm -f $(PROG)