#ifndef INOLINEARTRACKFITALG_H
#define INOLINEARTRACKFITALG_H
//AlgFitTrackCam

#include <vector>
using std::vector;
#include "TVector3.h"
#include <TRandom3.h>
#include "TGeoManager.h"
#include "vect_manager.h"
#include "micalDetectorParameterDef.hh"
#include "MultiSimAnalysisDigi.hh"
#include "StraightLineFit.h"
#include <string>
#include <cstdlib>
#include "InoCluster.h"
//const unsigned int doubleLa=12;//500
//const unsigned int shiftLa=250;
const int  layfirst =0; 
const int  laylast =9; 
const int nlayer=11;
const float xyPosDev=3*0.03/sqrt(12); // seven sigma 2.0; //maximum deviation of points from fit line (2 strp units) 2 *3cm = 0.06




class InoLinearTrackFitAlg 
{
 public:
  
  InoLinearTrackFitAlg();
  virtual ~InoLinearTrackFitAlg();
  
  virtual void RunAlg();
  void Initialise();
  void RunTheFitter();
  

 private:
  
  int Nx,Ny;
   
  int occulyr;
  double Xpos[nlayer];
  bool Xusedpos[nlayer];
  double Xdev[nlayer];
  double Ypos[nlayer];
  bool Yusedpos[nlayer];
  double Ydev[nlayer];
	
  double zexppos;
  
  InoTrackCand* fLinearTrackCand;
  
  bool debug_fit;
  
  bool ZIncreasesWithTime;



  // InoFittedTrack_Manager* inoFittedTrack_pointer; 
  InoTrackCand_Manager* inoTrackCand_pointer;
  InoHit_Manager* inoHit_pointer; 
  InoCluster_Manager *InoCluster_pointer;
  MultiSimAnalysisDigi *pAnalysis; 
  InoTrack_Manager *pinotrack;


  double CorrTimeError;
  double UnCorrTimeError;
  double timeerr;
  
  double errxsq[nlayer], errysq[nlayer];
    
  double xslope, xinters;
  double yslope, yinters;
    
  double xchi2, ychi2;
  int nmnhits =5;
  int mxchisq =2;
  double xerrcst, xerrcov, xerrlin;
  double yerrcst, yerrcov, yerrlin;
    
  double topmostlay;


  double theta, phi;
  double therr;
  double pherr ;

  double xexterr;
  double yexterr;
	

};

#endif   // ALGFITTRACKCAM_H
