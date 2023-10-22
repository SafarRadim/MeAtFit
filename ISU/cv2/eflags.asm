%include "rw32-2022.inc"

section .data

section .text
CMAIN:
    push ebp
    mov ebp,esp
    
    ; Removing ZF
    mov al,1
    add al,0

    ; Adding ZF, PF
    mov ah,0
    add ah,0 

    ; Adding SF
    sub ah,1

    ; Adding CF, OF
    mov ah,128
    add ah,128

    pop ebp
    ret
