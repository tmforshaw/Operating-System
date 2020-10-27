
PrintString:
    mov ah, 0x0E ; Move magic number into higher part of A register
    .Loop:
    cmp [bx], byte 0 ; Compare the value of B register with 0
    je .Exit ; If it is zero jump to exit, otherwise continue
        mov al, [bx]
        int 0x10
        inc bx ; Increment the part of bx that we are looking at
        jmp .Loop
    
    .Exit:
    ret