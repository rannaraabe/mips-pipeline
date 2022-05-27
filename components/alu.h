#ifndef ALU
#define ALU

#include "systemc.h"

SC_MODULE(alu){
    // *** Signals ***
    sc_in<sc_uint<32>> rs, rt;
    sc_in<sc_uint<5>> alu_op;
    sc_out<sc_uint<32>> alu_out; 
    sc_out<bool> zero, neg;

    sc_uint<9> op_1, op_2, result;

    // *** Methods ***
    void operate() {
        zero = false;
        neg = false;
        op_1 = rs.read();
        op_2 = rt.read();

        switch(alu_op.read()) {
            case 0:
                // NOT
                result = ~op_1;
                break;
            case 1:
                // AND
                result = op_1 & op_2;
                break;
            case 2:
                // OR
                result = op_1 | op_2;
                break;
            case 3:
                // XOR
                result = op_1 ^ op_2;
                break;
            case 4:
                // ADD
                result = op_1 + op_2;
                break;
            case 5:
                // SUB
                result = op_1 - op_2;
                break;
            case 6:
                // CMP
                if(op_1 == op_2) {
                    result = 1;
                    zero = true;
                    neg = false;
                }
                else if(op_1 < op_2) {
                    result = 0;
                    zero = false;
                    neg = true;
                }
                else {
                    result = 2;
                    zero = false;
                    neg = false;
                }
                break;
            default:
			    result = 0;
			    break; 
        }

        alu_out.write(result);
    }

    // *** Constructor ***
    SC_CTOR(alu) {
        SC_METHOD(operate);
        sensitive << rs << rt << alu_op;
    }
}

#endif