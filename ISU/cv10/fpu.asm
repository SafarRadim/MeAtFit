%include "rw32-2022.inc"

section .data
    a   dd 1.0  ; float         16 bit
    b   dq 2.0  ; double        32 bit
    c   dt 3.0  ; long oduble   80 bit on linux
    d   dd 4    ; int           32 bit
    e   dw 5    ; short         16 bit
    

section .text
CMAIN:
    push    ebp
    mov     ebp, esp

    fld     dword [a]
    
    push    5
    fild    dword [a]
    add     esp, 4    

    fld1

    ; fadd
    fadd    st0, st2    ; st0 = st0 + st2
    fadd    st2, st0    ; st2 = st2 + st0

    fadd    st1         ; st0 = st1 + st0

    fadd    qword [b]   ; st0 = st0 + [b]

    faddp   st2         ; st2 = st2 + st0 & pop
    
    faddp   st1, st0    ; st1 = st1 + st0 & pop

    ; fsub
    fsub    st0, st2    ; st0 = st0 - st2
    fsubr   st0, st2    ; st0 = st2 - st0
    fsubp   st2         ; st2 = st2 - st0

    ; div mul etc. -- same

    fabs                ; abs of st0
    fchs                ; st0 = -st0
    fsqrt               ; sqrt of st0
 

    pop     ebp
    ret
