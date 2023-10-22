%include "rw32-2022.inc"

section .data
    cw  dw
    

section .text
CMAIN:
    push    ebp
    mov     ebp, esp

; basic "cmp"

    fld1
    fldpi
    fcom                ; compare st0 st1
    
    fstsw   ax          ; mov fpu fstat to [mem16/ax]    
    sahf                ; mov ax into eflags
;   fcomi               ; cmp st0 st1 and mov fstat into eflags

;   fxam                ; "tests" st0 and changes fstat accordingly
;   fsts                ; cmp st0 0.0

    ja      .st_0_bigger
    jb      .st_1_bigger
    je      .equal

.st_0_bigger:
    nop

.st_1_bigger:
    nop

.equal:
    nop

; unmasking fpu error - comment out at first
    fstcw   [cw]
    and     word [cw], 0xfff0
    fldcw   [cw]



; Check if num is NaN

    fld1
    fchs
    fsqrt               ; make NaN - unless unmasked

    fld0
    fdiv    st1, st0    ; make inf - unless unmasked (div by 0)

    FPU_INF_POS EQU 0x7F800000  ; making some constatnts, keyword EQU
    FPU_INF_NEG EQU 0xFF800000

    FPU_QNAN_POS EQU 0x7FC00001
    FPU_QNAN_NEG EQU 0xFFC00001
    
    push    dword FPU_QNAN_POS
    fld     dword [esp]
    add     esp, 4

    fxam
    fstsw   ax
   
    and     ah, 0x45    ; set mask for NaN 
    cmp     ah, 0x01
    je      .is_nan

    nop

.is_nan:
    nop
    

    pop     ebp
    ret
