..\ca65 helloworld.s
..\ld65 -o helloworld#066000 -C apple2bin.cfg helloworld.o
del HELLOWORLD.DSK
copy ..\EMPTY.DSK HELLOWORLD.DSK
..\a2tools in b.6000 HELLOWORLD.DSK HELLOWORLD helloworld#066000
rem ..\java\bin\java -jar ..\AppleCommander.jar -p HELLOWORLD.DSK HELLOWORLD "bin" "0x6000" < helloworld#066000