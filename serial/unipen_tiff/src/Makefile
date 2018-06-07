#
# This is a makefile for the unipen_to_tiff application from the DIBS Benchmarking
# suite.
#
# Author: Anthony Cabrera
# Date: 9/13/17

CC := gcc
FLAGS := -Wall

LOC_MY_MEMCPY := ../../my_memcpy

LIB := -lmemcpy -lm

INC := ${LOC_MY_MEMCPY}

#FLAGS := -Wall -I${LOC_MY_MEMCPY}
FLAGS := -Wall

APP_NAME := unipen_to_tiff

SRC_FILES := ${APP_NAME}

SRC := $(patsubst %, %.c, ${SRC_FILES})

OBJ_DIR = obj
OBJ := $(patsubst %, ${OBJ_DIR}/%.o, ${SRC_FILES})

.PHONY := all clean

all: ${OBJ}
	@mkdir -p ../output 
	${CC} ${FLAGS} -L${LOC_MY_MEMCPY} -Wl,-rpath=${LOC_MY_MEMCPY} -o ${APP_NAME} \
		${OBJ} ${LIB} -fopenmp

${OBJ_DIR}/%.o: %.c
	@mkdir -p ${OBJ_DIR}
	${CC} ${FLAGS} -c -o $@ $<

clean:
	@echo "Removing all obj files and binary"
	rm -rf obj/* ${APP_NAME}
