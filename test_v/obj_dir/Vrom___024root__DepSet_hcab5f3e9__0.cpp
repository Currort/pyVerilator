// Verilated -*- C++ -*-
// DESCRIPTION: Verilator output: Design implementation internals
// See Vrom.h for the primary calling header

#include "verilated.h"

#include "Vrom__Syms.h"
#include "Vrom___024root.h"

VL_INLINE_OPT void Vrom___024root___ico_sequent__TOP__0(Vrom___024root* vlSelf) {
    if (false && vlSelf) {}  // Prevent unused
    Vrom__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vrom___024root___ico_sequent__TOP__0\n"); );
    // Body
    vlSelf->data_o = ((0xffffff00U & vlSelf->data_o) 
                      | vlSelf->rom__DOT__u_SSS__DOT__mem_r
                      [(0x3ffU & (IData)(vlSelf->data_addr_i))]);
    vlSelf->data_o = ((0xffff00ffU & vlSelf->data_o) 
                      | (vlSelf->rom__DOT__u_SSS__DOT__mem_r
                         [(0x3ffU & ((IData)(1U) + (IData)(vlSelf->data_addr_i)))] 
                         << 8U));
    vlSelf->data_o = ((0xff00ffffU & vlSelf->data_o) 
                      | (vlSelf->rom__DOT__u_SSS__DOT__mem_r
                         [(0x3ffU & ((IData)(2U) + (IData)(vlSelf->data_addr_i)))] 
                         << 0x10U));
    vlSelf->data_o = ((0xffffffU & vlSelf->data_o) 
                      | (vlSelf->rom__DOT__u_SSS__DOT__mem_r
                         [(0x3ffU & ((IData)(3U) + (IData)(vlSelf->data_addr_i)))] 
                         << 0x18U));
}

void Vrom___024root___eval_ico(Vrom___024root* vlSelf) {
    if (false && vlSelf) {}  // Prevent unused
    Vrom__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vrom___024root___eval_ico\n"); );
    // Body
    if ((1ULL & vlSelf->__VicoTriggered.word(0U))) {
        Vrom___024root___ico_sequent__TOP__0(vlSelf);
    }
}

void Vrom___024root___eval_act(Vrom___024root* vlSelf) {
    if (false && vlSelf) {}  // Prevent unused
    Vrom__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vrom___024root___eval_act\n"); );
}

void Vrom___024root___eval_nba(Vrom___024root* vlSelf) {
    if (false && vlSelf) {}  // Prevent unused
    Vrom__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vrom___024root___eval_nba\n"); );
}

void Vrom___024root___eval_triggers__ico(Vrom___024root* vlSelf);
#ifdef VL_DEBUG
VL_ATTR_COLD void Vrom___024root___dump_triggers__ico(Vrom___024root* vlSelf);
#endif  // VL_DEBUG
void Vrom___024root___eval_triggers__act(Vrom___024root* vlSelf);
#ifdef VL_DEBUG
VL_ATTR_COLD void Vrom___024root___dump_triggers__act(Vrom___024root* vlSelf);
#endif  // VL_DEBUG
#ifdef VL_DEBUG
VL_ATTR_COLD void Vrom___024root___dump_triggers__nba(Vrom___024root* vlSelf);
#endif  // VL_DEBUG

void Vrom___024root___eval(Vrom___024root* vlSelf) {
    if (false && vlSelf) {}  // Prevent unused
    Vrom__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vrom___024root___eval\n"); );
    // Init
    CData/*0:0*/ __VicoContinue;
    VlTriggerVec<0> __VpreTriggered;
    IData/*31:0*/ __VnbaIterCount;
    CData/*0:0*/ __VnbaContinue;
    // Body
    vlSelf->__VicoIterCount = 0U;
    __VicoContinue = 1U;
    while (__VicoContinue) {
        __VicoContinue = 0U;
        Vrom___024root___eval_triggers__ico(vlSelf);
        if (vlSelf->__VicoTriggered.any()) {
            __VicoContinue = 1U;
            if (VL_UNLIKELY((0x64U < vlSelf->__VicoIterCount))) {
#ifdef VL_DEBUG
                Vrom___024root___dump_triggers__ico(vlSelf);
#endif
                VL_FATAL_MT("rom.v", 22, "", "Input combinational region did not converge.");
            }
            vlSelf->__VicoIterCount = ((IData)(1U) 
                                       + vlSelf->__VicoIterCount);
            Vrom___024root___eval_ico(vlSelf);
        }
    }
    __VnbaIterCount = 0U;
    __VnbaContinue = 1U;
    while (__VnbaContinue) {
        __VnbaContinue = 0U;
        vlSelf->__VnbaTriggered.clear();
        vlSelf->__VactIterCount = 0U;
        vlSelf->__VactContinue = 1U;
        while (vlSelf->__VactContinue) {
            vlSelf->__VactContinue = 0U;
            Vrom___024root___eval_triggers__act(vlSelf);
            if (vlSelf->__VactTriggered.any()) {
                vlSelf->__VactContinue = 1U;
                if (VL_UNLIKELY((0x64U < vlSelf->__VactIterCount))) {
#ifdef VL_DEBUG
                    Vrom___024root___dump_triggers__act(vlSelf);
#endif
                    VL_FATAL_MT("rom.v", 22, "", "Active region did not converge.");
                }
                vlSelf->__VactIterCount = ((IData)(1U) 
                                           + vlSelf->__VactIterCount);
                __VpreTriggered.andNot(vlSelf->__VactTriggered, vlSelf->__VnbaTriggered);
                vlSelf->__VnbaTriggered.thisOr(vlSelf->__VactTriggered);
                Vrom___024root___eval_act(vlSelf);
            }
        }
        if (vlSelf->__VnbaTriggered.any()) {
            __VnbaContinue = 1U;
            if (VL_UNLIKELY((0x64U < __VnbaIterCount))) {
#ifdef VL_DEBUG
                Vrom___024root___dump_triggers__nba(vlSelf);
#endif
                VL_FATAL_MT("rom.v", 22, "", "NBA region did not converge.");
            }
            __VnbaIterCount = ((IData)(1U) + __VnbaIterCount);
            Vrom___024root___eval_nba(vlSelf);
        }
    }
}

#ifdef VL_DEBUG
void Vrom___024root___eval_debug_assertions(Vrom___024root* vlSelf) {
    if (false && vlSelf) {}  // Prevent unused
    Vrom__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vrom___024root___eval_debug_assertions\n"); );
    // Body
    if (VL_UNLIKELY((vlSelf->finish_i & 0xfeU))) {
        Verilated::overWidthError("finish_i");}
}
#endif  // VL_DEBUG
