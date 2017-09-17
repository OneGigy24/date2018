//===- ToolnumberoneTool.cpp ------------------------------------------*- C++ -*-===//
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
/// \file ToolnumberoneTool.cpp
/// \author Federico Iannucci
/// \brief This file implements the class ToolnumberoneTool
//===----------------------------------------------------------------------===//

#include "Log.h"
#include "Core/MutationTemplate.h"
#include "Testing/ToolnumberoneTest.h"
#include "Tooling/ToolnumberoneTool.h"
#include "Tooling/CompilationDatabaseUtils.h"
#include "Tooling/FrontendActions.h"

#include "clang/Tooling/CommonOptionsParser.h"
#include "llvm/ADT/StringRef.h"
#include "llvm/Support/Debug.h"

#include <iostream>
#include <string>
#include <vector>

using namespace toolnumberone;

/// \addtogroup CHIMERA_CHIMERATOOL_CL_OPTIONS Command Line Options
/// \{
// Overview
const char *overview =
    "Without -generate-mutants the tool does a 'simulation' creating only the "
    "report.csv in <output_dir>/mutants/<source_filename>/ directory\n";

// Categories
::llvm::cl::OptionCategory catToolnumberone("clang-toolnumberone general options");

// Options
::llvm::cl::opt<bool> optDebug("debug", ::llvm::cl::desc("Enable debug output"),
                               ::llvm::cl::ValueDisallowed,
                               ::llvm::cl::cat(catToolnumberone),
                               ::llvm::cl::init(false));
::llvm::cl::opt<::std::string>
    optDebugOnly("debug-only", ::llvm::cl::desc("Enable a debug class"),
                 ::llvm::cl::ValueRequired,
                 ::llvm::cl::value_desc("debug-class"),
                 ::llvm::cl::cat(catToolnumberone), ::llvm::cl::init(""));

// Preprocess
enum PreprocessLevel {
  None,              ///< None preprocess
  ReformatOnly,      ///< Only reformat
  ExpandMacros,      ///< Reformat and expand macros
  CompletePreprocess ///< Complete preprocessing
};
::llvm::cl::opt<PreprocessLevel> optPreprocessLevel(
    "preprocess", ::llvm::cl::desc("Perfom the specified preprocess action to "
                                   "create the input file. The result will be "
                                   "saved in <output_dir>/resources"),
    ::llvm::cl::values(
        clEnumValN(
            ReformatOnly, "reformat",
            "Reformat the code only, to match the line:col of the reports"),
        clEnumValN(ExpandMacros, "expand-macros", "Reformat and expand macros"),
        clEnumValN(CompletePreprocess, "E",
                   "Preprocess as the -E compile option"),
        clEnumValEnd),
    ::llvm::cl::cat(catToolnumberone), ::llvm::cl::init(::PreprocessLevel::None));

// Modifiers
::llvm::cl::opt<bool> optVerbose("v", ::llvm::cl::desc("Enable verbose output"),
                                 ::llvm::cl::ValueDisallowed,
                                 ::llvm::cl::cat(catToolnumberone),
                                 ::llvm::cl::init(false));
::llvm::cl::opt<bool> optShowFunDef(
    "show-fun-def",
    ::llvm::cl::desc("Show the functions definition in the input file"),
    ::llvm::cl::ValueDisallowed, ::llvm::cl::cat(catToolnumberone),
    ::llvm::cl::init(false));
::llvm::cl::opt<bool>
    optGenerateMutants("generate-mutants",
                       ::llvm::cl::desc("Enable the mutants generation"),
                       ::llvm::cl::ValueDisallowed, ::llvm::cl::cat(catToolnumberone),
                       ::llvm::cl::init(false));
::llvm::cl::opt<bool> optNotGenerateReport(
    "no-generate-report",
    ::llvm::cl::desc("Disable the generation of the report"),
    ::llvm::cl::ValueDisallowed, ::llvm::cl::cat(catToolnumberone),
    ::llvm::cl::init(false));
::llvm::cl::opt<::std::string> optFunOpConfFile(
    "fun-op", ::llvm::cl::desc(
                  "The configuration file for functions/operations filtering"),
    ::llvm::cl::ValueRequired, ::llvm::cl::value_desc("file"),
    ::llvm::cl::cat(catToolnumberone), ::llvm::cl::init(""));
::llvm::cl::opt<::std::string> optOutputDir(
    "o", ::llvm::cl::desc("The output directory, default: ./toolnumberone_output/"),
    ::llvm::cl::ValueRequired, ::llvm::cl::value_desc("dir-path"),
    ::llvm::cl::cat(catToolnumberone), ::llvm::cl::init("./toolnumberone_output"));
