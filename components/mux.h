#ifndef MUX
#define MUX

#include "systemc.h"

SC_MODULE(mux){
    // *** Signals ***
    // TODO:

    // *** Methods ***
    void operate() {
        // TODO:
    }

    // *** Constructor ***
    SC_CTOR(mux) {
        cout << ">> Mux" << endl;
        SC_METHOD(operate);
        // TODO: sensitive
    }
}

#endif