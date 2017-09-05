## Very basic Makefile

all: mshinfo

mshinfo:	libmeshb7.c mshinfo.c libmeshb7.h
	gcc libmeshb7.c mshinfo.c  -I. -o mshinfo
