#include <components/MemoryLoopback/MemoryLoopbackImpl.cpp>
#include <components/MemoryMap/MemoryMapImpl.cpp>
#include <core/flexus.hpp>
#include <gtest/gtest.h>

// Create fixture for testing the DUT
class MemoryLoopbackTestFixture : public testing::Test {
  static MemoryMapConfiguration_struct *aMMCfg;      // A dummy configuration
  static MemoryMapJumpTable *aMMJumpTable;           // A dummy jump table
  static nMemoryMap::MemoryMapComponent *aMemoryMap; // A dummy MemoryMap

protected:
  static void SetUpTestCase() {

    // Create Flexus base
    Flexus::Core::CreateFlexusObject();

    // Create a memory map to construct a memory map factory object which is used by
    aMMCfg = new MemoryMapConfiguration_struct("A MemoryMap config"); // A dummy configuration
    aMMJumpTable = new MemoryMapJumpTable;                            // A dummy jump table
    Flexus::Core::index_t anIndex = 1;
    Flexus::Core::index_t aWidth = 1;
    aMemoryMap = new nMemoryMap::MemoryMapComponent( // A dummy MemoryMap instance
        *aMMCfg, *aMMJumpTable, anIndex, aWidth);
  }

  static void TearDownTestCase() {
    delete aMMCfg;
    delete aMMJumpTable;
    delete aMemoryMap;

    // Exit flexus
    Flexus::Core::flexusStop();
  }

  void SetUp() {
  }
  void TearDown() {
  }
};

MemoryMapConfiguration_struct *MemoryLoopbackTestFixture::aMMCfg = nullptr; // A dummy configuration
MemoryMapJumpTable *MemoryLoopbackTestFixture::aMMJumpTable = nullptr;      // A dummy jump table
nMemoryMap::MemoryMapComponent *MemoryLoopbackTestFixture::aMemoryMap =
    nullptr; // A dummy MemoryMap

#include "delayTest.h"
#include "maxRequestsTest.h"
#include "useFetchReplyTest.h"
#include "correctMessageTypeTest.h"
