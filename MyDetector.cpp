#include "MyDetector.h"
#include "G4Box.hh"
#include "G4Cons.hh"
#include "G4Orb.hh"
#include "G4Sphere.hh"
#include "G4Trd.hh"
#include "G4Tubs.hh"
#include "G4UnionSolid.hh"
#include "G4Material.hh"
#include "G4NistManager.hh"
#include "G4LogicalVolume.hh"
#include "G4VPhysicalVolume.hh"
#include "G4PVPlacement.hh"
#include "G4SubtractionSolid.hh"
#include "CLHEP/Units/SystemOfUnits.h"
using namespace CLHEP;

G4VPhysicalVolume* MyDetector::Construct(){
    // Find Material 
    auto lAr = G4NistManager::Instance()->FindOrBuildMaterial("G4_lAr");
    auto al = G4NistManager::Instance()->FindOrBuildMaterial("G4_Al");
    auto ca = G4NistManager::Instance()->FindOrBuildMaterial("G4_Ca");
    auto air = G4NistManager::Instance()->FindOrBuildMaterial("G4_AIR");
    auto fe = G4NistManager::Instance()->FindOrBuildMaterial("G4_Fe");
    auto c = G4NistManager::Instance()->FindOrBuildMaterial("G4_C");
    auto ssteel = G4NistManager::Instance()->FindOrBuildMaterial("G4_STAINLESS-STEEL");

    // World volume
    //G4Box("Name of box",sizeX,sizeY,sizeZ); Create BOX
    //G4LogicalVolume("variable of box", Material, "Create Name of Logical"); Create material of box
    //G4PVPlacement(rotation, vector position, "variable of Logical", "Create Name of World", 0, false, 0); Create Box in world position
    auto worldBox = new G4Box("worldBox", 0.5*m, 0.5*m, 0.5*m); 
    auto logicalWorld = new G4LogicalVolume(worldBox, air, "LogicalWorld");
    auto physicalWorld = new G4PVPlacement(0, {0,0,0}, logicalWorld, "World", 0, false, 0);

    //Phanthom 
    auto * rot = new G4RotationMatrix();
    rot-> rotateX(90*deg);
    auto posPhan = G4ThreeVector(0*cm,0*cm,0*cm);
    double innerRadius = 0.0*cm, outerRadius = 5*cm, hz = 8*cm,startAngle = 0*deg, spanningAngle = 360*deg;
    auto cadpan = new G4Tubs("cadpan600", innerRadius, outerRadius, hz, startAngle, spanningAngle);
    auto logicalCadpan = new G4LogicalVolume(cadpan, ssteel, "logCadpan");
    auto physicsCadpan = new G4PVPlacement(rot, posPhan, logicalCadpan, "cadpan", logicalWorld, false, 0);
    //small tube inside
    auto posInside = G4ThreeVector(3.5*cm,0*cm,0*cm);
    auto insideCad = new G4Tubs("Inside", 0.2*innerRadius, 0.2*outerRadius, 0.8*hz, startAngle, spanningAngle);
    auto logicInside = new G4LogicalVolume(insideCad, c, "logicInside");
    auto physicsInside = new G4PVPlacement(rot,posInside, logicInside, "InsideCad", logicalWorld, false, 0);
    auto posInside2 = G4ThreeVector(-3.5*cm,0*cm,0*cm);
    auto logicInside2 = new G4LogicalVolume(insideCad, al, "logicInside2");
    auto physicsInside2 = new G4PVPlacement(rot,posInside2, logicInside2, "InsideCad", logicalWorld, false, 0);
    auto posInside3 = G4ThreeVector(0*cm,0*cm,3.5*cm);
    auto logicInside3 = new G4LogicalVolume(insideCad, ca, "logicInside3");
    auto physicsInside3 = new G4PVPlacement(rot,posInside3, logicInside3, "InsideCad", logicalWorld, false, 0);
    auto posInside4 = G4ThreeVector(0*cm,0*cm,-3.5*cm);
    auto logicInside4 = new G4LogicalVolume(insideCad, fe, "logicInside4");
    auto physicsInside4 = new G4PVPlacement(rot,posInside4, logicInside4, "InsideCad", logicalWorld, false, 0);

    //rotation
    auto posCal1 = G4ThreeVector(0*cm,-9*cm,0*cm);
    double x_cal1 = 6*cm, y_cal1 = 1*cm, z_cal1 = 6*cm;
    auto trackcal = new G4Box("cal", x_cal1, y_cal1, z_cal1); 
    auto logicalcal = new G4LogicalVolume(trackcal, ssteel, "Logicalcall");
    auto physicalcal = new G4PVPlacement(0, posCal1, logicalcal, "Track", logicalWorld, false, 0);
    
    //track1
    auto * rotTrack = new G4RotationMatrix();
    rotTrack-> rotateY(90*deg);
    auto pos1 = G4ThreeVector(10*cm,0*cm,0*cm);
    double x_size = 10*cm, y_size = 10*cm, z_size = 0.2*cm;
    auto track1 = new G4Box("track1", x_size, y_size, z_size); 
    auto logicalTrack1 = new G4LogicalVolume(track1, ssteel, "LogicalTrack1");
    auto physicalTrack1 = new G4PVPlacement(rotTrack, pos1, logicalTrack1, "Track", logicalWorld, false, 0);

    //track2
    //auto pos2 = G4ThreeVector(-10*cm,0*cm,0*cm); 
    //auto logicalTrack2 = new G4LogicalVolume(track1, ssteel, "LogicalTrack2");
    //auto physicalTrack2 = new G4PVPlacement(rotTrack, pos2, logicalTrack2, "Track2", logicalWorld, false, 0);

    //calorimeter
    auto * rotCal = new G4RotationMatrix();
    rotCal-> rotateY(90*deg);
    auto posCal = G4ThreeVector(20*cm,0*cm,0*cm);
    double x_cal = 10*cm, y_cal=10*cm, z_cal = 2*cm;
    auto Cal = new G4Box("Cal", x_cal, y_cal, z_cal); 
    auto logicalCal = new G4LogicalVolume(Cal, ssteel, "LogicalCal");
    auto physicalCal = new G4PVPlacement(rotCal, posCal, logicalCal, "Cal", logicalWorld, false, 0);

    return physicalWorld;
}
