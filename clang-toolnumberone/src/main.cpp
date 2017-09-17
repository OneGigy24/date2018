//===- main.cpp -------------------------------------------------*- C++ -*-===//
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
/// \file main.cpp
/// \author Federico Iannucci
/// \brief Application main function
//===----------------------------------------------------------------------===//

#include "Operators/Operators.h"
#include "Tooling/ToolnumberoneTool.h"

// For testing purpose
#include "Testing/MutatorsTesting.h"

int main(int argc, const char **argv) {
  // Create a Toolnumberone Tool
  ::toolnumberone::ToolnumberoneTool toolnumberoneTool;
  
  toolnumberoneTool.registerMutationOperator(::toolnumberone::flapmutator::getFLAPOperator());
  toolnumberoneTool.registerMutationOperator(::toolnumberone::vpamutator::getVPAOperator());
  toolnumberoneTool.registerMutationOperator(::toolnumberone::perforation::getPerforationFirstOperator());
  toolnumberoneTool.registerMutationOperator(::toolnumberone::perforation::getPerforationSecondOperator());

  return toolnumberoneTool.run(argc, argv);
}
