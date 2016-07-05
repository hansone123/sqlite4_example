include config.mk

SRC := $(wildcard $(TOP)/src/*.cc)
HDR := $(wildcard $(TOP)/src/*.h)

OBJ := $(patsubst %.cc,%.o,$(wildcard $(TOP)/src/*.cc))
OBJ := $(patsubst $(TOP)/src/%, %,$(OBJ))
	
TEST := $(patsubst %.cc,%$(EXE),$(wildcard $(TOP)/test/*.cc))
TEST := $(patsubst $(TOP)/test/%, %,$(TEST))

all: check sqlite4example.a sqlite4example.exe 
	
check:
	echo 'HDR: $(HDR)'
	echo 'SRC: $(SRC)'
	echo 'OBJ: $(OBJ)'
	echo 'TEST: $(TEST)'
	
sqlite4example.a: $(OBJ)
	$(AR) sqlite4example.a $(TOP)/obj/*.o
	$(RANLIB) sqlite4example.a
%.o: $(TOP)/src/%.cc
	g++ -o $(TOP)/obj/$@ -c $<

sqlite4example$(EXE):
	g++ -o $(TOP)/sqlite4example$(EXE) $(TOP)/src/sqlite4example.cc $(TOP)/include/* -lleveldb -lpthread

clean:
	rm $(TOP)/*$(EXE)
	rm $(TOP)/obj/*
testcmd:
	echo $(OBJ)