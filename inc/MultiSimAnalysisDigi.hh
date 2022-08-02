#ifndef MULTISIMDIGI_H
#define MULTISIMDIGI_H 1
#include <vector>
using std::vector;

#include "ParameterMessenger.hh"
#include "micalDetectorParameterDef.hh"
#include "TH1.h"
#include "TH2.h"
#include "TH3.h"
#include "TGraph.h"
#include "TGraphErrors.h"
#include "TGraph2D.h"
#include "TGraph2DErrors.h"
#include "TTree.h"
#include "TFile.h"
// #include "evetree.h"
#include "RPCEve.h"
#include "globals.hh"
#include "Hits.h"
#include "HitPos.h"
#include "TProfile.h"
#include <iostream>
#include <fstream>
using namespace std;

struct vectGr{
  float x;
  float y;
  float z;
  float dx;
  float dy;
  float dz;
};

class MultiSimAnalysisDigi {
public:
  MultiSimAnalysisDigi();
  ~MultiSimAnalysisDigi();
  static MultiSimAnalysisDigi* AnPointer;

  void OpenInputRootFiles(char* inf);
  void OpenOutputRootFiles(char* outf);
  void OpenCollatedRootFile();
  void CloseInputRootFiles();
  void CloseOutputRootFiles();
  void SaveGenVisFile();

  void SetCorrTimeError(G4double val);
  void SetUnCorrTimeError(G4double val);
  void SetTimeToDigiConvVal(G4double val);
  void SetSignalSpeedVal(G4double val);
  	void SetPhotonSpeedVal(G4double val);
	void SetCMVadctons(G4double val);
  double GetCorrTimeError() {return CorrTimeError;}
  double GetUnCorrTimeError() {return UnCorrTimeError;}
  double GetTimeToDigiConvVal() {return TimeToDigiConv;}
  double GetSignalSpeedVal() {return SignalSpeed;}
	double GetPhotonSpeedVal() {return PhotonSpeed;}
	double GetCMVadctons() {return CMVadctons;}
  int isInOut;
  int isVisOut;
  int isXtermOut;
  int collatedIn;
   
  TH1F *strpXtime;
  TH1F *strpYtime;
  TH1F *strpXtimeCorr;
  TH1F *strpYtimeCorr;
  TH1F *hitXtime;
  TH1F *hitYtime;
  TH1D* smagFieldX;
  TH1D* smagFieldY;
  TH2D* smag2dX;
  TH2D* smag2dY;
  TH1D* rmagFieldX;
  TH1D* rmagFieldY;
  TH2D* rmag2dX;
  TH2D* rmag2dY;
  TH2D* smag2dXYpixel_iron;
  TH2D* smag2dXYpixel_air;
  TH2D* rmag2dXYpixel_iron;
  TH2D* rmag2dXYpixel_air;
  TH2D* xyvsbxin;
  TH2D* xyvsbyin;
  TH2D* xyvsbxdiff;
  TH2D* xyvsbydiff;
  TH2D* xyvsbxindiff;
  TH2D* xyvsbyindiff;
    TH2D* xyvsbxout;
    TH2D* xyvsbyout;
  // Collated Histograms
  TH2D* inefficiency_corx[20];
  TH2D* inefficiency_uncx[20];
  TH2D* inefficiency_uncy[20];
  TH2D* triggereffi_xevt[20];
  TH2D* triggereffi_yevt[20];
  TH2D* strp_xmulsim_cor[20];
  TH2D* strp_ymulsim_cor[20];

  TH2D* block_xmulsim[20][16][16];
  TH2D* block_ymulsim[20][16][16];







  TH1F *DeadStripX;
  TH1F *NoisyStripX;
  TH1F *DeadStripY;
  TH1F *NoisyStripY;
  TH1F *DiffTime;


  TH2F   *RC;
  TH1F   *DGap;
  TH1D* hdifftime1[20];
  TH1D* hdifftime2[20];
  TH1D* hxtime_ext[20];
  TH1D* hytime_ext[20];

  TH1D* hxpos_ext[20];
  TH1D* hypos_ext[20];
  TH1D* hxpos_ext_kalman[20];
  TH1D* hypos_ext_kalman[20];
  TH1D* h_hit_time_ext[20];

  TH1D* xtdc_minus_ref[20][8];
  TH1D* ytdc_minus_ref[20][8];

  TH1D* tshift_xtdc_minus_ref[20][8];
  TH1D* tshift_ytdc_minus_ref[20][8];

    Hits *H;
    HitPos *Hp;
  int EveCnt;
  int nloops;
    TH1F  *pdedz[20];  
    TH1F  *hitDist;    //asm
    TH1F  *TrkDist;    //asm
    TH1F  *EffDist;    //asm
    TH1F  *InoTrack_listsize;   //asm
  TFile *pRootFile;
  TFile *inputRootFile;  
  TFile *pVisFile;
  TFile* collatedRootFile;

  TTree *pEventTree;
  TTree *inputEventTree;
  TTree *visTree;
  RPCEve *data_event;
  // evetree *data_event;

  TH1F* ShwXw;
  TH1F* ShwYw;
  TH1D* trk_gap;
  TH2D* trk_edge;
  TH1F* pPosX;
  TH1F* pPosY;  
  TH1F* pPosZ;
  TH2F* pPosXX;
  TH2F* pPosYY;  
  TH2F* pPosZZ;

  
  static const int nhistmx=1000;
  int   ihist;
  TH3F* gens_list[6][nhistmx]; // Not used
  vector<vectGr> gens_vect[6];
  
  // Common input read and output store for ICALsim and miniICALsim
  static const unsigned int ngenmx=50;
  UInt_t          irun;
  UInt_t          ievt;
  UInt_t          ievt2;
  UInt_t          ievt3;
  UInt_t          ngent;
  Int_t           pidin[ngenmx];   //[ngent]
  Float_t         ievt_wt;
  Int_t           intxn_id;
  Float_t         momin[ngenmx];   //[ngent]
  Float_t         thein[ngenmx];   //[ngent]
  Float_t         phiin[ngenmx];   //[ngent]
  Float_t         posxin[ngenmx];   //[ngent]
  Float_t         posyin[ngenmx];   //[ngent]
  Float_t         poszin[ngenmx];   //[ngent]

