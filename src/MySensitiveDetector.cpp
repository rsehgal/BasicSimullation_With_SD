/*
**	Filename : MySensitiveDetector.cpp
**	2022-01-14
**	username : rsehgal
*/
#include "G4HCofThisEvent.hh"
#include "G4SDManager.hh"
#include "G4Step.hh"
#include "G4SteppingManager.hh"
#include "G4ThreeVector.hh"
#include "G4UnitsTable.hh"
#include "G4VProcess.hh"
#include "G4ios.hh"
#include "MyHit.h"
#include "MySensitiveDetector.h"
#include "colors.h"
#include <TVector3.h>
//#include "HitCollection.h"
#include "Output.h"
int MySensitiveDetector::evNo = -1;

MySensitiveDetector::MySensitiveDetector(G4String name, G4String collName)
    : G4VSensitiveDetector(name), collectionID(-1) {
  collectionName.insert(collName);
  energyDep = 0.;
}

MySensitiveDetector::~MySensitiveDetector() {}

void MySensitiveDetector::Initialize(G4HCofThisEvent *hce) {
  evNo++;
  if (!(evNo % 10000) && (evNo != 0))
    std::cout << BLUE << "Processed : " << evNo << " events..." << RESET << std::endl;
  hitsCollection = new SDHitCollection(SensitiveDetectorName, collectionName[0]);
  G4int hcID = G4SDManager::GetSDMpointer()->GetCollectionID(collectionName[0]);
  hce->AddHitsCollection(hcID, hitsCollection);
}

G4bool MySensitiveDetector::ProcessHits(G4Step *step, G4TouchableHistory *touchableHist) {
  MyHit *newHit = new MyHit();
  G4Track *track = step->GetTrack();
  bool isPrimary = (track->GetParentID() == 0);

  G4TouchableHandle touch1 = step->GetPreStepPoint()->GetTouchableHandle();

  if (isPrimary && step->GetPreStepPoint()->GetStepStatus() == fGeomBoundary) {
    std::string geomName = std::string(touch1->GetVolume()->GetName());
    //std::cout << BLUE << "GeomName : " << geomName << RESET << std::endl;
    newHit->SetName(geomName);
    newHit->SetPosition(step->GetPreStepPoint()->GetPosition());
    newHit->SetEnergyDep(step->GetTotalEnergyDeposit());
    newHit->SetParticleName(track->GetParticleDefinition()->GetParticleName());
    newHit->SetProcessName(step->GetPreStepPoint()->GetProcessDefinedStep()->GetProcessName());
    hitsCollection->insert(newHit);
  }

  return true;
}

void MySensitiveDetector::EndOfEvent(G4HCofThisEvent *HCE) {
  unsigned int evId = 0;
  if (1) {
    // std::cout << RED << "Reached end of event from SD : " << evId << RESET <<  std::endl;
    unsigned int noOfHits = hitsCollection->entries();
    // std::cout << "No. Of hits in EvNo : " << evId <<" : " << noOfHits << std::endl;
    // fAnal->FillEnergy(energyDep);

    bool hitInLayer_9 = false;
    bool hitInLayer_0 = false;
    G4ThreeVector hitPosLayer_9(0., 0., 0.);
    G4ThreeVector hitPosLayer_0(0., 0., 0.);

    for (unsigned int i = 0; i < noOfHits; i++) {
      //(*hitsCollection)[i]->Print();
      if ((*hitsCollection)[i]->GetLayerIndex() == 9) {
        hitInLayer_9 = true;
        hitPosLayer_9 = (*hitsCollection)[i]->GetPosition();
      }
      if ((*hitsCollection)[i]->GetLayerIndex() == 0) {
        hitInLayer_0 = true;
        hitPosLayer_0 = (*hitsCollection)[i]->GetPosition();
      }


	G4ThreeVector ref(0.,0.,-1.);
	if(hitInLayer_9 && hitInLayer_0){
		//std::cout << RED <<  "Accepted Event Found " << RESET << std::endl;
		//std::cout << BLUE << "Start : " << hitPosLayer_9 <<" :: End : " << hitPosLayer_0 << RESET << std::endl;
		G4ThreeVector diff = (hitPosLayer_0-hitPosLayer_9);
		//std::cout << BLUE << "DIFF : " << diff << RESET << std::endl;
		//std::cout << BLUE << "Angle with Vertical : " << diff.angle(ref) <<RESET << std::endl;
		//hitPosLayer_0.setX(hitPosLayer_0.x()+10.67);
	diff = (hitPosLayer_0-hitPosLayer_9);
	//std::cout << BLUE << "Angle Disturbed with Vertical : " << diff.angle(ref) <<RESET << std::endl;
	//Output::instance()->GetDetHist()->Fill(diff.angle(ref));
	Output::instance()->FillDetHist(diff.angle(ref));
}
      // fAnal->EnergyDepositInSteps((*hitsCollection)[i]->GetName(), (*hitsCollection)[i]->GetEnergyDep());
      // fAnal->PreStepPositionInSteps((*hitsCollection)[i]->GetName(), (*hitsCollection)[i]->GetPosition());
    }
  }

  return;
}
