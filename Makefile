EXEC = nicecmp loopcmp

all: $(EXEC)

nicecmp: nicecmp.o
	gcc nicecmp.o -o nicecmp

nicecmp.o: nicecmp.c
	gcc -c nicecmp.c
	
loopcmp: loopcmp.o
	gcc loopcmp.o -o loopcmp

loopcmp.o: loopcmp.c
	gcc -c loopcmp.c
	
clean:
	rm -f *.o $(EXEC)
