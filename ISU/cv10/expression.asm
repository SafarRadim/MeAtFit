%include "rw32-2022.inc"

section .data

 

section .text

; === EXPR ===
; a     +8
; b     +12
; c     +16
expr:
    push    ebp
    mov     ebp, esp

    fld1
    
    fld     dword [ebp+8]
    fld     dword [ebp+12]
    fmulp
    fabs

    faddp

    push    dword 100
    fild    dword [esp]
    fld     dword [ebp+16]
    
    fld     dword [ebp+8]
    fld     dword [ebp+12]
    faddp
    fsqrt

    fdivp
    fsubp

    fld     dword [ebp+8]
    fld     dword [ebp+12]
    fld     st1
    fld     dword [ebp+16]
    fld1

    fsubp
    faddp
    fmulp
    faddp

    fmulp
    
    mov     esp, ebp
    pop     ebp
    ret

; === END EXPR ===

CMAIN:
    push    ebp
    mov     ebp, esp

    push    __float32__(100.0)
    push    __float32__(10.0)
    push    __float32__(1.0)
    call    expr
    add     esp, 12

    call    WriteDouble
    call    WriteNewLine

    pop     ebp
    ret
