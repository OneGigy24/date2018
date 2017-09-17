/******************************************************************************
 * @file   OperatorIIDEAA.cpp
 * @author Federico Iannucci
 * @date   24 gen 2016
 * @brief  TODO
 ******************************************************************************/

#include "Operators/FLAP/Operator.h"
#include "Operators/FLAP/Mutators.h"

std::unique_ptr<::toolnumberone::m_operator::MutationOperator> 
toolnumberone::flapmutator::getFLAPOperator() {
  std::unique_ptr<::toolnumberone::m_operator::MutationOperator> Op(
      new ::toolnumberone::m_operator::MutationOperator(
        "FLAPOperator",
        "IIDEAA Code Instrumentation",
        true)
      );

  // Add mutators to the current operator
  Op->addMutator(
      ::toolnumberone::m_operator::MutatorPtr(new ::toolnumberone::flapmutator::FLAPFloatOperationMutator()));

  return Op;
}
