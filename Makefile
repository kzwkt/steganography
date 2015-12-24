CFLAGS=-Wall -g

make:
	gcc image_funcs.c -c
	gcc stego.c -c
	gcc stego_extract.c -c
	gcc stego.o image_funcs.o -o stego $(CFLAGS)
	gcc stego_extract.o image_funcs.o -o stego_extract $(CFLAGS)
clean:
	rm -f stego
	rm -f stego_extract
