all: my_grep

.PHONY: all clean test 
	

test:
	run_all.sh

clean:
	-rm *.o my_grep textLine_test commandParser_test IO_test matchInLine_test	

my_grep: commandParser.o textLine.o matchInLine.o IO.o main.o
	gcc -o my_grep commandParser.o textLine.o matchInLine.o IO.o main.o -g -O1

commandParser.o: commandParser.c commandParser.h
	gcc -c commandParser.c -g -O1
	
textLine.o:	textLine.c textLine.h
	gcc -c textLine.c -g -O1
	
IO.o: IO.c IO.h commandParser.h textLine.h
	gcc -c IO.c
	
matchInLine.o: matchInLine.c matchInLine.h commandParser.h textLine.h IO.h
	gcc -c matchInLine.c -g -O1
	
main.o: main.c commandParser.o IO.o textLine.o matchInLine.o
	gcc -c main.c -g -O1
	
	
	
	
textLine_test: textLine_test.o textLine.o
	gcc -o textLine_test textLine_test.o textLine.o
	
textLine_test.o: textLine_test.c textLine.o
	gcc -c textLine_test.c -g -O1
	
commandParser_test: commandParser_test.o commandParser.o
	gcc -o commandParser_test commandParser_test.o commandParser.o  
	
commandParser_test.o: commandParser_test.c commandParser.o
	gcc -c commandParser_test.c -g -O1
	
IO_test: IO_test.o IO.o
	gcc -o IO_test IO_test.o IO.o
	runIO_test.sh

IO_test.o: IO_test.c IO.o commandParser.h textLine.h
	gcc -c IO_test.c -g -O1 
	
matchInLine_test: matchInLine_test.o matchInLine.o
	gcc -o matchInLine_test matchInLine_test.o matchInLine.o

matchInLine_test.o: matchInLine_test.c matchInLine.o commandParser.h textLine.h
	gcc -c matchInLine_test.c -g -O1 
	

	
	

	
	


