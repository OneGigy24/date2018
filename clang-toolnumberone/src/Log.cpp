//===- Log.cpp --------------------------------------------------*- C++ -*-===//
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
/// \file Log.cpp
/// \author Federico Iannucci
/// \brief This file implements a logging facility
//===----------------------------------------------------------------------===//

#include "Log.h"

using namespace toolnumberone;

INITIALIZE_EASYLOGGINGPP

const char* toolnumberone::log::ToolnumberoneLogger::loggerName = "toolnumberoneLogger";  ///< Member initialization
el::Configurations toolnumberone::log::ToolnumberoneLogger::configurator =
    el::Configurations();
log::VerboseLevel toolnumberone::log::ToolnumberoneLogger::actualVLevel = 0;

void toolnumberone::log::ToolnumberoneLogger::init() {
  /// Configure el++ : toolnumberoneLogger
  configurator.setGlobally(el::ConfigurationType::Enabled, "false");
  configurator.setGlobally(el::ConfigurationType::ToFile, "false");
  configurator.set(el::Level::Error, el::ConfigurationType::ToStandardOutput,
                   "true");
  // Debug level
  configurator.set(el::Level::Debug, el::ConfigurationType::Enabled, "true");
  configurator.set(el::Level::Debug, el::ConfigurationType::Format,
                   "[CHIMERA DEBUG] %msg");

  // Warning Level
  configurator.set(el::Level::Info, el::ConfigurationType::Enabled, "true");
  configurator.set(el::Level::Info, el::ConfigurationType::Format,
                   "[CHIMERA INFO] %msg");

  // Warning Level
  configurator.set(el::Level::Warning, el::ConfigurationType::Enabled, "true");
  configurator.set(el::Level::Warning, el::ConfigurationType::Format,
                   "[CHIMERA WARNING] %msg");

  // Error Level
  configurator.set(el::Level::Error, el::ConfigurationType::Enabled, "true");
  configurator.set(el::Level::Error, el::ConfigurationType::Format,
                   "[CHIMERA ERROR] %msg");
  // Fatal Level
  configurator.set(el::Level::Fatal, el::ConfigurationType::Enabled, "true");
  ;
  configurator.set(el::Level::Fatal, el::ConfigurationType::Format,
                   "[CHIMERA FATAL] %msg");
  el::Loggers::reconfigureLogger(loggerName, configurator);
}

void toolnumberone::log::ToolnumberoneLogger::initVerbose() {
  // Verbose Level
  configurator.set(el::Level::Verbose, el::ConfigurationType::Enabled, "true");
  configurator.set(el::Level::Verbose, el::ConfigurationType::ToFile, "false");
  configurator.set(el::Level::Verbose, el::ConfigurationType::ToStandardOutput,
                   "true");
  configurator.set(el::Level::Verbose, el::ConfigurationType::Format,
                   "[CHIMERA VERBOSE-%vlevel] %msg");
  el::Loggers::reconfigureLogger(loggerName, configurator);
}

void toolnumberone::log::ToolnumberoneLogger::initTrace() {
  // Init the verbose level
  initVerbose();
  // Debug Level
  configurator.set(el::Level::Trace, el::ConfigurationType::Enabled, "true");
  configurator.set(el::Level::Trace, el::ConfigurationType::ToFile, "false");
  configurator.set(el::Level::Trace, el::ConfigurationType::ToStandardOutput,
                   "true");
  configurator.set(el::Level::Trace, el::ConfigurationType::Format,
                   "[CHIMERA TRACE] %msg");
  el::Loggers::reconfigureLogger(loggerName, configurator);
}

void toolnumberone::log::ToolnumberoneLogger::init(int argc, const char** argv) {
  START_EASYLOGGINGPP(argc, argv);
  init();
}

void toolnumberone::log::ToolnumberoneLogger::verbose(VerboseLevel vlevel,
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
