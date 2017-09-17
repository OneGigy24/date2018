/* * Toolnumbertwo - Design Space Exploration for Approximate Computing technique using
 * Gentic Algorithm
 * Copyright (C) 2017 Antonio Tammaro <ntonjeta@autistici.org>
 *
 * This file is part of Toolnumbertwo.
 *
 * Toolnumbertwo is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * Toolnumbertwo is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with Toolnumbertwo.  If not, see <http://www.gnu.org/licenses/>.
 */

/******************************************************************************
 * @file   FlapAprxTechnique.h
 * @author Antonio Tammaro
 * @date   20 mar 2017
 * @brief  Implementation for the apprx Contexts of FLAP library
 ******************************************************************************/

#ifndef INCLUDE_TOOLTWO_FLAPAPRXCONTEXT_H_
#define INCLUDE_TOOLTWO_FLAPAPRXCONTEXT_H_

#include "Plugins/FLAP/fap.h"
#include "Core/AprxContext.h"


namespace toolnumbertwo{
namespace flapContext{



class FlapAprxContext : public toolnumbertwo::core::AprxContext {
public:
  FlapAprxContext() { }
  FlapAprxContext (toolnumbertwo::core::AprxContextIdTy id, const ::std::string& desc) 
      : AprxContext(id,desc) {
  }
  /// \brief destructor  
  virtual ~FlapAprxContext(){}
  //*-------------------------------------------------------------------*
  // Set Methods
  //*-------------------------------------------------------------------*
  /// \brief set Location Vector 
  /// \pram tcnq Vector of location
  virtual void setLocation(::std::vector<core::AprxLocation> tcqn) override;  
  //*-------------------------------------------------------------------*
  // Functional Methods 
  //*-------------------------------------------------------------------*
  /// \brief Overload of = operator 
  virtual toolnumbertwo::core::AprxContext* operator= (
    toolnumbertwo::core::AprxContext& rhs) override; 

  /// \brief Get the aprx locations vector reading report
  /// \param reportPath The path to the report from which gather the info
  /// \return true if operation have success
  virtual bool readReport(::std::string report) override;
  /// \brief Get the maximum approximation grade
  /// \return Maximum approximation grade applicable
  virtual ::toolnumbertwo::core::AprxGrade getMaxApplicableGrade() const override;


};  // end class definition
}   // end flapContext namespace
}   // end toolnumbertwo namespace

/// \brief build specific AprxContext 
/// \detailed build an instance of FlapAprxContext
/// \return  Return the istance of flapContext to main program.
::std::shared_ptr<::toolnumbertwo::core::AprxContext> getFlapAprxContext();
#endif
