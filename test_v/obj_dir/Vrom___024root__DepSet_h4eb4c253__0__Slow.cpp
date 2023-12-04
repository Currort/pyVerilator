// Verilated -*- C++ -*-
// DESCRIPTION: Verilator output: Design implementation internals
// See Vrom.h for the primary calling header

#include "verilated.h"

#include "Vrom__Syms.h"
#include "Vrom__Syms.h"
#include "Vrom___024root.h"

#ifdef VL_DEBUG
VL_ATTR_COLD void Vrom___024root___dump_triggers__stl(Vrom___024root* vlSelf);
#endif  // VL_DEBUG

VL_ATTR_COLD void Vrom___024root___eval_triggers__stl(Vrom___024root* vlSelf) {
    if (false && vlSelf) {}  // Prevent unused
    Vrom__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vrom___024root___eval_triggers__stl\n"); );
    // Body
    vlSelf->__VstlTriggered.set(0U, (0U == vlSelf->__VstlIterCount));
#ifdef VL_DEBUG
    if (VL_UNLIKELY(vlSymsp->_vm_contextp__->debug())) {
        Vrom___024root___dump_triggers__stl(vlSelf);
    }
#endif
}
