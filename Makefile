objects = pathmark_main.o pathmark_db.o
CFLAGS = -O3 -Wall -g
DEST = $(HOME)/.local/bin
map: $(objects)
	cc -o map $(objects)
pathmark_main.o: pathmark_main.c pathmark_db.h realpath_w32.h
pathmark_db.o: pathmark_db.h 

##code bought from http://scc.qibebt.cas.cn/docs/linux/base/%B8%FA%CE%D2%D2%BB%C6%F0%D0%B4Makefile-%B3%C2%F0%A9.pdf
.PHONY:clean
clean:
	-rm map $(objects)


