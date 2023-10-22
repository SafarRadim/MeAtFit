%include "rw32-2022.inc"

section .data
    array    dd  5.0, -10.0, -125.0, -1.0, 25.0
    arrayLen dd  5

section .text

CEXTERN malloc

;   float *map(const float *array, long N, float (f*) (float)))){
;       float *newArray = (float*) malloc (N * sizeof(float));
;       for (long i=0; i<N; i++){
;           float tmp = (f*) (array[i]);
;           if (isnan(tmp)){
;               newArray[i] = inf;
;           } else {
;               newArray[i] = tmp;
;           }
;       }
;       return newArray;
;   }    
 
MY_FN:
    fld     dword [esp+4]
    fsqrt
    ret

%define arg_array ebp+8
%define arg_arrayLen ebp+12
%define arg_fn ebp+16

MAP:
    push    ebp
    mov     ebp, esp

    mov     eax, [arg_arrayLen]
    shl     eax, 2                   ; float 32-bit
    push    eax
    call    malloc
    add     esp, 4

    mov     ecx, [arg_arrayLen]
    mov     esi, [arg_array]

.map_loop:
    push    eax
    push    ecx    
    push    dword [esi + ecx * 4 - 4]
    call    dword [arg_fn]
    add     esp, 4
    pop     ecx
    pop     eax

    xchg    eax, edx
    fxam
    fstsw   ax
    and     ax, 0x4500
    xchg    eax, edx
    je      .is_nan
    fstp    dword [eax + ecx * 4 - 4]

.map_loop_end:
    loop    .map_loop

    pop     ebp
    ret

.is_nan:
    mov     edx, 0x7F800000
    mov     [eax + ecx * 4 - 4], edx
    jmp     .map_loop_end
    

CMAIN:
    push    ebp
    mov     ebp, esp

    push    MY_FN
    push    dword [arrayLen]
    push    dword array
    call    MAP
    add     esp, 2*4
     
    pop     ebp
    ret
