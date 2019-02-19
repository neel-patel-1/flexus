// DO-NOT-REMOVE begin-copyright-block 
//
// Redistributions of any form whatsoever must retain and/or include the
// following acknowledgment, notices and disclaimer:
//
// This product includes software developed by Carnegie Mellon University.
//
// Copyright 2012 by Mohammad Alisafaee, Eric Chung, Michael Ferdman, Brian 
// Gold, Jangwoo Kim, Pejman Lotfi-Kamran, Onur Kocberber, Djordje Jevdjic, 
// Jared Smolens, Stephen Somogyi, Evangelos Vlachos, Stavros Volos, Jason 
// Zebchuk, Babak Falsafi, Nikos Hardavellas and Tom Wenisch for the SimFlex 
// Project, Computer Architecture Lab at Carnegie Mellon, Carnegie Mellon University.
//
// For more information, see the SimFlex project website at:
//   http://www.ece.cmu.edu/~simflex
//
// You may not use the name "Carnegie Mellon University" or derivations
// thereof to endorse or promote products derived from this software.
//
// If you modify the software you must place a notice on or within any
// modified version provided or made available to any third party stating
// that you have modified the software.  The notice shall include at least
// your name, address, phone number, email address and the date and purpose
// of the modification.
//
// THE SOFTWARE IS PROVIDED "AS-IS" WITHOUT ANY WARRANTY OF ANY KIND, EITHER
// EXPRESS, IMPLIED OR STATUTORY, INCLUDING BUT NOT LIMITED TO ANY WARRANTY
// THAT THE SOFTWARE WILL CONFORM TO SPECIFICATIONS OR BE ERROR-FREE AND ANY
// IMPLIED WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE,
// TITLE, OR NON-INFRINGEMENT.  IN NO EVENT SHALL CARNEGIE MELLON UNIVERSITY
// BE LIABLE FOR ANY DAMAGES, INCLUDING BUT NOT LIMITED TO DIRECT, INDIRECT,
// SPECIAL OR CONSEQUENTIAL DAMAGES, ARISING OUT OF, RESULTING FROM, OR IN
// ANY WAY CONNECTED WITH THIS SOFTWARE (WHETHER OR NOT BASED UPON WARRANTY,
// CONTRACT, TORT OR OTHERWISE).
//
// DO-NOT-REMOVE end-copyright-block


#include <iostream>
#include <iomanip>

#include <core/boost_extensions/intrusive_ptr.hpp>
#include <boost/throw_exception.hpp>
#include <boost/function.hpp>
#include <boost/lambda/lambda.hpp>
#include <boost/lambda/bind.hpp>
namespace ll = boost::lambda;

#include <boost/none.hpp>

#include <boost/dynamic_bitset.hpp>

#include <core/target.hpp>
#include <core/debug/debug.hpp>
#include <core/types.hpp>

#include <components/uArchARM/uArchInterfaces.hpp>

#include "../SemanticInstruction.hpp"
#include "../Effects.hpp"
#include "../SemanticActions.hpp"

#define DBG_DeclareCategories armDecoder
#define DBG_SetDefaultOps AddCat(armDecoder)
#include DBG_Control()

namespace narmDecoder {

using namespace nuArchARM;

struct ReadRegisterAction : public BaseSemanticAction
{
  eOperandCode theRegisterCode;
  eOperandCode theOperandCode;
  bool theConnected;
  bool the64;
  bool theSP;

  ReadRegisterAction( SemanticInstruction * anInstruction, eOperandCode aRegisterCode, eOperandCode anOperandCode, bool aSP, bool is64)
    : BaseSemanticAction( anInstruction, 1 )
    , theRegisterCode( aRegisterCode )
    , theOperandCode( anOperandCode )
    , theConnected(false)
    , the64(is64)
    , theSP (aSP)
  {}

  bool bypass(register_value aValue)
  {
      theSP = false;

      if (!theSP) {
        if ( cancelled() || theInstruction->isRetired() || theInstruction->isSquashed() ) { return true; }

        if ( !signalled() ) {
          theInstruction->setOperand(theOperandCode, aValue);
          DBG_( VVerb, ( << *this << " bypassed " << theRegisterCode << " = " << aValue << " written to " << theOperandCode ) );
          satisfyDependants();
          setReady(0, true);
        }
        return false;
      }
      return true;
  }

  void doEvaluate()
  {
    DBG_(Dev, (<<*this));

    theSP = false;

        register_value aValue;
        uint64_t val;
        if (theSP) {
            if (core()->_PSTATE().SP() == 0) {
                val = core()->getSP_el(EL0);
            } else {
                switch (core()->_PSTATE().EL()) {
                case EL0:
                    val = core()->getSP_el(EL0);
                    break;
                case EL1:
                    val = core()->getSP_el(EL1);
                    break;
                case EL2:
                case EL3:
                default:
                    DBG_Assert(false);
//                    theInstruction->setWillRaise(kException_ILLEGALSTATE);
//                    core()->takeTrap(boost::intrusive_ptr<Instruction>(theInstruction), theInstruction->willRaise());
                    break;
                }
            }
        } else {
            if (! theConnected) {

              mapped_reg name = theInstruction->operand< mapped_reg > (theRegisterCode);
              setReady( 0, core()->requestRegister( name, theInstruction->makeInstructionDependance(dependance()) ) == kReady );
              core()->connectBypass( name, theInstruction, ll::bind( &ReadRegisterAction::bypass, this, ll::_1) );
              theConnected = true;

            }
            if (! signalled() ) {
              SEMANTICS_DBG("Signalling");

              mapped_reg name = theInstruction->operand< mapped_reg > (theRegisterCode);
              eResourceStatus status = core()->requestRegister( name );

              if (status == kReady) {
                aValue = core()->readRegister( name );
                val = boost::get<uint64_t>(aValue);
              } else {
                  setReady( 0, false );
                  return;
                }
            }
        }

        if (!the64){ // reading w reg >> only the botom half
            val &= 0xffffffff;
        }

        aValue = val;

        DBG_(Dev,(<< "Reading register " << theRegisterCode << " with a value " << aValue));

        theInstruction->setOperand(theOperandCode, val);
        satisfyDependants();
  }

  void describe( std::ostream & anOstream) const
  {
    anOstream << theInstruction->identify() << " ReadRegister " << theRegisterCode << " store in " << theOperandCode;
  }
};

simple_action readRegisterAction ( SemanticInstruction * anInstruction, eOperandCode aRegisterCode, eOperandCode anOperandCode, bool aSP, bool is64)
{
  return new(anInstruction->icb()) ReadRegisterAction( anInstruction, aRegisterCode, anOperandCode, aSP, is64);
}


} //narmDecoder
