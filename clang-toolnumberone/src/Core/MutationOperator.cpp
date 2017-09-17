//===- MutationOperator.cpp -------------------------------------*- C++ -*-===//
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
/// \file MutationOperator.cpp
/// \author Federico Iannucci
/// \date 30 nov 2015
/// \brief This file contains the implementation file for the class 
///        MutationOperator
//===----------------------------------------------------------------------===//

#include "Core/MutationOperator.h"

using namespace llvm;
using namespace toolnumberone::m_operator;

toolnumberone::m_operator::MutationOperator::MutationOperator(IdType id,
                                                        std::string descr,
                                                        bool isHOM)
    : isHOM(isHOM), identifier(id), description(descr) {}
