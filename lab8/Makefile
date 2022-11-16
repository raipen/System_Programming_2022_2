all: hello5 ticker_demo bounce1d blinking_hello

hello5: hello5.c
	gcc -o hello5 hello5.c -lcurses

ticker_demo: ticker_demo.c
	gcc -o ticker_demo ticker_demo.c

bounce1d: bounce1d.c set_ticker.c
	cc bounce1d.c set_ticker.c -lcurses -o bounce1d

blinking_hello: blinking_hello.c
	gcc -o blinking_hello blinking_hello.c -lcurses

clean:
	rm -f hello5 ticker_demo bounce1d blinking_hello