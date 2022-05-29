#ifndef REG_BANK
#define REG_BANK

#include "systemc.h"
using namespace std;

SC_MODULE(reg_bank){
    // *** Signals ***
    sc_in<bool> clock;
    sc_in<bool> enable;
    sc_in<bool> wr;

    sc_in<sc_uint<9>> rs, rt, rd;
    sc_out<sc_int<32>> op_1, op_2;   
    sc_in<sc_int<32>> write_reg;

    // *** Methods ***
    void operate() {
        if(enable.read()){
            if(wr.read()){
                bank[rd.read()] = write_reg.read();
            } else {
                op_1.write(bank[rs.read()]);
                op_2.write(bank[rt.read()]);
            }
        }

    }

    // *** Constructor ***
    SC_HAS_PROCESS(reg_bank);
    reg_bank (sc_module_name smn) : sc_module {smn} {
        cout << ">> Register Bank " << endl;
        SC_METHOD(operate);
        sensitive << clock.pos();

        bank = new sc_signal<sc_int<32>>[512];
    }

    private: 
        sc_signal<sc_int<32>> *bank; 
};

#endif