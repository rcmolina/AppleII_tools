..\ca65 add.asm
..\ld65 -o add#066000 -C apple2bin.cfg add.o
del ADD.DSK
copy ..\EMPTY.DSK ADD.DSK
rem ..\a2tools in b.6000 ADD.dsk TEST add#066000