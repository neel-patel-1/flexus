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
#include <signal.h>
#include <stdlib.h>
#include <unistd.h>

#include <iostream>
#include <fstream>

#include <core/debug/debug.hpp>

#include <boost/version.hpp>
#include <core/component.hpp>
#include <core/configuration.hpp>
#include <core/simulator_name.hpp>
#include <core/target.hpp>
#include <core/flexus.hpp>

#include <core/qemu/api_wrappers.hpp>

static void print_copyright(void);
extern "C" {

void qflex_sim_qmp(Flexus::Qemu::API::qmp_flexus_cmd_t aCMD, const char *anArgs) {
  Flexus::Core::flexusQMP(aCMD, anArgs);
}

void qflex_sim_start_timing() {
  Flexus::Core::flexusStartTiming(); 
}

// Hooked with `dlsym` in `flexus_proxy.c`
void qflex_sim_init(Flexus::Qemu::API::QFLEX_API_Interface_Hooks_t *hooks, int nb_cores, const char *config_file) {
  print_copyright();
  QFLEX_API_set_Interface_Hooks(hooks);
  if (getenv("WAITFORSIGCONT")) {
    std::cerr << "Waiting for SIGCONT..." << std::endl;
    std::cerr << "Attach gdb with the following command and 'c' from the gdb prompt:" << std::endl;
    std::cerr << "  gdb - " << getpid() << std::endl;
    raise(SIGSTOP);
  }
  Flexus::Core::flexusInit(nb_cores, config_file);
}

void qflex_sim_quit(void) {
  Flexus::Core::flexusStop();
}

}

// clang-format off
static void print_copyright(void) {
  std::cerr << ""
  "######################################################################################################\n"
  "#                                                                                                    #\n"
  "#           ************                                                                             #\n"
  "#          **************                                                                            #\n"
  "#   *      **************      *                                                                     #\n"
  "#  **      ***  ****  ***     ***                                                                    #\n"
  "# ***      **************      ***                                                                   #\n"
  "# ****     **************     ****                                                                   #\n"
  "#  ******************************   ***********    *                                                 #\n"
  "#    *********        **********   *************  ***                                                #\n"
  "#      *****            *****      ***            ***       *****      *        *                    #\n"
  "#      ****              ****      ***            ***    ***********  ****    ****                   #\n"
  "#      ****               ***      **********     ***   ****     ****   ********                     #\n"
  "#      ****          *** ****      **********     ***   *************     ****                       #\n"
  "#       ****          ******       ***            ***   ***             ********                     #\n"
  "#        *****        *******      ***            ***    ****   ****   ****  ****                    #\n"
  "#           ****    ****   ***     ***            ***      ********   ***      ***                   #\n"
  "#                                                                                                    #\n"
  "#   QFlex (C) 2016-2020, Parallel Systems Architecture Lab, EPFL                                     #\n"
  "#   All rights reserved.                                                                             #\n"
  "#   Website: https://qflex.epfl.ch                                                                   #\n"
  "#   QFlex uses software developed externally:                                                        #\n"
  "#   [NS-3](https://www.gnu.org/copyleft/gpl.html)                                                    #\n"
  "#   [QEMU](http://wiki.qemu.org/License)                                                             #\n"
  "#   [SimFlex] (http://parsa.epfl.ch/simflex/)                                                        #\n"
  "#                                                                                                    #\n"
  "#   Redistribution and use in source and binary forms, with or without modification,                 #\n"
  "#   are permitted provided that the following conditions are met:                                    #\n"
  "#                                                                                                    #\n"
  "#       * Redistributions of source code must retain the above copyright notice,                     #\n"
  "#         this list of conditions and the following disclaimer.                                      #\n"
  "#       * Redistributions in binary form must reproduce the above copyright notice,                  #\n"
  "#         this list of conditions and the following disclaimer in the documentation                  #\n"
  "#         and/or other materials provided with the distribution.                                     #\n"
  "#       * Neither the name of the Parallel Systems Architecture Laboratory, EPFL,                    #\n"
  "#         nor the names of its contributors may be used to endorse or promote                        #\n"
  "#         products derived from this software without specific prior written                         #\n"
  "#         permission.                                                                                #\n"
  "#                                                                                                    #\n"
  "#   THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS \"AS IS\" AND                  #\n"
  "#   ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED                    #\n"
  "#   WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE                           #\n"
  "#   DISCLAIMED. IN NO EVENT SHALL THE PARALLEL SYSTEMS ARCHITECTURE LABORATORY,                      #\n"
  "#   EPFL BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR                      #\n"
  "#   CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE                  #\n"
  "#   GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)                      #\n"
  "#   HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT                       #\n"
  "#   LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF                 #\n"
  "#   THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.                     #\n"
  "######################################################################################################\n" << std::endl << std::endl;
  std::cerr << " QFlex simulator - Built as " << Flexus::theSimulatorName << std::endl << std::endl;
}
// clang-format on


