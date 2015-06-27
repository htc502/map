objects = pathmark_main.o pathmark.o
map: $(objects)
	cc -o map $(objects)
pathmark_main.o: pathmark_main.c pathmark.h realpath_w32.h
pathmark.o: pathmark.h 


