%include "rw32-2022.inc"

section .data
    a   db -128
    b   dd -2147483648
    c   dw -32768
    d   dw -32768
    e   dw -32768

    q   dd 0
    r   dd 0
    

section .text
CMAIN:
    push    ebp
    mov     ebp, esp

    xor     edx, edx
    movsx   eax, byte [a]
    imul    dword [b]
    movsx   ebx, word [c]
    add     eax, ebx
    adc     edx, 
    sub     eax, 30
    sbb     edx, 0

    push    eax
    push    edx
    
    movsx   eax, word [d]
    imul    eax, 5
    movsx   ebx, word [e]
    add     eax, ebx
    sub     eax, 100
    mov     ebx, eax
    
    pop     edx
    pop     eax
    cdq
    idiv    ebx
    
    mov     [q], eax
    mov     [r], edx

    pop     ebp
    ret
