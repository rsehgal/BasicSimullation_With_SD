#include "MyDetectorConstruction.hh"

#include "G4Box.hh"
#include "G4Cons.hh"
#include "G4LogicalVolume.hh"
#include "G4NistManager.hh"
#include "G4Orb.hh"
#include "G4PVPlacement.hh"
#include "G4RunManager.hh"
#include "G4SDManager.hh"
#include "G4Sphere.hh"
#include "G4SystemOfUnits.hh"
#include "G4Trd.hh"
#include "MySensitiveDetector.h"
#include <G4GDMLParser.hh>
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

MyDetectorConstruction::MyDetectorConstruction() : G4VUserDetectorConstruction(), fScoringVolume(0) {}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

MyDetectorConstruction::~MyDetectorConstruction() {}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

G4VPhysicalVolume *MyDetectorConstruction::Construct() {
  // Get nist material manager
  G4NistManager *nist = G4NistManager::Instance();

  G4bool checkOverlaps = true;

  //
  // World
  //
  G4double world_sizeXYZ = 500 * cm;
  G4Material *world_mat = nist->FindOrBuildMaterial("G4_Galactic");

  G4Box *solidWorld = new G4Box("World",                                                        // its name
                                0.5 * world_sizeXYZ, 0.5 * world_sizeXYZ, 0.5 * world_sizeXYZ); // its size

  G4LogicalVolume *logicWorld = new G4LogicalVolume(solidWorld, // its solid
                                                    world_mat,  // its material
                                                    "World");   // its name

  G4VPhysicalVolume *physWorld = new G4PVPlacement(0,               // no rotation
                                                   G4ThreeVector(), // at (0,0,0)
                                                   logicWorld,      // its logical volume
                                                   "World",         // its name
                                                   0,               // its mother  volume
                                                   false,           // no boolean operation
                                                   0,               // copy number
                                                   checkOverlaps);  // overlaps checking

  //CreateScintMatrix();

  G4Material *bar_mat = nist->FindOrBuildMaterial("G4_Galactic");
double halfXOneBar = 5*cm;
double halfZOneBar = 5*cm;
double halfYOneBar = 50. * cm;

  // Lets try to build one bar of 10cm X 10cm X 10cm
  G4Box *psBar                  = new G4Box("PsBar", halfXOneBar, halfYOneBar, halfZOneBar);
  G4LogicalVolume *logicalPsBar = new G4LogicalVolume(psBar, bar_mat, "LogicalPsBar");


  /*--------------------------------------------------------------------------*/
  /* Making the logical volume sensitive
  ** Commenting the below four line, disable the senstivity of the logical volume
  */
  /*--------------------------------------------------------------------------*/
  MySensitiveDetector *mySD = new MySensitiveDetector("SensitiveDetector", "HitColSensitiveDetector");
  G4SDManager *sdman        = G4SDManager::GetSDMpointer();
  sdman->AddNewDetector(mySD);
  //Making it Sensitive
  logicalPsBar->SetSensitiveDetector(mySD);

  int barsInX = 9;
  int barsInZ = 10;
  double yval = 0;
  double xstart = -1. * barsInX * halfXOneBar;
  double zstart = -1. * barsInZ * halfZOneBar;
  std::cout << "XStart : " << xstart << " : YStart : " << zstart << std::endl;
  int counter = -1;
  G4RotationMatrix *yRot = new G4RotationMatrix; // Rotates X and Z axes only
  // yRot-­>rotateY(M_PI/2.*rad);
  yRot->rotateZ(1 * M_PI / 2. * rad);

  for (unsigned int zindex = 0; zindex < barsInZ; zindex++) {
    double zval = zstart + (2 * zindex + 1) * halfZOneBar;
    for (unsigned int xindex = 0; xindex < barsInX; xindex++) {
      counter++;
      double xval = xstart + (2 * xindex +1) * halfXOneBar;
      std::cout << xval << "," << yval << "," << zval << std::endl;
if(!(zindex%2))
new G4PVPlacement(0, G4ThreeVector(xval, yval, zval), logicalPsBar,
                          "PhysicalPsBar-" + std::to_string(xindex) + "-" + std::to_string(zindex)+"-"+std::to_string(counter), logicWorld, false,
                          counter, checkOverlaps);
else{
new G4PVPlacement(yRot, G4ThreeVector(0, xval, zval), logicalPsBar,
                          "PhysicalPsBar-" + std::to_string(xindex) + "-" + std::to_string(zindex)+"-"+std::to_string(counter), logicWorld, false,
                          counter, checkOverlaps);
}
      /*if (!(zindex % 2)) //&& yindex == 1)
        new G4PVPlacement(0, G4ThreeVector(xval, yval, zval), logicalPsBar,
                          "PhysicalPsBar-" + std::to_string(xindex) + "-" + std::to_string(zindex), logicWorld, false,
                          counter, checkOverlaps);
      else { //  if(yindex == 4)
        new G4PVPlacement(yRot, G4ThreeVector(yval, xval, zval), logicalPsBar,
                          "PhysicalPsBar-" + std::to_string(xindex) + "-" + std::to_string(zindex), logicWorld, false,
                          counter, checkOverlaps);
      }*/
    }
  }

  
  /*--------------------------------------------------------------------------*/
  /* Making the logical volume sensitive
  ** Commenting the below four line, disable the senstivity of the logical volume
  */
  /*--------------------------------------------------------------------------*/
/*  MySensitiveDetector *mySD = new MySensitiveDetector("SensitiveDetector", "HitColSensitiveDetector");
  G4SDManager *sdman = G4SDManager::GetSDMpointer();
  sdman->AddNewDetector(mySD);

  G4Box *trackingDetector = new G4Box("TrackingDetector",            // its name
                                      50. * cm, 50. * cm, 0.5 * cm); // its size

  // G4Material* box_mat = nist->FindOrBuildMaterial("G4_Pb");
  G4Material *detector_mat = nist->FindOrBuildMaterial("G4_Galactic");
  G4LogicalVolume *logicalTrackingDetector = new G4LogicalVolume(trackingDetector,           // its solid
                                                                 detector_mat,               // its material
                                                                 "LogicalTrackingDetector"); // its name

  // Making it Sensitive
  logicalTrackingDetector->SetSensitiveDetector(mySD);

  unsigned short numOfLayers = 4;
  unsigned short numOfDetInEachLayer = 2;
  std::string physicalDetName = "PhysicalTrackingDetector";
  double spaceBetweenDetLayers = 30. * cm;
  double offset = 30. * cm;
  short counter = -1;
  for (unsigned int i = 0; i < numOfLayers; i++) {
    unsigned short layerId = i;
    if (layerId == numOfLayers / 2)
      counter = -1;
    double zval = offset + (numOfLayers / 2 - 1) * spaceBetweenDetLayers;
    if (layerId < numOfLayers / 2) {
      zval *= -1.;
    } else {
      zval = offset;
    }
    counter++;
    zval += (counter * spaceBetweenDetLayers);
    for (unsigned int j = 0; j < numOfDetInEachLayer; j++) {
      unsigned short detId = (numOfDetInEachLayer * layerId) + j;
      physicalDetName = ("PhysicalTrackingDetector_" + std::to_string(layerId) + "_" + std::to_string(detId));
      new G4PVPlacement(0,                           // no rotation
                        G4ThreeVector(0., 0., zval), // at (0,0,0)
                        logicalTrackingDetector,     // its logical volume
                        physicalDetName,             // its name
                        logicWorld,                  // its mother  volume
                        false,                       // no boolean operation
                        detId,                       // copy number
                        checkOverlaps);
      // mySD->InitializeAnalyzer(physicalDetName, layerId, detId);
    }
  }
*/
  G4GDMLParser parser;
  parser.Write("geom.gdml", physWorld);
  return physWorld;
}

