# GCCPARAMS = -ffreestanding -mno-red-zone -m64 -Ttext 0x8000
GPPPARAMS = -m64 -fno-use-cxa-atexit -nostdlib -fno-builtin -fno-rtti -fno-exceptions -fno-leading-underscore -Ttext 0x8000 -std=c++2a
CUSTOMLDPARAMS = --oformat binary
ASMPARAMS = -f elf64

GCCCompiler = x86_64-elf-g++ $(GPPPARAMS)### This is the compiler and its parameters
CUSTOMLD = x86_64-elf-ld $(CUSTOMLDPARAMS)### This is the linker and its parameters
ASMCompiler = nasm $(ASMPARAMS)

OUTPUTPREFIX = Info: # Message to put before outputs

SRCDIR = src/
TEMPDIR = .temp/
OUTDIR = bin/
KERNELFILEDIR = Kernel_Files/

# Cpp files relative to Kernel_Files
cppFiles =  CommandLineInterface/CLI.cpp \
			CommandLineInterface/Commands.cpp \
			CommandLineInterface/Debug.cpp \
			Memory/Heap.cpp \
			InputOutput/Interrupts/IDT.cpp \
			InputOutput/IO.cpp \
			InputOutput/Keyboard/KBHandlers.cpp \
			.Kernel/Kernel.cpp \
			Memory/Memory.cpp \
			Memory/MemoryMap.cpp \
			InputOutput/Text/TextPrint.cpp \
			Types/String.cpp

# ASM files relative to $(SRCDIR)
asmFiles = $(KERNELFILEDIR)Misc/IncludeBinaries.asm \
64BitBoot/ExtendedProgram.asm

# Remove directories, remove extension, add .o, add directory
objects =   $(addprefix $(TEMPDIR),$(addsuffix .o,$(basename $(notdir $(asmFiles))))) \
			$(addprefix $(TEMPDIR),$(addsuffix .o,$(basename $(notdir $(cppFiles)))))


define compile_kernel # Compile the files
	@$(ASMCompiler) $(SRCDIR)16BitBoot/Boot.asm -f bin -o $(OUTDIR)Boot.bin
	

	@$(foreach file,$(asmFiles),$(ASMCompiler) $(addprefix $(SRCDIR),$(file)) -o $(addprefix $(TEMPDIR),$(addsuffix .o,$(basename $(notdir $(file)))));)


	@$(foreach file,$(cppFiles),$(GCCCompiler) -o $(addprefix $(TEMPDIR),$(addsuffix .o,$(basename $(notdir $(file))))) -c $(addprefix $(SRCDIR)$(KERNELFILEDIR),$(file));)
	
	
	@$(CUSTOMLD) -T $(SRCDIR)$(KERNELFILEDIR).Kernel/Linker.ld $(objects) -o $(OUTDIR)Kernel.bin

	@cat $(OUTDIR)Boot.bin $(OUTDIR)Kernel.bin > $(OUTDIR)Bootloader.bin

	@echo $(OUTPUTPREFIX)Compiled into binary
endef


define single_line_full_clean # Perform a full clean on one line
	rm -r $(TEMPDIR) && \
	mkdir $(TEMPDIR) && \
	\
	rm -r $(OUTDIR) && \
	mkdir $(OUTDIR)
endef


define full_clean # Perform a full clean
	@rm -r $(TEMPDIR)
	@mkdir $(TEMPDIR)

	@rm -r $(OUTDIR)
	@mkdir $(OUTDIR)
endef



define partial_clean
	@[ -f $(OUTDIR)Bootloader.bin ] && \
		rm -r $(TEMPDIR) && \
		mkdir $(TEMPDIR) && \
		mv $(OUTDIR)Bootloader.bin $(TEMPDIR) && \
		rm -r $(OUTDIR) && \
		mkdir $(OUTDIR) && \
		mv $(TEMPDIR)Bootloader.bin $(OUTDIR) && \
		echo $(OUTPUTPREFIX)Cleaned without removing Bootloader.bin \
	|| \
		($(call single_line_full_clean) && \
		echo $(OUTPUTPREFIX)Cleaned Fully)
endef


compile: # Compile the files
	$(call compile_kernel)


compile-c: # Run a compile then remove intermediate files
	$(call compile_kernel)
	$(call partial_clean)


run: # Compile and run the raw file
	$(call compile_kernel)

	@echo $(OUTPUTPREFIX)Running Bootloader.bin

	@qemu-system-x86_64 -drive file=$(OUTDIR)Bootloader.bin,format=raw


run-c: # Clean-compile and the raw file
	$(call compile_kernel)
	$(call partial_clean)

	@echo $(OUTPUTPREFIX)Running Bootloader.bin

	@qemu-system-x86_64 -drive file=$(OUTDIR)Bootloader.bin,format=raw


clean: # Remove all compiled files except Bootloader.bin
	$(call partial_clean)


clean-f: # Remove all compiled files
	$(call full_clean)

	@echo $(OUTPUTPREFIX)Cleaned Fully