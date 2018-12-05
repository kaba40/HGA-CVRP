# Filename: Makefile

# ----------------- #
# OPTIONS TO CHANGE #
# ----------------- #
OS := $(shell uname -s)
DEBUG = ON
OPTIMIZE = ON
# ----------------- #

SRC_DIR = .
INCLUDE_DIR = .
BUILD_DIR = .
BIN_DIR = .


CCC = g++ -O0
ifeq ($(OPTIMIZE),ON)
CCC = g++ -O3
endif	

ifeq ($(DEBUG),ON)
DEBUG_OPTIONS = -g -DSPLIT_ALGORITHM_DEBUG  -DDEBUG
endif	



CCOPT = -m64 -std=c++11 $(DEBUG_OPTIONS)

CCLNFLAGS = -m64 \
            -lm \
	    -lpthread

CCFLAGS = $(CCOPT) -I./include
SOURCES = ${wildcard ${SRC_DIR}/*.cpp}		

OBJ = ${patsubst $(SRC_DIR)/%.cpp, $(BUILD_DIR)/%.o, ${SOURCES}}

MY_SOLVER = solver

MY_PROGRAM_WITHOUT_EXTENSION = ${BIN_DIR}/$(MY_SOLVER)

all:
	make $(MY_PROGRAM_WITHOUT_EXTENSION)
clean : clean-lp
	/bin/rm -rf $(BUILD_DIR)/*.o *~ *.sav *.log $(MY_PROGRAM_WITHOUT_EXTENSION)
clean-lp :
	/bin/rm -rf *.lp
release:
	make $(MY_PROGRAM_WITHOUT_EXTENSION)

$(MY_PROGRAM_WITHOUT_EXTENSION): $(OBJ)
	$(CCC) $(CCFLAGS) -o $(MY_PROGRAM_WITHOUT_EXTENSION) $^ $(CCLNFLAGS)

build/%.o: src/%.cpp
	$(CCC)  $(CCFLAGS)    -o $@  -c $< 

