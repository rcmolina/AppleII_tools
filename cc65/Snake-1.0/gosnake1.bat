..\ca65 snake.s -l
..\ld65  -o SNAKE1#060800 -C linker.config snake.o
del SNAKE1.DSK
copy ..\EMPTY.DSK SNAKE1.DSK
..\a2tools in b.0800 SNAKE1.DSK SNAKE1 SNAKE1#060800
rem ..\java\bin\java -jar ..\AppleCommander.jar -p SNAKE1.DSK SNAKE1 "bin" "0x800" < SNAKE1#060800