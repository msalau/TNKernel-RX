PROJECTS = \
	tm_basic_processing_test.elf \
	tm_cooperative_scheduling_test.elf \
	tm_interrupt_processing_test.elf \
	tm_interrupt_preemption_processing_test.elf \
	tm_memory_allocation_test.elf \
	tm_message_processing_test.elf \
	tm_preemptive_scheduling_test.elf \
	tm_synchronization_processing_test.elf \
	$(END)

TOOL_PATH = /opt/rx-elf/bin
#TOOL_PREFIX = $(TOOL_PATH)/rx-elf
TOOL_PREFIX = rx-elf

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

all : $(PROJECTS)

PROJECT_PATH = .
TNKERNEL_PATH = ../TNKernel
TNKERNELPORT_PATH = $(TNKERNEL_PATH)/RX
COMMON_PATH = ../Common_RX62N8
THREADMETRIC_PATH = ../Thread-Metric

LINKSCRIPT = $(COMMON_PATH)/RX62N8.ld

PREPROCESSORFLAGS = \
	-I. \
	-I$(TNKERNEL_PATH) \
	-I$(TNKERNELPORT_PATH) \
	-I$(PROJECT_PATH) \
	-I$(COMMON_PATH) \
	-I$(THREADMETRIC_PATH) \
	-DTNKERNEL_PORT_RX \
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
	$(END)
#	-Wl,-Map=$(PROJECT_MAP) \
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
	$(COMMON_PATH)/crt0.o \
	$(COMMON_PATH)/sci2.o \
	$(COMMON_PATH)/stdout.o \
	$(COMMON_PATH)/xprintf.o \
	$(COMMON_PATH)/hwsetup.o \
	$(COMMON_PATH)/isr_vectors.o \
	\
	$(PROJECT_PATH)/tn_user.o \
	\
	$(THREADMETRIC_PATH)/tm_porting_layer_tnkernel.o \
	\
	$(END)

PROJECT_OBJS = $(addprefix $(THREADMETRIC_PATH)/,$(PROJECTS:.elf=.o))

tm_basic_processing_test.elf : $(OBJS) $(THREADMETRIC_PATH)/tm_basic_processing_test.o
	@echo -e "\tLD\t$@"
	@$(LD) $(LDFLAGS) $^ -o $@ -T $(LINKSCRIPT)

tm_cooperative_scheduling_test.elf : $(OBJS) $(THREADMETRIC_PATH)/tm_cooperative_scheduling_test.o
	@echo -e "\tLD\t$@"
	@$(LD) $(LDFLAGS) $^ -o $@ -T $(LINKSCRIPT)

tm_interrupt_processing_test.elf : $(OBJS) $(THREADMETRIC_PATH)/tm_interrupt_processing_test.o
	@echo -e "\tLD\t$@"
	@$(LD) $(LDFLAGS) $^ -o $@ -T $(LINKSCRIPT)

tm_interrupt_preemption_processing_test.elf : $(OBJS) $(THREADMETRIC_PATH)/tm_interrupt_preemption_processing_test.o
	@echo -e "\tLD\t$@"
	@$(LD) $(LDFLAGS) $^ -o $@ -T $(LINKSCRIPT)

tm_memory_allocation_test.elf : $(OBJS) $(THREADMETRIC_PATH)/tm_memory_allocation_test.o
	@echo -e "\tLD\t$@"
	@$(LD) $(LDFLAGS) $^ -o $@ -T $(LINKSCRIPT)

tm_message_processing_test.elf : $(OBJS) $(THREADMETRIC_PATH)/tm_message_processing_test.o
	@echo -e "\tLD\t$@"
	@$(LD) $(LDFLAGS) $^ -o $@ -T $(LINKSCRIPT)

tm_preemptive_scheduling_test.elf : $(OBJS) $(THREADMETRIC_PATH)/tm_preemptive_scheduling_test.o
	@echo -e "\tLD\t$@"
	@$(LD) $(LDFLAGS) $^ -o $@ -T $(LINKSCRIPT)

tm_synchronization_processing_test.elf : $(OBJS) $(THREADMETRIC_PATH)/tm_synchronization_processing_test.o
	@echo -e "\tLD\t$@"
	@$(LD) $(LDFLAGS) $^ -o $@ -T $(LINKSCRIPT)

%.o : %.c
	@echo -e "\tCC\t$@"
	@$(CC) -c $(CFLAGS) $< -o $@

%.o : %.S
	@echo -e "\tAS\t$@"
	@$(AS) $(ASFLAGS) -c $< -o $@

clean :
	@rm -f $(OBJS) $(PROJECT_OBJS) $(PROJECTS) $(PROJECT_PATH)/*.d $(TNKERNEL_PATH)/*.d $(TNKERNELPORT_PATH)/*.d $(COMMON_PATH)/*.d $(THREADMETRIC_PATH)/*.d

DEPS=$(wildcard $(TNKERNEL_PATH)/*.d) $(wildcard $(TNKERNELPORT_PATH)/*.d) $(wildcard $(PROJECT_PATH)/*.d) $(wildcard $(COMMON_PATH)/*.d) $(wildcard $(THREADMETRIC_PATH)/*.d)
ifneq ($(DEPS),)
include $(DEPS)
endif
