; Autor reseni: Radim Safar xsafar27

; Projekt 2 - INP 2022
; Vernamova sifra na architekture MIPS64
; xsafar27 - r19-r28-r13-r24 - r0-r4

; DATA SEGMENT
                .data
login:          .asciiz "xsafar27"  ; sem doplnte vas login
cipher:         .space  17  ; misto pro zapis sifrovaneho loginu

params_sys5:    .space  8   ; misto pro ulozeni adresy pocatku
                            ; retezce pro vypis pomoci syscall 5
                            ; (viz nize "funkce" print_string)

; NOT using ascii so I dont have to use up two registers
; just for storing the numerical value of those characters
key0:           .word   19 ; s
key1:           .word   1  ; a

overflow:       .word   122 ; value of ascii z
underflow:      .word   96  ; value of ascii a - 1
correction:     .word   26  ; amount to correct by

; CODE SEGMENT
                .text
main:
    daddu   r13, r0, r0     ; reset the value

mainWhile:
    ; number check
    lb      r24, login(r13)    
    lb      r19, underflow(r0)
    daddu   r28, r24, r0
    dsub    r28, r19, r28
    bgez    r28, mainEnd

    ; add
    lb      r19, key0(r0)
    daddu   r24, r24, r19
    lb      r19, overflow(r0)
    daddu   r28, r24, r0
    dsubu   r28, r19, r28
    bgez    r28, addEnd 
    lb      r19, correction(r0)
    dsub    r24, r24, r19
addEnd:
    
    ; store and ptr++
    sb      r24, cipher(r13)
    daddi   r13, r13, 1
    
    ; number check
    lb      r24, login(r13)    
    lb      r19, underflow(r0)
    daddu   r28, r24, r0
    dsub    r28, r19, r28
    bgez    r28, mainEnd

    ; sub
    lb      r19, key1(r0)
    dsubu   r24, r24, r19
    lb      r19, underflow(r0)
    daddi   r19, r19, 1
    daddu   r28, r24, r0
    dsubu   r28, r28, r19
    bgez    r28, subEnd 
    lb      r19, correction(r0)
    dadd    r24, r24, r19
subEnd:

    ; store and ptr++
    sb      r24, cipher(r13)
    daddi   r13, r13, 1

    ; repeat 
    b mainWhile

mainEnd:
    daddi   r4, r0, cipher
    jal     print_string    ; vypis pomoci print_string - viz nize
    syscall 0   ; halt

print_string:   ; adresa retezce se ocekava v r4
    sw      r4, params_sys5(r0)
    daddi   r14, r0, params_sys5    ; adr pro syscall 5 musi do r14
    syscall 5   ; systemova procedura - vypis retezce na terminal
    jr      r31 ; return - r31 je urcen na return address

