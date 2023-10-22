%include "rw32-2022.inc"

section .data
    array1      dd  10, -5, 0, 25, -10
    array1_len  dd  5

    str_sep     db  ", ",0
    str_avg     db  "Avg: ", 0

section .text
;==writeArray==
writeArray:
    push    ebx
    push    eax
    push    ecx
    push    esi
    push    edi
    push    edx
    pushfd

    xor     edx, edx
    xor     edi, edi
    mov     ebx, 0
    dec     ecx

    or      esi, esi
    jz      wAerror
    cmp     ecx, 0
    jng     wAerror

wAloop:
    cmp     ebx, ecx
    jz      wAEnd
    mov     eax, [esi + 4 * ebx]
    call    WriteInt32
    call    wAwriteSep
    inc     ebx
    call    wAsetSum
    jmp     wAloop   

wAEnd:
    mov     eax, [esi + 4 * ebx]
    call    WriteInt32
    call    wAsetSum
    mov     al, EOL
    call    WriteChar

    call    wAgetAvg
    call    wAwriteAvg

    popfd
    pop     edx
    pop     edi
    pop     esi
    pop     ecx
    pop     eax
    pop     ebx
    ret


    mov     esi, edi
wAerror:
    mov     eax, 0x80000000;
    ret


;-Avg-

wAsetSum:
    add     edx, eax
    adc     edi, 0
    ret

wAgetAvg:
    inc     ecx
    mov     eax, edx
    mov     edx, edi
    idiv    ecx
    dec     ecx
    ret
    
wAwriteAvg:
    push    esi
    push    eax

    mov     esi, str_avg
    call    WriteString
    call    WriteInt32
    mov     al, EOL
    call    WriteChar

    pop     eax
    pop     esi

    ret
    

wAwriteSep:
    push    esi
    mov     esi, str_sep
    call    WriteString
    pop     esi
    ret

;==writeArray==

CMAIN:
    push    ebp
    mov     ebp, esp

    mov     ecx, [array1_len]
    mov     esi, array1

    call    writeArray

    pop     ebp
    ret
