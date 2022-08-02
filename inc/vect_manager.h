#ifndef VECTMANAGER_H
#define VECTMANAGER_H

#include <vector>
#include "InoCal0Hit.hh"
#include "InoCal1Hit.hh"
#include "InoStrip.h"
#include "InoHit.h"
#include "InoCluster.h"
#include "InoTrack.h"
#include "InoFittedTrack.h"
#include "InoTrackCand.h"
#include "TGeoManager.h"
#include "CmvStrip.h"
#include "SipmHit.h"
#include "CmvHit.h"
#include "CmvCluster.h"
#include "CmvLayExtra.h"
const static int nlmx = 10;
const static int ntmx = 8;

class InoTDCHitx_Manager {
 public:
  InoTDCHitx_Manager();
  ~InoTDCHitx_Manager();
  static InoTDCHitx_Manager* APointer;
  vector<int> xtdctiming[nlmx][ntmx];
};

class InoTDCHity_Manager {
 public:
  InoTDCHity_Manager();
  ~InoTDCHity_Manager();
  static InoTDCHity_Manager* APointer;
  vector<int> ytdctiming[nlmx][ntmx];
};

class InoCal0Hit_Manager {
 public:
  InoCal0Hit_Manager();
  ~InoCal0Hit_Manager();
  static InoCal0Hit_Manager* APointer;
  vector<InoCal0Hit*> InoCal0Hit_list;
};
//RSA
class InoCal1Hit_Manager {
 public:
  InoCal1Hit_Manager();
  ~InoCal1Hit_Manager();
  static InoCal1Hit_Manager* APointer;
  vector<InoCal1Hit*> InoCal1Hit_list;
};

class InoStrip_Manager {
 public:
  InoStrip_Manager();
  ~InoStrip_Manager();
 public:
  
  static InoStrip_Manager* APointer;
  vector<InoStrip*> InoStrip_list;
};

class InoStripX_Manager {
 public:
  InoStripX_Manager();
  ~InoStripX_Manager();
 public:
  
  static InoStripX_Manager* APointer;
  vector<InoStrip*> InoStripX_list;
};


class InoStripY_Manager {
 public:
  InoStripY_Manager();
  ~InoStripY_Manager();
 public:
  
  static InoStripY_Manager* APointer;
  vector<InoStrip*> InoStripY_list;
};

class InoHit_Manager {
 public:
  InoHit_Manager();
  ~InoHit_Manager();
 public:
  
  static InoHit_Manager* APointer;
  vector<InoHit*> InoHit_list;
};

class InoCluster_Manager {
 public:
  InoCluster_Manager();
  ~InoCluster_Manager();
 public:
  
  static InoCluster_Manager* APointer;
  vector<InoCluster*> InoCluster_list;
};

class InoTrack_Manager {
 public:
  InoTrack_Manager();
  ~InoTrack_Manager();
 public:
  
  static InoTrack_Manager* APointer;
  vector<InoTrack*> InoTrack_list;
};

class InoFittedTrack_Manager {
 public:
  InoFittedTrack_Manager();
  ~InoFittedTrack_Manager();
 public:
  
  static InoFittedTrack_Manager* APointer;
  vector<InoFittedTrack*> InoFittedTrack_list;
};


class InoTrackCand_Manager {
 public:
  InoTrackCand_Manager();
  ~InoTrackCand_Manager();
 public:
  
  static InoTrackCand_Manager* APointer;
  vector<InoTrackCand*> InoTrackCand_list;
};

class InoGeometry_Manager {
 public:
  InoGeometry_Manager(G4String geoFiles);
  ~InoGeometry_Manager();
  
 public:
  static InoGeometry_Manager *APointer;
  TGeoManager *icalGeometry;
};

class InoRPCStrip_Manager {
 public:
  InoRPCStrip_Manager();
  ~InoRPCStrip_Manager();
  
 public:
  static InoRPCStrip_Manager *APointer;
  vector<pair<int,int> > InoRPCStrip;
};


//CMVA
class CmvStrip_Manager {
 public:
  CmvStrip_Manager();
  ~CmvStrip_Manager();
 public:
  
  static CmvStrip_Manager* APointer;
  vector<CmvStrip*> CmvStrip_list;
};

class SipmHit_Manager {
 public:
  SipmHit_Manager();
  ~SipmHit_Manager();
 public:
  
  static SipmHit_Manager* APointer;
  vector<SipmHit*> SipmHit_list;
};

class CmvHit_Manager {
 public:
  CmvHit_Manager();
  ~CmvHit_Manager();
 public:
  
  static CmvHit_Manager* APointer;
  vector<CmvHit*> CmvHit_list;
};


class CmvCluster_Manager {
 public:
  CmvCluster_Manager();
  ~CmvCluster_Manager();
 public:
  
  static CmvCluster_Manager* APointer;
  vector<CmvCluster*> CmvCluster_list;
};


class CmvLayExtra_Manager {
 public:
  CmvLayExtra_Manager();
  ~CmvLayExtra_Manager();
 public:
  
  static CmvLayExtra_Manager* APointer;
  vector<CmvLayExtra*> CmvLayExtra_list;
};







/*
class InoMuRange_Manager {
 public:
  InoMuRange_Manager();
  ~InoMuRange_Manager();
  
 public:
  static InoMuRange_Manager* APointer;
};
*/
#endif //VECTMANAGER_H
