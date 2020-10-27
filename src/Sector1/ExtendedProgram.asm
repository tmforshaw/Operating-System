
jmp EnterProtectedMode ; Enter 32-bit Protected Mode

%include "src/Sector1/GDT.asm"

EnterProtectedMode: ; Enter 32-bit Protected Mode
    call EnableA20
    cli ; Disable interrupts
    lgdt [gdt_descriptor] ; Load Global Descriptor Table
    mov eax, cr0
    or eax, 1
    mov cr0, eax
    jmp codeseg:StartProtectedMode

EnableA20:
    in al, 0x92
    or al, 2
    out 0x92, al
    ret

[bits 32]

%include "src/Sector0/Print.asm"
%include "src/Sector1/CPUID.asm"
%include "src/Sector1/SimplePaging.asm"

StartProtectedMode:
    mov ax, dataseg
    mov ds, ax
    mov ss, ax
    mov es, ax
    mov fs, ax
    mov gs, ax

    call DetectCPUID
    call DetectLongMode
    call SetupIdentityPaging
    call EditGDT
    jmp codeseg:Start64Bit

[bits 64]
[extern _StartKernel]

Start64Bit:
    mov edi, 0xB8000
    mov rax, 0x1F201F201F201F20
    mov ecx, 500
    rep stosq

    call _StartKernel

    jmp $

times 2048-($-$$) db 0 ; Fill the rest of the sectors with 0