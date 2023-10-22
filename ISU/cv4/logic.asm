%include "../rw32-2022.inc"

section .data
    x db 10000011b
    

section .text
CMAIN:
    push ebp
    mov ebp, esp

    mov al, [x]
    and al, 11111001b
    or  al, 11110000b
    xor al, 01111110b

    pop ebp
    ret
