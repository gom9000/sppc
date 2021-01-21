#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#
#   ________        _________________.________
#  /  _____/  ____ /   _____/   __   \   ____/
# /   \  ___ /  _ \\_____  \\____    /____  \
#  \    \_\  (  <_> )        \  /    //       \
#   \______  /\____/_______  / /____//______  /
#          \/              \/               \/
# 
# Autore....: Alessandro Fraschetti
# Funzione..: makefile del game "Impiccato"
# Input.....: -
# Output....: exec
# Note......:
#
#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#


SOURCES  = impiccato.c
EXEC     = impiccato.exe
OBJECTS  = $(SOURCES:.c=.o)
INCLUDES = ./
LIBINCLUDES = ../../libraries/distrib
LIBNAME  = mul
LIBPATH  = ../../libraries/distrib
CC       = gcc
RM       = del
CFLAGS   = -Wall -ansi -pedantic -O2 -I$(INCLUDES) -I$(LIBINCLUDES) -c


$(EXEC): $(OBJECTS) 
	$(CC) -o$(EXEC) $(OBJECTS) -L$(LIBPATH) -l$(LIBNAME)

$(OBJECTS): $(SOURCES)
	$(CC) $(CFLAGS) $(SOURCES)


.PHONY: clean
clean:
	$(RM) $(OBJECTS)
