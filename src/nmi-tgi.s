;
; nmi_tgi.s -- Catches Non-Maskable Interrupts when C64 TGI has disabled the ROM and I/O.
;
; 2018-05-08, Greg King
;
; void install_nmi_tgi (void);
;

        .export         _install_nmi_tgi

        .include        "c64.inc"

nmi_vec :=      $FFFA

.segment        "LOWCODE"

_install_nmi_tgi:
        lda     #<nmi_tgi
        ldx     #>nmi_tgi
        sta     nmi_vec
        stx     nmi_vec+1
        rts

nmi_tgi:
        pha
        lda     $01             ; save RAM config.
        pha
        ora     #%00000110      ; enable Kernal and I/O
        sta     $01
        jsr     vector
        pla
        sta     $01
        pla
        rti

vector: jmp     (NMIVec)
