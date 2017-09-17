//===- ToolnumberoneTest.cpp --------------------------------------------*- C++ -*-===//
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
/// \file ToolnumberoneTest.cpp
/// \author Federico Iannucci
/// \date 28 nov 2015
/// \brief This file contains the testing utility functions based on
///        Google C++ Test Framework
//===----------------------------------------------------------------------===//

#include "Core/Mutator.h"
#include "Testing/ToolnumberoneTest.h"

#include "Log.h"
#include "Utils.h"
#include "clang/Tooling/Tooling.h"
#include "clang/Frontend/FrontendActions.h"
#include "clang/ASTMatchers/ASTMatchFinder.h"
#include "llvm/Support/FileSystem.h"

#include "lib/csv.h"

#include <string>
#include <iostream>

//#include <boost/filesystem.hpp>

using namespace std;
using namespace clang::ast_matchers;
using namespace toolnumberone::fs;
using namespace toolnumberone::mutator;
using namespace toolnumberone::log;

#define LOG_TEST_(msg)                                                         \
  if (options.verbose) {                                                       \
    ::std::cout << "[ CHIMERA  ] " << msg << ::std::endl;                      \
  }

static ::toolnumberone::testing::TestingOptions options;
static std::string testDirectory;

int toolnumberone::testing::runAllTest(int argc, const char **argv,
                                 const std::string &testDir, TestingOptions o) {
  ::testing::InitGoogleTest(&argc, const_cast<char **>(argv));
  testDirectory = testDir;
  options = o;
  ToolnumberoneLogger::info("Searching tests in " + testDirectory);
  return RUN_ALL_TESTS();
}

///////////////////////////////////////////////////////////////////////////////
/// MatchCallback
using TestCallbackResultType = ::std::vector<::std::vector<::std::string>>;
using TestCallbackResultEntry = ::std::vector<::std::string>;
class MutatorMatchingTestCallback : public MatchFinder::MatchCallback {
public:
  MutatorMatchingTestCallback(::llvm::raw_ostream &out, Mutator &m)
      : out(out), mutator(m) {}
  /**
   * @brief Run implementation.
   */
  virtual void run(const MatchFinder::MatchResult &Result) {
    ::clang::ast_type_traits::DynTypedNode matched_node;
    if (mutator.getMatchedNode(Result, matched_node)) {
      // Create a rewriter
      clang::Rewriter rw(*Result.SourceManager, Result.Context->getLangOpts());
      // Create a full resource
      clang::FullSourceLoc matchedNodeLoc(
          matched_node.getSourceRange().getBegin(), rw.getSourceMgr());
      LOG_TEST_("\tCoarse grain match : " +
                rw.getRewrittenText(matched_node.getSourceRange()) + " at " +
                ::to_string(matchedNodeLoc.getSpellingLineNumber()) + ":" +
                ::to_string(matchedNodeLoc.getSpellingColumnNumber()));
      // Apply fine grain matching rule
      if (mutator.match(Result)) {
        LOG_TEST_("\t\tFine grain match : PASS");

        // Save result entry
        TestCallbackResultEntry resultEntry{
            ::to_string(matchedNodeLoc.getSpellingLineNumber()),
            ::to_string(matchedNodeLoc.getSpellingColumnNumber())};
        results.push_back(resultEntry);

        bool syntaxCheckResult = false;
        for (MutatorType type = 0; type < this->mutator.getTypes(); ++type) {
          // Create a rewriter
          clang::Rewriter rw2(*Result.SourceManager,
                              Result.Context->getLangOpts());

          this->mutator.mutate(Result, type, rw2);
          // Put the mutants on out
          out << "////////////////////////////// START MUTANT "
                 "////////////////////////////\n";
          out << "// MUTATION_LOCATION: " << resultEntry[0] << ":"
              << resultEntry[1] << "\n";
          out << "// MUTATION_TYPE: " << type << "\n";
          ::std::string mutant;
          ::llvm::raw_string_ostream mutantStream(mutant);
          rw2.getEditBuffer(rw2.getSourceMgr().getMainFileID())
              .write(mutantStream);
          out << mutantStream.str();

          clang::FrontendAction *syntaxCheck = new clang::SyntaxOnlyAction;
          out << "//SYNTAX_CHECK: ";
          syntaxCheckResult =
              ::clang::tooling::runToolOnCode(syntaxCheck, mutantStream.str());
          if (syntaxCheckResult) {
            out << "PASS";
          } else {
            out << "FAIL";
          }
          ASSERT_TRUE(syntaxCheckResult)
              << "Actual mutant doesn't pass the syntax check";
          out << "\n////////////////////////////// END MUTANT "
                 "//////////////////////////////\n";
        }
      } else {
        LOG_TEST_("\t\tFine grain match : FAILED");
      }
    }
  }

  const TestCallbackResultType &getResults() { return this->results; }

private:
  ::llvm::raw_ostream &out;
  Mutator &mutator;
  TestCallbackResultType results;
};

