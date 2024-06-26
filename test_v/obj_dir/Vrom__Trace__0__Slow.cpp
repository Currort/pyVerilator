// Verilated -*- C++ -*-
// DESCRIPTION: Verilator output: Tracing implementation internals
#include "verilated_vcd_c.h"
#include "Vrom__Syms.h"


VL_ATTR_COLD void Vrom___024root__trace_init_sub__TOP__0(Vrom___024root* vlSelf, VerilatedVcd* tracep) {
    if (false && vlSelf) {}  // Prevent unused
    Vrom__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vrom___024root__trace_init_sub__TOP__0\n"); );
    // Init
    const int c = vlSymsp->__Vm_baseCode;
    // Body
    tracep->declBus(c+1,"data_o", false,-1, 31,0);
    tracep->declQuad(c+2,"data_addr_i", false,-1, 63,0);
    tracep->declBit(c+4,"finish_i", false,-1);
    tracep->pushNamePrefix("rom ");
    tracep->declBus(c+1,"data_o", false,-1, 31,0);
    tracep->declQuad(c+2,"data_addr_i", false,-1, 63,0);
    tracep->declBit(c+4,"finish_i", false,-1);
    tracep->pushNamePrefix("u_SSS ");
    tracep->declBus(c+1,"data_o", false,-1, 31,0);
    tracep->declQuad(c+2,"data_addr_i", false,-1, 63,0);
    tracep->declBit(c+4,"finish_i", false,-1);
    tracep->popNamePrefix(2);
}

VL_ATTR_COLD void Vrom___024root__trace_init_top(Vrom___024root* vlSelf, VerilatedVcd* tracep) {
    if (false && vlSelf) {}  // Prevent unused
    Vrom__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vrom___024root__trace_init_top\n"); );
    // Body
    Vrom___024root__trace_init_sub__TOP__0(vlSelf, tracep);
}

VL_ATTR_COLD void Vrom___024root__trace_full_top_0(void* voidSelf, VerilatedVcd::Buffer* bufp);
void Vrom___024root__trace_chg_top_0(void* voidSelf, VerilatedVcd::Buffer* bufp);
void Vrom___024root__trace_cleanup(void* voidSelf, VerilatedVcd* /*unused*/);

VL_ATTR_COLD void Vrom___024root__trace_register(Vrom___024root* vlSelf, VerilatedVcd* tracep) {
    if (false && vlSelf) {}  // Prevent unused
    Vrom__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vrom___024root__trace_register\n"); );
    // Body
    tracep->addFullCb(&Vrom___024root__trace_full_top_0, vlSelf);
    tracep->addChgCb(&Vrom___024root__trace_chg_top_0, vlSelf);
    tracep->addCleanupCb(&Vrom___024root__trace_cleanup, vlSelf);
}

VL_ATTR_COLD void Vrom___024root__trace_full_sub_0(Vrom___024root* vlSelf, VerilatedVcd::Buffer* bufp);

VL_ATTR_COLD void Vrom___024root__trace_full_top_0(void* voidSelf, VerilatedVcd::Buffer* bufp) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vrom___024root__trace_full_top_0\n"); );
    // Init
    Vrom___024root* const __restrict vlSelf VL_ATTR_UNUSED = static_cast<Vrom___024root*>(voidSelf);
    Vrom__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    // Body
    Vrom___024root__trace_full_sub_0((&vlSymsp->TOP), bufp);
}

VL_ATTR_COLD void Vrom___024root__trace_full_sub_0(Vrom___024root* vlSelf, VerilatedVcd::Buffer* bufp) {
    if (false && vlSelf) {}  // Prevent unused
    Vrom__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vrom___024root__trace_full_sub_0\n"); );
    // Init
    uint32_t* const oldp VL_ATTR_UNUSED = bufp->oldp(vlSymsp->__Vm_baseCode);
    // Body
    bufp->fullIData(oldp+1,(vlSelf->data_o),32);
    bufp->fullQData(oldp+2,(vlSelf->data_addr_i),64);
    bufp->fullBit(oldp+4,(vlSelf->finish_i));
}
