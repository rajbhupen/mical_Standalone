#ifndef CMVDigiAlg_h
#define CMVDigiAlg_h 1

#include "micalDetectorParameterDef.hh"
#include "MultiSimAnalysisDigi.hh"
#include "GeneralRecoInfo.hh"
#include "vect_manager.h"
#include "CmvStrip.h"
#include "globals.hh"
#include "TH2.h"
#include "TH2D.h"
#include "TRandom.h"
#include "TMath.h"
using namespace std;

class CMVDigiAlg {
public:
  CMVDigiAlg();
  ~CMVDigiAlg();

  void ReadEvent(int ixt);
  void DigitiseSimData();
  void NoiseGenLoop();
  
  void SaveDigiData();
 
  
  void SetPhotonSpeed(G4double val){PhotonSpeed=val;};
  void SetCMVadctons(G4double val){CMVadctons=val;};
  micalDetectorParameterDef* paradef;
  MultiSimAnalysisDigi* pAnalysis;
  InoCal1Hit_Manager* inocal1hit_pointer;
  CmvStrip_Manager* CmvStrip_pointer;
  SipmHit_Manager* SipmHit_pointer;
	double partopscint[3];     //halflength of scintillators of thickness 1cm
	float AirGapScintTop;
	G4double Phys_TopScint_GPos[4][3];
	G4double Phys_SideScint_R_GPos[3][3];
	G4double Phys_SideScint_L_GPos[3][3];
	G4double Phys_SideScint_D_GPos[3][3];
	double  PhyVolGlPos[8][4][3];
  int NoScntStrpTop;
  int NoScntStrpSide;
  int Sipm_Pedestal;
  int Cmv_Threshold;

  double PhotonSpeed;
  double CMVadctons;
	unsigned int CellDetID[20000]; //16bit
	// CmvStrip_Manager* CmvStrip_pointer;
	// SipmHit_Manager* SipmHit_pointer;
  
};

#endif