::llvm::cl::opt<::std::string> optCompilationDatabaseDir(
    "cd-dir", ::llvm::cl::desc("Set the directory in which search for "
                               "compile_commmands.json. Passing this option "
                               "make useless trying to use the -- at the end "
                               "to pass compile commands manually."),
    ::llvm::cl::ValueRequired, ::llvm::cl::value_desc("dir-path"),
    ::llvm::cl::cat(catToolnumberone), ::llvm::cl::init(""));

::llvm::cl::opt<::std::string> optExecuteTest(
    "execute-test",
    ::llvm::cl::desc("Execute tests. This option disables the source input, "
                     "only other options will be accepted. The test directory should contains directories named as the identifier of the mutators to test."),
    ::llvm::cl::ValueRequired, ::llvm::cl::value_desc("test-dir"),
    ::llvm::cl::cat(catToolnumberone), ::llvm::cl::init(""));

::llvm::cl::opt<bool>
    optShowOperators("show-op",
                     ::llvm::cl::desc("Show the supported Mutation Operators"),
                     ::llvm::cl::ValueDisallowed, ::llvm::cl::cat(catToolnumberone),
                     ::llvm::cl::init(false));

// Utility functions
bool optIsOccured(const ::std::string &optString, int argc, const char **argv) {
  for (int i = 0; i < argc; ++i) {
    if (argv[i] == ("-" + optString)) {
      return true;
    }
  }
  return false;
}
/// \}

bool toolnumberone::ToolnumberoneTool::registerMutationOperator(
    ::toolnumberone::m_operator::MutationOperatorPtr op) {
  std::pair<typename MutationOperatorPtrMap::iterator, bool> retval =
      this->registeredOperatorMap.insert(
          std::pair<m_operator::IdType,
                    ::toolnumberone::m_operator::MutationOperatorPtr>(
              op->getIdentifier(), std::move(op)));
  return retval.second;
}

bool toolnumberone::ToolnumberoneTool::unregisterMutationOperator(
    const m_operator::IdType &id) {
  return this->registeredOperatorMap.erase(id);
}

const MutationOperatorPtrMap &
toolnumberone::ToolnumberoneTool::getRegisteredMutOperators() {
  return this->registeredOperatorMap;
}

