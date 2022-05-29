#include "systemc.h"
using namespace std;

SC_MODULE(inst_register){
    // *** Signals ***
    sc_in_clk clock;
	sc_in<bool> enable;
	sc_in<bool> wr;
	sc_in<sc_int<32>> inst_in;
	sc_out<sc_int<32>> inst_out;

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
    sc_int<32> instruction;
    SC_HAS_PROCESS(inst_register);
	inst_register(sc_module_name smn) : sc_module { smn } {
        cout << ">> Instruction Register" << endl;
        SC_METHOD(operate);
        sensitive << clock.pos();
    }
};