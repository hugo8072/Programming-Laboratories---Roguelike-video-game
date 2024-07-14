CFLAGS=-Wall
FICHEIROS=cgi.h estado.c estado.h roguelike.c Makefile

install: roguelike
	sudo cp roguelike /usr/lib/cgi-bin
	sudo cp -R images /var/www/html
	sudo cp -R files /var/www/html
	sudo chmod 755 * /var/www/html/images
	sudo chmod -R a+rw /var/www/html/files	
	touch install

roguelike: roguelike.o estado.o
	cc -o roguelike roguelike.o -lm

roguelike.zip: $(FICHEIROS)
	zip -9 roguelike.zip $(FICHEIROS)

doc:
	doxygen -g
	doxygen

clean:
	rm -rf *.o exemplo Doxyfile latex html install

estado.o: estado.c estado.h
roguelike.o: roguelike.c cgi.h estado.h
