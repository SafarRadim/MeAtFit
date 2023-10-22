%include "../rw32-2022.inc"

section .data
    letter_A dw 'A'
    letter_b dw 'b'
    

section .text
CMAIN:
    push ebp
    mov ebp, esp
    
    mov al, [letter_A]
    xor al, 32

    mov bl, [letter_b]
    xor bl, 32

    pop ebp
    ret