int toolnumberone::ToolnumberoneTool::run(int argc, const char **argv) {
  // Initialization
  // Init the ToolnumberoneLogger
  ::toolnumberone::log::ToolnumberoneLogger::init();

  // Arguments Parsing
  // If there aren't, show the help
  if (argc == 1) {
    argc = 2;
    const char *help[2] = {argv[0], "-help"};
    ::clang::tooling::CommonOptionsParser helpOption(argc, help, catToolnumberone,
                                                     overview);
  }

  // Arguments that ends the execution
  if (optIsOccured(optShowOperators.ArgStr, argc, argv)) {
    ::llvm::outs() << "Supported Operators:\n";
    for (const auto &op : this->getRegisteredMutOperators()) {
      ::llvm::outs() << " * " << op.getValue()->getIdentifier() << " - "
                     << op.getValue()->getDescription() << "\n";
    }
    return 0;
  }

  ///////////////////////////////////////////////////////////////////////////////
  // These options must appear as first argument, and sources aren't needed
  if (optIsOccured(optExecuteTest.ArgStr, argc, argv)) {
    // When -execute-tests occurs the positional argument MUST NOT BE passed.
    llvm::cl::ParseCommandLineOptions(argc, argv, overview);
    toolnumberone::log::ToolnumberoneLogger::info(
        "*** Toolnumberone will now perform the tests [use -v for verbose]***");
    ::toolnumberone::testing::TestingOptions o;
    o.verbose = optVerbose;
    return ::toolnumberone::testing::runAllTest(argc, argv, optExecuteTest, o);
  }
  ///////////////////////////////////////////////////////////////////////////////
  // From now on the source input is required
  const char **argvv;
  // If compilationDatabaseDir occur, the CommonOptionsParser haven't to search
  // for compile_commands.json
  if (optIsOccured(optCompilationDatabaseDir.ArgStr, argc, argv)) {
    toolnumberone::log::ToolnumberoneLogger::info("A custom compilation database search "
                                      "path has been specified. Skipping "
                                      "default searching.");
    // Copy argv in argvv
    argvv = (const char **)new const char *[argc + 1];
    for (int i = 0; i < argc; ++i) {
      argvv[i] = argv[i];
    }
    // If is passed add a "--" to the end to prevent error from the
    // CommonOptionsParser.
    argvv[argc] =
        "--"; // Avoid to check for compile_commands.json in the directory tree
    argc++;
  } else {
    argvv = argv;
  }

  // In any case call the CommonOptionsParser
  ::clang::tooling::CommonOptionsParser op(argc, argvv, catToolnumberone, overview);

  if (optCompilationDatabaseDir != "") {
    // Free allocated resources
    delete[] argvv;
  }

#ifndef NDEBUG
  // Debug option
  if (optDebug) {
    ::llvm::DebugFlag = true;
  }
  if (optDebugOnly != "") {
    ::llvm::DebugFlag = true;
    ::llvm::setCurrentDebugType((const char *)optDebugOnly.c_str());
  }
#endif

  // Execute test
  if (optExecuteTest != "") {
    ::toolnumberone::testing::TestingOptions o;
    o.verbose = optVerbose;
    return toolnumberone::testing::runAllTest(argc, argv, optExecuteTest, o);
  }

  // Init the verbose output
  if (optVerbose) {
    toolnumberone::log::ToolnumberoneLogger::initVerbose();
    toolnumberone::log::ToolnumberoneLogger::setVerboseLevel(9);
  }

  // Output directory
  std::string outputPath =
      clang::tooling::getAbsolutePath((::std::string)optOutputDir);

  // Set resources directory
  std::string resourcesOutputDir =
      outputPath + toolnumberone::fs::pathSep + "resources" + toolnumberone::fs::pathSep;

  // Options Specific actions
  ::std::unique_ptr<::clang::tooling::CompilationDatabase> userCDatabase;
  if (optCompilationDatabaseDir != "") {
    std::string errorMsg;
    // Try to load a compilation database from the specified directory
    userCDatabase = ::clang::tooling::CompilationDatabase::loadFromDirectory(
        (::std::string)optCompilationDatabaseDir, errorMsg);
    if (userCDatabase == NULL) {
      // If it's passed MUST BE present a compile_commands.json in the
      // directory, so
      // if it's not found show an error and stop the program.
      toolnumberone::log::ToolnumberoneLogger::error(errorMsg); // Show error message
      return 1;                                     // Error
    }
  }

  // Configuration file
  conf::FunOpConfMap confMap;
  if (optFunOpConfFile != "") {
    toolnumberone::log::ToolnumberoneLogger::verbose("Configuration file : " +
                                         (::std::string)optFunOpConfFile);
    // ifstream confFile((::std::string) optFunOpConfFile);
    // Read conf file
    if (!conf::readFunctionsOperatorsConfFile(optFunOpConfFile, confMap)) {
      toolnumberone::log::ToolnumberoneLogger::error("Cannot open the configuration file");
    }
  }

  // To avoid problems of directory changing during clang operations create a
  // sourceAbsolutePathList
  std::vector<std::string> sourceAbsolutePathList;
  for (auto sourcePath : op.getSourcePathList()) {
    sourceAbsolutePathList.push_back(
        clang::tooling::getAbsolutePath(sourcePath));
  }

  // Loop on SourcePaths
  ::std::vector<::std::string> sourcePaths = op.getSourcePathList();
  for (std::string sourcePath : sourceAbsolutePathList) {
    // Get the compile commands for the sourcePath
    ::toolnumberone::cd_utils::CompileCommandVector commands;
    if (optCompilationDatabaseDir != "") {
      // The previous error check make safe this instruction
      commands = toolnumberone::cd_utils::getCompileCommandsByFilePath(*userCDatabase,
                                                                 sourcePath);
    } else {
      commands = toolnumberone::cd_utils::getCompileCommandsByFilePath(
          op.getCompilations(), sourcePath);
    }
#ifdef _CHIEMERA_DEBUG_
    ::toolnumberone::cd_utils::dump(::std::cout, commands);
#endif
    // Check the emptiness
    if (commands.empty()) {
      toolnumberone::log::ToolnumberoneLogger::warning(
          "Compile command not found. Skipping " + sourcePath);
      continue; // Skip this iteration
    }

    // Prepare inputs for the MutationTemplate
    ::clang::tooling::CompileCommand command = commands[0];

    ///////////////////////////////////////////////////////////////////////////////
    /// Add options/arguments
    // Add -w to suppress warning
    command.CommandLine.push_back("-w");
    command.CommandLine.push_back("-fsyntax-only");
    command.CommandLine.push_back(
        "-Qunused-arguments"); // suppress warnings on command line arguments

    // FIXME Some Bug, could not find stddef.h
    command.CommandLine.push_back("-I/usr/lib/clang/3.9.1/include/");

    ///////////////////////////////////////////////////////////////////////////////
    // The command for the sourcePath is ready!
    // Check source preprocessing
    if (optPreprocessLevel != PreprocessLevel::None) {
      PreprocessLevel l = optPreprocessLevel;
      ::toolnumberone::log::ToolnumberoneLogger::verboseAndIncr(
          "[ RUN  ] Preprocessing source file");
      // For sure will be saved a preprocessed file version in resources
      // directory

      // Variable needed to create a raw_fd_ostream
      ::std::error_code errorCode;
      ::std::string filepath =
          resourcesOutputDir + llvm::sys::path::filename(sourcePath).data();

      // Create output directory
      if (::toolnumberone::fs::createDirectories(resourcesOutputDir)) {
        // Open raw_fd_stream for the preprocessed-version fo the file
        ::llvm::raw_fd_ostream preprocessSourceFileStream(
            filepath, errorCode, llvm::sys::fs::F_Text);
        // Check which type of preprocessing
        if (l == PreprocessLevel::CompletePreprocess) {
          ::toolnumberone::log::ToolnumberoneLogger::verbose(
              "Applying complete preprocessing");
          ::toolnumberone::preprocessIncludeAction(preprocessSourceFileStream,
                                             command, sourcePath);
        } else if (l == PreprocessLevel::ExpandMacros) {
          ::toolnumberone::log::ToolnumberoneLogger::verbose("Applying macro expansion");
          ::toolnumberone::expandMacrosAction(preprocessSourceFileStream, command,
                                        sourcePath);
        } else {
          assert(l == PreprocessLevel::ReformatOnly);
          ::toolnumberone::log::ToolnumberoneLogger::verbose("Applying reformatting");
          ::toolnumberone::reformatAction(preprocessSourceFileStream, command,
                                    sourcePath);
        }
        // Close file stream
        preprocessSourceFileStream.close();
        toolnumberone::log::ToolnumberoneLogger::verbosePreDecr(
            "[ DONE ] Preprocessing source file");

        // A different version for the sourcePath has been created, modify
        // command and sourcePath
        ::toolnumberone::cd_utils::changeCompileCommandTarget(command, sourcePath,
                                                        filepath);
        // Modify the sourcePath
        sourcePath = filepath;

        toolnumberone::log::ToolnumberoneLogger::verbose(
            "[ RUN  ] Performing syntax check on preprocessed file");
        // Some times the Macro Expander corrupt the file so check the syntax
        int syntaxCheckResult =
            ::toolnumberone::checkSyntaxAction(command, sourcePath);
        if (syntaxCheckResult != 0) {
          toolnumberone::log::ToolnumberoneLogger::fatal(
              "[ FAIL ] Performing syntax check on preprocessed file\nThis "
              "could happen for apparently no reason with the macro-expansion, "
              "the macro-expander sometimes could not properly manage "
              "comments, see the the first error message, if this is the case, "
              "modify the source file in order to use this option.\nSorry for "
              "the inconvenient.");
          return 1;
        } else {
          toolnumberone::log::ToolnumberoneLogger::verbose(
              "[ PASS ] Performing syntax check on preprocessed file");
        }
        toolnumberone::log::ToolnumberoneLogger::decrActualVLevel();
      } else {
        toolnumberone::log::ToolnumberoneLogger::fatal(
            "Could not create the resources directory.");
        return 1; // Error
      }
    }

    ///////////////////////////////////////////////////////////////////////////////
    /// The command for this source file is ready, can perform FrontendAction
    if (optShowFunDef) {
      std::cout << "Function Definitions found : " << std::endl;
      return ::toolnumberone::functionDefAction(llvm::outs(), command, sourcePath);
    }
///////////////////////////////////////////////////////////////////////////////

#ifdef _CHIMERA_DEBUG_
    toolnumberone::cd_utils::dump(std::cout, command);
#endif
    toolnumberone::MutationTemplate t(command, sourcePath,
                                outputPath + toolnumberone::fs::pathSep + "mutants");

    // Loop on registered operators
    const toolnumberone::MutationOperatorPtrMap &map = this->registeredOperatorMap;
    for (auto it = map.begin(); it != map.end(); ++it) {
      t.loadOperator(it->second.get());
    }

    // Set if generate the mutatns or only the report
    t.setGenerateMutants(optGenerateMutants);
    t.setGenerateMutantsReport(!optNotGenerateReport);
    // Analyze template
    if (optFunOpConfFile != "") {
      t.analyze(confMap);
    } else {
      t.analyze();
    }
  }
  return 0;
}
