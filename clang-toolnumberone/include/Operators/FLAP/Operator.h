/******************************************************************************
 * @file   IIDEAA.h
 * @author Federico Iannucci
 * @date   23 gen 2016
 * @brief  TODO
 ******************************************************************************/
#ifndef INCLUDE_IIDEAA_FLAPOPERATOR_H_
#define INCLUDE_IIDEAA_FLAPOPERATOR_H_

#include "Core/MutationOperator.h"

namespace toolnumberone {
namespace flapmutator{

  /// @brief FLAP Operation mutator
  /// @brief Create and return the IIDEAA operator
  /// @return the IIDEAA operator
  std::unique_ptr<::toolnumberone::m_operator::MutationOperator> getFLAPOperator();

}  // End namespace flap
}  // End namespace iideaa
#endif /* INCLUDE_IIDEAA_FLAPOPERATOR_H_ */
