MODULE = top
TESTBENCH = tb.cc
VERILATOR = verilator
VERILATOR_FLAGS += -cc --exe
VERILATOR_FLAGS += --x-assign unique --x-initial unique
VERILATOR_FLAGS += -Wall
VERILATOR_FLAGS += --trace

.dafault: run build verilate clean
.PHONY: run
run: obj_dir/V$(MODULE)
	@echo
	@echo "--RUN----------------"
	obj_dir/V$(MODULE) +verilator+rand+reset+2

obj_dir/V$(MODULE): obj_dir tb.cc
	@echo
	@echo "--BUILD--------------"
	$(MAKE) -C obj_dir -f V$(MODULE).mk

obj_dir: top.v
	@echo
	@echo "--VERILATE-----------"
	$(VERILATOR) $(VERILATOR_FLAGS) $(MODULE).v $(TESTBENCH)

clean:
	rm -rf obj_dir
	rm -rf *.vcd
