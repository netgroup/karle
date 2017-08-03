.PHONY: client server clean

client : client.c global.h utilities.c utilities.h packet.h
	gcc -Wall -o $@.bin -g client.c global.h utilities.c utilities.h packet.h

server : server.c global.h utilities.c utilities.h
	gcc -Wall -o $@.bin -g server.c global.h utilities.c utilities.h

clean :
	@-rm -f client.bin
	@-rm -f server.bin