void toolnumberone::testing::testMutatorMatch(toolnumberone::mutator::Mutator *mutator) {
  ::toolnumberone::mutator::Mutator &m = *mutator;
  // Load N sources from .cpp file in <mutator_identifier>/test_N.cpp
  unsigned int testNum = 0;
  int toolRetVal = -1;
  LOG_TEST_("Start Mutator Testing - " + m.getIdentifier());
  std::string test_file_directory(testDirectory + m.getIdentifier() + pathSep);
  std::string test_file_path;

  do {
    test_file_path = test_file_directory + "test_" + to_string(testNum);
    // Check file existence
    if (::llvm::sys::fs::exists(test_file_path + ".cpp")) {
      LOG_TEST_("Running on file - " + test_file_path + ".cpp");
      // Exists at least one file, open it and load it into a string
      std::ifstream test_file(test_file_path + ".cpp");
      std::string test_file_source(static_cast<std::stringstream const &>(
                                       std::stringstream() << test_file.rdbuf())
                                       .str());
      // Close the file
      test_file.close();

      ///////////////////////////////////////////////////////////////////////////////
      /// Check test file syntax
      clang::FrontendAction *syntaxCheck = new clang::SyntaxOnlyAction();
      if (!clang::tooling::runToolOnCode(syntaxCheck, test_file_source,
                                         "test.cpp")) {
        toolnumberone::log::ToolnumberoneLogger::error(
            "The test file IS NOT syntactically correct. Skipping this file.");

        // FIXME Freeing syntaxCheck produce segfault
      } else {
        // Create mutation output
        ::std::string mutationOutputFilePath = test_file_path + "_mutants.cpp";
        ::std::error_code errorCode;
        ::llvm::raw_fd_ostream mutationOutputStream(
            mutationOutputFilePath, errorCode, ::llvm::sys::fs::F_Text);

        if (mutationOutputStream.has_error()) {
          ::toolnumberone::log::ToolnumberoneLogger::fatal(
              "Error occurred in opening " + mutationOutputFilePath +
              ". Error message: " + errorCode.message());
        }

        // Create a matchFinder, load the mutator on it and run
        MatchFinder finder;
        // Create a MatchCallback
        MatchFinder::MatchCallback *callback =
            new MutatorMatchingTestCallback(mutationOutputStream, m);
        switch (m.getMatcherType()) {
        case StatementMatcherType:
          finder.addMatcher(m.getStatementMatcher(), callback);
          break;
        case DeclarationMatcherType:
          finder.addMatcher(m.getDeclarationMatcher(), callback);
          break;
        case TypeMatcherType:
          finder.addMatcher(m.getTypeMatcher(), callback);
          break;
        case TypeLocMatcherType:
          finder.addMatcher(m.getTypeLocMatcher(), callback);
          break;
        case NestedNameSpecifierMatcherType:
          finder.addMatcher(m.getNestedNameSpecifierMatcher(), callback);
          break;
        case NestedNameSpecifierLocMatcherType:
          finder.addMatcher(m.getNestedNameSpecifierLocMatcher(), callback);
          break;
        default:
          // Error!
          break;
        }

        clang::FrontendAction *finderAction =
            clang::tooling::newFrontendActionFactory(&finder)->create();
        toolRetVal = clang::tooling::runToolOnCode(
            finderAction, test_file_source, "test.cpp");
        ASSERT_TRUE(toolRetVal) << "Running failure";

        // Get results from the callback
        TestCallbackResultType results =
            ((MutatorMatchingTestCallback *)callback)->getResults();

        try {
          // Read from csv file and compare the result
          io::CSVReader<2, io::trim_chars<' '>,
                        io::double_quote_escape<',', '\"'>>
              oracle(test_file_path + "_match.csv");
          ::std::string line;
          ::std::string col;

          TestCallbackResultType oracleResults;
          while (oracle.read_row(line, col)) {
            TestCallbackResultEntry e{line, col};
            oracleResults.push_back(e);
          }
          // Firs check the number of entries
          ASSERT_EQ(oracleResults.size(), results.size())
              << "Number of matching mismatch. More matching than expected.";

          for (size_t i = 0; i < results.size(); ++i) {
            // Compare
            EXPECT_EQ(oracleResults.at(i), results.at(i))
                << "Result's entry mismatch";
          }
        } catch (::io::error::can_not_open_file &e) {
          ToolnumberoneLogger::info("Oracle file NOT FOUND. Skipping.");
        }
      }
      // Increase file test number
      testNum++;
      LOG_TEST_("For mutants check - " + test_file_path + "_mutants.cpp");
    } else
      testNum = 0;
    // testNum should be increased during the elaboration of test_0 so this
    // condition is met only if
    // a test_N.cpp file isn't found
  } while (testNum != 0);
  LOG_TEST_("Finish Mutator Testing - " + m.getIdentifier());
}