void MyDetectorConstruction::CreateScintMatrix() {

  /*
   * Lets try to build the full matrix of PsBars
   */

  /*int barsInX = 9;
  int barsInY = 10;
  double zval = 0;
  double xstart = -1. * barsInX * halfXOneBar;
  double ystart = -1. * barsInY * halfYOneBar;
  std::cout << "XStart : " << xstart << " : YStart : " << ystart << std::endl;
  int counter = 0;
  G4RotationMatrix *yRot = new G4RotationMatrix; // Rotates X and Z axes only
  // yRot-­>rotateY(M_PI/2.*rad);
  yRot->rotateY(1 * M_PI / 2. * rad);

  for (unsigned int yindex = 0; yindex < barsInY; yindex++) {
    double yval = ystart + (2 * yindex + 1) * halfYOneBar;
    for (unsigned int xindex = 0; xindex < barsInX; xindex++) {
      double xval = xstart + (2 * xindex + 1) * halfXOneBar;
      std::cout << xval << "," << yval << "," << zval << std::endl;

      if (!(yindex % 2)) //&& yindex == 1)
        new G4PVPlacement(0, G4ThreeVector(xval, yval, zval), logicalPsBar,
                          "PhysicalPsBar-" + std::to_string(xindex) + "-" + std::to_string(yindex), logicWorld, false,
                          counter, checkOverlaps);
      else { //  if(yindex == 4)
        new G4PVPlacement(yRot, G4ThreeVector(zval, yval, xval), logicalPsBar,
                          "PhysicalPsBar-" + std::to_string(xindex) + "-" + std::to_string(yindex), logicWorld, false,
                          counter, checkOverlaps);
      }
    }
  }*/
}
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
