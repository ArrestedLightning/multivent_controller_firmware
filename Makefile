TARGET_NAME ?= app
TARGET_ELF  ?= $(TARGET_NAME).elf
TARGET_HEX  ?= $(TARGET_NAME).hex
TARGET_BIN  ?= $(TARGET_NAME).bin

TOOLCHAIN_DIR ?= ~/.local/xPacks/riscv-none-elf-gcc/xpack-riscv-none-elf-gcc-13.2.0-2/bin
AS := $(TOOLCHAIN_DIR)/riscv-none-elf-gcc
CC := $(TOOLCHAIN_DIR)/riscv-none-elf-gcc
CXX := $(TOOLCHAIN_DIR)/riscv-none-elf-g++
OBJCOPY := $(TOOLCHAIN_DIR)/riscv-none-elf-objcopy
SIZEBIN := $(TOOLCHAIN_DIR)/riscv-none-elf-size

BUILD_DIR ?= ./build
SRC_DIRS ?= ./app ./vendor/Core ./vendor/Debug ./vendor/Peripheral ./vendor/Startup ./vendor/User

SRCS := $(shell find $(SRC_DIRS) -name *.cpp -or -name *.c -or -name *.S)
OBJS := $(SRCS:%=$(BUILD_DIR)/%.o)
DEPS := $(OBJS:.o=.d)

INC_DIRS := $(shell find $(SRC_DIRS) -type d)
INC_FLAGS := $(addprefix -I,$(INC_DIRS))

FLAGS ?= -march=rv32imafc -mabi=ilp32f -msmall-data-limit=8 -mno-save-restore -O1 -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections -Wunused -Wuninitialized  -g
ASFLAGS ?= $(FLAGS) -x assembler $(INC_FLAGS) -MMD -MP
CPPFLAGS ?=  $(FLAGS) $(INC_FLAGS) -std=gnu99 -MMD -MP
LDFLAGS ?= $(FLAGS) -T ./vendor/Ld/Link.ld -nostartfiles -Xlinker --gc-sections -Wl,-Map,"$(BUILD_DIR)/CH32V203.map" --specs=nano.specs --specs=nosys.specs

all: $(BUILD_DIR)/$(TARGET_ELF)

proj_init: 
	@echo "Project Init"
	@echo "please follow the following steps:"
	@echo "npm install xpm@latest"
	@echo "find xpm executable in node_modules/xpm/bin"
	@echo "xpm init # Only at first use."
	@echo "xpm install @xpack-dev-tools/riscv-none-elf-gcc@latest --verbose"
	@echo "ls -l xpacks/.bin"

.PHONY: proj_init

$(BUILD_DIR)/$(TARGET_ELF): $(OBJS)
	$(CC) $(OBJS) -o $@ $(LDFLAGS)
	$(OBJCOPY) -Oihex   $@ $(BUILD_DIR)/$(TARGET_HEX)
	$(OBJCOPY) -Obinary $@ $(BUILD_DIR)/$(TARGET_BIN)
	$(SIZEBIN) $@

# assembly
$(BUILD_DIR)/%.S.o: %.S
	$(MKDIR_P) $(dir $@)
	$(CC) $(ASFLAGS) -c $< -o $@

# c source
$(BUILD_DIR)/%.c.o: %.c
	$(MKDIR_P) $(dir $@)
	$(CC) $(CPPFLAGS) $(CFLAGS) -c $< -o $@

# c++ source
$(BUILD_DIR)/%.cpp.o: %.cpp
	$(MKDIR_P) $(dir $@)
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -c $< -o $@

.PHONY: clean

clean:
	$(RM) -r $(BUILD_DIR)

-include $(DEPS)

MKDIR_P ?= mkdir -p
