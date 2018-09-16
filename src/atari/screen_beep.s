.include "atari.inc"
	
.export _screen_beep

.code
_screen_beep:
	ldx #$06
	ldy #$FF
screen_beep1:	
	sty CONSOL
	sty WSYNC
	sty WSYNC
	dey
	bne screen_beep1
	dex
	bne screen_beep1
	rts
	
