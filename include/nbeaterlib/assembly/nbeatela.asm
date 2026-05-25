.code

NBEATERREADMEMORY PROC
    mov r10, rcx
    mov eax, 63
    syscall
    ret
NBEATERREADMEMORY ENDP

NBEATERWRITEMEMORY PROC
    mov r10, rcx
    mov eax, 58
    syscall
    ret
NBEATERWRITEMEMORY ENDP

NBEATERPROTECTMEMORY PROC
    mov r10, rcx
    mov eax, 80
    syscall
    ret
NBEATERPROTECTMEMORY ENDP

END