// Callbacks from Trace mode
extern "C" {

void qflex_sim_callbacks_periodic(void) {
  DBG_Assert(Flexus::qflex_sim_callbacks.periodic.fn != NULL, (<< "Callback was never intialized but still was called"));
  ((Flexus::QFLEX_SIM_CALLBACK_PERIODIC) Flexus::qflex_sim_callbacks.periodic.fn)(Flexus::qflex_sim_callbacks.periodic.obj);
}

void qflex_sim_callbacks_trace_mem(int cpu_index, Flexus::Qemu::API::memory_transaction_t *mem_trans) {
  DBG_Assert(Flexus::qflex_sim_callbacks.trace_mem != NULL, (<< "Callback was never intialized but still was called"));
  ((Flexus::QFLEX_SIM_CALLBACK_TRACE_MEM) Flexus::qflex_sim_callbacks.trace_mem[cpu_index].fn)(Flexus::qflex_sim_callbacks.trace_mem[cpu_index].obj, mem_trans);
}

void qflex_sim_callbacks_trace_mem_dma(Flexus::Qemu::API::memory_transaction_t *mem_trans) {
  DBG_Assert(Flexus::qflex_sim_callbacks.trace_mem_dma.obj != NULL, (<< "Callback was never intialized but still was called"));
  ((Flexus::QFLEX_SIM_CALLBACK_TRACE_MEM_DMA) Flexus::qflex_sim_callbacks.trace_mem_dma.fn)(Flexus::qflex_sim_callbacks.trace_mem_dma.obj, mem_trans);
}

void qflex_sim_callbacks_magic_inst(int cpu_index, long long aBreakpoint) {
  for (int type = 0; type < Flexus::Qemu::API::MagicInstsTotalHooks; type++) {
    if(Flexus::qflex_sim_callbacks.magic_inst[type].obj != NULL){
      ((Flexus::QFLEX_SIM_CALLBACK_MAGIC_INST) Flexus::qflex_sim_callbacks.magic_inst[type].fn)(Flexus::qflex_sim_callbacks.magic_inst[type].obj, cpu_index, aBreakpoint);
    } else {
     DBG_(Crit, (<< "Callback was never intialized but still was called"));
    }
  }
}

void qflex_sim_callbacks_ethernet_frame(int32_t aNetworkID, int32_t aFrameType, long long aTimestamp) {
  DBG_Assert(Flexus::qflex_sim_callbacks.ethernet_frame.obj != NULL, (<< "Callback was never intialized but still was called"));
  ((Flexus::QFLEX_SIM_CALLBACK_ETHERNET_FRAME) Flexus::qflex_sim_callbacks.ethernet_frame.fn)(Flexus::qflex_sim_callbacks.ethernet_frame.obj, aNetworkID, aFrameType, aTimestamp);
}

void qflex_sim_callbacks_xterm_break_string(char *aString) {
  DBG_Assert(Flexus::qflex_sim_callbacks.xterm_break_string.obj != NULL, (<< "Callback was never intialized but still was called"));
  ((Flexus::QFLEX_SIM_CALLBACK_XTERM_BREAK_STRING) Flexus::qflex_sim_callbacks.xterm_break_string.fn)(Flexus::qflex_sim_callbacks.xterm_break_string.obj, aString);
}
	
}
