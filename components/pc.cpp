#include "systemc.h"
using namespace std;

SC_MODULE(pc){
    // *** Signals ***
    sc_in_clk clock;
    sc_in<bool> enable;
    sc_in<bool> load;
    sc_in<bool> reset;
    sc_in<sc_uint<9>> next_inst;
    sc_out<sc_uint<9>> pc_out;

    // *** Methods ***
    sc_uint<9> counter = 0;
    void operate() {
        if(enable.read()){
            counter++;
            pc_out.write(counter);
        } else if(load.read()){
            counter = next_inst.read();
            pc_out.write(counter);
        } else if(reset.read()){
            counter = 0;
            pc_out.write(counter);
        }
    }

    // *** Constructor ***
    SC_CTOR(pc) {
        cout << ">> PC" << endl;
        SC_METHOD(operate);
        sensitive << reset << clock.pos();
    }
};