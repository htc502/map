objects = pathmark_main.o pathmark_db.o
CFLAGS = -O3 -Wall
map: $(objects)
	cc -o map $(objects)
pathmark_main.o: pathmark_main.c pathmark_db.h realpath_w32.h
pathmark_db.o: pathmark_db.h 

install:map
	cp map /usr/local/bin
	cp map_go.sh /usr/local/bin
	echo 'source /usr/local/bin/map_go.sh' >> $(HOME)/.bashrc
uninstall:map
	rm /usr/local/bin/map
	rm /usr/local/bin/map_go.sh
	echo "Don't forget to mannually remove the line with: source /usr/local/bin/map_go.sh in your $(HOME)/.bashrc file"
##code bought from http://scc.qibebt.cas.cn/docs/linux/base/%B8%FA%CE%D2%D2%BB%C6%F0%D0%B4Makefile-%B3%C2%F0%A9.pdf
.PHONY:clean
clean:
	-rm map $(objects)


