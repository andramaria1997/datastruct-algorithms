CC=cl
CFLAGS=/MD /W3 /D_CRT_SECURE_NO_DEPRECATE

build: tema1.exe

tema1.exe: tema1.obj compare.lib
	$(CC) $(CFLAGS) /Fe$@ tema1.obj compare.lib

tema1.obj: tema1.c
	$(CC) $(CFLAGS) /c /Fo$@ tema1.c

clean:
	del *.obj tema1.exe
