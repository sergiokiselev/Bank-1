#############################################################################
# Makefile for building: bank-api
#############################################################################

####### Compiler, tools and options

CC            = gcc
CFLAGS        = -Wall
LIBS          = -pthread -ldl

####### Files

SOURCES = main.c \
		adminFunctions.c \
		clientFunctions.c \
		operationistFunctions.c \
		unsignedFunctions.c \
		util.c \
		sqlite3.c
OBJECTS = main.o \
		adminFunctions.o \
		clientFunctions.o \
		operationistFunctions.o \
		unsignedFunctions.o \
		util.o \
		sqlite3.o

TARGET  = bank-api

####### Build rules

all: $(TARGET)

clean:
	$(RM) -f $(TARGET) *.o *~

$(TARGET):  $(OBJECTS)
	$(CC) $(CFLAGS) $(OBJECTS) $(LIBS) -o $(TARGET)

%.o: %.c
	$(CC) $(CFLAGS) -c $<