  // ICALsim Root Files Input Data Read (SIM)
  static const unsigned int nsimhtmx=4000;
  UInt_t          nsimht;
  UInt_t          detid[nsimhtmx];   //[nsimht]
  Int_t           simpdgid[nsimhtmx];   //[nsimht]
  Float_t         simtime[nsimhtmx];   //[nsimht]
  Float_t         simenr[nsimhtmx];   //[nsimht]
  Float_t         simvx[nsimhtmx];   //[nsimht]
  Float_t         simvy[nsimhtmx];   //[nsimht]
  Float_t         simvz[nsimhtmx];   //[nsimht]
  Float_t         simpx[nsimhtmx];   //[nsimht]
  Float_t         simpy[nsimhtmx];   //[nsimht]
  Float_t         simpz[nsimhtmx];   //[nsimht]
  Float_t         simlocvx[nsimhtmx];   //[nsimht]
  Float_t         simlocvy[nsimhtmx];   //[nsimht]
    
  // ICALsim Root Files Input Data Read
  static const unsigned int ndigihtmx=5000;
  UInt_t          ndigiht;
  Int_t           trigx;
  Int_t           trigy;
  UInt_t          stripid[ndigihtmx];   //[ndigiht]
  Int_t           digipdgid[ndigihtmx];   //[ndigiht]
  Int_t           digitime[ndigihtmx];   //[ndigiht]
  Int_t           digitruetime[ndigihtmx];   //[ndigiht]
  Float_t         digienr[ndigihtmx];   //[ndigiht]
  Float_t         digivx[ndigihtmx];   //[ndigiht]
  Float_t         digivy[ndigihtmx];   //[ndigiht]
  Float_t         digivz[ndigihtmx];   //[ndigiht]
  Float_t         digipx[ndigihtmx];   //[ndigiht]
  Float_t         digipy[ndigihtmx];   //[ndigiht]
  Float_t         digipz[ndigihtmx];   //[ndigiht]
  int diginoise[ndigihtmx];

  // Reco Output store for both sim.
  static const unsigned int nvishtmx=5000;
  G4float fitposxx[nvishtmx];
  G4float fitposyy[nvishtmx];
  G4float fitposzz[nvishtmx];
  G4float fitlayzz[nvishtmx];
  G4float fitlayx2[nvishtmx];
  G4float fitlayx3[nvishtmx];
  G4float fitlayx4[nvishtmx];
  G4float fitlaymom[nvishtmx];
  G4float fitlaythe[nvishtmx];
  G4float fitlayphi[nvishtmx];
  G4float extrapolxx[nvishtmx];
  G4float extrapolyy[nvishtmx];
  G4float extrapolmom[nvishtmx];

  G4float momdiff1;
  G4float radialdiff1;

  unsigned int nvisht;
  G4float clstposxx[nvishtmx];
  G4float clstposyy[nvishtmx];
  G4float clstposzz[nvishtmx];
  G4int clstposzpln[nvishtmx];
  unsigned int nvisclst;

  static  const unsigned int nthtmx=100;

  Int_t ntrecord1x;
  Int_t ntrecord1y;
  Int_t ntrecord2x;
  Int_t ntrecord2y;
  Int_t striprec1x[nthtmx];
  Int_t striprec1y[nthtmx];
  Int_t striprec2x[nthtmx];
  Int_t striprec2y[nthtmx];
  Float_t tdcrec1x[nthtmx];
  Float_t tdcrec1y[nthtmx];
  Float_t tdcrec2x[nthtmx];
  Float_t tdcrec2y[nthtmx];

  Int_t nhits_last;
  Int_t nhits_last_m1;

  Int_t strtnhitsx;
  Int_t strtnhitsy;
  Float_t strtchisqx;
  Float_t strtchisqy;
  Float_t strtintercptx;
  Float_t strtintercpty;
  Float_t strtslopex;
  Float_t strtslopey;

  Float_t simpleradii;
  Float_t simplecurv;
  Float_t simplex0;
  Float_t simplez0;
  Float_t simplechisqpos;
  Float_t simplechisqneg;
  Float_t simplechisqcndn;
  Float_t simpleavgxpos;
  Float_t simpleavgxneg;
  Float_t simpleavgxcndn;
  Float_t simpleavgxmeas;
  Float_t simplenhits;
  
  Int_t ntdc1x;
  Int_t ntstrp1x;
  Int_t tdcID1x[nthtmx];
  Int_t StrpID1x[nthtmx];
  Float_t TDCval1x[nthtmx];

  Int_t ntdc2x;
  Int_t ntstrp2x;
  Int_t tdcID2x[nthtmx];
  Int_t StrpID2x[nthtmx];
  Float_t TDCval2x[nthtmx];

  Int_t ntdc1y;
  Int_t ntstrp1y;
  Int_t tdcID1y[nthtmx];
  Int_t StrpID1y[nthtmx];
  Float_t TDCval1y[nthtmx];

  Int_t ntdc2y;
  Int_t ntstrp2y;
  Int_t tdcID2y[nthtmx];
  Int_t StrpID2y[nthtmx];
  Float_t TDCval2y[nthtmx];

  Int_t nhits_below;
  Float_t ftime_last;

  static  const unsigned int ntrkmx=20;
  // Int_t           hw_trigx;
  UInt_t          ngenerated;
  UInt_t          naperture;
  UInt_t          triggeracceptance;
  Int_t           hw_trig;
  Int_t           sw_trigx;
  Int_t           sw_trigy;
  UInt_t          ntrkt;
  Int_t           itype[ntrkmx];   //[ntrkt]
  Int_t           nLayer;
  Int_t           nhits[ntrkmx];   //[ntrkt]
  Int_t           nhits_finder[ntrkmx];   //[ntrkt]
  Float_t         chisq[ntrkmx];   //[ntrkt]
  Float_t chisq2[ntrkmx];  
  Float_t         cvalue[ntrkmx];   //[ntrkt]
  Int_t           fc_or_pc[ntrkmx];   //[ntrkt]
  Float_t         trkmm[ntrkmx];   //[ntrkt]
  Float_t         trkth[ntrkmx];   //[ntrkt]
  Float_t         trkph[ntrkmx];   //[ntrkt]
  Float_t         momvx[ntrkmx];   //[ntrkt]
  Float_t         thevx[ntrkmx];   //[ntrkt]
  
  Float_t         phivx[ntrkmx];   //[ntrkt]
  Float_t         posxvx[ntrkmx];   //[ntrkt]
  Float_t         posyvx[ntrkmx];   //[ntrkt]
  Float_t         poszvx[ntrkmx];   //[ntrkt]

