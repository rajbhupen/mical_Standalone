//

#ifndef InoCal1Hit_h
#define InoCal1Hit_h 1

#include "G4VHit.hh"


#include "G4ThreeVector.hh"

class InoCal1Hit : public G4VHit
{
  public:

      InoCal1Hit();
      ~InoCal1Hit();
      InoCal1Hit(const InoCal1Hit &right);
      const InoCal1Hit& operator=(const InoCal1Hit &right);
      G4int operator==(const InoCal1Hit &right) const;

      // inline void *operator new(size_t);
      // inline void operator delete(void *aHit);

      void Draw();
      void Print();

  private:
      G4double edep;
      G4int    pdgid;  //Particle ID
      G4ThreeVector pos;
      G4ThreeVector Lpos;
      G4double localx;
      G4double localy;
    G4double localz;
      G4ThreeVector mom; //Momentum of track at earliest energy deposite, has meaning only for muon track, not usefull at all for hadronic shower
      G4double toff;
      G4double tofx;
      G4double tofy;
      unsigned long    HitId;
  unsigned long SiPMId;
  public:
      inline void SetpdgId(G4int id)
      { pdgid = id; }
      inline void SetEdep(G4double de)
      { edep = de; }
  
  inline void AddEdep(G4double de, G4ThreeVector poss,G4ThreeVector localposs ) 
      {
	G4cout<<"      inline void AddEdep(G4double de, G4ThreeVector poss,G4ThreeVector localposs )    "<<G4endl;
	G4cout<<pos<<"  "<<poss<<"  "<<Lpos<<"  "<<localposs<<"  "<<edep<<"  "<<de<<G4endl;
	
	 pos=pos*edep;
	 pos+=(de*poss);
	 pos=pos/(edep+de);



	 Lpos=Lpos*edep;
	 Lpos+=(de*localposs);
	 Lpos=Lpos/(edep+de);


	
	edep+=de;


	
      }

  
      inline G4double GetEdep()
      { return edep; }
      inline void SetPos(G4ThreeVector xyz)
      { pos = xyz; }
      inline G4ThreeVector GetPos()
      { return pos; }
      inline void SetMom(G4ThreeVector xyz)
      { mom = xyz; }
      inline G4ThreeVector GetMom()
      { return mom; }
      inline void SetTime(G4double tf)
      { toff = tf; }
      inline G4double GetTime()
      { return toff; }

      inline void SetLocalXPos(G4double xyz)
     { localx = xyz; }
      // inline G4double GetLocalXPos()
      // { return localx; }
       inline G4double GetLocalXPos()
  { return Lpos.x(); }
  
      inline void SetLocalYPos(G4double xyz)
      { localy = xyz; }
      // inline G4double GetLocalYPos()
      // { return localy; }
      inline G4double GetLocalYPos()
  { return Lpos.y(); }
  
  inline void SetLocalZPos(G4double xyz)
      { localz = xyz; }
      // inline G4double GetLocalZPos()
      // { return localz; }
      inline G4double GetLocalZPos()
  { return Lpos.z(); }
  
    inline void SetLocalPos(G4ThreeVector xyz)
      { Lpos = xyz; }
      inline G4ThreeVector GetLocalPos()
 { return Lpos; }
  //      inline void SetTimeX(G4double tf)
  //      { tofx = tf; }
  //      inline G4double GetTimeX()
  //      { return tofx; }
  //      inline void SetTimeY(G4double tf)
  //      { tofy = tf; }
  //     inline G4double GetTimeY()
  //      { return tofy; }
      inline void SetHitId (unsigned long id)
      { HitId = id; }
      inline unsigned long GetHitId()
      { return HitId; }

   inline void SetSiPMId (unsigned long id)
      { SiPMId = id; }
      inline unsigned long GetSiPMId()
      { return SiPMId; }

      inline G4int GetpdgId()
      { return pdgid; }
};

// typedef G4THitsCollection<Inocal1Hit> Inocal1HitsCollection;

// extern G4Allocator<Inocal1Hit> Inocal1HitAllocator;

// inline void* Inocal1Hit::operator new(size_t)
// {
//   void *aHit;
//   aHit = (void *) Inocal1HitAllocator.MallocSingle();
//   return aHit;
// }

// inline void Inocal1Hit::operator delete(void *aHit)
// {
//   Inocal1HitAllocator.FreeSingle((Inocal1Hit*) aHit);
// }

#endif
