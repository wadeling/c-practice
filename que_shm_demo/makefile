CC=g++
DEBUG=n
RM=rm -rf
MAKE=make

DIR_INC=./include
SOURCE_DIR=./src
LIB_SRC=./lib
DIR_BIN=./bin
DIR_OBJ=./obj
BENCH_DIR=./bench

$(shell mkdir -p $(DIR_BIN))
$(shell mkdir -p $(DIR_OBJ))

SOURCE_FILE=$(wildcard ${SOURCE_DIR}/*.cpp)
SOURCE_DEPS=$(wildcard ${SOURCE_DIR}/*.h)
LIB_SRC_FILE=$(wildcard ${LIB_SRC}/*.cpp)
LIB_DEPS=$(wildcard ${LIB_SRC}/*.h)

OBJ_SOURCE=$(patsubst %.cpp,${DIR_OBJ}/%.o,$(notdir ${SOURCE_FILE}))
OBJ_LIB=$(patsubst %.cpp,${DIR_OBJ}/%.o,$(notdir ${LIB_SRC_FILE}))

TARGET=demo
BIN_TARGET=${DIR_BIN}/${TARGET}


CFLAGS=-Wall -std=c++11
INC=-I$(DIR_INC) -I$(LIB_SRC) -I$(SOURCE_DIR)
ifeq ($(DEBUG), y)
CFLAGS += -g
else
CFLAGS += -O3
endif

.PHONY:all clean

all:${BIN_TARGET}

${BIN_TARGET}: ${OBJ_SOURCE} ${OBJ_LIB}
	$(CC) $(CFLAGS) $^ -o $@ 

${DIR_OBJ}/%.o:$(SOURCE_DIR)/%.cpp $(SOURCE_DEPS)
	$(CC) $(CFLAGS) $(INC) -c $< -o $@

${DIR_OBJ}/%.o:$(LIB_SRC)/%.cpp $(LIB_DEPS)
	$(CC) $(CFLAGS) $(INC) -c $< -o $@

clean:
	find ${DIR_BIN} . -name '*.o' -exec $(RM) '{}' \;
	$(RM) ${BIN_TARGET}
	$(RM) ${BENCH_TARGET}
