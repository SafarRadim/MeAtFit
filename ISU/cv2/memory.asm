%include "rw32-2022.inc"

section .data
    ; Byte Array
    x db 1, 2, 3, 4
    
    ; Word Array
    y dw 1, 2, 3, 4


section .text
CMAIN:
    push ebp
    mov ebp,esp

    mov al, [x]
    add al, [x + 1]
    add al, [x + 2]
    add al, [x + 3]

    mov bx, [y]
    add bx, [y + 2]
    add bx, [y + 4]
    add bx, [y + 6]  

    pop ebp
    ret
