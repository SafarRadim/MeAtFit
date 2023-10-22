%include "rw32-2022.inc"
    jz      factorial_ret_1
    js      factorial_ret_1    

section .data

    

section .text

factorial:
    push    ebx
    mov     ebx, [esp+8]
    cmp     ebx, 1
    jbe     factorial_ret_1

    dec     ebx
    push    ebx
    call    factorial
    add     esp, 4
    imul    dword [esp+8]
    jmp     factorial_exit

factorial_ret_1:
    mov     eax, 1

factorial_exit:
    pop     ebx
    ret

;------

add_xy:
    push    ebx
    add     eax, ebx
    pop     ebx
    ret

;------

CMAIN:
    push    ebp
    mov     ebp, esp

    mov     eax, 10
    mov     ebx, 20
    call    add_xy

    mov     ebx, 3
    call    factorial

    pop     ebp
    ret
