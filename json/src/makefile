PROJ_HOME=${HOME}/code/json
INCL=${PROJ_HOME}/incl

OBJ_LIST= json_util.o json_object.o json_string.o json_error.o  json_array.o \
            main.o

CC=gcc
CFLAGS= -g -c


all: jsont

.c.o:
	${CC} ${CFLAGS} -I${INCL} $*.c

jsont: ${OBJ_LIST}
	${CC} $^ -o $@

co:
	rm -f *.o
