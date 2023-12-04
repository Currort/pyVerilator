// Verilated -*- C++ -*-
// DESCRIPTION: Verilator output: Design internal header
// See Vrom.h for the primary calling header

#ifndef VERILATED_VROM___024ROOT_H_
#define VERILATED_VROM___024ROOT_H_  // guard

#include "verilated.h"


class Vrom__Syms;

class alignas(VL_CACHE_LINE_BYTES) Vrom___024root final : public VerilatedModule {
  public:

    // DESIGN SPECIFIC STATE
    VL_IN8(finish_i,0,0);
    CData/*0:0*/ __VactContinue;
    VL_OUT(data_o,31,0);
    IData/*31:0*/ __VstlIterCount;
    IData/*31:0*/ __VicoIterCount;
    IData/*31:0*/ __VactIterCount;
    VL_IN64(data_addr_i,63,0);
    VlUnpacked<CData/*7:0*/, 1024> rom__DOT__u_SSS__DOT__mem_r;
    VlTriggerVec<1> __VstlTriggered;
    VlTriggerVec<1> __VicoTriggered;
    VlTriggerVec<0> __VactTriggered;
    VlTriggerVec<0> __VnbaTriggered;

    // INTERNAL VARIABLES
    Vrom__Syms* const vlSymsp;

    // CONSTRUCTORS
    Vrom___024root(Vrom__Syms* symsp, const char* v__name);
    ~Vrom___024root();
    VL_UNCOPYABLE(Vrom___024root);

    // INTERNAL METHODS
    void __Vconfigure(bool first);
};


#endif  // guard
