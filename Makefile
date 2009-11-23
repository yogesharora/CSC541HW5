.SUFFIXES: .o .c .cpp 

all: assn_5
default: assn_5

OBJ =  filereader.o str.o main.o DatabaseFile.o ChainedIndex.o
 	
%.o: %.c 
	gcc -Wall -Wno-unused-function -Wno-deprecated -g  -c $(CFLAGS) $(DEFS) $< -o $@
	
%.o: %.cpp 
	gcc -Wall -Wno-unused-function -Wno-deprecated -g  -c $(CFLAGS) $(DEFS) $< -o $@

assn_5: ${OBJ}
	gcc -o $@ ${OBJ} $(DEFS) -lstdc++

clean:
	-rm *.o
	-rm assn_5.exe
	-rm assn_5
