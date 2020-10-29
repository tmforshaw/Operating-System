
[org 0x7C00]

mov [BOOT_DISK], dl

# Create stack
mov bp, 0x7C00
mov sp, bp

call ReadDisk
jmp PROGRAM_SPACE

%include "src/16BitBoot/Print.asm"
%include "src/16BitBoot/DiscRead.asm"

# Declare as a bootloader
times 510-($-$$) db 0 ; Fill the rest of the sector with 0
dw 0xAA55 ; Define last two bytes as 0x55AA