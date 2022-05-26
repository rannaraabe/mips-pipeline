#include "systemc.h"

SC_MODULE(AND){
    sc_in<bool> a, b;
    sc_out<bool> and_out;

    void operate(){
        and_out = a & b;
    }

    SC_CTOR(AND){
        SC_METHOD(operate)
        sensitive << a << b;
    }

};