#include <cassert>
#include <iostream>
#include "SipmHit.h"
using namespace std;
//______________________________________________________________________

//______________________________________________________________________
SipmHit::SipmHit() {
  //	fCmvStrip = 0;
  fpdgSipm = -100;
  fXPos=-1000.0;
  fYPos=-1000.0;
  fZPos=-1000.0;
  fXLocPos=-1000.0;
  fYLocPos=-1000.0;
  fZLocPos=-1000.0;
  iTimePulse=0;
  
  fSimMom=-1000.0;
  fSimThe=-1000.0;
  fSimPhi=-1000.0;
  isUsed = 0;
  //	gRandom3 = new TRandom3();
}

//______________________________________________________________________
SipmHit::SipmHit(SipmHit* cd) {
  //	fCmvStrip = cd->fCmvStrip;
  fpdgSipm	= cd->fpdgSipm; 
  fSipmId    = cd->fSipmId;    
  fXPos			= cd->fXPos;     
  fYPos			= cd->fYPos;     
  fZPos			= cd->fZPos;     
  fXLocPos	= cd->fXLocPos;  
  fYLocPos	= cd->fYLocPos;  
  fZLocPos	= cd->fZLocPos;  						                 
  iTimePulse	= cd->iTimePulse;     
  
  fSimMom		= cd->fSimMom;   
  fSimThe		= cd->fSimThe;   
  fSimPhi		= cd->fSimPhi;
  isUsed    = cd->isUsed;
}

SipmHit::SipmHit(CmvStrip* str, int Sipm) {
	//	fCmvStrip = str->fCmvStrip;
	
  fpdgSipm	= str->GetpdgId(); 
  fSipmId = str->GetId() + Sipm;
  fXPos			= str->GetXPos();     
  fYPos			= str->GetYPos();     
  fZPos			= str->GetZPos();     
  fXLocPos	= str->GetXLocPos();  
  fYLocPos	= str->GetYLocPos();  
  fZLocPos	= str->GetZLocPos(); 						                 					                 
  fSimMom		= str->GetSimMom();   
  fSimThe		= str->GetSimThe();   
  fSimPhi		= str->GetSimPhi();

  double time = str->GetTime();
  double pulse = str->GetPulse();
  cout<<"fSipmId "<<fSipmId<<endl;

  MultiSimAnalysisDigi* pAnalysis = MultiSimAnalysisDigi::AnPointer;
  micalDetectorParameterDef* paradef = micalDetectorParameterDef::AnPointer;
  

  int  Sipm_Pedestal = paradef->Sipm_Pedestal;
  int iplane = (fSipmId>>11)&0x07;
  
  cout<<"iplane:"<<iplane<<endl;
  double layhalflength = paradef->partopscint[1];//2300;
  double dist;
  //  double  ScntLayShifSide = paradef->GetScntLayShifSide();
  //  double SidePlaneHalfLength = paradef->GetSidePlaneHalfLength();
    if (iplane==4){
   layhalflength = paradef->partopscint[1]+50;//2350 i.e 4.7m;
   //   dist= (Sipm<=1) ? layhalflength+fYLocPos : layhalflength-fYLocPos; //Assume in cm
        cout<<"dist fYLocPos layhalflength "<<iplane<<" "<<dist<<" "<<fYLocPos<<" "<<layhalflength<<endl;
      }
      if (iplane==1){
   layhalflength = paradef->partopscint[1]-50;//2250 i.e 4.5m;
   //   dist= (Sipm<=1) ? layhalflength+fYLocPos : layhalflength-fYLocPos; //Assume in cm
        cout<<"dist fYLocPos layhalflength "<<iplane<<" "<<dist<<" "<<fYLocPos<<" "<<layhalflength<<endl;
      }

           if (iplane==6 || iplane==7){
   layhalflength = 1000;//
 
        cout<<"dist fYLocPos layhalflength "<<iplane<<" "<<dist<<" "<<fYLocPos<<" "<<layhalflength<<endl;
      }

      
    dist= (Sipm<=1) ? layhalflength+fYLocPos : layhalflength-fYLocPos; //Assume in cm
    
    cout<<"dist fXLocPos layhalflength "<<iplane<<" "<<dist<<" "<<fYLocPos<<" "<<layhalflength<<endl;
     
    cout <<"SipmHit::SipmHit time edep dist "<<iplane<<"  " <<time<<" "<<pulse<<" "<<dist<<endl; //pulse is just edep in keV
  
  //	if (Sipm>1 ) { dist = 457.8 - dist;} // may be different for different CMV, change accordingly
  //Exponential loss of signal in fibre
  double lam1 = 883.3;
  double lam2 = 10121.0;
  double frac = 0.96;
  
    double signal =pulse*(lam1*exp(-dist/lam1) + frac*lam2*exp(-dist/lam2)) /(lam1+lam2*frac); // in KeV
  
  //Average number of ohotons will be vary for different scintillator, SiPM
  double lambda = signal* 0.0225; // Assume 1cm scint (2MeV), observed signal in 45 photon in nearer SiPM
  //lambda *=0.1; // GMA Manually reduced it (Too much energy deposit in the scintillator veto detector : Probably multiple insertion of the chamber
  // lambda *=1;//material changed from lead to hydrocarbon
  //Poission fluctuations of observed SiPM
  int npho = gRandom->Poisson(lambda);
  
  //Charge in pC
   double picoC = npho*0.2; 
  //Smeaing of charge
 	cout <<"picoC "<<picoC<<endl;
  picoC += gRandom->Gaus(0, 0.05); // Random noise in the signal
  
  if (picoC<0) { picoC = 0;}
  //Digitisation of signal, assuming least count 0.01pC
    int iPul = picoC*100+ Sipm_Pedestal;//100= pedestal
  // int iPul = pulse;
  //Assuming only 12bit adc
    
   if (iPul > 4095) iPul = 4095;
  
   cout<<"pulse: "<<pulse<<" signal: "<<signal<<" lambda: "<<lambda<<" npho: "<<npho<<" picoC: "<<picoC<<" ipul: "<<iPul<<" "<<endl;
 
  //Time at SiPM with the assumption that velocity inside fibre is 16.2cm/ns
  int sipmtime = time + (dist/pAnalysis->GetPhotonSpeedVal());
   cout <<"SipmHit::SipmHit "<<iPul<<" "<<picoC<<" n"<<npho<<" "<<lambda<<" "<<signal<<" p"<<pulse<<" "<<time<<" "<<sipmtime<<endl;
  //Time resolution, 3ns for to photon and 1ns for 50 photon
          double timereso =0.707* exp(1.151-0.0031*npho);
   //   double  timereso =1;
 
   cout<<"timeresol  "<<timereso<<endl;
	 sipmtime += gRandom->Gaus(0, timereso);
	//// cout<<"sipmtime "<<sipmtime<<endl;
  //TDC least count 100ps
	 int iTime = sipmtime/pAnalysis->GetCMVadctons();
       //       int iTime = sipmtime;
  cout<<"itime "<<iTime<<endl;
  if (iTime <0) { iTime = 0;}
  if (iTime >1048575) { iTime =1048575;} // 20bits for TDC 2^20-1 ==1048575
      
  iTimePulse	= iPul;
  iTimePulse<<=20;
  iTimePulse +=iTime;

  


  
      isUsed = false;

      cout <<"iTimePulse iTime iPul  "<<iTimePulse<<"  " <<iTime<<" "<<iPul<<" "<<endl; //pulse is just edep in keV
      cout<<"iTimePulse to time and pulse: "<< iTimePulse<<" "<<((iTimePulse)&0x0fffff) <<" "<< ((iTimePulse>>20)&0x0fff) <<endl;

      
}

