#
# This is a makefile for the tstcsv_to_csv application from the DIBS Benchmarking
# suite.
#
# Author: Anthony Cabrera
# Date: 9/13/17
# Modified by Clayton Faber for testing

CC := gcc
FLAGS := -Wall

LIB := -fopenmp -ffast-math -lm -O3

FLAGS := -Wall -g

APP_NAME := tstcsv_to_csv

SRC_FILES := ${APP_NAME}

SRC := $(patsubst %, %.c, ${SRC_FILES})

OBJ_DIR = obj
OBJ := $(patsubst %, ${OBJ_DIR}/%.o, ${SRC_FILES})

.PHONY := all clean

all: ${OBJ}
	${CC} ${FLAGS} -Wl,-o ${APP_NAME} \
		${OBJ} ${LIB}

${OBJ_DIR}/%.o: %.c
	@mkdir -p ${OBJ_DIR}
	${CC} ${FLAGS} -c -o $@ $<

clean:
	@echo "Removing all obj files and binary"
	rm -rf obj/* ${APP_NAME}
