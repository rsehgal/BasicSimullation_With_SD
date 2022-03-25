/*
**	Filename : Hit.cpp
**	2022-01-14
**	username : rsehgal
*/
#include "MyHit.h"
#include <string>

G4ThreadLocal G4Allocator<MyHit>* HitAllocator=0;
#include <G4ThreeVector.hh>

MyHit::MyHit(){}

MyHit::~MyHit(){}

unsigned short MyHit::GetLayerIndex(){
return hitData.GetLayerIndex(); 
}

G4ThreeVector MyHit::GetPosition()const{
return hitData.hitPos;
}

void MyHit::SetPosition(G4ThreeVector pos){
hitData.hitPos = pos;
}

G4String MyHit::GetName()const{
return hitData.touchableName;
}

void MyHit::SetEnergyDep(double edep){
//std::cout << "From MYHIT  : " << edep << std::endl; 
hitData.energyDep = edep;
}

double MyHit::GetEnergyDep()const{
return hitData.energyDep;
}

void MyHit::SetName(G4String name){
hitData.touchableName = name;
}

void MyHit::SetProcessName(std::string processName){
hitData.procName = processName;
}
std::string MyHit::GetProcessName() const{
return hitData.procName;
}

void MyHit::SetParticleName(std::string partiName){
hitData.particleName = partiName;
}

std::string MyHit::GetParticleName() const{
return hitData.particleName;
}

void MyHit::Print(){

/*std::cout << "--------------------------------------" << std::endl;
std::cout << "Touchable Name : " << touchableName << " : Pos : " << hitPos << " : EnergyDeposited : " << energyDep << std::endl;*/
 hitData.Print();
}

