%include "rw32-2022.inc"


section .data

    

section .text
CMAIN:
    push    ebp
    mov     ebp, esp
    
    mov     eax, 0xaabbccdd
    rol     ax,  8
    rol     eax, 16
    rol     ax,  8

    pop     ebp
    ret
