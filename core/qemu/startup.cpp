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

#define QEMUFLEX_FLEXUS_INTERNAL
#include <core/qemu/api_wrappers.hpp>

extern "C" void qmp_call(Flexus::Qemu::API::qmp_flexus_cmd_t aCMD, const char *anArgs) {
  Flexus::Core::callQMP(aCMD, anArgs);
}

extern "C" void flexDeinit() {
  Flexus::Core::deinitFlexus();
}

extern "C" void startTiming() {
  Flexus::Core::startTimingFlexus(); 
}

// Hooked with `dlsym` in `flexus_proxy.c`
static void print_copyright(void);
extern "C" void qflex_sim_init(Flexus::Qemu::API::QFLEX_API_Interface_Hooks_t *hooks, int nb_cores, const char *config_file) {
  print_copyright();

  QFLEX_API_set_Interface_Hooks(hooks);
  Flexus::Core::setCfg(config_file);

  if (getenv("WAITFORSIGCONT")) {
    std::cerr << "Waiting for SIGCONT..." << std::endl;
    std::cerr << "Attach gdb with the following command and 'c' from the gdb prompt:" << std::endl;
    std::cerr << "  gdb - " << getpid() << std::endl;
    raise(SIGSTOP);
  }

  DBG_(Dev, (<< "Initializing Flexus."));
  DBG_(Dev, (<< "Compiled with Boost: " << BOOST_VERSION / 100000 << "."
             << BOOST_VERSION / 100 % 1000 << "." << BOOST_VERSION % 100));

  // Do all the stuff we need to get Simics to know we are here
  Flexus::Core::PrepareFlexusObject();
  Flexus::Core::CreateFlexusObject();

  Flexus::Core::index_t system_width = nb_cores;

  DBG_(Crit, (<< "Instantiating Flexus components with SystemWidth = " << system_width));

  Flexus::Core::ComponentManager::getComponentManager().instantiateComponents(system_width);
  Flexus::Core::ConfigurationManager::getConfigurationManager().processCommandLineConfiguration(0, 0);

  DBG_(Iface, (<< "Flexus Configured."));

  Flexus::Core::initFlexus();

  DBG_(Iface, (<< "Flexus Initialized."));
}

extern "C" void qflex_quit(void) {
  flexDeinit();
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
