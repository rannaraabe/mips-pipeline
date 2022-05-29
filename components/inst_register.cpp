#include "systemc.h"

SC_MODULE(inst_register){
    // *** Signals ***
    sc_in_clk clock;
	sc_in<bool> enable;
	sc_in<bool> wr;
	sc_in<sc_uint<32>> inst_in;
	sc_out<sc_uint<32>> inst_out;

    // *** Methods ***
    void operate() {
	    if(enable.read()) {
            if(wr.read()) {
                instruction = inst_in.read();		
                inst_out.write(instruction);
            } else {
                inst_out.write(instruction);
            }
        }
	}

    // *** Constructor ***
    c_uint<32> instruction;
    SC_HAS_PROCESS(inst_register);
	inst_register(sc_module_name smn) : sc_module { smn } {
        cout << ">> Instruction Register" << endl;
        SC_METHOD(operate);
        sensitive << clock.pos();
    }
};