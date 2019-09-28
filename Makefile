


COMP = g++ -Wall -g -o
OBJ = g++ -Wall -g -c 

default: dragonshell

dragonshell: dragonshell.o builtinjobs.o dshelljob.o jobqueue.o tokenize.o dshellhelpers.o
	$(COMP) dragonshell dragonshell.o builtinjobs.o dshelljob.o jobqueue.o tokenize.o dshellhelpers.o

builtinjobs.o: builtinjobs.cc builtinjobs.h
	$(OBJ) builtinjobs.cc

dragonshell.o: dragonshell.cc dshellconstants.h dshelljob.h dshelltypes.h
	$(OBJ) dragonshell.cc

dshelljob.o: dshelljob.cc dshelljob.h dshelltypes.h
	$(OBJ) dshelljob.cc

jobqueue.o: JobQueue.cc JobQueue.h dshelljob.h
	$(OBJ) JobQueue.cc

tokenize.o: tokenize.cc tokenize.h
	$(OBJ) tokenize.cc

dshellhelpers.o: dshellhelpers.cc dshellhelpers.h
	$(OBJ) dshellhelpers.cc

clean:
	-rm *.o $(objects)

testProg: testProg.cc
	$(COMP) testProg testProg.cc