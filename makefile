CC = g++

compile : rdb-main.cpp rdb.h rdb-attr.o rdb-basics.o rdb-join.o
	$(CC) -g rdb-main.cpp -o test rdb-attr.o rdb-basics.o rdb-join.o

rdb-join.o : rdb-join.cpp rdb.h
	$(CC) -Wall -c rdb-join.cpp

rdb-basics.o : rdb-basics.cpp rdb.h
	$(CC) -Wall -c rdb-basics.cpp

rdb-attr.o : rdb-attr.cpp rdb.h
	$(CC) -Wall -c rdb-attr.cpp

clean :
	rm -r *.o