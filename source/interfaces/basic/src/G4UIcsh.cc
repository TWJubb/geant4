// This code implementation is the intellectual property of
// the GEANT4 collaboration.
//
// By copying, distributing or modifying the Program (or any work
// based on the Program) you indicate your acceptance of this statement,
// and all its terms.
//
// $Id: G4UIcsh.cc,v 1.3 2000/06/23 08:46:49 stesting Exp $
// GEANT4 tag $Name: geant4-02-00 $
//

#include "G4UIcsh.hh"

////////////////////////////////////////
G4UIcsh::G4UIcsh(const G4String& prompt)
 : G4VUIshell(prompt)
////////////////////////////////////////
{
}

///////////////////
G4UIcsh::~G4UIcsh()
///////////////////
{
}


//////////////////////////////////
G4String G4UIcsh::GetCommandLine()
//////////////////////////////////
{
  MakePrompt();
  G4cout << promptString << G4std::flush;

  G4String newCommand;
  newCommand.readLine(G4cin, FALSE);
  if (!G4cin.good()) {
    G4cin.clear(); 
    newCommand= "exit";
    return newCommand;
  }

  // multi-line
  while( (newCommand.length() > 0) &&
	 (newCommand[newCommand.length()-1] == '_') ) {
    G4String newLine;
    newCommand.remove(newCommand.length()-1);
    newLine.readLine(G4cin, FALSE);
    if (!G4cin.good()) { 
      G4cin.clear(); 
      newCommand= "exit";
      return newCommand;
    }
    newCommand.append(newLine);
  }
  
  return newCommand;
}