  //ROOF RSA

	G4float momrf[ntrkmx]; 	  //Measured momentum of reconstrued track
	G4float therf[ntrkmx];	  //Measured polar angle of track
	G4float phirf[ntrkmx];        //Measured azimuthal angle of track
	G4float posxrf[ntrkmx];	  //Starting X-position
	G4float posyrf[ntrkmx];       //Starting Y-position
	G4float poszrf[ntrkmx];       //Starting Z-position








  
  Float_t         momend[ntrkmx];   //[ntrkt]
  Float_t         theend[ntrkmx];   //[ntrkt]
  Float_t         phiend[ntrkmx];   //[ntrkt]
  Float_t         posxend[ntrkmx];   //[ntrkt]
  Float_t         posyend[ntrkmx];   //[ntrkt]
  Int_t            strpxend[ntrkmx];   //[ntrkt]
  Int_t            strpyend[ntrkmx];   //[ntrkt]
  Float_t         poszend[ntrkmx];   //[ntrkt]
  Float_t         tx_end[ntrkmx];   //[ntrkt]
  Float_t         ty_end[ntrkmx];   //[ntrkt]
  Float_t         momds[ntrkmx];   //[ntrkt]
  Float_t         momrg[ntrkmx];   //[ntrkt]
  Float_t         mcxgnvx[ntrkmx];   //[ntrkt]
  Float_t         mcygnvx[ntrkmx];   //[ntrkt]
  Float_t         momgnvx[ntrkmx];   //[ntrkt]
  Float_t         thegnvx[ntrkmx];   //[ntrkt]
  Float_t         phignvx[ntrkmx];   //[ntrkt]
  Float_t         momgnend[ntrkmx];   //[ntrkt]
  Float_t         thegnend[ntrkmx];   //[ntrkt]
  Float_t         phignend[ntrkmx];   //[ntrkt]
  Int_t           vtxzplane[ntrkmx];   //[ntrkt]
  Int_t           endzplane[ntrkmx];   //[ntrkt]
  Int_t           ntrkcl[ntrkmx];   //[ntrkt]
  Int_t           ntrkst[ntrkmx];   //[ntrkt]
  Int_t           ntotcl;
  Int_t           ntotst;
  Int_t           inohits;
  Int_t           orighits;
  Int_t           inoclust;
  Int_t           origclust;
  Float_t         hPathlength;
  Int_t           x_hits;
  Int_t           y_hits;
  Int_t           inohits_old;
  Int_t           orighits_old;
  Int_t           x_hits_old;
  Int_t           y_hits_old;
  Int_t           hit_wo_ghst;
  Float_t         e_hadron;
  Int_t           nhits_largest_cluster;
  Int_t           orighits_trape;
  Int_t           orighits_cluster;
  Int_t           hit_wogh_orighits;
  Float_t         theta_hadron_shw;
  Float_t         had_eigen_val[3];
  Float_t         phi_hadron_shw;
  Float_t         theta_hadron_in;
  Float_t         phi_hadron_in;
  Float_t         dot_angle_had_shw;
  Int_t           nhits_largest_cluster_selected;
  Float_t         range;
  Float_t         tx[ntrkmx];
  Float_t         ty[ntrkmx];
  Float_t         xxin[ntrkmx];
  Float_t         yyin[ntrkmx];
  Float_t         txin[ntrkmx];
  Float_t         tyin[ntrkmx];
  Float_t         therr[ntrkmx];
  Float_t         pherr[ntrkmx];
  Float_t         xxerr[ntrkmx];
  Float_t         yyerr[ntrkmx];
  Float_t         txerr[ntrkmx];
  Float_t         tyerr[ntrkmx];
  Float_t         qperr[ntrkmx];
  Float_t         xxenderr[ntrkmx];
  Float_t         yyenderr[ntrkmx];
  Float_t         txenderr[ntrkmx];
  Float_t         tyenderr[ntrkmx];
  Float_t         qpenderr[ntrkmx];
  
  Int_t nmxhit;





  G4float atimslope[ntrkmx];
  G4float atiminter[ntrkmx];

  G4float xxtxerr[ntrkmx];
  G4float xxtyerr[ntrkmx];
  G4float yytyerr[ntrkmx];
  G4float yytxerr[ntrkmx];
    G4float txtyerr[ntrkmx];
  //

  G4float XdevLay1[ntrkmx];
   G4float YdevLay1[ntrkmx];
  G4float XdevLay2[ntrkmx];
   G4float YdevLay2[ntrkmx];

  G4float XdevLay3[ntrkmx];
  G4float YdevLay3[ntrkmx];

  G4float XdevLay4[ntrkmx];
  G4float YdevLay4[ntrkmx];
  
  G4float XdevLay5[ntrkmx];
  G4float YdevLay5[ntrkmx];

  G4float XdevLay6[ntrkmx];
  G4float YdevLay6[ntrkmx];

  G4float XdevLay7[ntrkmx];
  G4float YdevLay7[ntrkmx];

  G4float XdevLay8[ntrkmx];
  G4float YdevLay8[ntrkmx];

  G4float XdevLay9[ntrkmx];
  G4float YdevLay9[ntrkmx];

  G4float XdevLay10[ntrkmx];
  G4float YdevLay10[ntrkmx];

  G4float XdevLay11[ntrkmx];
  G4float YdevLay11[ntrkmx];

  G4float XdevLay12[ntrkmx];
  G4float YdevLay12[ntrkmx];
  







  
  
  G4float L0_StrpNo[ntrkmx];
  G4float L1_StrpNo[ntrkmx];
  G4float L2_StrpNo[ntrkmx];
  G4float L3_StrpNo[ntrkmx];
  
  G4int cmv_lay[ntrkmx];
    G4int cmv_locno00[ntrkmx];
    G4int cmv_locno01[ntrkmx];
    G4int cmv_locno02[ntrkmx];
    G4int cmv_locno03[ntrkmx];


    G4int cmv_locno10[ntrkmx];
  G4int cmv_locno11[ntrkmx];
    G4int cmv_locno12[ntrkmx];

    G4int cmv_locno20[ntrkmx];
    G4int cmv_locno21[ntrkmx];
    G4int cmv_locno22[ntrkmx];

    G4int cmv_locno30[ntrkmx];
    G4int cmv_locno31[ntrkmx];
    G4int cmv_locno32[ntrkmx];

