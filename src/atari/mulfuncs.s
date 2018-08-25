
	;; Multiply functions, thanks to Christian Groessler

.include "zeropage.inc"
.export _mul0625, _mul0375
.code

; unsigned int __fastcall__ mul0625(unsigned int val);
; 0.625x = (1/2 + 1/8)x

_mul0625:
	stx tmp1	; high-byte of val
	clc
	ror tmp1
	ror a
	sta tmp2	; tmp1:tmp2 = val/2
	lda tmp1
	clc
	ror a
	sta tmp3
	lda tmp2
	ror a
;	sta tmp4	; tmp3:tmp4 = val/4
	clc
	ror tmp3
	ror a
;	sta tmp4	; tmp3:tmp4 = val/8
	clc
	adc tmp2
	sta tmp2
	lda tmp3
	adc tmp1
	tax
	lda tmp2
	rts


; unsigned int __fastcall__ mul0375(unsigned int val);
; 0.375x = (1/4 + 1/8)x

_mul0375:
	stx tmp1	; high-byte of val
	clc
	ror tmp1
	ror a
	sta tmp2	; tmp1:tmp2 = val/2
	clc
	ror tmp1
	ror tmp2	; tmp1:tmp2 = val/4
	lda tmp1
	clc
	ror a
	sta tmp3
	lda tmp2
	ror a
;	sta tmp4	; tmp3:tmp4 = val/8
	clc
	adc tmp2
	sta tmp2
	lda tmp3
	adc tmp1
	tax
	lda tmp2
	rts
	
.end
	
.endif
