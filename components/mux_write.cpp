#include "systemc.h"

SC_MODULE(mux_write){
    // *** Signals ***
    sc_in<2> selector;
    sc_in<sc_uint<9>> rs;
    sc_in<sc_uint<9>> rd;
    sc_out<sc_uint<9>> write_mem;

    // *** Methods ***
    void operate() {
        if(selector.read()) {
            write_mem.write(rs.read());
        } else {
            write_mem.write(rd.read());
        }
    }

    // *** Constructor ***
    SC_CTOR(mux_write) {
        cout << ">> Mux_Write" << endl;
        SC_METHOD(operate);
        sensitive << selector << rs << rd;
    }
};