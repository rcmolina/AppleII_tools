.list
.code
;SET SCREEN MODE
 STA $C050
 STA $C053
 STA $C055
 STA $C00C
 STA $C054

;INITIALIZE VARIABLES
PixelPointer = $06
 LDA #$00
 STA PixelPointer
 LDA #$04
 STA PixelPointer+1
 LDX #0
 LDA #0

FillRow: LDY #119
FillLoop1: STA (PixelPointer),Y
 JSR IncrementColor
 DEY
 BPL FillLoop1
 
 LDY #247
FillLoop2: STA (PixelPointer),Y
 JSR IncrementColor
 DEY
 CPY #128
 BPL FillLoop2

 INC PixelPointer+1
 LDX PixelPointer+1
 CPX #8
 BNE FillRow



Done:
JMP Done

IncrementColor:
 INC PixelColor
 LDA PixelColor
 AND #%00001111
 STA PixelColor
 ASL
 ASL
 ASL
 ASL
 CLC
 ADC PixelColor
 RTS
 

PixelColor: .byte $00

; 80 Column row map
HI: .byte $04,$04,$05,$05,$06,$06,$07,$07
.byte $04,$04,$05,$05,$06,$06,$07,$07
.byte $04,$04,$05,$05,$06,$06,$07,$07

LO: .byte $00,$80,$00,$80,$00,$80,$00,$80
.byte $28,$A8,$28,$A8,$28,$A8,$28,$A8
.byte $50,$D0,$50,$D0,$50,$D0,$50,$D0