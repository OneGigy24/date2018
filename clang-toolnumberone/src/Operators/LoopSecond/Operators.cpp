//===- Operators.cpp ---------------------------------------------*- C++-*-===//
//
//  Copyright (C) 2015, 2016  Antonio Tammaro (ntonjeta@autistici.org)
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
/// \file Operators.cpp
/// \author Antonio Tammaro  
/// \brief This file contains loop perforation operators
//===----------------------------------------------------------------------===//

#include "Operators/LoopSecond/Mutators.h"
#include "Operators/LoopSecond/Operator.h"

::std::unique_ptr<::toolnumberone::m_operator::MutationOperator>
toolnumberone::perforation::getPerforationSecondOperator()
{
  ::std::unique_ptr<::toolnumberone::m_operator::MutationOperator> Op(
      new ::toolnumberone::m_operator::MutationOperator(
          "LoopPerforationOperator2",   // Operator identifier
          "Define operator for loop perforation tecnique", // Description
          true)
      );

  // Add mutators to the current operator
  Op->addMutator(
      ::toolnumberone::m_operator::MutatorPtr(new ::toolnumberone::perforation::MutatorLoopPerforation2()));

  // Return the operator
  return Op;
}
