#ifndef PC
#define PC

#include "systemc.h"

SC_MODULE(pc){
    // *** Signals ***
    // TODO:

    // *** Methods ***
    void operate() {
        // TODO:
    }

    // *** Constructor ***
    SC_CTOR(pc) {
        cout << ">> PC" << endl;
        SC_METHOD(operate);
        // TODO: sensitive
    }
}

#endif