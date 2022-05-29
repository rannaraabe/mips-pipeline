#include "systemc.h"

SC_MODULE(inst_memory){
    // *** Signals ***
	sc_in<bool> clock;
	sc_in<bool> enable;
	sc_in<bool> wr;
    sc_in<sc_uint<32>> inst_pc;
	sc_out<sc_uint<32>> inst;

    sc_uint<32> instruction;
    
    // *** Methods ***
    void operate() {
        if(enable.read()){
            if(wr.read()){
                instruction = inst_pc.read();
            } else {
                inst.write(instruction);
            }
        }
    }

    // *** Constructor ***
    SC_HAS_PROCESS(inst_memory);
    inst_memory (sc_module_name smn) : sc_module {smn} {
        cout << ">> Instruction Memory" << endl;
        SC_METHOD(operate);
        sensitive << clock.pos();
    }
};