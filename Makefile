GPPPARAMS = -m64 -Ttext 0x8000 -fno-use-cxa-atexit -nostdlib -fno-builtin -fno-rtti -fno-exceptions -fno-leading-underscore
LDPARAMS = --oformat binary

SRCDIR = src/
TEMPDIR = .temp/
OUTDIR = bin/

run: # Run the raw file
	nasm $(SRCDIR)Sector0/boot.asm -f bin -o $(OUTDIR)boot.bin
	nasm $(SRCDIR)Sector1/ExtendedProgram.asm -f elf64 -o $(TEMPDIR)ExtendedProgram.o
	
	g++ $(GPPPARAMS) -o $(TEMPDIR)Kernel.o -c $(SRCDIR)Kernel.cpp

	ld -T $(SRCDIR)linker.ld $(LDPARAMS)

	cat $(OUTDIR)boot.bin $(OUTDIR)Kernel.bin > $(OUTDIR)bootloader.flp

	qemu-system-x86_64 -drive file=$(OUTDIR)bootloader.flp,format=raw


	# # Getting an error saying the disk is not aligned on a sector boundary
	# # The disk size is 2664 bytes
	# VBoxManage convertdd $(OUTDIR)bootloader.flp $(OUTDIR)bootloader.vdi


.PHONEY: clean
clean:
	rm -r $(OUTDIR)
	mkdir $(OUTDIR)

	rm -r $(TEMPDIR)
	mkdir $(TEMPDIR)