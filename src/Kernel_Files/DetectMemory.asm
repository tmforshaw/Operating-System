
MemoryRegionCount:
    db 0
    GLOBAL MemoryRegionCount

DetectMemory:
    mov ax, 0
    mov es, ax
    mov di, 0x5000 ; Store memory map here
    mov edx, 0x534D4150 ; Magic number for S-Map
    mov ebx, 0

    .Repeat:
        mov eax, 0xE820
        mov ecx, 24 ; Size of each Memory Table Listing (bytes)
        int 0x15

        cmp ebx, 0
        je .Finished

        add di, 24
        inc byte [MemoryRegionCount]
        jmp .Repeat

    .Finished:
    ret