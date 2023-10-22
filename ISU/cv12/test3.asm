%include "rw32-2022.inc"

section .data
    task31_array dd 1.5,-100.0,30.0,-5.5,1.0,-1.0,200.0
    task31_array_len dd 7
    task31_error dd -1

section .text

task31_fn_example:
    finit
    fld dword [esp+4]
    xor ecx,-1
    xor edx,-1
    xor eax,-1
    ret

CMAIN:
    push ebp
    mov ebp,esp
    sub esp,4

    ; un-mask FPU exceptions, comment it if you do not want it
    fstcw [esp]
    and [esp],word 1111_1111_1110_0000b
    fldcw [esp]

    ; the function calls below should be correct,
    ; if you feel you need to modify it
    ; you might be doing something wrong
    
    push task31_error
    push task31_fn_example
    push dword [task31_array_len]
    push task31_array
    call task31
    add esp,16
    
    push __float32__(21.250)
    push __float32__(16.250)
    call task32
    add esp,8


    mov esp,ebp
    pop ebp
    ret
;
;--- Task 1 ---
;
; Create a function: float task31(const float *array, long N, float (*f)(float), long *error) using the
; CDECL calling convention to calculate the arithmetic mean of the 'N' elements of an 'array' of the floats
; transformed by the function 'f'. In case of any failure set the 'error' value if possible.
;
;      1   N-1        a[0] + a[1] + ... + a[N-1]
; A = ------ * ? a[i] = ------------------------------------------------------------
;      N   i=0                  N
;
; Algorithm:
;   1) check conditions, calculate error code if no errors occured continue, 
;      otherwise set error code, if possible, and return +QNaN
;   2) set acc = 0
;   3) for i = 0..N-1: acc += f(array[i]), if f(array[i]) returns NaN => set *error=8 and return +QNaN
;   4) set *error=0 and return acc/N
;
; Function parameters:
;   array = pointer to the array of floats
;   N = count of the elements in the array (long, 32bit signed integer)
;   f = pointer to a function (C language calling convention CDECL) calculating value of f(x), where
;       x is a float and the function returns a float or QNaN in case f(x) is not defined for the given x
;   error = pointer to a long value, if valid, you will store the error code there
;
; Return values:
;    * arithmetic mean of the transformed array in case nothing has gone wrong
;    * +QNaN in case of any error
;    * if (error != NULL) then
;        *error = 0, if there was no error
;        *error = 1, if (                    array == 0)
;        *error = 2, if (          N <= 0              )
;        *error = 3, if (          N <= 0 && array == 0)
;        *error = 4, if (f == 0                        )
;        *error = 5, if (f == 0           && array == 0)
;        *error = 6, if (f == 0 && N <= 0              )
;        *error = 7, if (f == 0 && N <= 0 && array == 0)
;        *error = 8, if the function f(x) returned NaN for some x

;
; Important:
;   * you can use the logical OR to set multiple choices of the error code
;   * in the CDECL calling convention:
;       * the called function expects a clean FPU stack and can erase it
;       * the called function cleans the FPU before returning back (except for the st0 if it returns a float/double)
;       * the called function may change EAX, ECX and EDX without keeping its original value


%define array ebp + 8
%define N ebp + 12
%define func ebp + 16
%define error ebp + 20

QNAN EQU 0x7fc00001

task31:
    push    ebp
    mov     ebp, esp
    
    push    ebx
    push    esi

    ; acc
    sub     esp, 4
    
    ; errRet
    mov     edx, [ebp + 20]
    mov     [edx], dword 0

    ; arr
    mov     esi, [ebp + 8]

    ; error
    
    cmp     [ebp + 8], dword 0
    je      .arrayNull
    
.lenCheck:
    cmp     [ebp + 12], dword 0
    jle     .zeroLen

.funCheck:
    cmp     [ebp + 16], dword 0
    je      .funNull
    
.endErrorCheck:
    cmp     [edx], dword 0
    jne     .error

    mov     ebx, 0
    mov     ecx, [ebp + 12]

.loop:
    push    ecx
    push    edx
    push    dword [esi + 4 * ecx - 4]
    call    dword [ebp + 16]
    add     esp, 4
    pop     edx
    pop     ecx
    
    fxam
    fstsw   ax

    and     ah, 0x45
    cmp     ah, 0x01
    je      .isNan

    fstp    dword [ebp - 4]
    add     ebx, dword [ebp - 4]

    loop    .loop

    push    ebx
    fld     dword [esi]
    add     esp, 4
    
    fild    dword [ebp + 12]

    fdivp



.leave:
    pop     esi
    pop     ebx
    mov     esp, ebp
    pop     ebp
    ret

; helper jumps

.arrayNull:
    add      [edx], dword 1
    jmp     .lenCheck

.zeroLen:
    add     [edx], dword 2
    jmp     .funCheck

.funNull:
    add     [edx], dword 4
    jmp     .endErrorCheck

.error:
    push    dword QNAN
    fld     dword [esp]
    add     esp, 4
    jmp     .leave

.isNan:
    push    dword QNAN
    fld     dword [esp]
    add     esp, 4
    add     [edx], dword 8
    jmp     .leave

;
;--- Task 2 ---
;
; Create a function: float task32(float x, float y) to evaluate a function f(x,y) for the given input (x, y)
; using FPU. The function returns the result of the evaluation in the ST0 FPU register. The function receives its
; parameters x and y on the stack in order from the last to the first, the stack is cleared by the caller and the
; result is returned in the ST0 FPU register (C calling convention - CDECL). 
;
; The function f(x,y) is defined as follows:
;
;
;            sqrt(x + 6.250) - sin(x - pi*y)
; f(x,y) = ---------------------------------------------
;                    abs(x - y*2)
;
; Function parameters:
;   x: IEEE754 single precision 32bit floating point number (float),
;   y: IEEE754 single precision 32bit floating point number (float),
;
; Return values:
;   ST0 = f(x,y), ignore exceptions caused by the division by zero or square root of a negative number
;
; Important:
;   - you are not allowed to use the following FPU instructions: FINIT, FNINIT, FCLEX, FNCLEX, FFREE, FINCSTP, FDECSTP,
;     FLDCW, FLDENV, FRSTOR, FXRSTOR,
;   - assume that the FPU has already been initialized by the FINIT instruction,
;   - the function MUST preserve content of all the registers except for the EAX, ECX, EDX and flags registers,
;   - the function MUST empty the FPU stack except for the ST0 register, in which the function returns its result.

%define x ebp + 8
%define y ebp + 12
task32:
    ; insert your code here
    push    ebp
    mov     ebp, esp

    
    fld     dword [x]
    push    __float32__(6.250)
    fld     dword [esp]
    add     esp, 4
    faddp
    fsqrt
    
    fld     dword [x]
    fldpi
    fld    dword [ebp + 12]
    fmulp
    fsubp
    fsin

    fsubp

    fld     dword [x]
    fld     dword [ebp + 12]
    push    dword 2
    fild    dword [esp]
    add     esp, 4
    fmulp
    fsubp
    fabs
    
    fdivp

    mov     esp, ebp
    pop     ebp
    ret
