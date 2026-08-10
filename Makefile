CC := cc
LD := ld
AS := as

SRC_DIR := src
INC_DIR := inc
ASM_DIR := asm
OBJ_DIR := obj
BIN_DIR := bin
IMG_DIR := img
ISO_DIR := iso

CC_FLAGS := -ffreestanding -m64 -mno-red-zone -O0 -I$(INC_DIR)
AS_FLAGS :=
LD_FLAGS := -T linker.ld


SRCS := $(wildcard $(SRC_DIR)/*)
OBJS := $(patsubst $(SRC_DIR)/%,$(OBJ_DIR)/%.o,$(SRCS))

all: img

img: kernel
	mkdir -p $(ISO_DIR)/boot/grub
	cp $(BIN_DIR)/kernel $(ISO_DIR)/boot/kernel
	cp grub.cfg $(ISO_DIR)/boot/grub/grub.cfg
	grub-mkrescue -o $(IMG_DIR)/myos.iso $(ISO_DIR)

kernel: $(OBJS)
	$(LD) -o $(BIN_DIR)/kernel $(OBJS) $(LD_FLAGS)


$(OBJ_DIR)/%.c.o: $(ASM_DIR)/%.c.s
	$(AS) -o $@ -c $< $(AS_FLAGS)

$(OBJ_DIR)/%.s.o: $(ASM_DIR)/%.s.s
	$(AS) -o $@ -c $< $(AS_FLAGS)

$(ASM_DIR)/%.c.s: $(SRC_DIR)/%.c
	$(CC) -o $@ -S $< $(CC_FLAGS)

$(ASM_DIR)/%.s.s: $(SRC_DIR)/%.s
	cp $< $@


.PRECIOUS: $(ASM_DIR)/%.s.s $(ASM_DIR)/%.c.s

clean:
	rm -rf $(ASM_DIR) $(OBJ_DIR) $(BIN_DIR) $(IMG_DIR) $(ISO_DIR)
	mkdir -p $(SRC_DIR) $(ASM_DIR) $(OBJ_DIR) $(BIN_DIR) $(IMG_DIR) $(ISO_DIR)
