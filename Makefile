CC = cl
CFLAGS = /W3 /nologo -D_CRT_SECURE_NO_DEPRECATE

build: main.obj hash.obj hashtable.obj
	$(CC) $** /Fetema1.exe

main.obj: main.c
	$(CC) $(CFLAGS) /c main.c

hash.obj: hash.c
	$(CC) $(CFLAGS) /c hash.c

hashtable.obj: hashtable.c
	$(CC) $(CFLAGS) /c hashtable.c

clean: tema1.exe
	del *.obj tema1.exe
