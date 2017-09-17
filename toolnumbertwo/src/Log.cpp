//===- Log.cpp ----------------------------------------------------*- C++ -*-===//
//
//  Copyright (C) 2016, 2017  Antonio Tammaro (ntonjeta@autistici.org)
// 
//  This file is part of Toolnumbertwo.
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
/// \file Log.cpp
/// \author Antonio Tammaro
/// \brief This file contains a logging facility
//===----------------------------------------------------------------------===//

#include "Log.h"

using namespace toolnumbertwo;

INITIALIZE_EASYLOGGINGPP

const char* log::ToolnumbertwoLogger::loggerName = "tooltwoLogger";  ///< Member initialization
el::Configurations log::ToolnumbertwoLogger::configurator =
    el::Configurations();
log::VerboseLevel log::ToolnumbertwoLogger::actualVLevel = 0;

void log::ToolnumbertwoLogger::init() {
  /// Configure el++ : toolnumeroneLogger
  configurator.setGlobally(el::ConfigurationType::Enabled, "false");
  configurator.setGlobally(el::ConfigurationType::ToFile, "false");
  configurator.set(el::Level::Error, el::ConfigurationType::ToStandardOutput,
                   "true");
  // Debug level
  configurator.set(el::Level::Debug, el::ConfigurationType::Enabled, "true");
  configurator.set(el::Level::Debug, el::ConfigurationType::Format,
                   "[TOOLNUMBERTWO DEBUG] %msg");

  // Warning Level
  configurator.set(el::Level::Info, el::ConfigurationType::Enabled, "true");
  configurator.set(el::Level::Info, el::ConfigurationType::Format,
                   "[TOOLNUMBERTWO INFO] %msg");

  // Warning Level
  configurator.set(el::Level::Warning, el::ConfigurationType::Enabled, "true");
  configurator.set(el::Level::Warning, el::ConfigurationType::Format,
                   "[TOOLNUMBERTWO WARNING] %msg");

  // Error Level
  configurator.set(el::Level::Error, el::ConfigurationType::Enabled, "true");
  configurator.set(el::Level::Error, el::ConfigurationType::Format,
                   "[TOOLNUMBERTWO ERROR] %msg");
  // Fatal Level
  configurator.set(el::Level::Fatal, el::ConfigurationType::Enabled, "true");
  ;
  configurator.set(el::Level::Fatal, el::ConfigurationType::Format,
                   "[TOOLNUMBERTWO FATAL] %msg");
  el::Loggers::reconfigureLogger(loggerName, configurator);
}

void log::ToolnumbertwoLogger::initVerbose() {
  // Verbose Level
  configurator.set(el::Level::Verbose, el::ConfigurationType::Enabled, "true");
  configurator.set(el::Level::Verbose, el::ConfigurationType::ToFile, "false");
  configurator.set(el::Level::Verbose, el::ConfigurationType::ToStandardOutput,
                   "true");
  configurator.set(el::Level::Verbose, el::ConfigurationType::Format,
                   "[TOOLNUMBERTWO VERBOSE-%vlevel] %msg");
  el::Loggers::reconfigureLogger(loggerName, configurator);
}

void log::ToolnumbertwoLogger::initTrace() {
  // Init the verbose level
  initVerbose();
  // Debug Level
  configurator.set(el::Level::Trace, el::ConfigurationType::Enabled, "true");
  configurator.set(el::Level::Trace, el::ConfigurationType::ToFile, "false");
  configurator.set(el::Level::Trace, el::ConfigurationType::ToStandardOutput,
                   "true");
  configurator.set(el::Level::Trace, el::ConfigurationType::Format,
                   "[TOOLNUMBERTWO TRACE] %msg");
  el::Loggers::reconfigureLogger(loggerName, configurator);
}

void log::ToolnumbertwoLogger::init(int argc, const char** argv) {
  START_EASYLOGGINGPP(argc, argv);
  init();
}

void log::ToolnumbertwoLogger::verbose(VerboseLevel vlevel,
                                          const std::string& msg) {
  // Check if the logger has been registered
  if (!isInitialize())
    return;
  // Check the verbose level
  if (!VLOG_IS_ON(vlevel))
    return;
  std::string message = "";
  for (unsigned char i = 0; i < vlevel; i++) {
    message += "=";
  }
  message += "> " + msg;
  CVLOG(vlevel,loggerName) << message;
}
