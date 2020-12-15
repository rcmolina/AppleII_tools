..\ca65 bell.asm
..\ld65 -o bell#066000 -C apple2bin.cfg bell.o
del BELL.DSK
copy ..\EMPTY.DSK BELL.DSK
..\a2tools in b.6000 BELL.DSK BELL bell#066000