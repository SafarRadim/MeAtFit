%include "../rw32-2022.inc"

section .data
    x dw 1006
    y dw 10
    x_div_y dw 0
    x_mod_y dw 0
    

section .text
CMAIN:
    push ebp
    mov ebp, esp

    mov ax, [x]
    cwd
    div word [y]
    mov [x_div_y], ax
    mov [x_mod_y], dx

    pop ebp
    ret