   G4int cmv_locno40[ntrkmx];
    G4int cmv_locno41[ntrkmx];
    G4int cmv_locno42[ntrkmx];

     G4int cmv_locno50[ntrkmx];
    G4int cmv_locno51[ntrkmx];
    G4int cmv_locno52[ntrkmx];

     G4int cmv_locno60[ntrkmx];
    G4int cmv_locno61[ntrkmx];
    G4int cmv_locno62[ntrkmx];

  G4int cmv_stripno[ntrkmx];

  G4float extrapolatim00[ntrkmx];
  G4float extrapolatim01[ntrkmx];
    G4float extrapolatim02[ntrkmx];
    G4float extrapolatim03[ntrkmx];


    G4int Trig00[ntrkmx];
  G4int Trig01[ntrkmx];
    G4int Trig02[ntrkmx];
    G4int Trig03[ntrkmx];

  G4float atim[ntrkmx];


	static const unsigned int cmv_nhtmx=10000;

  unsigned int cmv_nhit;
  //cmvd cluster hit:
  unsigned int cmv_hitid[ cmv_nhtmx];
  G4int cmv_hitpdgid[ cmv_nhtmx];
  G4float cmv_hitLeTim[ cmv_nhtmx];
  G4float cmv_hitRiTim[ cmv_nhtmx];
  G4float cmv_hitLePul[ cmv_nhtmx];
  G4float cmv_hitRiPul[ cmv_nhtmx];
  G4float cmv_hitTrueposx[ cmv_nhtmx];
  G4float cmv_hitTrueposy[ cmv_nhtmx];
  G4float cmv_hitTrueposz[ cmv_nhtmx];
  G4float cmv_hitRecoposx[ cmv_nhtmx];
  G4float cmv_hitRecoposy[ cmv_nhtmx];
  G4float cmv_hitRecoposz[ cmv_nhtmx];
  

  //  G4int cmv_hitsiz[ cmv_nhtmx];

  //cmv_ cluster:


  static const unsigned int cmv_nclustmx=10000;
  unsigned int cmv_nclust;
  //cmvd cluster hit:
  unsigned int cmv_clustid[ cmv_nclustmx];
  G4int cmv_clustpdgid[ cmv_nclustmx];
  G4float cmv_clustLeTim[ cmv_nclustmx];
  G4float cmv_clustRiTim[ cmv_nclustmx];
  G4float cmv_clustLePul[ cmv_nclustmx];
  G4float cmv_clustRiPul[ cmv_nclustmx];
  G4float cmv_clustTrueposx[ cmv_nclustmx];
  G4float cmv_clustTrueposy[ cmv_nclustmx];
  G4float cmv_clustTrueposz[ cmv_nclustmx];
  G4float cmv_clustRecoposx[ cmv_nclustmx];
  G4float cmv_clustRecoposy[ cmv_nclustmx];
  G4float cmv_clustRecoposz[ cmv_nclustmx];
  G4int cmv_clustsiz[ cmv_nclustmx];









  
  //cmvd extrapol :


	static const unsigned int cmv_nexphtmx=1000;

  unsigned int cmv_nexphit;

  
  unsigned int cmv_expid[cmv_nexphtmx];
  
  G4float cmv_Expposx[cmv_nexphtmx];
 G4float cmv_Expposy[cmv_nexphtmx];
 G4float cmv_Expposz[cmv_nexphtmx];


  
  G4float cmv_DCAposx[cmv_nexphtmx];
 G4float cmv_DCAposy[cmv_nexphtmx];
 G4float cmv_DCAposz[cmv_nexphtmx];


  
  G4float distofclosapp[ntrkmx];
  G4int planeedge[ntrkmx];
  G4int clustersize00[ntrkmx];
  
  G4float extrapolposx00[ntrkmx];
  G4float extrapolposy00[ntrkmx];
  G4float extrapolposz00[ntrkmx];
  
  G4float extrapolposxerr00[ntrkmx];
  G4float extrapolposyerr00[ntrkmx];
  G4float extrapolposzerr00[ntrkmx];

  G4float cmvhitrecoposx00[ntrkmx];
  G4float cmvhitrecoposy00[ntrkmx];
  G4float cmvhitrecoposz00[ntrkmx];

  G4float cmvhittrueposx00[ntrkmx];
  G4float cmvhittrueposy00[ntrkmx];
  G4float cmvhittrueposz00[ntrkmx];
  
  G4float cmvhitrecoposxerr00[ntrkmx];
  G4float cmvhitrecoposyerr00[ntrkmx];
  G4float cmvhitrecoposzerr00[ntrkmx];
  
  G4float LeTime00[ntrkmx];
  G4float RiTime00[ntrkmx];
  G4float LePulse00[ntrkmx];
  G4float RiPulse00[ntrkmx];

  //   pEventTree->Branch("LeTime31", LeTime31, "LeTime31[ntrkt]/F");
  // pEventTree->Branch("RiTime31", RiTime31, "RiTime31[ntrkt]/F");

  //   pEventTree->Branch("LePulse31", LePulse31, "LePulse31[ntrkt]/F");
  // pEventTree->Branch("RiPulse31", RiPulse31, "RiPulse31[ntrkt]/F");





  G4int clustersize01[ntrkmx];
  
  G4float extrapolposx01[ntrkmx];
  G4float extrapolposy01[ntrkmx];
  G4float extrapolposz01[ntrkmx];
  
  G4float extrapolposxerr01[ntrkmx];
  G4float extrapolposyerr01[ntrkmx];
  G4float extrapolposzerr01[ntrkmx];

  G4float cmvhitrecoposx01[ntrkmx];
  G4float cmvhitrecoposy01[ntrkmx];
  G4float cmvhitrecoposz01[ntrkmx];

  G4float cmvhittrueposx01[ntrkmx];
  G4float cmvhittrueposy01[ntrkmx];
  G4float cmvhittrueposz01[ntrkmx];
  
  G4float cmvhitrecoposxerr01[ntrkmx];
  G4float cmvhitrecoposyerr01[ntrkmx];
  G4float cmvhitrecoposzerr01[ntrkmx];

  G4float LeTime01[ntrkmx];
  G4float RiTime01[ntrkmx];
  G4float LePulse01[ntrkmx];
  G4float RiPulse01[ntrkmx];


