#include "systemc.h"
using namespace std;

/* [op rs rt rd] => [op] [rs] [rt] [rd]
 *                    5    9    9    9
 * 55555 999999999 999999999 999999999
 */

SC_MODULE(decoder){
    // *** Signals ***
    sc_in<sc_int<32>> inst;
    sc_out<sc_int<9>> rs, rt, rd;
    sc_out<sc_int<5>> op;

    // *** Methods ***
    void operate() {
        sc_int<32> decode_inst = inst.read();
        cout << "> Decoder: Processing... " << inst << endl;
        
        rd.write(decode_inst.range(8,0));
        rt.write(decode_inst.range(17,9));
        rs.write(decode_inst.range(18,26));
        op.write(decode_inst.range(27,31));
    }

    // *** Constructor ***
    SC_CTOR(decoder) {
        cout << ">> Decoder" << endl;
        SC_METHOD(operate);
        sensitive << inst;
    }
};