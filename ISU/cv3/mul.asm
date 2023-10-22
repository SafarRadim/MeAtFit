%include "../rw32-2022.inc"

section .data
    x dw 50
    y dw 10
    z dd 0


section .text
CMAIN:
    push ebp
    mov ebp, esp

    mov ax, [x]
    mul word[y]
    mov [z], ax
    mov [z + 2], dx

    pop ebp
    ret