    G4int clustersize02[ntrkmx];
  G4float extrapolposx02[ntrkmx];
  G4float extrapolposy02[ntrkmx];
  G4float extrapolposz02[ntrkmx];
  
  G4float extrapolposxerr02[ntrkmx];
  G4float extrapolposyerr02[ntrkmx];
  G4float extrapolposzerr02[ntrkmx];

  G4float cmvhitrecoposx02[ntrkmx];
  G4float cmvhitrecoposy02[ntrkmx];
  G4float cmvhitrecoposz02[ntrkmx];

  G4float cmvhittrueposx02[ntrkmx];
  G4float cmvhittrueposy02[ntrkmx];
  G4float cmvhittrueposz02[ntrkmx];
  
  G4float cmvhitrecoposxerr02[ntrkmx];
  G4float cmvhitrecoposyerr02[ntrkmx];
  G4float cmvhitrecoposzerr02[ntrkmx];


  

  G4float LeTime02[ntrkmx];
  G4float RiTime02[ntrkmx];
  G4float LePulse02[ntrkmx];
  G4float RiPulse02[ntrkmx];



  G4int clustersize03[ntrkmx];




  

  G4float extrapolposx03[ntrkmx];
  G4float extrapolposy03[ntrkmx];
  G4float extrapolposz03[ntrkmx];
  
  G4float extrapolposxerr03[ntrkmx];
  G4float extrapolposyerr03[ntrkmx];
  G4float extrapolposzerr03[ntrkmx];

  G4float cmvhitrecoposx03[ntrkmx];
  G4float cmvhitrecoposy03[ntrkmx];
  G4float cmvhitrecoposz03[ntrkmx];

  G4float cmvhittrueposx03[ntrkmx];
  G4float cmvhittrueposy03[ntrkmx];
  G4float cmvhittrueposz03[ntrkmx];
  
  G4float cmvhitrecoposxerr03[ntrkmx];
  G4float cmvhitrecoposyerr03[ntrkmx];
  G4float cmvhitrecoposzerr03[ntrkmx];

  G4float LeTime03[ntrkmx];
  G4float RiTime03[ntrkmx];
  G4float LePulse03[ntrkmx];
  G4float RiPulse03[ntrkmx];


  G4float debug[ntrkmx];


G4int clustersize10[ntrkmx];
  
  G4float extrapolposx10[ntrkmx];
  G4float extrapolposy10[ntrkmx];
  G4float extrapolposz10[ntrkmx];
  
  G4float extrapolposxerr10[ntrkmx];
  G4float extrapolposyerr10[ntrkmx];
  G4float extrapolposzerr10[ntrkmx];

  G4float cmvhitrecoposx10[ntrkmx];
  G4float cmvhitrecoposy10[ntrkmx];
  G4float cmvhitrecoposz10[ntrkmx];

  G4float cmvhittrueposx10[ntrkmx];
  G4float cmvhittrueposy10[ntrkmx];
  G4float cmvhittrueposz10[ntrkmx];
  
  G4float cmvhitrecoposxerr10[ntrkmx];
  G4float cmvhitrecoposyerr10[ntrkmx];
  G4float cmvhitrecoposzerr10[ntrkmx];

  G4float LeTime10[ntrkmx];
  G4float RiTime10[ntrkmx];
  G4float LePulse10[ntrkmx];
  G4float RiPulse10[ntrkmx];

  G4int clustersize11[ntrkmx];

  G4float extrapolposx11[ntrkmx];
  G4float extrapolposy11[ntrkmx];
  G4float extrapolposz11[ntrkmx];
  
  G4float extrapolposxerr11[ntrkmx];
  G4float extrapolposyerr11[ntrkmx];
  G4float extrapolposzerr11[ntrkmx];

  G4float cmvhitrecoposx11[ntrkmx];
  G4float cmvhitrecoposy11[ntrkmx];
  G4float cmvhitrecoposz11[ntrkmx];

  G4float cmvhittrueposx11[ntrkmx];
  G4float cmvhittrueposy11[ntrkmx];
  G4float cmvhittrueposz11[ntrkmx];
  
  G4float cmvhitrecoposxerr11[ntrkmx];
  G4float cmvhitrecoposyerr11[ntrkmx];
  G4float cmvhitrecoposzerr11[ntrkmx];

  G4float LeTime11[ntrkmx];
  G4float RiTime11[ntrkmx];
  G4float LePulse11[ntrkmx];
  G4float RiPulse11[ntrkmx];


G4int clustersize12[ntrkmx];

  
  G4float extrapolposx12[ntrkmx];
  G4float extrapolposy12[ntrkmx];
  G4float extrapolposz12[ntrkmx];
  
  G4float extrapolposxerr12[ntrkmx];
  G4float extrapolposyerr12[ntrkmx];
  G4float extrapolposzerr12[ntrkmx];

  G4float cmvhitrecoposx12[ntrkmx];
  G4float cmvhitrecoposy12[ntrkmx];
  G4float cmvhitrecoposz12[ntrkmx];

  G4float cmvhittrueposx12[ntrkmx];
  G4float cmvhittrueposy12[ntrkmx];
  G4float cmvhittrueposz12[ntrkmx];
  
  G4float cmvhitrecoposxerr12[ntrkmx];
  G4float cmvhitrecoposyerr12[ntrkmx];
  G4float cmvhitrecoposzerr12[ntrkmx];

  G4float LeTime12[ntrkmx];
  G4float RiTime12[ntrkmx];
  G4float LePulse12[ntrkmx];
  G4float RiPulse12[ntrkmx];



G4int clustersize21[ntrkmx];
  
  G4float extrapolposx21[ntrkmx];
  G4float extrapolposy21[ntrkmx];
  G4float extrapolposz21[ntrkmx];
  
  G4float extrapolposxerr21[ntrkmx];
  G4float extrapolposyerr21[ntrkmx];
  G4float extrapolposzerr21[ntrkmx];

  G4float cmvhitrecoposx21[ntrkmx];
  G4float cmvhitrecoposy21[ntrkmx];
  G4float cmvhitrecoposz21[ntrkmx];

  G4float cmvhittrueposx21[ntrkmx];
  G4float cmvhittrueposy21[ntrkmx];
  G4float cmvhittrueposz21[ntrkmx];
  
  G4float cmvhitrecoposxerr21[ntrkmx];
  G4float cmvhitrecoposyerr21[ntrkmx];
  G4float cmvhitrecoposzerr21[ntrkmx];


