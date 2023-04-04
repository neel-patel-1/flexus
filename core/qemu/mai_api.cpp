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
// ALEX: THIS FILE IS CURRENTLY A DUMMY.
// Copied this file from SimFlex. Will gradually populate it properly.

#include <boost/regex.hpp>
#include <cmath>
#include <fstream>
#include <iostream>
#include <sstream>

#include <boost/throw_exception.hpp>
#include <boost/utility.hpp>
#include <functional>
#include <memory>

#include <core/qemu/api_wrappers.hpp>
#include <core/target.hpp>
#include <core/types.hpp>

#include <core/MakeUniqueWrapper.hpp>
#include <core/configuration.hpp>
#include <core/exception.hpp>
#include <core/flexus.hpp>
#include <core/qemu/bitUtilities.hpp>
#include <core/qemu/configuration_api.hpp>
#include <core/qemu/mai_api.hpp>
#include <core/target.hpp>
#include <core/types.hpp>

using namespace Flexus::Core;

namespace Flexus {
namespace Qemu {

// ProcessorMapper * ProcessorMapper::theMapper = NULL;

// ProcessorMapper::ProcessorMapper() {
//  // Find the Processors, Find All of them

//  int proc_count = qemu_callbacks.QEMU_get_num_cores();
//  API::conf_object_t *proc_list = qemu_callbacks.QEMU_get_all_cpus();
//  std::vector<cpu_desc_t> cpu_list;
//  int num_flexus_cpus = 0;
//  int num_client_cpus = proc_count;
//  int num_besim_cpus = 0;
//  int max_vm = -1;

//  DBG_(VVerb, ( << "Searching " << proc_count << " cpus." ));
//  DBG_(VVerb, ( << "WARNING: Need to fix this to distinguish between different
//  cpu types (server, client, etc.).")); //For now, all the cpus are considered
//  to be server cpus

//  for (int i = 0; i < proc_count; i++) {
//    API::conf_object_t * cpu = &(proc_list[i]);
//    int qemu_id = qemu_callbacks.QEMU_get_cpu_index(cpu);
//    DBG_(VVerb, ( << "Processor " << i << ": " << cpu->name << " - CPU " <<
//    qemu_id ));

//    bool is_client = false;
//    num_flexus_cpus++;
//    int vm = 0;
//    cpu_list.push_back(cpu_desc_t(vm, proc_count, qemu_id, is_client));
//    //ALEX: Not sure about 2nd parameter
//  }

//  DBG_(VVerb  , ( << "Found " << num_flexus_cpus << " Flexus CPUs and " <<
//  num_client_cpus << " Client CPUs and " << num_besim_cpus << " Besim CPUs in
//  " << (max_vm + 1) << " VMs")); theProcMap.resize(num_flexus_cpus,
//  std::make_pair(0, 0)); theClientMap.resize(num_client_cpus + num_besim_cpus,
//  0); theReverseMap.resize(proc_count, std::make_pair(0, false));

//  int client_index = 0;
//  int flexus_index = 0;

//  std::sort(cpu_list.begin(), cpu_list.end(), cpu_desc_sorter_t());

//  std::vector<cpu_desc_t>::iterator iter = cpu_list.begin();
//#if 0
//  for (; iter != cpu_list.end(); iter++) {
//    theReverseMap[iter->qemu_id] = std::make_pair(flexus_index,
//    iter->is_client); if (iter->is_client) {
//      theClientMap[client_index] = iter->qemu_id;
//      DBG_(Verb, ( << "Client[" << client_index << "] = " << iter->qemu_id ));
//      client_index++;
//    } else {
//      theProcMap[flexus_index].first = iter->qemu_id;
//      theProcMap[flexus_index].second = iter->vm;
//      DBG_(Verb, ( << "theProcMap[" << flexus_index << "] = (" <<
//      iter->qemu_id << ", " << iter->vm << ")" )); flexus_index++;
//    }
//  }
//#endif
//  theNumVMs = max_vm + 1;
//  if (theNumVMs == 0) {
//    theNumVMs = 1;
//  }

////  char *charThreads,*charManual;
//  int /*found,start,end,*/numThreads,manual;

//  // Search for the number of threads and read the value of this parameter
//  std::string strThreads =
//  Flexus::Core::ConfigurationManager::getParameterValue(":threads", false); if
//  (strThreads == "not_found")
//    numThreads = 1;
//  else
//    numThreads = atoi(strThreads.c_str());

//  DBG_Assert(numThreads > 0, ( << "Number of threads per core: " <<
//  numThreads));

//  // Search whether we have manual organization or not
//  std::string strManual =
//  Flexus::Core::ConfigurationManager::getParameterValue("-mapper:manual_organization");
//  if (strManual == "not_found")
//    manual = 0;
//  else
//    manual = atoi(strManual.c_str());

//  int num_rows=0,cores_per_vm=0, sqrt_cores_per_vm=0, sqrt_numVMs=0,
//  vms_per_row=0,vms_per_col=0,num_vm_rows=0,num_vm_cols=0; std::vector<int>
//  *machines = new std::vector<int>[theNumVMs]; std::vector<int>
//  organization(num_flexus_cpus/numThreads);

//  if (manual == 0){
//    DBG_(Dev, ( << "Automatic organization of equal-size VMs"));
//    cores_per_vm = (num_flexus_cpus/numThreads) / theNumVMs;
//    sqrt_cores_per_vm = (int)std::ceil(std::sqrt(cores_per_vm));
//    sqrt_numVMs = (int)std::ceil(std::sqrt(theNumVMs));
//    if (sqrt_numVMs*sqrt_numVMs == theNumVMs){ // Square number of VMs
//      vms_per_row = (int)std::ceil(std::sqrt(theNumVMs));
//      vms_per_col = vms_per_row;
//      DBG_(Crit, ( << "Square topology of VMs: " << vms_per_col << "x" <<
//      vms_per_row )); if (sqrt_cores_per_vm*sqrt_cores_per_vm ==
//      cores_per_vm){ // Square number of cores per VM
//        num_vm_rows = (int)std::ceil(std::sqrt(cores_per_vm));
//        num_vm_cols = num_vm_rows;
//        DBG_(Crit, ( << "Square VM: " << num_vm_rows << "x" << num_vm_cols ));
//      }
//      else { // Non square number of cores per VM. Dividing by 2 it becomes
//      square number.
//             // e.g. 32 --> 32/2 = 16 and hence 8 rows of 4 cores each.
//         // so the VMs are mapped correctly according to the chip topology.
//        num_vm_cols = (int) std::ceil(std::sqrt(cores_per_vm/2));
//        num_vm_rows = cores_per_vm/num_vm_cols;
//        DBG_(Crit, ( << "Rectangular VM: " << num_vm_rows << "x" <<
//        num_vm_cols ));
//      }
//    }
//    else { // Non square number of VMs. Dividing by it becomes square number.
//       // e.g. 8 --> 8/2 = 4 and hence 2 rows of 4 VMs each.
//      vms_per_row = (int)std::ceil(std::sqrt(theNumVMs/2));
//      vms_per_col = theNumVMs/vms_per_row;
//      DBG_(Crit, ( << "Rectangular topology of VMs (rowsXcols): " <<
//      vms_per_col << "x" << vms_per_row )); if
//      (sqrt_cores_per_vm*sqrt_cores_per_vm == cores_per_vm){ // Square number
//      of cores per VM
//        num_vm_rows = (int)std::ceil(std::sqrt(cores_per_vm));
//        num_vm_cols = num_vm_rows;
//        DBG_(Crit, ( << "Square VM: " << num_vm_rows << "x" << num_vm_cols ));
//      }
//      else { // Non square number of cores per VM. Dividing by 2 it becomes
//      square number.
//         // e.g. 32 --> 32/2 = 16 and hence 4 rows of 8 cores each.
//        num_vm_rows = (int) std::ceil(std::sqrt(cores_per_vm/2));
//        num_vm_cols = cores_per_vm/num_vm_rows;
//        DBG_(Crit, ( << "Rectangular VM: " << num_vm_rows << "x" <<
//        num_vm_cols ));
//      }
//    }
//    num_rows = num_vm_rows*vms_per_col;
//    DBG_(Crit, ( << "VMS per row = " << vms_per_row << ", CPVM = " <<
//    cores_per_vm << ", GridOfVM = " << num_vm_cols << "*" << num_vm_rows << ",
//    NumRows = " << num_rows ));
//  }
//  else {
//    DBG_(Dev, ( << "Manual organization of VMs according to the configuration
//    file")); int i=0,j=0;
//    // Search for the organization parameter
//    std::string strOrg =
//    Flexus::Core::ConfigurationManager::getParameterValue("-mapper:organization");
//    DBG_Assert(strOrg != "not_found", ( << "-mapper:organization parameter
//    does not exist in the configuration file" ));
///*
//    found=(ss.str()).find(str4)+str4.length();
//    start=(ss.str()).find(str,found);
//    end=(ss.str()).find(str,start+1);
//    std::string strOrg = (ss.str()).substr(start+1,end-start-1);
//*/
//    char *charOrg = new char[strOrg.length()+1];
//    strcpy(charOrg, strOrg.c_str());
//    std::string *tokens = new std::string[theNumVMs];
//    char *temp=strtok(charOrg,";");
//    while (temp!=NULL){
//      tokens[i++]=temp;
//      temp = strtok (NULL, ";");
//    }
//    delete charOrg;
//    // the organization vector will have the new indices and the machine
//    vector will have them in muliple rows; one for each VM for
//    (i=0;i<theNumVMs;i++){
//      char *charMachine = new char[tokens[i].length()+1];
//      strcpy(charMachine,tokens[i].c_str());
//      temp=strtok(charMachine,",");
//      while (temp!=NULL){
//        organization[j++]=atoi(temp);
//        machines[i].push_back(atoi(temp));
//        temp = strtok (NULL,",");
//      }
//      delete charMachine;
//    }
//  }

//  for(; iter != cpu_list.end(); iter++) {
//    theReverseMap[iter->qemu_id] = std::make_pair(flexus_index,
//    iter->is_client); if (iter->is_client) {
//      theReverseMap[iter->qemu_id] = std::make_pair(-1, iter->is_client);
//      theClientMap[client_index] = iter->qemu_id;
//      DBG_(Crit, ( << "Client[" << client_index << "] = " << iter->qemu_id ));
//      client_index++;
//    }
//    else {
//      if (manual==0){ // re-map flexus index, so VMs are organized in a nice
//      grid
//        int core_index = flexus_index / numThreads;
//        int vm = core_index / cores_per_vm;
//        int core = core_index % cores_per_vm;
//        int row_in_vm = core / (num_vm_cols);
//        int col_in_vm = core % (num_vm_cols);
//        int row_of_vm = vm / vms_per_row;
//        int col_of_vm = vm % vms_per_row;
//        int abs_row = row_in_vm + row_of_vm*num_vm_rows;
//        int abs_col = col_in_vm + col_of_vm*num_vm_cols;
//        int new_index = abs_row*(num_flexus_cpus/num_rows) +
//        abs_col*numThreads+flexus_index%numThreads;
//        theReverseMap[iter->qemu_id] = std::make_pair(new_index,
//        iter->is_client); theProcMap[new_index].first = iter->qemu_id;
//        theProcMap[new_index].second = iter->vm;
//        DBG_(Crit, ( << "theProcMap[" << new_index << "] = (" << iter->qemu_id
//        << ", " << iter->vm << ") (abs_index = " << flexus_index << ")" ));
//        flexus_index++;
//      }
//      else { // re-map flexus index, so VMs are organized according to user
//      needs
//        int new_index =
//        organization[flexus_index/numThreads]*numThreads+flexus_index%numThreads;
//        theReverseMap[iter->qemu_id] = std::make_pair(new_index,
//        iter->is_client); theProcMap[new_index].first = iter->qemu_id;
//        theProcMap[new_index].second = iter->vm;
//        DBG_(Crit, ( << "theProcMap[" << new_index << "] = (" << iter->qemu_id
//        << ", " << iter->vm << ") (abs_index = " << flexus_index << ")" ));
//        flexus_index++;
//      }
//    }
//  }
//  delete [] machines;
//  DBG_(Dev, ( << "Finished creating Processor Mapper."));
//}

// int ProcessorMapper::mapFlexusIndex2ProcNum(int index) {
//  if (theMapper == NULL) {
//    theMapper = new ProcessorMapper();
//  }
//  return theMapper->theProcMap[index].first;
//}

// int ProcessorMapper::mapClientNum2ProcNum(int index) {
//  if (theMapper == NULL) {
//    theMapper = new ProcessorMapper();
//  }
//  return theMapper->theClientMap[index];
//}

// int ProcessorMapper::mapProcNum2FlexusIndex(int index) {
//  if (theMapper == NULL) {
//    theMapper = new ProcessorMapper();
//  }
//  return theMapper->theReverseMap[index].first;
//}

// int ProcessorMapper::mapFlexusIndex2VM(int index) {
//  if (theMapper == NULL) {
//    theMapper = new ProcessorMapper();
//  }
//  if (theMapper->theNumVMs == 0) {
//    return 0;
//  }
//  return theMapper->theProcMap[index].second;
//}

// int ProcessorMapper::numVMs() {
//  if (theMapper == NULL) {
//    theMapper = new ProcessorMapper();
//    DBG_Assert(theMapper != NULL);
//  }
//  return theMapper->theNumVMs;
//}

// int ProcessorMapper::numClients() {
//  if (theMapper == NULL) {
//    theMapper = new ProcessorMapper();
//  }
//  return (int)(theMapper->theClientMap.size());
//}

// int ProcessorMapper::numProcessors() {
//  if (theMapper == NULL) {
//    theMapper = new ProcessorMapper();
//  }
//  return (int)(theMapper->theProcMap.size());
//}

} // end Namespace Qemu
} // end namespace Flexus
