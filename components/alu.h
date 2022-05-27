#include "systemc.h"

SC_MODULE(ALU){
    sc_in<sc_uint<32>> op1, op2;
    sc_in<sc_uint<3>> alu_op;
    sc_out<bool> zero, neg;
    sc_out<sc_uint<32>> alu_out;

    sc_uint<32> data1, data2, uout;

    void operate() {
        zero = false;
        neg = false;
        data1 = op1.read();
        data2 = op2.read();

        switch(alu_op.read()) {
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
            case 6:
                if(data1 == data2) {
                    uout = 1;
                    zero = true;
                    neg = false;
                }
                else if(data1 < data2) {
                    uout = 0;
                    zero = false;
                    neg = true;
                }
                else {
                    uout = 2;
                    zero = false;
                    neg = false;
                }
        }

        alu_out.write(uout);
    }

    SC_CTOR(ALU) {
        SC_METHOD(operate);
        sensitive << op1 << op2 << alu_op;
    }
}