  G4float LeTime21[ntrkmx];
  G4float RiTime21[ntrkmx];
  G4float LePulse21[ntrkmx];
  G4float RiPulse21[ntrkmx];

G4int clustersize20[ntrkmx];

  G4float extrapolposx20[ntrkmx];
  G4float extrapolposy20[ntrkmx];
  G4float extrapolposz20[ntrkmx];
  
  G4float extrapolposxerr20[ntrkmx];
  G4float extrapolposyerr20[ntrkmx];
  G4float extrapolposzerr20[ntrkmx];

  G4float cmvhitrecoposx20[ntrkmx];
  G4float cmvhitrecoposy20[ntrkmx];
  G4float cmvhitrecoposz20[ntrkmx];

  G4float cmvhittrueposx20[ntrkmx];
  G4float cmvhittrueposy20[ntrkmx];
  G4float cmvhittrueposz20[ntrkmx];
  
  G4float cmvhitrecoposxerr20[ntrkmx];
  G4float cmvhitrecoposyerr20[ntrkmx];
  G4float cmvhitrecoposzerr20[ntrkmx];

  G4float LeTime20[ntrkmx];
  G4float RiTime20[ntrkmx];
  G4float LePulse20[ntrkmx];
  G4float RiPulse20[ntrkmx];

G4int clustersize22[ntrkmx];

  G4float extrapolposx22[ntrkmx];
  G4float extrapolposy22[ntrkmx];
  G4float extrapolposz22[ntrkmx];
  
  G4float extrapolposxerr22[ntrkmx];
  G4float extrapolposyerr22[ntrkmx];
  G4float extrapolposzerr22[ntrkmx];

  G4float cmvhitrecoposx22[ntrkmx];
  G4float cmvhitrecoposy22[ntrkmx];
  G4float cmvhitrecoposz22[ntrkmx];

  G4float cmvhittrueposx22[ntrkmx];
  G4float cmvhittrueposy22[ntrkmx];
  G4float cmvhittrueposz22[ntrkmx];
  
  G4float cmvhitrecoposxerr22[ntrkmx];
  G4float cmvhitrecoposyerr22[ntrkmx];
  G4float cmvhitrecoposzerr22[ntrkmx];

  G4float LeTime22[ntrkmx];
  G4float RiTime22[ntrkmx];
  G4float LePulse22[ntrkmx];
  G4float RiPulse22[ntrkmx];

G4int clustersize30[ntrkmx];
  
  G4float extrapolposx30[ntrkmx];
  G4float extrapolposy30[ntrkmx];
  G4float extrapolposz30[ntrkmx];
  
  G4float extrapolposxerr30[ntrkmx];
  G4float extrapolposyerr30[ntrkmx];
  G4float extrapolposzerr30[ntrkmx];

  G4float cmvhitrecoposx30[ntrkmx];
  G4float cmvhitrecoposy30[ntrkmx];
  G4float cmvhitrecoposz30[ntrkmx];

  G4float cmvhittrueposx30[ntrkmx];
  G4float cmvhittrueposy30[ntrkmx];
  G4float cmvhittrueposz30[ntrkmx];
  
  G4float cmvhitrecoposxerr30[ntrkmx];
  G4float cmvhitrecoposyerr30[ntrkmx];
  G4float cmvhitrecoposzerr30[ntrkmx];

  G4float LeTime30[ntrkmx];
  G4float RiTime30[ntrkmx];
  G4float LePulse30[ntrkmx];
  G4float RiPulse30[ntrkmx];



G4int clustersize31[ntrkmx];

  
  G4float extrapolposx31[ntrkmx];
  G4float extrapolposy31[ntrkmx];
  G4float extrapolposz31[ntrkmx];
  
  G4float extrapolposxerr31[ntrkmx];
  G4float extrapolposyerr31[ntrkmx];
  G4float extrapolposzerr31[ntrkmx];

  G4float cmvhitrecoposx31[ntrkmx];
  G4float cmvhitrecoposy31[ntrkmx];
  G4float cmvhitrecoposz31[ntrkmx];

  G4float cmvhittrueposx31[ntrkmx];
  G4float cmvhittrueposy31[ntrkmx];
  G4float cmvhittrueposz31[ntrkmx];
  
  G4float cmvhitrecoposxerr31[ntrkmx];
  G4float cmvhitrecoposyerr31[ntrkmx];
  G4float cmvhitrecoposzerr31[ntrkmx];

  G4float LeTime31[ntrkmx];
  G4float RiTime31[ntrkmx];
  G4float LePulse31[ntrkmx];
  G4float RiPulse31[ntrkmx];


G4int clustersize32[ntrkmx];

  
  G4float extrapolposx32[ntrkmx];
  G4float extrapolposy32[ntrkmx];
  G4float extrapolposz32[ntrkmx];
  
  G4float extrapolposxerr32[ntrkmx];
  G4float extrapolposyerr32[ntrkmx];
  G4float extrapolposzerr32[ntrkmx];

  G4float cmvhitrecoposx32[ntrkmx];
  G4float cmvhitrecoposy32[ntrkmx];
  G4float cmvhitrecoposz32[ntrkmx];

  G4float cmvhittrueposx32[ntrkmx];
  G4float cmvhittrueposy32[ntrkmx];
  G4float cmvhittrueposz32[ntrkmx];
  
  G4float cmvhitrecoposxerr32[ntrkmx];
  G4float cmvhitrecoposyerr32[ntrkmx];
  G4float cmvhitrecoposzerr32[ntrkmx];



  G4float LeTime32[ntrkmx];
  G4float RiTime32[ntrkmx];
  G4float LePulse32[ntrkmx];
  G4float RiPulse32[ntrkmx];


  //11022022

  G4int clustersize40[ntrkmx];
  
  G4float extrapolposx40[ntrkmx];
  G4float extrapolposy40[ntrkmx];
  G4float extrapolposz40[ntrkmx];
  
  G4float extrapolposxerr40[ntrkmx];
  G4float extrapolposyerr40[ntrkmx];
  G4float extrapolposzerr40[ntrkmx];

  G4float cmvhitrecoposx40[ntrkmx];
  G4float cmvhitrecoposy40[ntrkmx];
  G4float cmvhitrecoposz40[ntrkmx];

  G4float cmvhittrueposx40[ntrkmx];
  G4float cmvhittrueposy40[ntrkmx];
  G4float cmvhittrueposz40[ntrkmx];
  
