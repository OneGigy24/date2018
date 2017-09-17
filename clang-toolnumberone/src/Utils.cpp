//===- Utils.cpp ------------------------------------------------*- C++ -*-===//
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
/// \file Utils.cpp
/// \author Federico Iannucci
/// \brief This file implements a utility functions
//===----------------------------------------------------------------------===//

#include "Utils.h"

#include "llvm/Support/FileSystem.h"
#include "llvm/Support/Path.h"

#include <fstream>
#include <sstream>

using namespace std;
using namespace llvm;

using StringVector = std::vector<std::string>;

bool toolnumberone::conf::readFunctionsOperatorsConfFile(
    const std::string& filename, std::map< ::std::string, StringVector> &fileMap) {
  // Key
  ::std::string function;
  ::std::vector<std::string> operators;
  // Open the file
  fstream confFile(filename);
  if (!confFile.good()) {
    return false;
  }
  // Read line by line
  std::string line;
  std::getline(confFile, line);  // Read the first line
  while (confFile.good()) {
    // Clear vector
    operators.clear();
    // Read tokens delimited by ','
    stringstream lineTokens(line);
    std::string op;
    // Read first token -> it's the key of the entry
    std::getline(lineTokens, function, ',');
    // Check if it's a comment
    if (function.substr(0, 2) != "//") {
      while (lineTokens.good()) {
        std::getline(lineTokens, op, ',');
        // Populate the vector
        operators.push_back(op);
      }
      // Create map entry
      fileMap.insert(std::pair<std::string, StringVector>(function, operators));
    }
    std::getline(confFile, line);  // Read a new line
  }
  return true;
}

// Filesystem
const char toolnumberone::fs::pathSep = getPathSeparator();

char toolnumberone::fs::getPathSeparator() {
  return *(sys::path::get_separator().data());
}

std::string toolnumberone::fs::getParentPath(const std::string& path) {
  // Use this trick because llvm::sys::path::parent_path doesn't work
  std::size_t found = path.find_last_of(pathSep);
  return path.substr(0, found);
}

bool toolnumberone::fs::createDirectories(const llvm::Twine& path,
                                    bool ignoreExisting) {
  // Create directories
  sys::fs::create_directories(path, ignoreExisting);
  return sys::fs::is_directory(path);
}
