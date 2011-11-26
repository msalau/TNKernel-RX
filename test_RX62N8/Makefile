PROJECT = test

PROJECT_FILE = $(PROJECT).elf
PROJECT_MAP = $(PROJECT).map
PROJECT_LST = $(PROJECT).lst

TOOL_PATH = /opt/rx-elf/bin
TOOL_PREFIX = $(TOOL_PATH)/rx-elf
#TOOL_PREFIX = rx-elf

CC = $(TOOL_PREFIX)-gcc
CXX = $(TOOL_PREFIX)-g++
AS = $(TOOL_PREFIX)-gcc
LD = $(TOOL_PREFIX)-gcc
RM = $(TOOL_PREFIX)-rm
AR = $(TOOL_PREFIX)-ar
OBJCOPY = $(TOOL_PREFIX)-objcopy
OBJDUMP = $(TOOL_PREFIX)-objdump
SIZE = $(TOOL_PREFIX)-size
LINT = splint

FLASH_TOOL = rxusb

all : $(PROJECT_FILE)

PROJECT_PATH = .
TNKERNEL_PATH = ../TNKernel
TNKERNELPORT_PATH = $(TNKERNEL_PATH)/RX

LINKSCRIPT = $(PROJECT_PATH)/RX62N8.ld

PREPROCESSORFLAGS = \
	-I. \
	-I$(TNKERNEL_PATH) \
	-I$(TNKERNELPORT_PATH) \
	-I$(PROJECT_PATH) \
	-DTNKERNEL_PORT_RX \
	$(END)

# ToDo add search path for system headers of rx-elf toolchain
LINTFLAGS = \
	$(PREPROCESSORFLAGS) \
	$(END)

CFLAGS = \
	-O2 \
	-g2 \
	-Wall \
	-Wextra \
	-Wnested-externs \
	-Wpointer-arith \
	-Wswitch \
	-Wredundant-decls \
	-Wreturn-type \
	-Wshadow \
	-Wstrict-prototypes \
	-Wunused \
	-Wno-main \
	-MMD \
	-mlittle-endian-data \
	-mint-register=0 \
	-ffunction-sections \
	-fdata-sections \
	-std=gnu99 \
	$(PREPROCESSORFLAGS) \
	$(END)
#	-fomit-frame-pointer \
#	-fno-builtin-printf \

ASFLAGS = \
	$(CFLAGS) \
	$(END)

LDFLAGS = \
	-nostartfiles \
	-Wl,--gc-sections \
	-Wl,-Map=$(PROJECT_MAP) \
	$(END)
#	-msim \

OBJS = \
	$(TNKERNEL_PATH)/tn.o \
	$(TNKERNEL_PATH)/tn_dqueue.o \
	$(TNKERNEL_PATH)/tn_event.o \
	$(TNKERNEL_PATH)/tn_mem.o \
	$(TNKERNEL_PATH)/tn_mutex.o \
	$(TNKERNEL_PATH)/tn_sem.o \
	$(TNKERNEL_PATH)/tn_tasks.o \
	$(TNKERNEL_PATH)/tn_utils.o \
	$(TNKERNELPORT_PATH)/tn_port_rx.o \
	$(TNKERNELPORT_PATH)/tn_port_rx_gcc.o \
	\
	$(PROJECT_PATH)/crt0.o \
	$(PROJECT_PATH)/tn_user.o \
	\
	$(PROJECT_PATH)/test.o \
	$(PROJECT_PATH)/sci2.o \
	$(PROJECT_PATH)/stdout.o \
	$(PROJECT_PATH)/xprintf.o \
	$(PROJECT_PATH)/hwsetup.o \
	\
	$(END)

$(PROJECT_FILE) : $(OBJS)
	@echo -e "\tLD\t$@"
	@$(LD) $(LDFLAGS) $^ -o $@ -T $(LINKSCRIPT)
	@echo -e "\tOBJDUMP\t$@"
	@$(OBJDUMP) -DS $@ > $(PROJECT_LST)
	@echo -e "\tSIZE\t$@"
	@$(SIZE) $@

%.o : %.c
	@echo -e "\tCC\t$@"
	@$(CC) -c $(CFLAGS) $< -o $@

%.o : %.S
	@echo -e "\tAS\t$@"
	@$(AS) $(ASLAGS) -c $< -o $@

lint:
	@echo -e "\tLINT\tALL"
	$(LINT) $(LINTFLAGS) $(PROJECT_PATH)/*.c $(TNKERNEL_PATH)/*.c $(TNKERNELPORT_PATH)/*.c

flash : $(PROJECT_FILE)
	@$(FLASH_TOOL) -v $<

clean :
	@rm -f $(OBJS) $(PROJECT_FILE) $(PROJECT_MAP) $(PROJECT_MAP) $(PROJECT_PATH)/*.d $(TNKERNEL_PATH)/*.d $(TNKERNELPORT_PATH)/*.d

DEPS=$(wildcard $(TNKERNEL_PATH)/*.d) $(wildcard $(TNKERNELPORT_PATH)/*.d) $(wildcard $(PROJECT_PATH)/*.d)
ifneq ($(DEPS),)
include $(DEPS)
endif
