GPPPARAMS = -m32 -Iinclude -fno-use-cxa-atexit -nostdlib -fno-builtin -fno-rtti -fno-exceptions -fno-leading-underscore -fno-PIC -Wno-write-strings
ASPARAMS = --32
LDPARAMS = -melf_i386

objects = obj/loader.o \
		  obj/gdt.o \
		  obj/memorymanagement.o \
		  obj/drivers/driver.o \
		  obj/hardwarecomm/port.o \
		  obj/hardwarecomm/interruptstubs.o \
		  obj/hardwarecomm/interrupts.o \
		  obj/multitasking.o \
		  obj/drivers/amd_am79c973.o \
		  obj/hardwarecomm/pci.o \
		  obj/drivers/keyboard.o \
		  obj/drivers/mouse.o \
		  obj/drivers/vga.o \
		  obj/drivers/ata.o \
		  obj/gui/widget.o \
		  obj/gui/window.o \
		  obj/gui/desktop.o \
		  obj/kernel.o

obj/%.o: src/%.cpp
	mkdir -p $(@D)
	g++ $(GPPPARAMS) -o $@ -c $<

obj/%.o: src/%.s
	mkdir -p $(@D) 
	as $(ASPARAMS) -o $@ $<

cernos.bin: linker.ld $(objects)
	ld $(LDPARAMS) -T $< -o $@ $(objects)

install: cernos.bin
	sudo cp $< /boot/cernos.bin
	
cernos.iso: cernos.bin
	mkdir iso
	mkdir iso/boot
	mkdir iso/boot/grub
	cp $< iso/boot/
	echo 'set timeout=0' >> iso/boot/grub/grub.cfg
	echo 'set default=0' >> iso/boot/grub/grub.cfg
	echo 'menuentry "CernOS" {' >> iso/boot/grub/grub.cfg
	echo '	multiboot /boot/cernos.bin' >> iso/boot/grub/grub.cfg
	echo '	boot' >> iso/boot/grub/grub.cfg
	echo '}' >> iso/boot/grub/grub.cfg
	grub-mkrescue --output=$@ iso
	rm -rf iso

run: cernos.iso
	(killall virtualboxvm && sleep 1) || true 
	virtualboxvm --startvm CernOS &
 
.PHONY: clean
clean:
	rm -rf obj cernos.bin cernos.iso