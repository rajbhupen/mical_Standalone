
#include "InoCal1Hit.hh"

using namespace std;



InoCal1Hit::InoCal1Hit() {
  pdgid=-25;
  edep = 0;
  toff = 1000000;
  
}

InoCal1Hit::~InoCal1Hit()
{;}

InoCal1Hit::InoCal1Hit(const InoCal1Hit &right)
  : G4VHit(),
   pos(G4ThreeVector())
{
  pdgid  = right.pdgid;
  edep = right.edep;
  pos = right.pos;
  mom = right.mom;
  toff = right.toff;
  HitId = right.HitId;
}

const InoCal1Hit& InoCal1Hit::operator=(const InoCal1Hit &right) {
  pdgid = right.pdgid;
  edep = right.edep;
  pos = right.pos;
  mom = right.mom;
  toff = right.toff;
  HitId = right.HitId;

  return *this;
}

G4int InoCal1Hit::operator==(const InoCal1Hit &right) const {
  return (this==&right) ? 1 : 0;
}

void InoCal1Hit::Draw(){;}

void InoCal1Hit::Print() {
  G4cout<<"hit "<<HitId<<" "<<pos<<endl;
  
  
}


