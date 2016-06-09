//
// ********************************************************************
// * DISCLAIMER                                                       *
// *                                                                  *
// * The following disclaimer summarizes all the specific disclaimers *
// * of contributors to this software. The specific disclaimers,which *
// * govern, are listed with their locations in:                      *
// *   http://cern.ch/geant4/license                                  *
// *                                                                  *
// * Neither the authors of this software system, nor their employing *
// * institutes,nor the agencies providing financial support for this *
// * work  make  any representation or  warranty, express or implied, *
// * regarding  this  software system or assume any liability for its *
// * use.                                                             *
// *                                                                  *
// * This  code  implementation is the  intellectual property  of the *
// * GEANT4 collaboration.                                            *
// * By copying,  distributing  or modifying the Program (or any work *
// * based  on  the Program)  you indicate  your  acceptance of  this *
// * statement, and all its terms.                                    *
// ********************************************************************
//
///////////////////////////////////////////////////////////////////////////////
// File: CCalMaterial.cc
// Description: CCalMaterial transient class to store information from 
//              material table (database) which is used to make a G4Material
///////////////////////////////////////////////////////////////////////////////
#include "CCalMaterial.hh"

//#define debug

CCalMaterial::CCalMaterial(G4String mat, double dens, int nconst, 
			   CCalMaterial** constituents, double* weights,
			   FractionType ft): name(mat), density(dens) {
  nElem = 0;
  
  int i=0;
  for (i=0; i<nconst; i++)
    nElem += constituents[i]->NElements();

  theElements = new G4String[nElem];
  theWeights  = new double[nElem];

  double factor;
  int nelem=0;
  for (i=0; i<nconst; i++) {
    if (ft==FTWeight)
      factor=1.0;
    else
      factor=constituents[i]->Density();
    for (int j=0; j<constituents[i]->NElements(); j++) {
      theElements[nelem] = constituents[i]->Element(j);
      theWeights[nelem]  = constituents[i]->Weight(j)* weights[i] * factor;
      nelem++;
    }
  }

  if (density<0) { //Let's compute density
    computeDensity(nconst, constituents, weights, ft);
  }
  closeMaterial();
}

CCalMaterial::CCalMaterial(const CCalMaterial& mat):
  name(mat.name), density(mat.density), nElem(mat.nElem) {
  theElements = new G4String[nElem];
  theWeights  = new double[nElem];
  for (int i=0; i<nElem; i++){
    theElements[i]=mat.theElements[i];
    theWeights[i]=mat.theWeights[i];
  }
}

CCalMaterial::~CCalMaterial() {
  if (theElements)
    delete[] theElements;
  if (theWeights)
    delete[] theWeights;
}

void CCalMaterial::computeDensity(int nconst, 
				  CCalMaterial** constituents, 
				  double* weights, FractionType ft) {
  double mass=0;
  double volume=0;
  for (int i=0; i<nconst; i++) {
    if (ft==FTWeight) {
      mass+=weights[i];
      volume+=(weights[i]/constituents[i]->Density());
    }
    else { //by volume
      mass+=(weights[i]*constituents[i]->Density());
      volume+=weights[i];
    }
  }
  density=mass/volume;
}

CCalMaterial& CCalMaterial::operator=(const CCalMaterial& mat){
  if(theElements)
    delete[] theElements;
  if(theWeights)
    delete[] theWeights;

  name=mat.name;
  density=mat.density;
  nElem=mat.nElem;
  
  theElements = new G4String[nElem];
  theWeights  = new double[nElem];
  for (int i=0; i<nElem; i++){
    theElements[i]=mat.theElements[i];
    theWeights[i]=mat.theWeights[i];
  }
  return *this;
}

G4bool CCalMaterial::operator==(const CCalMaterial& mat) const{
  return (name==mat.name);
}

G4bool CCalMaterial::operator!=(const CCalMaterial& mat) const{
  return (name!=mat.name);
}

void CCalMaterial::closeMaterial() {
  int trueConst=0;

  double norm=0;

  for (int i=0; i<nElem; i++) {
    norm+=theWeights[i];
    if (theElements[i]!="") {
      trueConst++;
      for (int j=i+1; j<nElem; j++) {
	if(theElements[i]==theElements[j]){
	  theWeights[i]+=theWeights[j];
	  theElements[j]="";
	}
      }//for j
    } //if
  }//for i

  if (trueConst != nElem) {
    G4String* newConst = new G4String[trueConst];
    double* newWeight = new double[trueConst];
    
    int newi=0;
    for(int i=0; i<nElem; i++){
      if (theElements[i]!="") {
	newConst[newi]  = theElements[i];
	newWeight[newi] = theWeights[i]/norm;
	newi++;
      }
    }

#ifdef debug    
    G4cout << "\tGoing from " << nElem <<" constituents to " << trueConst << G4endl;
#endif
    nElem=trueConst;
    
    delete[] theElements;
    delete[] theWeights;

    theElements=newConst;
    theWeights=newWeight;
  }
  else { //Let's normalize the weights
    for (int i=0; i<nElem; i++)
      theWeights[i] = theWeights[i]/norm;
  }
}

std::ostream& operator<<(std::ostream& os, const CCalMaterial& mat) {
  os << mat.name << G4endl;
  os << "Density= " << mat.density << " g/cm3. Number of Elements: "
     << mat.nElem << G4endl;
  for (int i=0; i<mat.nElem; i++)
    os << '\t' << mat.theElements[i] << '\t' << mat.theWeights[i] << G4endl;
  return os;
}