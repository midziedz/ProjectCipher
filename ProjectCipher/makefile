all:
	g++ -Wall -g -std=c++11 -c src/cypher.cpp -o lib/cypher.o `pkg-config gtk+-2.0 --cflags` `pkg-config gtk+-2.0 --libs`
	ar rvs lib/libcypher.a lib/cypher.o
	g++ -Wall -g -std=c++11 -c src/cyphermanager.cpp -o lib/cyphermanager.o `pkg-config gtk+-2.0 --cflags` `pkg-config gtk+-2.0 --libs`
	ar rvs lib/libcyphermanager.a lib/cyphermanager.o
	g++ -Wall -g -std=c++11 -c src/iomanager.cpp -o lib/iomanager.o `pkg-config gtk+-2.0 --cflags` `pkg-config gtk+-2.0 --libs`
	ar rvs lib/libiomanager.a lib/iomanager.o
	g++ -Wall -g -std=c++11 -c src/gtkappmanager.cpp -o lib/gtkappmanager.o `pkg-config gtk+-2.0 --cflags` `pkg-config gtk+-2.0 --libs`
	ar rvs lib/libgtkappmanager.a lib/gtkappmanager.o
	g++ -Wall -g -std=c++11 src/main.cpp -L./lib/ -lcypher -lcyphermanager -liomanager -lgtkappmanager -o VigenereCypherCentre `pkg-config gtk+-2.0 --cflags` `pkg-config gtk+-2.0 --libs`
	

main.bin:
	g++ -Wall -g -std=c++11 src/main.cpp -L./lib/ -lcypher -lcyphermanager -liomanager -lgtkappmanager -o VigenereCypherCentre `pkg-config gtk+-2.0 --cflags` `pkg-config gtk+-2.0 --libs`

run:
	./VigenereCypherCentre
