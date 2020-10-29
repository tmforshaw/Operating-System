
jmp EnterProtectedMode ; Enter 32-bit Protected Mode

%include "src/64BitBoot/GDT.asm"
%include "src/16BitBoot/Print.asm"
%include "src/Kernel_Files/Memory/DetectMemory.asm"
; %include "src/16BitBoot/DiscRead.asm" ; Include if you need access to sector amount

EnterProtectedMode: ; Enter 32-bit Protected Mode
    
    call DetectMemory
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

%include "src/64BitBoot/CPUID.asm"
%include "src/64BitBoot/SimplePaging.asm"

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

%include "src/Kernel_Files/InputOutput/Interrupts/IDT.asm"

Start64Bit:
    mov edi, 0xB8000
    mov rax, 0x0F000F000F000F00
    mov ecx, 500
    rep stosq

    call ActivateSSE

    call _StartKernel

    jmp $

ActivateSSE:
    mov rax, cr0
    and ax, 0b11111101
    or ax, 0b00000001
    mov cr0, rax

    mov rax, cr4
    or ax, 0b1100000000
    mov cr4, rax

    ret


times 8*512-($-$$) db 0 ; Fill the rest of the sectors with 0