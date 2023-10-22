%include "../rw32-2022.inc"

section .data
    ; 32b
    x dd 68754
    y dd 2006
    z dd 0

section .text
CMAIN:
    push ebp
    mov ebp, esp

    sub al, al

    mov al, [x]
    adc al, [y]
    mov [z], al
        
    mov al, [x + 1]
    adc al, [y + 1]
    mov [z + 1], al

    mov al, [x + 2]
    adc al, [y + 2]
    mov [z + 2], al

    mov al, [x + 3]
    adc al, [y + 3]
    mov [z + 3], al


    pop ebp
    ret
