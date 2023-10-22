%include "rw32-2022.inc"

section .data

    arrayA  dd  1, 2, 3, 4
    arrayB  dd  2, 3, 4, 5

    sEnd    db  EOL, 0

section .text
; == Extern ==

CEXTERN malloc
CEXTERN free

; == Extern end ==

; == mySum ==
; +8    a
; +12   b
mySum:
    push    ebp
    mov     ebp, esp
    
    mov     eax, [ebp + 8]
    add     eax, [ebp + 12]

    mov     esp, ebp
    pop     ebp 
    ret

; == mySum end ==


; == myMult ==
; +8    a
; +12   b
myMult:
    push    ebp
    mov     ebp, esp
    
    mov     eax, [ebp + 8]
    imul    eax, [ebp + 12]

    mov     esp, ebp
    pop     ebp
    ret

; == myMult end ==


; == modify ==
; +20   transform [function]
; +16   N
; +12   b [array]
; +8    a [array]
modify:
    push    ebp
    mov     ebp, esp
    push    ecx
    push    esi


    mov     eax, [ebp + 16]
    shl     eax, 2
    push    eax
    call    malloc
    add     esp, 4
    mov     esi, eax


    mov     ecx, [ebp + 16]

modifyLoop:
    push    ecx
    dec     ecx
    mov     eax, [ebp + 12]
    push    dword [eax + ecx * 4]
    
    mov     eax, [ebp + 8]
    push    dword [eax + ecx * 4]


    call    dword [ebp + 20]
    add     esp, 2*4
    
    mov     [esi + ecx * 4], eax
    pop     ecx   
 
    loop    modifyLoop
    
    mov     eax, esi   
 
    pop     esi
    pop     ecx
    mov     esp, ebp
    pop     ebp
    ret

; == modify end ==

CMAIN:
    push    ebp
    mov     ebp, esp

    push    mySum
    push    dword 4
    push    arrayB
    push    arrayA

    call    modify

    add     esp, 4*4

    mov     esi, eax
    mov     ecx, 4
    call    WriteArrayInt32

    mov     esi, sEnd
    call    WriteString

    push    eax
    call    free
    add     esp, 4

    pop     ebp
    ret
