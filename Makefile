PROGRAM = slerp
CC      = g++
CSTD	= -std=c++17
CFLAGS  = -Wall -Wextra -Wno-unused-parameter -Wno-ignored-attributes -I/usr/X11R6/include -I/usr/pkg/include
LDFLAGS = -L/usr/X11R6/lib -L/usr/pkg/lib
FLAGS   = -lm -lglut -lGLU -lGL

$(PROGRAM): slerp.o kretanja.o
	$(CC) slerp.o kretanja.o $(LDFLAGS) -o $(PROGRAM) $(FLAGS)

slerp.o: slerp.cpp kretanja.hpp
	$(CC) -c slerp.cpp kretanja.hpp $(CSTD) $(CFLAGS)

kretanja.o: kretanja.cpp kretanja.hpp
	$(CC) -c kretanja.cpp kretanja.hpp $(CSTD) $(CFLAGS)
