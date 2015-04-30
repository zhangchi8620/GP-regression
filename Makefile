CC = g++

OBJECTS = tree.o gen.o Zhang-Project2.o
EXEC = gp

$(EXEC): $(OBJECTS)
	$(CC) -Wno-write-strings -g -o $@ $^

tree.o: tree.cc tree.h param.h
	$(CC) -Wno-write-strings -g -c $<
gen.o:gen.cc gen.h tree.o
	$(CC) -Wno-write-strings -g -c $<
Zhang-Project2.o:Zhang-Project2.cc gen.o 
	$(CC) -Wno-write-strings -g -c $<

clean:
	rm -rf $(EXEC) *.o *~
