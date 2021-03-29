# specify the compiler
CC=g++

# specify options for the compiler
CFLAGS=-Wall

BUILD:=Build/

FILE1:=Position
FILE2:=Components
FILE3:=Ray
FILE4:=RayTracer
FILE5:=main

FILE1o:=$(BUILD)Position
FILE2o:=$(BUILD)Components
FILE3o:=$(BUILD)Ray
FILE4o:=$(BUILD)RayTracer
FILE5o:=$(BUILD)main

a: $(FILE1o).o $(FILE2o).o $(FILE3o).o $(FILE4o).o $(FILE5o).o
	$(CC) $(FILE5o).o $(FILE1o).o $(FILE2o).o $(FILE3o).o $(FILE4o).o -o a

$(FILE1o).o: $(FILE1).h $(FILE1).cpp
	$(CC) -c $(CFLAGS) $(FILE1).cpp -o $(FILE1o).o

$(FILE2o).o: $(FILE1).h $(FILE1).cpp $(FILE2).h $(FILE2).cpp
	$(CC) -c $(CFLAGS) $(FILE2).cpp -o $(FILE2o).o

$(FILE3o).o: $(FILE1).h $(FILE1).cpp $(FILE2).h $(FILE2).cpp $(FILE3).h $(FILE3).cpp
	$(CC) -c $(CFLAGS) $(FILE3).cpp -o $(FILE3o).o

$(FILE4o).o: $(FILE1).h $(FILE1).cpp $(FILE2).h $(FILE2).cpp $(FILE3).h $(FILE3).cpp $(FILE4).h $(FILE4).cpp
	$(CC) -c $(CFLAGS) $(FILE4).cpp -o $(FILE4o).o

$(FILE5o).o: $(FILE1).h $(FILE1).cpp $(FILE2).h $(FILE2).cpp $(FILE3).h $(FILE3).cpp $(FILE4).h $(FILE4).cpp $(FILE5).cpp 
	$(CC) -c $(CFLAGS) $(FILE5).cpp -o $(FILE5o).o

clean:
	rm -rf *o a
