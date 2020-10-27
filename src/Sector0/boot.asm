
[org 0x7C00]

mov [BOOT_DISK], dl

# Create stack
mov bp, 0x7C00
mov sp, bp

call ReadDisk

jmp PROGRAM_SPACE

%include "src/Sector0/Print.asm"
%include "src/Sector0/DiscRead.asm"

# Declare as a bootloader
times 510-($-$$) db 0 ; Fill the rest of the sector with 0
db 0x55, 0xAA ; Define last two bytes as 0x55AA