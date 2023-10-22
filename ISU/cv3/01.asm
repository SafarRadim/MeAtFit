%include "../rw32-2022.inc"

section .data
    x dw 10
    y dw 5
    z dw 0
    

section .text
CMAIN:
    push ebp
    mov ebp, esp

    mov ax, [x]
    imul word [y]

    mov bx, ax
    mov ax, 100
    imul word [x]
    add ax, bx

    sub ax, 50

    mov bx, 100
    cwd
    idiv bx

    mov [z], ax

    pop ebp
    ret
