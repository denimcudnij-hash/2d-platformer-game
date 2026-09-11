.PHONY: all release clean

CC = cl.exe
CFLAGS_DEBUG = /Zi /Od /W4
CFLAGS_RELEASE = /O2 /DNDEBUG
LDFLAGS = user32.lib gdi32.lib xinput.lib /SUBSYSTEM:WINDOWS
OBJ = game.obj
EXE = game.exe

all: $(EXE)

release: CFLAGS = $(CFLAGS_RELEASE)
release: clean $(EXE)

$(EXE): $(OBJ)
	$(CC) $(OBJ) /link $(LDFLAGS) /OUT:$(EXE)
	@echo Build complete: $(EXE)

$(OBJ): game.c
	$(CC) /c $(CFLAGS) game.c

clean:
	@if exist $(OBJ) del $(OBJ)
	@if exist $(EXE) del $(EXE)
	@if exist *.ilk del *.ilk
	@if exist *.pdb del *.pdb
	@echo Clean complete

run: $(EXE)
	$(EXE)
