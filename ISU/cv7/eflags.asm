%include "rw32-2022.inc"

section .data
    str_true    db "True",EOL,0
    str_false   db "False",EOL,0
    
    str_zero    db "Zero: ",0
    str_carry   db "Carry: ",0
    str_sign    db "Sign: ",0
    str_over    db "Overflow: ",0
     

section .text

writeTrue:
    mov     esi, str_true
    call    WriteString
    ret

writeFalse:
    mov     esi, str_false
    call    WriteString
    ret

writeFlags:
    mov     esi, str_zero
    call    WriteString 
    jz      writeZeroTrue
    call    writeFalse
retZero:
    mov     esi, str_carry
    call    WriteString
    jc      writeCarryTrue
    call    writeFalse    
retCarry:
    mov     esi, str_sign
    call    WriteString
    js      writeSignTrue
    call    writeFalse    
retSign:
    mov     esi, str_over
    call    WriteString
    jo      writeOverTrue
    call    writeFalse
retOver:
    ret

writeZeroTrue:
    call    writeTrue
    jmp     retZero

writeCarryTrue:
    call    writeTrue
    jmp     retCarry

writeSignTrue:
    call    writeTrue 
    jmp     retSign

writeOverTrue:
    call    writeTrue 
    jmp     retOver


CMAIN:
    push    ebp
    mov     ebp, esp

    mov     eax, 10
    sub     eax, 11

    call    WriteFlags
    call    writeFlags

    pop     ebp
    ret
