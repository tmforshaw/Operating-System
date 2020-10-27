
PROGRAM_SPACE equ 0x7E00 ; Define PROGRAM_SPACE as memory address 0x7E00

ReadDisk:
    mov ah, 0x02
    mov bx, PROGRAM_SPACE
    mov al, 4 ; This defines the number of sectors
    mov dl, [BOOT_DISK]
    mov ch, 0x00
    mov dh, 0x00
    mov cl, 0x02

    int 0x13

    jc DiskReadFailed

    ret

BOOT_DISK:
    db 0

DiskReadErrorString:
    db 'Disk Read Failed.', 0

DiskReadFailed:
    mov bx, DiskReadErrorString
    call PrintString
    
    jmp $