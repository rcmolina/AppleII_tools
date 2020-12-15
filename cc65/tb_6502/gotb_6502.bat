..\ca65 tb_6502.s -l
..\ld65  -o TB6502#060C00 -C apple2_c00.inc tb_6502.o
del TB6502.DSK
copy ..\EMPTY.DSK TB6502.DSK
..\a2tools in b.0C00 TB6502.DSK TB6502 TB6502#060C00
rem ..\java\bin\java -jar ..\AppleCommander.jar -p TB6502.DSK TB6502 "bin" "0x0C00" < TB6502#060C00

rem ..\make_b TB6502#060C00 TB6502 0x0C00
rem ..\dos33 TB6502.dsk SAVE B TB6502
