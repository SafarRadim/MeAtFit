%include "../rw32-2022.inc"

; (a*b - b*c + 10)/a

section .data
    a dd 20
    b dw 30
    c db 10
    y dw 0
    

section .text
CMAIN:
    push    ebp
    mov     ebp, esp

    movsx   eax, word [b]
    imul    dword [a]
    mov     ebx, eax
    mov     edi, edx

    movsx   eax, word [b]
    movsx   ecx, byte [c]
    imul    ecx
    
    sub     ebx, eax
    sub     edi, edx
    
    add     ebx, 10
    adc     edi, 0

    mov     eax, ebx
    mov     edx, edi
    idiv    dword [a]
    mov     [y], eax

    pop     ebp
    ret
