//
// ********************************************************************
// * License and Disclaimer                                           *
// *                                                                  *
// * The  Geant4 software  is  copyright of the Copyright Holders  of *
// * the Geant4 Collaboration.  It is provided  under  the terms  and *
// * conditions of the Geant4 Software License,  included in the file *
// * LICENSE and available at  http://cern.ch/geant4/license .  These *
// * include a list of copyright holders.                             *
// *                                                                  *
// * Neither the authors of this software system, nor their employing *
// * institutes,nor the agencies providing financial support for this *
// * work  make  any representation or  warranty, express or implied, *
// * regarding  this  software system or assume any liability for its *
// * use.  Please see the license in the file  LICENSE  and URL above *
// * for the full disclaimer and the limitation of liability.         *
// *                                                                  *
// * This  code  implementation is the result of  the  scientific and *
// * technical work of the GEANT4 collaboration.                      *
// * By using,  copying,  modifying or  distributing the software (or *
// * any work based  on the software)  you  agree  to acknowledge its *
// * use  in  resulting  scientific  publications,  and indicate your *
// * acceptance of all terms of the Geant4 Software license.          *
// ********************************************************************
//
// $Id: G4LindhardSorensenData.hh 96909 2016-05-17 10:16:09Z vnivanch $

#ifndef G4LindhardSorensenData_h
#define G4LindhardSorensenData_h 1

//---------------------------------------------------------------------------
//
// ClassName:   G4LindhardSorensenData
//
// Description: Data on stopping power
//
// Author:        Alexander Bagulya & Vladimir Ivanchenko
//
// Creation date: 16.04.2018
//
// Class Description:
//
// Parameterised data on corrections to ion stopping power
//

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#include "globals.hh"

class G4PhysicsVector;
class G4Pow;

class G4LindhardSorensenData 
{ 
public: 

  explicit G4LindhardSorensenData();

  ~G4LindhardSorensenData();

  G4double GetDeltaL(G4int Z, G4double gamma) const;

private:

  void InitialiseData();

  G4double ComputeDeltaL(G4int Z, G4double x) const;

  // hide assignment operator
  G4LindhardSorensenData & operator=
  (const  G4LindhardSorensenData &right) = delete;
  G4LindhardSorensenData(const  G4LindhardSorensenData&) = delete;

  G4double xmin;
  G4double xmax;

  G4PhysicsVector* data[9];
  G4Pow* g4calc;
};

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#endif