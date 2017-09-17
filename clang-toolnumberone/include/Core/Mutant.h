//===- Mutant.h ------------------------------------------------*- C++ -*-===//
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
/// \file Mutant.h
/// \author Federico Iannucci
/// \brief This file contains the class Mutant
//===----------------------------------------------------------------------===//

#ifndef INCLUDE_MUTANT_H_
#define INCLUDE_MUTANT_H_

namespace toolnumberone {
namespace mutant {

using IdType = unsigned int;

/**
 * @brief Generic Mutant Class
 */
class Mutant {
  // Typedefs and using
 public:
 private:
  IdType id; /**< Unique identifier */
};

}  // End toolnumberone::mutant namespace
}  // End toolnumberone namespace

#endif /* INCLUDE_MUTANT_H_ */
