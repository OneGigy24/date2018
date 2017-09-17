//===- Mutators.h -----------------------------------------------*- C++ -*-===//
//
//  Copyright (C) 2015, 2016  Federico Iannucci (fed.iannucci@gmail.com)
//
//  This file is part of Clang-Toolnumberone.
//
//  Clang-Toolnumberone is free software: you can redistribute it and/or modify
//  it under the terms of the GNU Affero General Public License as published by
//  the Free Software Foundation, either version 3 of the License, or
//  (at your option) any later version.
//
//  Clang-Toolnumberone is distributed in the hope that it will be useful,
//  but WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//  GNU Affero General Public License for more details.
//
//  You should have received a copy of the GNU Affero General Public License
//  along with Clang-Toolnumberone. If not, see <http://www.gnu.org/licenses/>.
//
//===----------------------------------------------------------------------===//
/// \file Mutators.h
/// \author Federico Iannucci
/// \brief This file contains sample mutators
//===----------------------------------------------------------------------===//

#ifndef INCLUDE_OPERATORS_EXAMPLES_MUTATORS_H
#define INCLUDE_OPERATORS_EXAMPLES_MUTATORS_H

#include "Core/Mutator.h"

namespace toolnumberone
{
namespace examples
{

/// \addtogroup OPERATORS_SAMPLE_MUTATORS Sample Mutators
/// \{

/**
 * @brief This mutator matches the binary relational operator >, and replaces it with
 *        others (at the moment < and <=).
 */
class MutatorGreaterOpReplacement : public toolnumberone::mutator::Mutator
{
public:
    /**
     * @brief Constuctor
     */
    MutatorGreaterOpReplacement()
        : Mutator ( ::toolnumberone::mutator::StatementMatcherType, // A binary operator is a statement
                    "mutator_greater_op_replacement", // String identifier
                    "Replaces > with < and <=", // Description
                    2 // Two mutation types
                  ) {}
    virtual clang::ast_matchers::StatementMatcher getStatementMatcher() override; // Need to override this method, first part of matching rules
    virtual bool match ( const ::toolnumberone::mutator::NodeType &node ) override; // Also this one, second part of matching rules
    virtual bool getMatchedNode ( const toolnumberone::mutator::NodeType &,
                                  clang::ast_type_traits::DynTypedNode & ) override; // This is pure virtual and must be implemented
    virtual clang::Rewriter &mutate ( const toolnumberone::mutator::NodeType &node,
                                      mutator::MutatorType type,
                                      clang::Rewriter &rw ) override; // mutation rules
};

/// \}
} // end namespace toolnumberone::examples
} // end namespace toolnumberone

#endif /* INCLUDE_OPERATORS_EXAMPLES_MUTATORS_H */
