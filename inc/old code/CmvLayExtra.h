#ifndef CMVLAYEXTRA_H
#define CMVLAYEXTRA_H
//CandStripHandle
#include <iostream>
#include "MultiSimAnalysisDigi.hh"
using namespace std;

class CmvLayExtra
{

public:
  CmvLayExtra();
  CmvLayExtra(CmvLayExtra* cd);
  ~CmvLayExtra();
  CmvLayExtra *DupHandle() const;
  void Print();
  void Trace(const char *c = "") const;
	
  int    GetId() const { return flay;}
  int    GetPlane() const { return (flay>>2)&0x07;};
  int    GetLayer() const { return flay&0x03;};
  
  bool GetUsed() const {return isUsed;};	
  double GetExtXPos() const {return fExtXPos;};
  double GetExtYPos() const {return fExtYPos;};
  double GetExtZPos() const {return fExtZPos;};
    double GetClosDist() const {return fClosDist;};
  
  void SetId(int id) { flay=id;}
  void SetUsed(bool fd) {isUsed=fd;};
  void SetExtXPos(double fd) {fExtXPos=fd;}
  void SetExtYPos(double fd) {fExtYPos=fd;}
  void SetExtZPos(double fd) {fExtZPos=fd;}
 void SetClosDist (double fd){fClosDist = fd;};



  
  int    flay ;   //StripID
 
  double fExtXPos;
  double fExtYPos;
  double fExtZPos;
  double fClosDist;
  bool isUsed;
 
};
#endif                                              // INOSTRIP_H
