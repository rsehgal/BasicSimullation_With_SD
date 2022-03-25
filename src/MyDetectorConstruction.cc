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

  // CreateScintMatrix();

  G4Material *bar_mat = nist->FindOrBuildMaterial("G4_Galactic");

  double pixelHalf = 1 * cm;
  double pixelFull = 2 * pixelHalf;
  unsigned int numOfPixelsInOneDirection = 100 * cm / pixelFull;

  // Lets try to build one outer envelop of for n X n pixels
  G4Box *pixelEnvelop = new G4Box("PixelEnvelop", 50 * cm, 50 * cm, 1. * cm);
  G4LogicalVolume *pixelEnvelopLogical = new G4LogicalVolume(pixelEnvelop, bar_mat, "LogicalPixelEnvelop");

  // Lets try to build one pixel to be placed in envelop
  G4Box *pixel = new G4Box("Pixel", pixelHalf, pixelHalf, 0.5 * cm);
  G4LogicalVolume *pixelLogical = new G4LogicalVolume(pixel, bar_mat, "LogicalPixel");

  // G4Box *psBar = new G4Box("PsBar", halfXOneBar, halfYOneBar, halfZOneBar);
  // G4LogicalVolume *logicalPsBar = new G4LogicalVolume(psBar, bar_mat, "LogicalPsBar");

  /*--------------------------------------------------------------------------*/
  /* Making the Pixel volume sensitive
  ** Commenting the below four line, disable the senstivity of the logical volume
  */
  /*--------------------------------------------------------------------------*/
  MySensitiveDetector *mySD = new MySensitiveDetector("SensitiveDetector", "HitColSensitiveDetector");
  G4SDManager *sdman = G4SDManager::GetSDMpointer();
  sdman->AddNewDetector(mySD);
  // Making it Sensitive
  pixelLogical->SetSensitiveDetector(mySD);

  double xstart = -1. * pixelHalf * numOfPixelsInOneDirection;
  double ystart = -1. * pixelHalf * numOfPixelsInOneDirection;
  double pixelZVal = 0.;
  unsigned int pixelIndex = 0;
  unsigned int envelopIndex = 0;
  for (unsigned int yindex = 0; yindex < numOfPixelsInOneDirection; yindex++) {
    double yval = ystart + (2 * yindex + 1) * pixelHalf;
    for (unsigned int xindex = 0; xindex < numOfPixelsInOneDirection; xindex++) {
      double xval = xstart + (2 * xindex + 1) * pixelHalf;
      new G4PVPlacement(0,                                                       // no rotation
                        G4ThreeVector(xval,yval,pixelZVal),                                         // at (0,0,0)
                        pixelLogical,                                            // its logical volume
                        ("PhysicalPixel_" + std::to_string(pixelIndex)).c_str(), // its name
                        pixelEnvelopLogical,                                     // its mother  volume
                        false,                                                   // no boolean operation
                        pixelIndex,                                              // copy number
                        checkOverlaps);
	pixelIndex++;
    }
  }


  new G4PVPlacement(0,                                                       // no rotation
                        G4ThreeVector(),                                         // at (0,0,0)
                        pixelEnvelopLogical,                                            // its logical volume
                        ("PhysicalPixelEnvelop_" + std::to_string(envelopIndex)).c_str(), // its name
                        logicWorld,                                     // its mother  volume
                        false,                                                   // no boolean operation
                        envelopIndex,                                              // copy number
                        checkOverlaps);
  // G4GDMLParser parser;
  // parser.Write("geom.gdml", physWorld);
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
