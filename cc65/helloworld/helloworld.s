.segment "DATA"
MSG: .ASCIIZ "Hello world!"

.segment "CODE"
LDA #$8D ; next line
JSR $FDED
LDX #0
LDA MSG,X ; load initial char
@LP: ORA #$80
JSR $FDED ; cout
INX
LDA MSG,X
BNE @LP
LDA #$8D ; next line
JSR $FDED
JMP $03D0 ; warm start

