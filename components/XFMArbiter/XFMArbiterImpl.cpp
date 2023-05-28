//  DO-NOT-REMOVE begin-copyright-block
// QFlex consists of several software components that are governed by various
// licensing terms, in addition to software that was developed internally.
// Anyone interested in using QFlex needs to fully understand and abide by the
// licenses governing all the software components.
//
// ### Software developed externally (not by the QFlex group)
//
//     * [NS-3] (https://www.gnu.org/copyleft/gpl.html)
//     * [QEMU] (http://wiki.qemu.org/License)
//     * [SimFlex] (http://parsa.epfl.ch/simflex/)
//     * [GNU PTH] (https://www.gnu.org/software/pth/)
//
// ### Software developed internally (by the QFlex group)
// **QFlex License**
//
// QFlex
// Copyright (c) 2020, Parallel Systems Architecture Lab, EPFL
// All rights reserved.
//
// Redistribution and use in source and binary forms, with or without modification,
// are permitted provided that the following conditions are met:
//
//     * Redistributions of source code must retain the above copyright notice,
//       this list of conditions and the following disclaimer.
//     * Redistributions in binary form must reproduce the above copyright notice,
//       this list of conditions and the following disclaimer in the documentation
//       and/or other materials provided with the distribution.
//     * Neither the name of the Parallel Systems Architecture Laboratory, EPFL,
//       nor the names of its contributors may be used to endorse or promote
//       products derived from this software without specific prior written
//       permission.
//
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
// ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
// WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
// DISCLAIMED. IN NO EVENT SHALL THE PARALLEL SYSTEMS ARCHITECTURE LABORATORY,
// EPFL BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
// CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE
// GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
// HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
// LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF
// THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
//  DO-NOT-REMOVE end-copyright-block
#include <iostream>
#include <components/XFMArbiter/XFMArbiter.hpp>

#define FLEXUS_BEGIN_COMPONENT XFMArbiter
#include FLEXUS_BEGIN_COMPONENT_IMPLEMENTATION()

namespace nXFMArbiter {

class FLEXUS_COMPONENT(XFMArbiter) {
  FLEXUS_COMPONENT_IMPL(XFMArbiter);

public:
  FLEXUS_COMPONENT_CONSTRUCTOR(XFMArbiter) : base(FLEXUS_PASS_CONSTRUCTOR_ARGS) {
  }
  // std::vector<CPUState> theCPUState;

  bool isQuiesced() const {
    return true;
  }

  // Initialization
  void initialize() {
    std::cout << "Called Init \n";
    curState = cfg.InitState;
  }

  void finalize() {
    std::cout << "Final state is:" << curState << std::endl;
  }

  int getStateNonWire() {
    return curState;
  }

  FLEXUS_PORT_ALWAYS_AVAILABLE(DispatchIn);
  void push(interface::DispatchIn const &,
          boost::intrusive_ptr<AbstractInstruction> &anInstruction) {

    DBG_(VVerb, (<< "Get the inst in XFMArbiter: "));

    try {
      boost::intrusive_ptr<Instruction> insn =
          boost::polymorphic_pointer_downcast<armInstruction>(anInstruction);
      if (insn.isPageFault()){
        DBG_(VVerb, (<< "XFMArbiter Intercepted page fault: "));
      }
      // theCore->dispatch(insn);
    } catch (...) {
      DBG_(Crit, (<< "Unable to cast from AbstractInstruction to Instruction"));
    }

    
  }


  // pullStateRetDyn Dynamic PullOutput Port
  // =======================================
  FLEXUS_PORT_ARRAY_ALWAYS_AVAILABLE(pullStateRetDyn);
  int pull(interface::pullStateRetDyn const &, index_t anIndex) {
    return anIndex * 1000;
  }
  // Drive Interfaces
  void drive(interface::XFMDrive const &) {
    curState++;
    std::cout << "Drive Called. Sending incremented state " << curState << " over Port:getState\n";
    if (FLEXUS_CHANNEL(getState).available())
      FLEXUS_CHANNEL(getState) << curState;
    if (FLEXUS_CHANNEL(pullStateIn).available())
      FLEXUS_CHANNEL(pullStateIn) >> curState;
    int out = 20;
    if (FLEXUS_CHANNEL_ARRAY(getStateDyn, 3).available())
      FLEXUS_CHANNEL_ARRAY(getStateDyn, 3) << out;
    if (FLEXUS_CHANNEL_ARRAY(pullStateInDyn, 3).available())
      FLEXUS_CHANNEL_ARRAY(pullStateInDyn, 3) >> curState;
  }

private:
  int curState = 100;
};

} // End namespace nXFM

FLEXUS_COMPONENT_INSTANTIATOR(XFMArbiter, nXFM);

FLEXUS_PORT_ARRAY_WIDTH(XFMArbiter, setStateDyn) {
  return 4;
}

FLEXUS_PORT_ARRAY_WIDTH(XFMArbiter, getStateDyn) {
  return 4;
}

FLEXUS_PORT_ARRAY_WIDTH(XFMArbiter, pullStateInDyn) {
  return 4;
}

FLEXUS_PORT_ARRAY_WIDTH(XFMArbiter, pullStateRetDyn) {
  return 4;
}

#include FLEXUS_END_COMPONENT_IMPLEMENTATION()
#define FLEXUS_END_COMPONENT XFMArbiter
