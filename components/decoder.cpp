#include "systemc.h"

/* [op rs rt rd] => [op] [rs] [rt] [rd]
 *                    5    9    9    9
 * 55555 999999999 999999999 999999999
 */

SC_MODULE(decoder){
    // *** Signals ***
    sc_in<sc_uint<32>> inst;
    sc_out<sc_uint<9>> rs, rt, rd;
    sc_out<sc_uint<5>> op;

    // *** Methods ***
    void operate() {
        long long decode_inst = inst.read();
        cout << "> Decoder: Processing... " << inst_ << endl;
        
        rd.write(tinst.range(8,0));
        rt.write(tinst.range(17,9));
        rs.write(tinst.range(18,26));
        opcode.write(tinst.range(27,31));
    }

    // *** Constructor ***
    SC_CTOR(decoder) {
        cout << ">> Decoder" << endl;
        SC_METHOD(operate);
        sensitive << inst;
    }
};