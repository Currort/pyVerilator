XLEN ?= 64
CROSS   = riscv$(XLEN)-unknown-elf-
CC      = $(CROSS)gcc 
CPP     = $(CROSS)cpp
OBJCOPY = $(CROSS)objcopy
OBJDUMP = $(CROSS)objdump
ARCH    = $(CROSS)ar
BASE_ADDRESS ?= 0x000000000
PERIPHERAL_LEN ?= 1K

VERILATOR  = verilator
VCC        = $(VERILATOR) --cc
ifeq ($(XLEN), 64)
    MARCH = rv64gc_zicsr
    MABI = lp64d
	STACK_SIZE = 6000
else
    MARCH = rv32ima
    MABI = ilp32d
	STACK_SIZE = 300
endif

CPPFLAGS = \
	-D__riscv_float_abi_soft \
	-DportasmHANDLE_INTERRUPT=handle_trap \
	-I . -I ../Common/include \
	-I $(C_DIR)
CFLAGS  = -march=$(MARCH) -mabi=$(MABI) -mcmodel=medany \
	-Wall \
	-fmessage-length=0 \
	-ffunction-sections \
	-fdata-sections \
	-fno-builtin-printf

VFLAGS  = \
	--vpi \
	--trace --exe -Wall -j 0 -O3 \
	-y $(VTOP_DIR)/ALU_submodule \
	-y $(VTOP_DIR)/Library \
	-y $(VTOP_DIR)/Trap_submodule \
	-y $(VTOP_DIR)/include \
	-y $(VTOP_DIR) \
	-y $(TESTBENCH_DIR) \


TEMPLATE_DIR  = template
TESTBENCH_DIR = $(ABSOLUTE_PATH)/testbench
VTOP_DIR      = /home/zero/master_share/QianTang_Processors/user/src
VSRC_DIR      = ALU_submodule Library Trap_submodule include
VSRC_DIR     := $(addprefix $(VTOP_DIR)/, $(VSRC_DIR))
VSRC_DIR     += $(VTOP_DIR)
BUILD_DIR     = build
C_DIR         = test_src
OBJS_DIR      = $(addprefix $(BUILD_DIR)/, $(PREFIXES))

CSRCS         = $(wildcard  $(C_DIR)/*.c)
PREFIXES      = $(basename  $(notdir $(CSRCS)))
ABSOLUTE_PATH = $(shell pwd)
INSTR_TXT     = $(addsuffix /instr.txt,	   $(OBJS_DIR))
INSTR_V       = $(addsuffix /instr.v,      $(OBJS_DIR))
BIN           = $(addsuffix /run,		   $(OBJS_DIR))
VINSTR_PATH   = $(addsuffix /Instr_path.v, $(OBJS_DIR))
	
V_OBJ_DIR     = $(addsuffix /v_obj,	       $(OBJS_DIR))

VSRC          = $(foreach dir,$(VSRC_DIR),$(wildcard $(dir)/*.v))
VTOP          = SOC.v
VTOP         := $(VTOP_DIR)/$(VTOP)
VVTOP         = $(addprefix V,$(basename $(notdir $(VTOP))))
TESTBENCH     = testbench.cpp monitor.cpp
TESTBENCH     :=$(addprefix $(TESTBENCH_DIR)/,$(TESTBENCH))
VOUT_RUN      = $(addsuffix /$(VVTOP), $(V_OBJ_DIR))
VOUT_MK       = $(addsuffix /$(VVTOP).mk, $(V_OBJ_DIR))
VCD           = $(addsuffix /waveform.vcd,$(OBJS_DIR))
GTKW          = soc.gtkw
all: $(BUILD_DIR) $(OBJS_DIR) $(V_OBJ_DIR) $(VCD)

$(VCD): $(VOUT_RUN) 
	$(foreach i,$(OBJS_DIR),\
	cd $(i) && ./v_obj/$(VVTOP) && cd $(ABSOLUTE_PATH);)

$(VOUT_RUN): $(VOUT_MK)
	$(foreach i,$(V_OBJ_DIR),\
	make -j -C $(i) -f $(VVTOP).mk;)

$(VOUT_MK): $(VTOP) $(TESTBENCH) $(VINSTR_PATH) $(VSRC)
	echo $(VSRC) 
	$(foreach i,$(OBJS_DIR),rm -rf $(i)/v_obj && \
	$(VCC) $(VFLAGS) -y $(ABSOLUTE_PATH)/$(i) --Mdir $(i)/v_obj $(VTOP) $(TESTBENCH);)


$(VINSTR_PATH):$(INSTR_V) $(TEMPLATE_DIR)/Instr_path.v  $(INSTR_TXT) 
	@$(foreach i,$(OBJS_DIR),\
	sed 's#replace instr path#$(ABSOLUTE_PATH)/$(i)/instr.v#g' $(TEMPLATE_DIR)/Instr_path.v > $(i)/Instr_path.v;)

$(INSTR_TXT): $(BIN) 
	@$(foreach i,$(OBJS_DIR),$(OBJDUMP) -S $(i)/run > $(i)/instr.txt;)


$(BIN): $(CSRCS) $(BUILD_DIR)/mem$(BASE_ADDRESS).lds Makefile 
	@$(CC) $(CFLAGS)  $< -T $(BUILD_DIR)/mem$(BASE_ADDRESS).lds -o $@

$(INSTR_V):$(BIN)
	@$(foreach i,$(OBJS_DIR),$(OBJCOPY) -O verilog $(i)/run $(i)/instr.v;)

# 使用 DBASE_ADDRESS替换模板LDS中的 DBASE_ADDRESS
$(BUILD_DIR)/%$(BASE_ADDRESS).lds:  $(TEMPLATE_DIR)/mem.lds Makefile 
	@$(CPP) $(CPPFLAGS) -DBASE_ADDRESS=$(BASE_ADDRESS) $< | grep -v '^#' | sed 's/PERIPHERAL_LEN/$(PERIPHERAL_LEN)/g' > $@

$(BUILD_DIR) $(OBJS_DIR) $(V_OBJ_DIR):
	$(call check_dir,$@)

define check_dir
    @mkdir -p $(1)
endef


clean:
	chmod 777 build
	rm -rf $(BUILD_DIR)


	

