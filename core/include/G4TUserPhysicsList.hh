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
// Author: Tarik Elghalbzouri,  Abdelmalek Essaâdi University,
// faculty of sciences Tetouane, morocco. email : telghalbzouri@uae.ac.ma
//
// This application is based on code developed by :
// G. Guerrieri, University of Genova, Italy .
// S. Guatelli. University of Wollongong, Australia.
//

#ifndef G4TUserPhysicsList_h
#define G4TUserPhysicsList_h 1

#include "G4VUserPhysicsList.hh"
#include "globals.hh"
//#include "G4TOutputText.hh"
#include "G4TPhysicsMessenger.hh"
#include "G4VPhysicsConstructor.hh"
#include "G4TComptonProcess.hh"

class G4TUserPhysicsList: public G4VUserPhysicsList
{
public:
    G4TUserPhysicsList();
    ~G4TUserPhysicsList();

private:

    G4bool IsEcutsSet;
    G4bool IsDcutsSet;

    G4double CutsEnergy;
    G4double CutsDistance;
    G4String ParticlePysics;

    G4String PhotoElectricEffectModel;
    G4String PolarizedPhotoElectricEffectModel;
    G4String ComptonScatteringModel;
    G4String PolarizedComptonModel;
    G4String GammaConversionModel;
    G4String PolarizedGammaConversionModel;
    G4String RayleighScatteringModel;
    G4String GammaConversionToMuonModel;

    G4String ElectronIonisationModel;
    G4String ElectronBremModel;
    G4String HadronIonisationModel;
    G4String HadronBremModel;
    G4String IonIonisationModel;

    void ShowSourceParameters();

    G4bool GenerateCrossSectionTableFlag;
    G4String ParticleForCrossSection;
    std::vector<G4double> EnergiesForCrossSectionValues;

    //G4TComptonProcess* ComptonWrappedProcess;
public:

    void setGenerateCrossSectionTableFlag(G4bool n ){ GenerateCrossSectionTableFlag = n; }
    G4bool getGenerateCrossSectionTableFlag() const {return GenerateCrossSectionTableFlag;}

    void setParticleForCrossSection(G4String n ){ ParticleForCrossSection = n; }
    G4String getParticleForCrossSectionValues() const {return ParticleForCrossSection;}

    void setEnergiesForCrossSectionValues(G4double n ){ EnergiesForCrossSectionValues.push_back(n); }
    std::vector<G4double> getEnergiesForCrossSectionValues() const {return EnergiesForCrossSectionValues;}

    void setPhysics(G4String);
    void setCutsEnergy(G4double);
    void setCutsDistance(G4double);

    void setPhotoElectricEffectModel(G4String);
    void setPolarizedPhotoElectricEffectModel(G4String);
    void setComptonScatteringModel(G4String);
    void setPolarizedComptonModel(G4String);
    void setGammaConversionModel(G4String);
    void setPolarizedGammaConversionModel(G4String);
    void setRayleighScatteringModel(G4String);
    void setGammaConversionToMuonModel(G4String);

    void setElectronIonisationModel(G4String);
    void setElectronBremModel(G4String);
    void setHadronIonisationModel(G4String);
    void setIonIonisationModel(G4String);

    G4double getCutsEnergy() const { return CutsEnergy;}
    G4double getCutsDistance() const { return CutsDistance;}

    //G4String getPhysics()const {return ParticlePysics;};
    //G4double getCutsEnergy()const {return CutsEnergy;};
    //G4double getCutsDistance()const {return CutsDistance;};

protected:
    // Construct particle and physics
    void ConstructParticle();
    void ConstructProcess();
    void SetCuts();

private:

    G4TPhysicsMessenger* messengerPhyObj;

    G4VPhysicsConstructor* G4VPhysicsConstructorObj;
    G4VPhysicsConstructor* decPhysicsList;

};
#endif
