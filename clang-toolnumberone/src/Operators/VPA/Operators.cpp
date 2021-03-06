//===- Operators.cpp -----------------------------------------------*- C++ -*-===//
//
//  Copyright (C) 2017  
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
/// \author 
/// \brief This file contains operators implementation for VPA
//===----------------------------------------------------------------------===//

#include "Operators/VPA/Operator.h"
#include "Operators/VPA/Mutators.h"

std::unique_ptr<::toolnumberone::m_operator::MutationOperator> 
toolnumberone::vpamutator::getVPAOperator() {
  std::unique_ptr<::toolnumberone::m_operator::MutationOperator> Op(
      new ::toolnumberone::m_operator::MutationOperator(
        "VPAOperator",
        "IIDEAA Code Instrumentation",
        true)
      );

  // Add mutators to the current operator
  Op->addMutator(
      ::toolnumberone::m_operator::MutatorPtr(new ::toolnumberone::vpamutator::VPAFloatOperationMutator()));

  return Op;
}
