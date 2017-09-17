

#ifndef INCLUDE_OPERATORS_PERFORATION2_OPERATORS_H
#define INCLUDE_OPERATORS_PERFORATION2_OPERATORS_H

#include "Core/MutationOperator.h"

namespace toolnumberone
{
namespace perforation 
{

// @brief Create and return the Loop Perforation Operator
::std::unique_ptr<::toolnumberone::m_operator::MutationOperator> getPerforationSecondOperator();


} // end namespace toolnumberone::perforation
} // end namespace toolnumberone

#endif /* INCLUDE_OPERATORS_PERFORATION_OPERATORS_H */

