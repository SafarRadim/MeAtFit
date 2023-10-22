%include "rw32-2022.inc"

section .data
    sPrintf  db  "%c: result = %d", EOL, 0
    

section .text
CEXTERN printf

; == calc ==
; +16 -- control char
; +12 -- b
; +8  -- a
calc:
    push    ebp
    mov     ebp, esp
   

    xor     eax, eax 
    cmp     [ebp + 16], byte 'a'
    je      calc_switch_a
    cmp     [ebp + 16], byte 's'
    je      calc_switch_s
    cmp     [ebp + 16], byte 'm'
    je      calc_switch_m
    mov     eax, 0x80000000

calc_exit:
    mov     esp, ebp
    pop     ebp
    ret

calc_switch_a:
    mov     eax, [ebp + 8]
    add     eax, [ebp + 12]
    jmp     calc_exit

calc_switch_s:
    mov     eax, [ebp + 8]
    sub     eax, [ebp + 12]
    jmp     calc_exit

calc_switch_m:
    mov     eax, [ebp + 8]
    imul    eax, [ebp + 12]
    jmp     calc_exit

; == end calc ==

CMAIN:
    push    ebp
    mov     ebp, esp
    
    push    'a'
    push    dword 10
    push    dword 20
    call    calc
    add     esp, 3*4

    push    eax
    push    'a'
    push    sPrintf
    call    printf
    add     esp, 3*4

    pop     ebp
    ret