  G4float cmvhitrecoposxerr40[ntrkmx];
  G4float cmvhitrecoposyerr40[ntrkmx];
  G4float cmvhitrecoposzerr40[ntrkmx];

  G4float LeTime40[ntrkmx];
  G4float RiTime40[ntrkmx];
  G4float LePulse40[ntrkmx];
  G4float RiPulse40[ntrkmx];



G4int clustersize41[ntrkmx];

  
  G4float extrapolposx41[ntrkmx];
  G4float extrapolposy41[ntrkmx];
  G4float extrapolposz41[ntrkmx];
  
  G4float extrapolposxerr41[ntrkmx];
  G4float extrapolposyerr41[ntrkmx];
  G4float extrapolposzerr41[ntrkmx];

  G4float cmvhitrecoposx41[ntrkmx];
  G4float cmvhitrecoposy41[ntrkmx];
  G4float cmvhitrecoposz41[ntrkmx];

  G4float cmvhittrueposx41[ntrkmx];
  G4float cmvhittrueposy41[ntrkmx];
  G4float cmvhittrueposz41[ntrkmx];
  
  G4float cmvhitrecoposxerr41[ntrkmx];
  G4float cmvhitrecoposyerr41[ntrkmx];
  G4float cmvhitrecoposzerr41[ntrkmx];

  G4float LeTime41[ntrkmx];
  G4float RiTime41[ntrkmx];
  G4float LePulse41[ntrkmx];
  G4float RiPulse41[ntrkmx];


G4int clustersize42[ntrkmx];

  
  G4float extrapolposx42[ntrkmx];
  G4float extrapolposy42[ntrkmx];
  G4float extrapolposz42[ntrkmx];
  
  G4float extrapolposxerr42[ntrkmx];
  G4float extrapolposyerr42[ntrkmx];
  G4float extrapolposzerr42[ntrkmx];

  G4float cmvhitrecoposx42[ntrkmx];
  G4float cmvhitrecoposy42[ntrkmx];
  G4float cmvhitrecoposz42[ntrkmx];

  G4float cmvhittrueposx42[ntrkmx];
  G4float cmvhittrueposy42[ntrkmx];
  G4float cmvhittrueposz42[ntrkmx];
  
  G4float cmvhitrecoposxerr42[ntrkmx];
  G4float cmvhitrecoposyerr42[ntrkmx];
  G4float cmvhitrecoposzerr42[ntrkmx];



  G4float LeTime42[ntrkmx];
  G4float RiTime42[ntrkmx];
  G4float LePulse42[ntrkmx];
  G4float RiPulse42[ntrkmx];


G4int clustersize50[ntrkmx];
  
  G4float extrapolposx50[ntrkmx];
  G4float extrapolposy50[ntrkmx];
  G4float extrapolposz50[ntrkmx];
  
  G4float extrapolposxerr50[ntrkmx];
  G4float extrapolposyerr50[ntrkmx];
  G4float extrapolposzerr50[ntrkmx];

  G4float cmvhitrecoposx50[ntrkmx];
  G4float cmvhitrecoposy50[ntrkmx];
  G4float cmvhitrecoposz50[ntrkmx];

  G4float cmvhittrueposx50[ntrkmx];
  G4float cmvhittrueposy50[ntrkmx];
  G4float cmvhittrueposz50[ntrkmx];
  
  G4float cmvhitrecoposxerr50[ntrkmx];
  G4float cmvhitrecoposyerr50[ntrkmx];
  G4float cmvhitrecoposzerr50[ntrkmx];

  G4float LeTime50[ntrkmx];
  G4float RiTime50[ntrkmx];
  G4float LePulse50[ntrkmx];
  G4float RiPulse50[ntrkmx];



G4int clustersize51[ntrkmx];

  
  G4float extrapolposx51[ntrkmx];
  G4float extrapolposy51[ntrkmx];
  G4float extrapolposz51[ntrkmx];
  
  G4float extrapolposxerr51[ntrkmx];
  G4float extrapolposyerr51[ntrkmx];
  G4float extrapolposzerr51[ntrkmx];

  G4float cmvhitrecoposx51[ntrkmx];
  G4float cmvhitrecoposy51[ntrkmx];
  G4float cmvhitrecoposz51[ntrkmx];

  G4float cmvhittrueposx51[ntrkmx];
  G4float cmvhittrueposy51[ntrkmx];
  G4float cmvhittrueposz51[ntrkmx];
  
  G4float cmvhitrecoposxerr51[ntrkmx];
  G4float cmvhitrecoposyerr51[ntrkmx];
  G4float cmvhitrecoposzerr51[ntrkmx];

  G4float LeTime51[ntrkmx];
  G4float RiTime51[ntrkmx];
  G4float LePulse51[ntrkmx];
  G4float RiPulse51[ntrkmx];


G4int clustersize52[ntrkmx];

  
  G4float extrapolposx52[ntrkmx];
  G4float extrapolposy52[ntrkmx];
  G4float extrapolposz52[ntrkmx];
  
  G4float extrapolposxerr52[ntrkmx];
  G4float extrapolposyerr52[ntrkmx];
  G4float extrapolposzerr52[ntrkmx];

  G4float cmvhitrecoposx52[ntrkmx];
  G4float cmvhitrecoposy52[ntrkmx];
  G4float cmvhitrecoposz52[ntrkmx];

  G4float cmvhittrueposx52[ntrkmx];
  G4float cmvhittrueposy52[ntrkmx];
  G4float cmvhittrueposz52[ntrkmx];
  
  G4float cmvhitrecoposxerr52[ntrkmx];
  G4float cmvhitrecoposyerr52[ntrkmx];
  G4float cmvhitrecoposzerr52[ntrkmx];



  G4float LeTime52[ntrkmx];
  G4float RiTime52[ntrkmx];
  G4float LePulse52[ntrkmx];
  G4float RiPulse52[ntrkmx];


G4int clustersize60[ntrkmx];
  
  G4float extrapolposx60[ntrkmx];
  G4float extrapolposy60[ntrkmx];
  G4float extrapolposz60[ntrkmx];
  
  G4float extrapolposxerr60[ntrkmx];
  G4float extrapolposyerr60[ntrkmx];
  G4float extrapolposzerr60[ntrkmx];

  G4float cmvhitrecoposx60[ntrkmx];
  G4float cmvhitrecoposy60[ntrkmx];
  G4float cmvhitrecoposz60[ntrkmx];

