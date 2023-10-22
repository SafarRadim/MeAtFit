%include "rw32-2022.inc"

section .data
    task21A dw 384,304,16,96,16,288,-736,-656
    task21B dw 38094,45722,62362,59911,18394,30239,59390,25086
    task22A dw 384,304,16,96,16,288,-736,-656
    task22B dw 57430,52554,45682,15524,26776,25428,43266,34456
    task23A dd 6144,4864,256,1536,256,4608,-11776,-10496

section .text
CMAIN:
    push    ebp
    mov     ebp, esp

    mov     eax, task21A
    mov     BX, 16
    mov     ecx, 8
    call    task21

    
    ; eax = task22(task22A,8,0)
    push    dword 16
    push    dword 8
    push    task22A

    call    task22

    
    mov     ecx, 5
    call    task23

    pop     ebp
    ret    
;
;--- Task 1 ---
;
; Create a function 'task21' to find if there is a value in an array of the 16bit unsigned values.  
; Pointer to the array is in the register EAX, the value to be found is in the register BX 
; and the count of the elements of the array is in the register ECX.
;
; Function parameters:
;   EAX = pointer to the array of the 16bit unsigned values (EAX is always a valid pointer)
;   BX = 16bit unsigned value to be found
;   ECX = count of the elements of the array (ECX is an unsigned 32bit value, always greater than 0)
;
; Return values:
;   EAX = 1, if the value has been found in the array, otherwise EAX = 0
;
; Important:
;   - the function does not have to preserve content of any register
;

task21:
    ; insert your code here
    push    ebp
    mov     ebp, esp
    
.loop:
    push    ecx
    dec     ecx
    cmp     bx, [eax + ecx * 2]
    je      .foundval
    pop     ecx
    loop    .loop
    mov     eax, 0

.end:
    mov     esp, ebp
    pop     ebp
    ret


.foundval:
    mov     eax, 1
    jmp     .end

;
;--- Task 2 ---
;
; Create a function: void* task22(const unsigned short *pA, int N, unsigned short x) to search an array pA of N 16bit unsigned
; values for the first occurrence of the value x. The function returns pointer to the value in the array.
; The parameters are passed, the stack is cleaned and the result is returned according to the CDECL calling convention.
;
; Function parameters:
;   pA: pointer to the array A to search in
;    N: length of the array A
;    x: value to be searched for
;
; Return values:
;   EAX = 0 if the pointer pA is invalid (pA == 0) or N <= 0 or the value x has not been found in the array
;   EAX = pointer to the value x in the array (the array elements are indexed from 0)
;
; Important:
;   - the function MUST preserve content of all the registers except for the EAX and flags registers.
;


; pA    +8
; N     +12
; x     +16
task22:
    ; insert your code here
    push    ebp
    mov     ebp, esp

    ; save regs
    push    ecx
    push    esi
    push    ebx
    push    edx

    ; set vals
    mov     eax, 0
    mov     ecx, [ebp + 12]
    mov     ebx, [ebp + 16]

    ; value check
    cmp     dword [ebp + 8], 0
    je      .exit

    cmp     dword [ebp + 12], 0
    jle     .exit

.loop:
    push    ecx
    dec     ecx
    shl     ecx, 1
    mov     esi, [ebp + 8]
    lea     edx, [esi + ecx]
    cmp     bx, [esi + ecx]
    cmove   eax, edx
    pop     ecx
    loop    .loop

.exit:
    ; load regs
    pop     edx
    pop     ebx
    pop     esi
    pop     ecx

    mov     esp, ebp
    pop     ebp
    ret

;
;--- Task 3 ---
;
; Create a function 'task23' to allocate and fill an array of the 32bit unsigned elements by the elements
; of the Padovan sequence P(0), P(1), ... , P(N-1). Requested count of the elements of the Padovan sequence
; is in the register ECX (32bit signed integer) and the function returns a pointer to the array
; allocated using the 'malloc' function from the standard C library in the register EAX.
;
; Elements of the Padovan sequence are defined as follows:
;
;   P(0) = 1
;   P(1) = 1
;   P(2) = 1
;   P(n) = P(n-2) + P(n-3)
;
; Function parameters:
;   ECX = requested count of the elements of the Padovan sequence (32bit signed integer).
;
; Return values:
;   EAX = 0, if ECX <= 0, do not allocate any memory and return value 0 (NULL),
;   EAX = 0, if memory allocation by the 'malloc' function fails ('malloc' returns 0),
;   EAX = pointer to the array of N 32bit unsigned integer elements of the Padovan sequence.
;
; Important:
;   - the function MUST preserve content of all the registers except for the EAX and flags registers,
;   - the 'malloc' function may change the content of the ECX and EDX registers.
;
; The 'malloc' function is defined as follows:
;
;   void* malloc(size_t N)
;     N: count of bytes to be allocated (32bit unsigned integer),
;     - in the EAX register it returns the pointer to the allocated memory
;     - in the EAX register it returns 0 (NULL) in case of a memory allocation error,
;     - the function may change the content of the ECX and EDX registers.

; param +8

CEXTERN malloc

task23:
    ; insert your code here
    push    ebp
    mov     ebp, esp

    ; reg save
    push    ecx
    push    edx
    push    edi

    ; set val
    mov     edi, -1

    ; val check
    cmp     ecx, 0
    jle     .err    

    ; malloc
    shl     ecx, 2
    push    ecx
    call    malloc
    pop     ecx
    shr     ecx, 2
    cmp     eax, 0
    je      .err

.loop:
    inc     edi
    cmp     edi, 2
    jle     .setOne
    mov     edx, [eax + edi * 4 - 8]
    add     edx, [eax + edi * 4 - 12]
    mov     [eax + edi * 4], edx
    cmp     edi, ecx
    jne     .loop

.exit:
    ; reg load
    pop     edi
    pop     edx
    pop     ecx

    mov     esp, ebp
    pop     ebp
    ret

.setOne:
    mov     [eax + edi * 4], dword 1
    cmp     edi, ecx
    je      .exit
    jmp     .loop

.err:
    mov     eax, 0
    jmp     .exit

