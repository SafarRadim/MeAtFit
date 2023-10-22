%include "rw32-2022.inc"

section .data
    sCdecl   db  "sDecl", EOL, 0
    

section .text
; C std function
fn_cdecl:
    push    ebp
    mov     ebp, esp
    sub     esp, 8
    push    esi

    mov     esi, [ebp + 16]
    call    WriteString

    mov     eax, [ebp + 8]
    sub     eax, [ebp + 12]
    mov     [ebp - 4], eax

    mov     eax, [ebp + 8]
    add     eax, [ebp + 12]
    mov     [ebp - 8], eax

    mov     eax, [ebp - 4]
    imul    eax, [ebp - 8] 

    
    pop     esi
    mov     esp, ebp
    pop     ebp
    ret

; Pascal std function
fn_pascal:
    push    ebp
    mov     ebp, esp
    sub     esp, 8
    push    esi

    mov     esi, [ebp + 8]
    call    WriteString

    mov     eax, [ebp + 16]
    sub     eax, [ebp + 12]
    mov     [ebp - 4], eax

    mov     eax, [ebp + 16]
    add     eax, [ebp + 12]
    mov     [ebp - 8], eax

    mov     eax, [ebp - 4]
    imul    eax, [ebp - 8] 

    
    pop     esi
    mov     esp, ebp
    pop     ebp
    ret     3*4

; STDCALL std function
fn_stdcall:
    push    ebp
    mov     ebp, esp
    sub     esp, 8
    push    esi

    mov     esi, [ebp + 16]
    call    WriteString

    mov     eax, [ebp + 8]
    sub     eax, [ebp + 12]
    mov     [ebp - 4], eax

    mov     eax, [ebp + 8]
    add     eax, [ebp + 12]
    mov     [ebp - 8], eax

    mov     eax, [ebp - 4]
    imul    eax, [ebp - 8] 

    
    pop     esi
    mov     esp, ebp
    pop     ebp
    ret     3*4

; FASTCALL std function
fn_fastcall:
    push    ebp
    mov     ebp, esp
    sub     esp, 8
    push    esi

    mov     esi, [ebp + 8]
    call    WriteString

    mov     eax, ecx
    sub     eax, edx
    mov     [ebp - 4], eax

    mov     eax, ecx
    add     eax, edx
    mov     [ebp - 8], eax

    mov     eax, [ebp - 4]
    imul    eax, [ebp - 8] 

    
    pop     esi
    mov     esp, ebp
    pop     ebp
    ret     1*4



CMAIN:
    push    ebp
    mov     ebp, esp

    push    sCdecl
    push    dword 20
    push    dword 10
    call    fn_cdecl
    add     esp, 3*4 

    pop     ebp
    ret
