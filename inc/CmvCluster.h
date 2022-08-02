#ifndef CMVCLUSTER_H
#define CMVCLUSTER_H
//ClusterCam
//GMA use proper fView value to calcualte X/Y-direction(length) peoperly

//#include "MessageService/MsgService.h"
#include "CmvHit.h"
#include "micalDetectorParameterDef.hh"
#include "MultiSimAnalysisDigi.hh"

class CmvCluster
{

public:
  CmvCluster();
  
  CmvCluster(CmvHit* L0,  CmvHit* L1);
    CmvCluster(CmvCluster* L0,  CmvCluster* L1);
  CmvCluster(CmvCluster* cluster);
  ~CmvCluster();


void CombineClusts(CmvCluster* C2);	

  int    GetPlane()  { return plane;}; 
  int    GetLayer()  { return layer;}; 
  int    GetStrip()  { return strip;};
  int GetClustersize(){return clustersize;}
  //  int GetClustersizeL1(){return clustersize;}
  //  int GetClustersizeL2(){return clustersize;}
  
  void Print();
	bool GetUsed() const {return isUsed;};		
  /* double GetPulse() const { */
  /* 		double pulse =0; */
  /* 		int count =0; */
  /*   if (fLeMiSipm) {count++; pulse = fLeMiSipm->GetPulse();} */
  /*   if (fLePlSipm) {count++; pulse = fLePlSipm->GetPulse();} */
  /*   if (fRiMiSipm) {count++; pulse = fRiMiSipm->GetPulse();} */
  /*   if (fRiPlSipm) {count++; pulse = fRiPlSipm->GetPulse();} */
  /* 		if (count ==0) count = 1; */
  /* 		return pulse/count; */
  /* }; */

  void SetClustersize(int fd){clustersize = fd;}
  //  void SetClustersizeL1(int fd){clustersize = fd;}
  //  void SetClustersizeL2(int fd){clustersize = fd;}

  
  void SetPlane(double fd){plane = fd;}
 void SetLayer(double fd){layer = fd;}
   void SetStrip(double fd){strip = fd;}
	void SetTruePosX(double fd) { fXTruePosX=fd;}
	void SetTruePosY(double fd) { fYTruePosY=fd;}
	void SetTruePosZ(double fd) { fZTruePosZ=fd;}

	void SetRecoPosX(double fd) { fXRecoPosX=fd;}
	void SetRecoPosY(double fd) { fYRecoPosY=fd;}
	void SetRecoPosZ(double fd) { fZRecoPosZ=fd;}

	void SetPosXErr(double fd) { fXPosErr=fd;}
	void SetPosYErr(double fd) { fYPosErr=fd;}
	void SetPosZErr(double fd) { fZPosErr=fd;}
	
	void SetLeTime(double fd) { fLeTime=fd;}	
	void SetRiTime(double fd) { fRiTime=fd;}	

	void SetLePulse(double fd) { fLePulse=fd;}	
	void SetRiPulse(double fd) { fRiPulse=fd;}	
	
	void SetGenMom(double fd) { fMomentum=fd;}
	void SetGenThe(double fd) { fTheta=fd;}
	void SetGenPhi(double fd) { fPhi=fd;}	
	
	double GetTruePosX() { return fXTruePosX;}
	double GetTruePosY() { return fYTruePosY;}
	double GetTruePosZ() { return fZTruePosZ;}

	double GetRecoPosX() { return fXRecoPosX;}
	double GetRecoPosY() { return fYRecoPosY;}
	double GetRecoPosZ() { return fZRecoPosZ;}

	double GetPosXErr() { return fXPosErr;}
	double GetPosYErr() { return fYPosErr;}
	double GetPosZErr() { return fZPosErr;}
	
	double GetLeTime() { return fLeTime;}	
	double GetRiTime() { return fRiTime;}	

	double GetLePulse() { return fLePulse;}	
	double GetRiPulse() { return fRiPulse;}	

	
	double GetGenMom() { return fMomentum;}
	double GetGenThe() { return fTheta;}
	double GetGenPhi() { return fPhi;}	

  bool isIdentical(CmvCluster* cluster);
	void SetUsed(bool fd) {isUsed=fd;};
private:
  micalDetectorParameterDef* paradef;
  MultiSimAnalysisDigi* pAnalysis;

  int clustersize;
  double fXTruePosX;      //Global X-position from Simulation
  double fYTruePosY;      //Global Y-position from Simulation
  double fZTruePosZ;      //Global Z-position from Simulation

  double fXRecoPosX;      //Global X-position from Reconstruction
  double fYRecoPosY;      //Global Y-position from Reconstruction
  double fZRecoPosZ;      //Global Z-position from Reconstruction

  double fLeTime;      // Timing of Left-SiPMs (ns)
  double fRiTime;      // Timing of Right-SiPMs (ns)

  double fLePulse;      // Timing of Left-SiPMs (ns)
  double fRiPulse;      // Timing of Right-SiPMs (ns)
	
  double fXPosErr; //Error in X-position (m)
  double fYPosErr; //Error in Y-position (m)
  double fZPosErr; //Error in Y-position (m)

  double fMomentum; //Momentum of track which is behind this cluster (use only one SiPM)
  double fTheta;  //Theta
  double fPhi;    // Phi

  double PhotonSpeed;
  double layergap;

  int plane;
    int layer;
  int strip;

	bool isUsed;
  
};

#endif
