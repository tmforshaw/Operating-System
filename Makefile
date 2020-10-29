GPPPARAMS = -m64 -Ttext 0x8000 -fno-use-cxa-atexit -nostdlib -fno-builtin -fno-rtti -fno-exceptions -fno-leading-underscore
LDPARAMS = --oformat binary

CPPCompiler = g++ $(GPPPARAMS)### This is the compiler and its parameters
LINKER = ld $(LDPARAMS)### This is the linker and its parameters
ASMCompiler = nasm

OUTPUTPREFIX = Info: # Message to put before outputs

SRCDIR = src/
TEMPDIR = .temp/
OUTDIR = bin/
KERNELDIR = Kernel_Files/

srcFiles = Heap.cpp IDT.cpp IO.cpp KBHandlers.cpp Kernel.cpp Memory.cpp MemoryMap.cpp TextPrint.cpp# Relative to Kernel_Files
asmFiles = $(KERNELDIR)Binaries.asm 64BitBoot/ExtendedProgram.asm#Relative to $(SRCDIR)

# Remove directories, remove extension, add .o, add directory
objects = $(addprefix $(TEMPDIR),$(addsuffix .o,$(basename $(notdir $(asmFiles))))) $(addprefix $(TEMPDIR),$(addsuffix .o,$(basename $(notdir $(srcFiles)))))


define compile_kernel # Compile the files
	@$(ASMCompiler) $(SRCDIR)16BitBoot/Boot.asm -f bin -o $(OUTDIR)Boot.bin
	@$(ASMCompiler) $(SRCDIR)64BitBoot/ExtendedProgram.asm -f elf64 -o $(TEMPDIR)ExtendedProgram.o
	
	@$(ASMCompiler) $(SRCDIR)$(KERNELDIR)Binaries.asm -f elf64 -o $(TEMPDIR)Binaries.o


	@$(foreach CppFile,$(srcFiles),$(CPPCompiler) -o $(addprefix $(TEMPDIR),$(addsuffix .o,$(basename $(CppFile)))) -c $(addprefix $(SRCDIR)$(KERNELDIR),$(CppFile));)


	@$(LINKER) -T $(SRCDIR)$(KERNELDIR)Linker.ld $(objects) -o $(OUTDIR)Kernel.bin

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
		cp $(OUTDIR)Bootloader.bin $(TEMPDIR) && \
		rm -r $(OUTDIR) && \
		mkdir $(OUTDIR) && \
		cp $(TEMPDIR)Bootloader.bin $(OUTDIR) && \
		rm -r $(TEMPDIR) && \
		mkdir $(TEMPDIR) && \
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