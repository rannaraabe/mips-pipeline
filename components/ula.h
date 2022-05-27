#include "systemc.h"

SC_MODULE(ULA){
    sc_in<sc_uint<32>> op1, op2;
    sc_in<sc_uint<3>> ula_op;
    sc_out<bool> zero;
    sc_out<sc_uint<32>> ula_out;

    sc_uint<32> data1, data2, uout;

    void operate() {
        zero = false;
        data1 = op1.read();
        data2 = op2.read();

        switch(ula_op.read()) {
            case 0:
                uout = ~data1;
                break;
            case 1:
                uout = data1 & data2;
                break;
            case 2:
                uout = data1 | data2;
                break;
            case 3:
                uout = data1 ^ data2;
                break;
            case 4:
                uout = data1 + data2;
                break;
            case 5:
                uout = data1 - data2;
                break;
        }

        ula_out.write(uout);
        if(uout == 0)
            zero = true;
    }

    SC_CTOR(ULA) {
        SC_METHOD(operate);
        sensitive << op1 << op2 << ula_op;
    }
}