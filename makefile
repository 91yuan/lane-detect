LIBS = `pkg-config --cflags --libs opencv`

run:main.o cmdline.o
	g++ -o run main.o cmdline.o ${LIBS}

main.o:main.cc
	g++ -c main.cc ${LIBS}

cmdline.o:cmdline.c
	gcc -c cmdline.c

cmdline.c:cmdline.ggo
	gengetopt < cmdline.ggo	
