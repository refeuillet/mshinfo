## Very basic Makefile

all: mshinfo

mshinfo:	libMeshb/sources/libmeshb7.c mshinfo.c libMeshb/sources/libmeshb7.h
	gcc libMeshb/sources/libmeshb7.c mshinfo.c -Wall -I./libMeshb/sources -o mshinfo

clean : mshinfo
	rm -f mshinfo
