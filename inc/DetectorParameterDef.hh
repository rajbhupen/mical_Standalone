#ifndef FullDetectorParameterDef_h
#define FullDetectorParameterDef_h 1

#include "Ical0DetectorParameterDef.hh"
#include "micalDetectorParameterDef.hh"

#include <fstream>
#include <iostream>
#include <iomanip>
#include <vector>
#include "G4ios.hh"
#include <G4String.hh>
#include <strings.h>

class DetectorParameterDef {
  
public:
  
  DetectorParameterDef();
  ~DetectorParameterDef();
  static DetectorParameterDef* AnPointer;
  Ical0DetectorParameterDef* icalparadef;
  micalDetectorParameterDef* miniparadef;
  void PrintDetectorParameters();
  int GetDetectorType() {return DetectorType;}
  double GetParworld(int i) { return parworld[i];}
  double GetParino(int i) { return parino[i];}
  double GetParirlay(int i) { return parirlay[i];}
  double GetParlay(int i) { return parlay[i];}
  double GetPargas(int i) { return pargas[i];}
  double GetParmod(int i) { return parmod[i];}
  double GetParchm(int i) { return parchm[i];}
  double GetParhcoil(int i) { return parhcoil[i];}
  double GetParcoilsupport(int i) { return parcoilsupport[i];}
  double GetGapino()       { return gapino;} 
  double GetRPCShift(int i) {return RPCShift[i];}
  double GetStackShift(int i) {return StackShift[i];}
  double GetXStrwd() { return Xstrwd;}
  double GetYStrwd() { return Ystrwd;}
  int    GetNumino()       { return numino;} 
  int GetnLayer() { return nLayer;}
  int GetnIRLayer(){return  nIRLayer;}
  int GetnModule() { return nModule;}
  int GetnChamber() { return nChamber;} 
  int GetnXStrip() { return nXStrip;} //AAR:
  int GetnYStrip() { return nYStrip;} //AAR:
  double GetRPCLayerPosZ(int j){return RPCLayerPosZ[j];}
  double GetIRONLayerPosZ(int j){return IRONLayerPosZ[j];}
  double GetLayerThickness(int j) {return LayerZdim[j]+IronLayerZdim[j];}
  double GetShiftInZ(int i)       { return ShiftInZ[i];} 
  double GetLayerZdim(int j) {return LayerZdim[j];}
  double GetIronLayerZdim(int j) {return IronLayerZdim[j];}

 double Getpartopscint(int j) {return partopscint[j];}
  double GetParwallscint(int j) {return parwallscint[j];}
  double GetScintFromBottom() {return ScintFromBottom;}
  double GetScintUnitX() {return ScintUnitX;}
  double GetScintUnitY() {return ScintUnitY;}
  double GetScintUnitZ() {return ScintUnitZ;}
  double GetAirGapScintTop() {return AirGapScintTop;}
double GetGapBtwCMVLay() {return GapBtwCMVLay;}
  
  double GetAirGapScintWall() {return AirGapScintWall;}

   double GetTopPlaneHalfLength(){return TopPlaneHalfLength;}
  double GetSidePlaneHalfLength(){return SidePlaneHalfLength;}
 double GetSideSmallPlaneHalfLength(){return SideSmallPlaneHalfLength;}
 double GetScntLayShifTop(){return ScntLayShifTop;}
double GetScntLayShifSide(){return ScntLayShifSide;}


  double GetfiberDia(){return fiberDia;}
  double GetfiberXpos(){return fiberXpos;}

  
 int  GetNoScntStrpTop(){return NoScntStrpTop;}
   int  GetNoScntStrpSide(){return NoScntStrpSide;}
  int  GetNoScntStrpSideSmallay(){return NoScntStrpSideSmallay ;}


  
  int GetSipm_Pedestal(){return Sipm_Pedestal;}
  int GetCmv_Threshold(){return Cmv_Threshold;}


  
private:
  const static int nlayermx = 256;
  void UpdateDetectorPars();
  int DetectorType;
  double parworld[3];
  double parino[3];
  double parirlay[3];
  double parlay[3];
  double pargas[3];
  double parmod[3];
  double parchm[3];
  double parhcoil[3];
  double parcoilsupport[3];
  double parcoilspacerpc[3];
  double gapino;
  double ShiftInX;
  double ShiftInY;
  double Xstrwd;
  double Ystrwd;
  int numino;
  int nLayer;
  int nIRLayer;
  int nChamber;
  int nModule;

  int nXStrip;
  int nYStrip;
  double RPCLayerPosZ[300];
  double IRONLayerPosZ[300];
  double RPCShift[3];
  double StackShift[3];

  double LayerZdim[10];
  double IronLayerZdim[11];
  double ShiftInZ[nlayermx];

  //cmv
 float ScintUnitX;// = 5.0*cm;
  float ScintUnitY;// = 4.6*m;
  float ScintUnitZ;// = 1*cm;
  float ScintFromBottom;// = 300*mm;
  float partopscint[3];
  float parwallscint[3];
  float postopcoil;
  float posmagnet;
  float AirGapScintTop;
  float AirGapScintWall;
  float GapBtwCMVLay;
  double TopPlaneHalfLength;
  double  SidePlaneHalfLength;
 double  SideSmallPlaneHalfLength;
  double  ScntLayShifTop;
   double  ScntLayShifSide;
  int  NoScntStrpTop;
 int NoScntStrpSide;

double  NoScntStrpSideSmallay;
  double fiberDia;
  double fiberXpos;

  double  Sipm_Pedestal;
  double Cmv_Threshold;

  double PhyVolGlPos[8][4][3];
  
};
#endif

