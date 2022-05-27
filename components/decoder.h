#ifndef DECODER
#define DECODER

#include "systemc.h"

/* [op rs rt rd] => [op] [rs] [rt] [rd]
 *                    5    9    9    9
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
        
        // FIXME: verificar se está quebrando os bits no tamanho correto
        rt.write(decode_inst%(1e3));
        decode_inst /= 1000;
        rs.write(decode_inst%1000);
        decode_inst /= 1000;
        rd.write(decode_inst%1000);
        decode_inst /= 1000;
        op.write(decode_inst);
    }

    // *** Constructor ***
    SC_CTOR(decoder) {
        cout << ">> Decoder" << endl;
        SC_METHOD(operate);
        sensitive << inst;
    }
}

#endif