  G4float cmvhittrueposx60[ntrkmx];
  G4float cmvhittrueposy60[ntrkmx];
  G4float cmvhittrueposz60[ntrkmx];
  
  G4float cmvhitrecoposxerr60[ntrkmx];
  G4float cmvhitrecoposyerr60[ntrkmx];
  G4float cmvhitrecoposzerr60[ntrkmx];

  G4float LeTime60[ntrkmx];
  G4float RiTime60[ntrkmx];
  G4float LePulse60[ntrkmx];
  G4float RiPulse60[ntrkmx];



G4int clustersize61[ntrkmx];

  
  G4float extrapolposx61[ntrkmx];
  G4float extrapolposy61[ntrkmx];
  G4float extrapolposz61[ntrkmx];
  
  G4float extrapolposxerr61[ntrkmx];
  G4float extrapolposyerr61[ntrkmx];
  G4float extrapolposzerr61[ntrkmx];

  G4float cmvhitrecoposx61[ntrkmx];
  G4float cmvhitrecoposy61[ntrkmx];
  G4float cmvhitrecoposz61[ntrkmx];

  G4float cmvhittrueposx61[ntrkmx];
  G4float cmvhittrueposy61[ntrkmx];
  G4float cmvhittrueposz61[ntrkmx];
  
  G4float cmvhitrecoposxerr61[ntrkmx];
  G4float cmvhitrecoposyerr61[ntrkmx];
  G4float cmvhitrecoposzerr61[ntrkmx];

  G4float LeTime61[ntrkmx];
  G4float RiTime61[ntrkmx];
  G4float LePulse61[ntrkmx];
  G4float RiPulse61[ntrkmx];


G4int clustersize62[ntrkmx];

  
  G4float extrapolposx62[ntrkmx];
  G4float extrapolposy62[ntrkmx];
  G4float extrapolposz62[ntrkmx];
  
  G4float extrapolposxerr62[ntrkmx];
  G4float extrapolposyerr62[ntrkmx];
  G4float extrapolposzerr62[ntrkmx];

  G4float cmvhitrecoposx62[ntrkmx];
  G4float cmvhitrecoposy62[ntrkmx];
  G4float cmvhitrecoposz62[ntrkmx];

  G4float cmvhittrueposx62[ntrkmx];
  G4float cmvhittrueposy62[ntrkmx];
  G4float cmvhittrueposz62[ntrkmx];
  
  G4float cmvhitrecoposxerr62[ntrkmx];
  G4float cmvhitrecoposyerr62[ntrkmx];
  G4float cmvhitrecoposzerr62[ntrkmx];



  G4float LeTime62[ntrkmx];
  G4float RiTime62[ntrkmx];
  G4float LePulse62[ntrkmx];
  G4float RiPulse62[ntrkmx];



  //11
  
  G4float extra_diff1[ntrkmx];
  G4float extra_diff2[ntrkmx];
  G4float extra_diff3[ntrkmx];


  
   G4float ellip_diff00[ntrkmx];
  G4float ellip_diff01[ntrkmx];
  G4float ellip_diff02[ntrkmx];
  G4float ellip_diff03[ntrkmx];

  G4float ellip_diff10[ntrkmx];
  G4float ellip_diff11[ntrkmx];
  G4float ellip_diff12[ntrkmx];

  G4float ellip_diff20[ntrkmx];
  G4float ellip_diff21[ntrkmx];
  G4float ellip_diff22[ntrkmx];

    G4float ellip_diff30[ntrkmx];
  G4float ellip_diff31[ntrkmx];
  G4float ellip_diff32[ntrkmx];


  //cmv
  
  //simulation
	static const unsigned int cmv_nsimhtmx=1000;
  unsigned int cmv_nsimhit;
  int cmv_detid[cmv_nsimhtmx];
  int  cmv_simpdgid[cmv_nsimhtmx];
  float cmv_simtime[cmv_nsimhtmx];
  float cmv_simenr[cmv_nsimhtmx];
  float cmv_simposx[cmv_nsimhtmx]; 
  float cmv_simposy[cmv_nsimhtmx];
  float cmv_simposz[cmv_nsimhtmx];
  
	Float_t cmv_simpx[cmv_nsimhtmx];
	Float_t cmv_simpy[cmv_nsimhtmx];
	Float_t cmv_simpz[cmv_nsimhtmx];
  
  float cmv_simlocx[cmv_nsimhtmx];
  float cmv_simlocy[cmv_nsimhtmx];
	 float cmv_simlocz[cmv_nsimhtmx];
  //digitization
	// for data only one 16 bit integer + one 32bit integer arrays will be there
	static const unsigned int cmv_ndigihtmx=1000;

  unsigned int cmv_ndigihit;
  int  cmv_digipdgid[cmv_ndigihtmx];
	unsigned int cmv_sipmid[cmv_ndigihtmx];
	// 3 bit for CMVD
	// 2 bit for Layer
	// 7 bit for Strip
	// 2 bit for SiPM
  unsigned int cmv_digitimpul[cmv_ndigihtmx]; // 12bit for energy and 20 bit for time
	
  float cmv_digiposx[cmv_ndigihtmx]; 
  float cmv_digiposy[cmv_ndigihtmx];
  float cmv_digiposz[cmv_ndigihtmx];
  
	float cmv_digimom[cmv_ndigihtmx];
	float cmv_digithe[cmv_ndigihtmx];
	float cmv_digiphi[cmv_ndigihtmx];
  
  float cmv_digilocx[cmv_ndigihtmx];
  float cmv_digilocy[cmv_ndigihtmx];
  float cmv_digilocz[cmv_ndigihtmx];
  //cmv sipm noise
  TFile* sipmnoise;
  TH1F* noise_hist[10][4];



  
private:
  micalDetectorParameterDef *paradef;
  ParameterMessenger *CardFile;

  double CorrTimeError;
  double UnCorrTimeError;
  double TimeToDigiConv;
  double SignalSpeed;

  int numberInLA;
  double parirlay[3];
  double parlay[3];

    double h2dDimX;
  double h2dDimY;
  int nbinxMag2d;
  int nbinyMag2d;
  double magZmax;
  int nbinxMagZ;
  
  
  int nUp;
  int nDown;
  //  int DetectorType;
	double PhotonSpeed;
	double CMVadctons;
};

#endif


// exp([0]*x+[1])+[2]
