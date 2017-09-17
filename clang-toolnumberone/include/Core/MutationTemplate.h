//===- MutationTemplate.h ---------------------------------------*- C++ -*-===//
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
/// \file MutationTemplate.h
/// \author Federico Iannucci
/// \date 14 ott 2015
/// \brief This file contains the class Mutation Template
//===----------------------------------------------------------------------===//

#ifndef INCLUDE_MUTATION_TEMPLATE_H_
#define INCLUDE_MUTATION_TEMPLATE_H_

#include "Utils.h"
#include "Log.h"
#include "Core/Mutant.h"
#include "Core/MutationOperator.h"

#include "clang/Tooling/Tooling.h"
#include "clang/Tooling/CompilationDatabase.h"
#include "llvm/ADT/StringRef.h"
#include "llvm/Support/Path.h"

#include <fstream>
#include <map>
#include <string>
#include <utility>
#include <vector>

// Forward declarations
namespace clang
{
namespace ast_matchers
{
class MatchFinder;
}
}

namespace toolnumberone
{
/// @brief This class represent the context of mutation for a single .h/.cpp
/// file.
class MutationTemplate
{
    // Usings
    using OperatorPtr = m_operator::MutationOperator *;
    using OperatorPtrMap = std::map<m_operator::IdType, OperatorPtr>;

public:
    /// @brief Build a Mutation Template from :
    /// @param A clang::tooling::CompileCommand for the target
    /// @param target The path to the target file (relative or absolute)
    /// @param outputDirectory The directory for the outputs
    MutationTemplate ( const clang::tooling::CompileCommand &, std::string target,
                       std::string outputDirectory = "." );

    // Getter and Setter
    const std::string &getTargetPath() const {
        return targetPath;
    }
    const clang::StringRef getTargetFilename() {
        return llvm::sys::path::filename ( this->targetPath );
    }

    void setTargetPath ( const std::string &target ) {
        this->targetPath = ::clang::tooling::getAbsolutePath ( target );
        ::toolnumberone::log::ToolnumberoneLogger::verbose ( "Setting target path: " +
                this->targetPath );
    }

    const std::string &getOutputDirectory() const {
        return this->outputDirectory;
    }

    /// @brief Return the Output directory (with trailing pathSep)
    /// @return The output directory for the target
    std::string getTargetOutputDirectory() {
        return this->outputDirectory + this->getTargetFilename().data() +
               ::toolnumberone::fs::pathSep;
    }

    void setOutputDirectory ( const std::string &outputDirectory ) {
        this->outputDirectory = clang::tooling::getAbsolutePath ( outputDirectory );
        // Check the last char, it MUST BE a path separator
        if ( this->outputDirectory.back() != toolnumberone::fs::pathSep ) {
            this->outputDirectory.push_back ( toolnumberone::fs::pathSep );
        }
        toolnumberone::log::ToolnumberoneLogger::verbose ( "Setting output directory: " +
                                               this->outputDirectory );
    }

    bool isGenerateMutantsReport() {
        return this->generateMutantsReport;
    }
    void setGenerateMutantsReport ( bool val ) {
        this->generateMutantsReport = val;
    }

    bool isGenerateMutants() {
        return this->generateMutants;
    }
    void setGenerateMutants ( bool val ) {
        this->generateMutants = val;
    }

    /// @defgroup
    /// @brief Functions to manage the mutation template's report stream
    /// @{

    bool openReportStream ( const char * );
    std::ostream &getReportStream();
    void closeReportStream();

    /// @}

    const clang::tooling::CompileCommand &getCompileCommand() const {
        return compileCommand;
    }

    /**
     * @brief Load an operator
     * @param mut_op Mutation operator
     * @retval bool If the Operator has been inserted, otherwise another with same
     * identifier already exists.
     */
    bool loadOperator ( OperatorPtr mut_op ) {
        std::pair<typename OperatorPtrMap::iterator, bool> ret =
            this->operators.insert ( std::pair<m_operator::IdType, OperatorPtr> (
                                         mut_op->getIdentifier(), mut_op ) );
        return ret.second;
    }

    /**
     * @brief Analyze the target
     * @return State of the analysis
     */
    int analyze();

    /// @param A FunOpConfMap map to filter operators per functions
    /// @return ClangTool.run return value.
    int analyze ( const toolnumberone::conf::FunOpConfMap & );

    // Public member
    /// @brief A counter for the created mutants. It'll contain the total number
    /// after an analysis.
    ///        It starts from 1. Mutant #0 is reserved.
    mutant::IdType mutantCounter;

private:
    void initMutantIds_();
    void addMatchers_ ( ::clang::ast_matchers::MatchFinder &,
                        const m_operator::IdType &, const std::string & = "" );
    void addMatchers_ ( ::clang::ast_matchers::MatchFinder &,
                        const ::std::vector<m_operator::IdType> &,
                        const ::std::string & );
    int run ( clang::ast_matchers::MatchFinder & );

    ::clang::tooling::CompileCommand
    compileCommand;               ///< Compile command for this target.
    ::clang::tooling::ClangTool tool; /**< Inner ClangTool to do the analysis */
    /// @details To avoid multiple tool calling, it' used only one CompileCommand.
    /// In particular
    ///          the first found.

    ::std::string
    targetPath; /**< The mutation template target: path to source file */
    OperatorPtrMap
    operators; /**< The mutation operators to apply to the target */

    bool generateMutantsReport; ///< If mutants report has to be save
    bool generateMutants;       ///< If mutants have to be saved.

    ::std::string outputDirectory; ///< Output directory in which write outputs,
    ///it's saved as absolute path
    ::std::ofstream reportStream;
};
} // End toolnumberone namespace

#endif /* INCLUDE_MUTATION_TEMPLATE_H_ */
