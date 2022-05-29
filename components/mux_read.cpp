#include "systemc.h"

SC_MODULE(mux_read){
    // *** Signals ***
    sc_in<2> selector;
    sc_in<sc_uint<32>> mem_out;
    sc_in<sc_uint<32>> alu_out;
    sc_in<sc_uint<9>> rs;
    sc_out<sc_int<32>> write_reg;

    // *** Methods ***
    void operate() {
       if(selector.read() == 0) {
           write_reg.write(mem_out.read());
       } else if(selector.read() == 1) {
           write_reg.write(alu_out.read());
       } else if(selector.read() == 2) {
            int result = rs.read();
		    write_reg.write(result);
       }
    }

    // *** Constructor ***
    SC_CTOR(mux_read) {
        cout << ">> Mux_Read" << endl;
        SC_METHOD(operate);
        sensitive << selector << mem_out << alu_out << rs;
    }
};