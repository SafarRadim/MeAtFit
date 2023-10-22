%include "rw32-2022.inc"

section .data
    False   db "False",EOL,0
    True    db "True",EOL,0
    

section .text

writeZF:
    je      writeZF_false
    mov     esi, False
    call    WriteString
    ret

writeZF_false:
    mov     esi, True
    call    WriteString
    ret


CMAIN:
    push    ebp
    mov     ebp, esp

    mov     eax, 10
    cmp     eax, 10
    call    writeZF

    mov     eax, 10
    cmp     eax, 11
    call    writeZF    

    pop     ebp
    ret
