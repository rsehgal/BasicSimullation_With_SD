/*
**	Filename : Hit.h
**	2022-01-14
**	username : rsehgal
*/
#ifndef Hit_h
#define Hit_h

#include "G4THitsCollection.hh"
#include <G4ThreeVector.hh>
#include <G4VHit.hh>
#include <string>

struct HitData {
  G4ThreeVector hitPos;
  G4String touchableName;
  double energyDep;
  std::string procName;
  std::string particleName;

  void Print() {
    std::cout << "--------------------------------------" << std::endl;
    std::cout << "Touchable Name : " << touchableName << " : Pos : " << hitPos << " : EnergyDeposited : " << energyDep
              << " : ParticleName : " << particleName << " : ProcessName : " << procName << " : LayerIndex : " << GetLayerIndex() << std::endl;
  }

  unsigned short GetLayerIndex() { return std::atoi(touchableName.substr(16, 1).c_str()); }
};

class MyHit : public G4VHit {
public:
  MyHit();
  virtual ~MyHit();
  void Print();

public:
  unsigned short GetLayerIndex();

  G4ThreeVector GetPosition() const;
  void SetPosition(G4ThreeVector pos);

  G4String GetName() const;
  void SetName(G4String name);

  void SetEnergyDep(double edep);
  double GetEnergyDep() const;

  void SetProcessName(std::string processName);
  std::string GetProcessName() const;

  void SetParticleName(std::string partiName);
  std::string GetParticleName() const;

  inline void *operator new(size_t);
  inline void operator delete(void *aHit);

private:
  /*G4ThreeVector hitPos;
  G4String touchableName;
  double energyDep;*/
  HitData hitData;
};

typedef G4THitsCollection<MyHit> SDHitCollection;
extern G4ThreadLocal G4Allocator<MyHit> *HitAllocator;

inline void *MyHit::operator new(size_t) {
  if (!HitAllocator)
    HitAllocator = new G4Allocator<MyHit>;
  return (void *)HitAllocator->MallocSingle();
}

inline void MyHit::operator delete(void *aHit) { HitAllocator->FreeSingle((MyHit *)aHit); }

#endif
