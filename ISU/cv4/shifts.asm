%include "../rw32-2022.inc"

section .data
    i db 5
    j db 3
    k dw 0
    

section .text
CMAIN:
    push    ebp
    mov     ebp, esp

    xor     ax, ax
    movzx   bx, byte [i]
    mov     cl, [j]
    
    shr     cl
    xor     dx, dx
    sbb     dx
    and     dx, bx
    add     ax, bx

    shl     bx
    shr     cl
    xor     dx, dx
    sbb     dx, 0
    and     dx, bx
    add     ax, bx

    mov     [k], ax

    pop     ebp
    ret
