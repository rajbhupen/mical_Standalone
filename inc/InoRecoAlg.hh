#ifndef InoRecoAlg_h
#define InoRecoAlg_h 1

#include "micalDetectorParameterDef.hh"
#include "ParameterMessenger.hh"
#include "MultiSimAnalysisDigi.hh"
#include "GeneralRecoInfo.hh"
#include "vect_manager.h"
#include "InoHit.h"
#include "InoTrackCand.h"
#include "InoTrack.h"
#include "InoTrackFinder.h"
#include "InoOldTrackFitAlg.h"
#include "InoNewTrackFitAlg.h"
#include "InoLinearTrackFitAlg.h"
#include "InoVertex.h"
#include "globals.hh"
#include "TCanvas.h"
#include "TStyle.h"
#include "TMatrixD.h"
#include "TMath.h"
#include "TMatrixDEigen.h"
#include "CmvHit.h"
#include "CmvLayExtra.h"
#include "CmvCluster.h"

using namespace std;

class InoRecoAlg {
public:
  InoRecoAlg(int isInOut);
  ~InoRecoAlg();
  
  void ReadEvent(int ixt);
  void PerformTrackReconstruction();
  void PerformHadronReconstruction();


  
private:
  void SaveRecoDataToRootFiles( unsigned int iTrackNum, InoTrackCand* pfittedTrack);

  // InoTrackCand_Manager* inoTrackCand_pointer; //declared public
  micalDetectorParameterDef* paradef;
  MultiSimAnalysisDigi* pAnalysis;
  ParameterMessenger *MessengerFile;
  GeneralRecoInfo* grecoi;
  InoStripX_Manager* inoStripX_pointer;
  InoStripY_Manager* inoStripY_pointer;
  InoTDCHitx_Manager* inoTDCx_pointer;
  InoTDCHity_Manager* inoTDCy_pointer;
  InoTrack_Manager *pinotrack;
     InoTrackCand_Manager* pfitTrack;
  vector <InoCluster*> totcluster;
  vector<InoStrip*> StripBankX[500];  //GMA Same
  vector<InoStrip*> StripBankY[500];  //GMA Same
  int nUp;
  int nDown;

  static const int nLayerMx = 10;//10
  static const int nTDCpLayer = 8;
  
  double TimeToDigiConv;
  int nTriggerX[200];
  int nTriggerY[200];
  int TrgLayer[200];
  int triglays;
  int isData;
  int isTrackFit;
  double LayerThickness;
  int nLayer;
  int nIRLayer;
  int numberInMO;
  int numberInCH;
  int numberInX;
  int numberInY;
  double parino[3];
  double parlay[3];
  double parirlay[3];
  //  double gapino;
  double parmod[3];
  double ShiftInX;
  double ShiftInY;
  double ShiftInZ;
  double parchm[3];
  double pargas[3];
  double Xstrwd;
  double Ystrwd;
  //  int DetectorType;
  double ZLayerPos[300];
  bool debug_hits;
  int magfield;
  double ztinter,dZdT;

  
};
#endif