void SipmHit::Update(int edep, int time) {
  int oldenr = (iTimePulse>>20)&0x0fff;
  int oldtime = iTimePulse&0xfffff;
  micalDetectorParameterDef* paradef = micalDetectorParameterDef::AnPointer;
  int  Sipm_Pedestal = paradef->Sipm_Pedestal;
  
  int enr = oldenr+edep-Sipm_Pedestal;//both signal and noise contains pedestal shift 
  cout<<"enr: "<<enr<<" "<<oldenr<<" "<<edep<<endl;
  cout<<"time:"<<time<<" "<<oldtime<<endl;
  if (enr>4097) enr = 4097;
  if (time >oldtime) time=oldtime;
  SetTimePulse((enr<<20)+time);
}

//______________________________________________________________________
SipmHit::~SipmHit() {
	//	delete gRandom3;
}

//______________________________________________________________________
SipmHit *SipmHit::DupHandle() const {
   return (new SipmHit(*this));
}


//______________________________________________________________________
void SipmHit::Trace(const char *c) const {
  std::cout<<"**********Begin SipmHit::Trace(\"" << c << "\")" << std::endl
	   << "Information from SipmHit's CandHandle: " << std::endl;
  //  CandHandle::Trace(c);
  std::cout
    << "**********End SipmHit::Trace(\"" << c << "\")" << std::endl;
}

void SipmHit::Print() {
	cout<< "SipmHit():" 
			<< "Plane "<< std::setw(3)<<  GetPlane()
			<< std::setw(3)<<  GetLayer()
			<< std::setw(5)<<  GetStrip()
			<< std::setw(5)<<  GetSiPM()
      << " X_Loc=" <<std::setw(8)<<  GetXLocPos() 
      << " Y_Loc=" <<std::setw(8)<<  GetYLocPos()
 << " Z_Loc=" <<std::setw(8)<<  GetZLocPos()	  
      << " X_Pos=" <<std::setw(8)<<  GetXPos()
      << " Y_Pos=" <<std::setw(8)<<  GetYPos()
      << " Z_Pos=" <<std::setw(8)<<  GetZPos()
			<< " chg="   <<std::setw(8)<<  GetPulse()
      << " time="  <<std::setw(8)<<  GetTime()
      << endl;
	//  cout<<"......................................................................"<<endl;
}
