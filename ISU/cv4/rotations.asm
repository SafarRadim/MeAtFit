%include "../rw32-2022.inc"

section .data
    i dd 0x11223344

section .text
CMAIN:
    push    ebp
    mov     ebp, esp

    mov     eax, [i]

    ror     ax, 8
    ror     eax, 16
    ror     ax, 8

    pop     ebp
    ret
