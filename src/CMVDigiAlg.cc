#include "CMVDigiAlg.hh"
#include "TRandom.h"
#include "CLHEP/Random/RandGauss.h"

CMVDigiAlg::CMVDigiAlg() {
  cout<<"CMVDigiAlg::CMVDigiAlg() {"<<endl;
  pAnalysis = MultiSimAnalysisDigi::AnPointer;
  paradef = micalDetectorParameterDef::AnPointer;

  for(int op=0; op<3;op++) {
    partopscint[op] = paradef->GetPartopscint(op);
  }  
  AirGapScintTop= paradef->GetAirGapScintTop();
  int jmax;

	

  NoScntStrpTop = paradef->GetNoScntStrpTop();//88
  NoScntStrpSide = paradef->GetNoScntStrpSide();//40
  Sipm_Pedestal = paradef->GetSipm_Pedestal();//100
  Cmv_Threshold = paradef->GetCmv_Threshold();//120
	      
  SetPhotonSpeed(162.0);
  SetCMVadctons(0.1);
  
  inocal1hit_pointer = new InoCal1Hit_Manager();
  SipmHit_pointer = new SipmHit_Manager();
  CmvStrip_pointer = new CmvStrip_Manager();
}

CMVDigiAlg::~CMVDigiAlg() {
  cout<<"CMVDigiAlg::~CMVDigiAlg() {"<<endl;
  if(inocal1hit_pointer) {inocal1hit_pointer->InoCal1Hit_list.clear(); delete inocal1hit_pointer; inocal1hit_pointer=0;}

   
   if (CmvStrip_pointer) {delete CmvStrip_pointer;CmvStrip_pointer->CmvStrip_list.clear(); CmvStrip_pointer=0;}
  
  if (SipmHit_pointer) { SipmHit_pointer->SipmHit_list.clear();delete SipmHit_pointer; SipmHit_pointer=0;}
  cout << "Deleting CMVDigiAlg ..." << endl;
}

void CMVDigiAlg::ReadEvent(int ixt) {

  pAnalysis->inputRootFile->cd();
  pAnalysis->inputEventTree->GetEntry(ixt);
   cout <<"cmv_siminput "<< pAnalysis->cmv_nsimhit<<endl;
  for(unsigned ij=0;ij<pAnalysis->cmv_nsimhit;ij++) {
    InoCal1Hit* newHit = new InoCal1Hit();
      G4ThreeVector mom(pAnalysis->cmv_simpx[ij],pAnalysis->cmv_simpy[ij],pAnalysis->cmv_simpz[ij]);
      //    G4ThreeVector mom;
    //    cout<<pAnalysis->cmv_simmom[ij]<<" "<< pAnalysis->cmv_simthe[ij]<<" "<<pAnalysis->cmv_simphi[ij]<<endl;
    // mom.setMag(pAnalysis->cmv_simmom[ij]);
    // mom.setTheta(pAnalysis->cmv_simthe[ij]);
    // mom.setPhi(pAnalysis->cmv_simphi[ij]);
      
    G4ThreeVector pos(pAnalysis->cmv_simposx[ij],pAnalysis->cmv_simposy[ij],pAnalysis->cmv_simposz[ij]);
    newHit->SetHitId(pAnalysis->cmv_detid[ij]);
    newHit->SetpdgId(pAnalysis->cmv_simpdgid[ij]);
    newHit->SetEdep( pAnalysis->cmv_simenr[ij] );
    newHit->SetTime( pAnalysis->cmv_simtime[ij] );
    newHit->SetPos( pos );
    newHit->SetMom(mom);
    G4ThreeVector localpos(pAnalysis->cmv_simlocx[ij],pAnalysis->cmv_simlocy[ij],pAnalysis->cmv_simlocz[ij]);		
    newHit->SetLocalPos( localpos );
    //  cout<<"ij "<<ij<<" "<<pos<<" "<<pAnalysis->simlocvx[ij]<<" "<<pAnalysis->simlocvy[ij]<<endl;
    inocal1hit_pointer->InoCal1Hit_list.push_back(newHit);
  }
   cout<<"Event reading complete. "<<inocal1hit_pointer->InoCal1Hit_list.size()<<" Total sim hit stored."<<endl;
}
//..//
void CMVDigiAlg::DigitiseSimData() {
  // cout<<"Digitising data..."<<endl;
// inocal1hit_pointer = new InoCal1Hit_Manager();
//   SipmHit_pointer = new SipmHit_Manager();
//   CmvStrip_pointer = new CmvStrip_Manager();

  
 for (unsigned ij=0; ij<inocal1hit_pointer->InoCal1Hit_list.size(); ij++) {
   inocal1hit_pointer-> InoCal1Hit_list[ij]->Print();}
 
 cout<<"inocal1hit_pointer-> "<<inocal1hit_pointer->InoCal1Hit_list.size()<<endl;
 for (unsigned ij=0; ij<inocal1hit_pointer->InoCal1Hit_list.size(); ij++) {

	G4ThreeVector posvec2 =inocal1hit_pointer-> InoCal1Hit_list[ij]->GetPos();

	//	inocal1hit_pointer-> InoCal1Hit_list[ij]->Print();
	cout<<"posvec2: "<<posvec2<<endl;
	unsigned long detid =inocal1hit_pointer-> InoCal1Hit_list[ij]->GetHitId();
		cout<<"Check 1 "<<  detid;// This id is just shifted by 2 bits, sipm no. not added
	CmvStrip* strip = new CmvStrip(); //GMA Memory leakages ??

		        
	strip->SetId(detid);
	//
	 unsigned long  Id = detid;
	 Id>>=9;
				
	 int laynoo = Id%4;

	 Id = detid;
	 Id>>=11;
	 int locno = Id%8;
	 cout<<"Fill:: "<<laynoo<< " "<<locno<< endl;
	 // if(laynoo==0 && locno==2 ){
	 //    pAnalysis->hist33->Fill(posvec2.x());
	 //    //	  pAnalysis->hist11->Fill(posvec2.z());
	 // }
	
	//	cout <<"detid "<<detid<<endl;
	strip->SetpdgId(inocal1hit_pointer->InoCal1Hit_list[ij]->GetpdgId());
	strip->SetXPos(posvec2.x());
	strip->SetYPos(posvec2.y());
	strip->SetZPos(posvec2.z());
	strip->SetXLocPos(inocal1hit_pointer->InoCal1Hit_list[ij]->GetLocalXPos());
	strip->SetYLocPos(inocal1hit_pointer->InoCal1Hit_list[ij]->GetLocalYPos());
	strip->SetZLocPos(inocal1hit_pointer->InoCal1Hit_list[ij]->GetLocalZPos());
	strip->SetTime(inocal1hit_pointer->InoCal1Hit_list[ij]->GetTime());
	strip->SetPulse(inocal1hit_pointer->InoCal1Hit_list[ij]->GetEdep());
	G4ThreeVector momvec2 = inocal1hit_pointer->InoCal1Hit_list[ij]->GetMom();
	strip->SetSimMom(momvec2.mag());
	strip->SetSimThe(momvec2.theta());
	strip->SetSimPhi(momvec2.phi());
	cout<<"cmv: strip:"<<endl;
	strip->Print();
	cout<<endl;
	CmvStrip_pointer->CmvStrip_list.push_back(strip);
	for (int jk=0; jk<4; jk++) {
	  cout<<"Strip to SiPM "<<endl;
	  SipmHit* sipmht = new SipmHit(strip, jk); //GMA Memory leakage ??
	  sipmht->Print();
					
	  int isipmid = sipmht->GetId();
	  cout<<"isipmid "<<isipmid<<endl;
	  int oldid=-1;
	  for (unsigned int kl=0; kl<SipmHit_pointer->SipmHit_list.size(); kl++) {
	    if (isipmid==SipmHit_pointer->SipmHit_list[kl]->GetId()) {
	      cout<<"check Update"<<endl;
	      SipmHit_pointer->SipmHit_list[kl]->Update(sipmht->GetPulse(), sipmht->GetTime());
	      oldid = kl;
	      break;
	    }
	  }
	  if (oldid<0 && sipmht->GetPulse() > 0) {//0.2pC
	    SipmHit_pointer->SipmHit_list.push_back(sipmht);
	  }
	  //	delete sipmht;	
	}
	//	cout <<"ijcmstrip "<< ij<<" "<<inocal1hit_pointer->InoCal1Hit_list[ij]->GetEdep()<<" "<<SipmHit_pointer->SipmHit_list.size()<<endl;
	//delete strip;
 } //  for (unsigned ij=0; ij<inocal1hit_pointer->InoCal1Hit_list.size(); ij++) {
			
 //Noise Addition:  

      /*
      // Addition of noise:
      double noise,tim;
      unsigned int id=0;
      int cmax,bmax;
      
      for(int ia=1; ia<5; ia++){//locno
	id++;
	id<<=2;
				
	cmax = (ia==1) ? NoScntStrpTop : NoScntStrpSide; //88:40
	bmax = (ia==1) ? 4 : 3;
	
	for(int jb =0; jb<bmax; jb++) {//layerno
	  if(jb==0){ id+=0;} else{id++;}
	  id<<=7;
	  
	  for(int kc =0; kc<cmax; kc++){//stripno
	    if(kc==0){id+=0;} else{	id++; }
	    id<<=2;
	    
	    for(int med=0; med<4; med++){//sipmno
	      if(med==0){id+=0;} else{id++;	} 
	      //  cout<<"id " <<id<<"  "<<ia<<"  "<<jb<<"  "<<kc<<"  " <<med<<endl;
	      SipmHit* hit = new SipmHit();
	    
	      noise = pAnalysis->noise_hist[1][2]->GetRandom();
	      tim = 1000*(gRandom->Uniform());
	      //tim = 500*(2*gRandom->Uniform()-1);
	      //	cout<<"noise " <<noise<<" time "<<tim<<" "<<Sipm_Pedestal<<" "<<Sipm_Pedestal+100*noise<<" Getpulse "<<hit->GetPulse()<<endl;
	      hit->SetId(id);
	      
	      hit->SetPulse(max(0.0, Sipm_Pedestal +100*noise)) ;//pedestal = 100 in 0.01pC
	      hit->SetTime(tim);
	      //	cout<<"noisepulse noisetime "<<hit->GetPulse()<<" "<<hit->GetTime()<<endl;
	      int isipmid = hit->GetId();
	      int oldid=-1;
	      for (unsigned int jk=0; jk<SipmHit_pointer->SipmHit_list.size(); jk++) {
		if (isipmid==SipmHit_pointer->SipmHit_list[jk]->GetId()) {
		  
		  cout<<"Adding Noise in the hits: "<<isipmid<<" "<<SipmHit_pointer->SipmHit_list[jk]->GetPulse()<<" "<<SipmHit_pointer->SipmHit_list[jk]->GetTime()<<endl;
		  SipmHit_pointer->SipmHit_list[jk]->Update(hit->GetPulse(), hit->GetTime());
		  cout<<"After Update: "<<SipmHit_pointer->SipmHit_list[jk]->GetPulse()<<" "<<SipmHit_pointer->SipmHit_list[jk]->GetTime()<<endl;
		  oldid = jk;
		  break;
		}
	      }
	      if (oldid<0 && hit->GetPulse() > Cmv_Threshold ) { //120
		cout<<"New hit created only due to Noise: "<<endl;

		
		if(ia==1){

		  hit->SetXPos( PhyVolGlPos[ia-1][jb][0] - 0.5*((2*NoScntStrpTop*partopscint[0])+((NoScntStrpTop+1)*AirGapScintTop)) +(kc+1)*(AirGapScintTop)+(2*kc+1)*partopscint[0]);
		  hit->SetXLocPos( - 0.5*((2*NoScntStrpTop*partopscint[0])+((NoScntStrpTop+1)*AirGapScintTop)) +(kc+1)*(AirGapScintTop)+(2*kc+1)*partopscint[0]);

		  hit->SetYPos(PhyVolGlPos[ia-1][jb][1]);
		  hit->SetYLocPos(0);
		    
		  hit->SetZPos(PhyVolGlPos[ia-1][jb][2]);
	          hit->SetZLocPos(0);
		}//	if(ia==1){



		else	if(ia==2 || ia==3){

		  hit->SetXPos(PhyVolGlPos[ia-1][jb][0]);
        
		  hit->SetXLocPos( - 0.5*((2* NoScntStrpSide*partopscint[0])+((NoScntStrpSide+1)*AirGapScintTop)) +(kc+1)*(AirGapScintTop)+(2*kc+1)*partopscint[0]);
	
		  hit->SetYPos(PhyVolGlPos[ia-1][jb][1]);
		  hit->SetYLocPos(0);

		  hit->SetZPos( PhyVolGlPos[ia-1][jb][2] - 0.5*((2*NoScntStrpSide*partopscint[0])+((NoScntStrpSide+1)*AirGapScintTop)) +(kc+1)*(AirGapScintTop)+(2*kc+1)*partopscint[0]);
		  hit->SetZLocPos(0);


		}//	if(ia==2 || ia==3){


		else	if(ia==4){

		  hit->SetXPos(PhyVolGlPos[ia-1][jb][0]);
		  
		  hit->SetXLocPos(-0.5*((2*NoScntStrpSide*partopscint[0])+((NoScntStrpSide+1)*AirGapScintTop)) +( kc+1)*(AirGapScintTop)+(2*kc+1)*partopscint[0]);


		  hit->SetYLocPos(0);
		  hit->SetYPos(PhyVolGlPos[ia-1][jb][1]);

		  hit->SetZPos( PhyVolGlPos[ia-1][jb][2] -  0.5*((2*NoScntStrpSide*partopscint[0])+((NoScntStrpSide+1)*AirGapScintTop)) +( kc+1)*(AirGapScintTop)+(2*kc+1)*partopscint[0]);
		  hit->SetZLocPos(0);
		  
		}


		  

		
		//	cout<<"oldid=-1  "<<hit->GetPulse()<<" "<<hit->GetTime()<<endl;
        
		SipmHit_pointer->SipmHit_list.push_back(hit);
		hit->Print();

		cout<<hit->GetId()<<" "<<hit->GetStripId()<<" "<<hit->GetPlane()<<" "<<hit->GetLayer()<<" "<<hit->GetSiPM()<<" "<<
		  hit->GetStrip()<<" "<<endl;		
	      }
	      else{ delete hit; } // added to save memory since these hits are not useful
	     
	    }//sipm no
	    id-=3;
	    id>>=2;
	  }//stripno
	  id-=(cmax-1);
	  id>>=7;
	  
	  //cout<<endl;
	}//layerno
	id-=(bmax-1);
	id>>=2;
	//cout<<endl<<endl;
      }//locno
      
              */


  /*
    //delete the entry with pulse(signal + noise) < 12
    cout<<"SipmHit_pointer->SipmHit_list.size() "<<SipmHit_pointer->SipmHit_list.size()<<endl;
    for(int ij=0;ij<SipmHit_pointer->SipmHit_list.size();ij++){
	   
      cout<<SipmHit_pointer->SipmHit_list[ij]->GetId()<<"  "<<SipmHit_pointer->SipmHit_list[ij]->GetPulse()<<endl;
      if(SipmHit_pointer->SipmHit_list[ij]->GetPulse()<12){
	cout<<"/delete the entry with pulse(signal + noise) < 12 "<<endl;
	cout<<SipmHit_pointer->SipmHit_list[ij]->GetId()<<endl;
	SipmHit_pointer->SipmHit_list.erase(SipmHit_pointer->SipmHit_list.begin()+ij);
	       
	ij--;
 
      }
    }

    cout<<"After  SipmHit_pointer->SipmHit_list.size() "<<SipmHit_pointer->SipmHit_list.size()<<endl;
     */


  
  // cout<<"Digitising of data complete."<<endl;


}



void CMVDigiAlg::SaveDigiData() {
  pAnalysis->pRootFile->cd();

  pAnalysis->cmv_ndigihit = SipmHit_pointer->SipmHit_list.size();
      cout<<"  SipmHit_pointer->SipmHit_list.size():  "<<  SipmHit_pointer->SipmHit_list.size()<<endl;
      cout <<"  pAnalysis->cmv_ndigihit "<<  pAnalysis->cmv_ndigihit<<endl;
	    
      if (pAnalysis->cmv_ndigihit >pAnalysis->cmv_ndigihtmx) pAnalysis->cmv_ndigihit =pAnalysis->cmv_ndigihtmx;
      for (unsigned ij=0; ij<SipmHit_pointer->SipmHit_list.size() && ij<pAnalysis->cmv_ndigihit; ij++) {

	cout<<"checkl"<<endl;
	pAnalysis->cmv_digipdgid[ij] =SipmHit_pointer->SipmHit_list[ij]->GetpdgId();

	cout<<"sipmid "<<SipmHit_pointer->SipmHit_list[ij]->GetId()<<endl;
        pAnalysis->cmv_sipmid[ij] =SipmHit_pointer->SipmHit_list[ij]->GetId();
	cout<< pAnalysis->cmv_sipmid[ij]<<endl;
	pAnalysis->cmv_digitimpul[ij] =SipmHit_pointer->SipmHit_list[ij]->GetTimePulse();
	pAnalysis->cmv_digiposx[ij] =SipmHit_pointer->SipmHit_list[ij]->GetXPos();
	pAnalysis->cmv_digiposy[ij] =SipmHit_pointer->SipmHit_list[ij]->GetYPos();
	pAnalysis->cmv_digiposz[ij] =SipmHit_pointer->SipmHit_list[ij]->GetZPos();
	pAnalysis->cmv_digimom[ij] =SipmHit_pointer->SipmHit_list[ij]->GetSimMom();
	pAnalysis->cmv_digithe[ij] =SipmHit_pointer->SipmHit_list[ij]->GetSimThe();
	pAnalysis->cmv_digiphi[ij] =SipmHit_pointer->SipmHit_list[ij]->GetSimPhi();
	pAnalysis->cmv_digilocx[ij] =SipmHit_pointer->SipmHit_list[ij]->GetXLocPos();
	pAnalysis->cmv_digilocy[ij] =SipmHit_pointer->SipmHit_list[ij]->GetYLocPos();
	pAnalysis->cmv_digilocz[ij] =SipmHit_pointer->SipmHit_list[ij]->GetZLocPos();
	
	if (ij >=pAnalysis->cmv_ndigihtmx) break; //redundant
      }


      //
   for(unsigned ij=0;ij<pAnalysis->cmv_ndigihit;ij++) {
      //      unsigned istrp = pAnalysis->stripid[ij];
      SipmHit*  sipmht = new SipmHit(); //VALGRIND
      sipmht->SetId(pAnalysis->cmv_sipmid[ij]);
      sipmht->SetpdgId(pAnalysis->cmv_digipdgid[ij]);
      sipmht->SetTimePulse(pAnalysis->cmv_digitimpul[ij]);
      sipmht->SetXPos(pAnalysis->cmv_digiposx[ij]);
      sipmht->SetYPos(pAnalysis->cmv_digiposy[ij]);
      sipmht->SetZPos(pAnalysis->cmv_digiposz[ij]);
      sipmht->SetSimMom(pAnalysis->cmv_digimom[ij]);
      sipmht->SetSimThe(pAnalysis->cmv_digithe[ij]);
      sipmht->SetSimPhi(pAnalysis->cmv_digiphi[ij]);
      sipmht->SetXLocPos(pAnalysis->cmv_digilocx[ij]);
      sipmht->SetYLocPos(pAnalysis->cmv_digilocy[ij]);
    sipmht->SetZLocPos(pAnalysis->cmv_digilocz[ij]);
      int isipmid = sipmht->GetId();
      int oldid=-1;
      for (unsigned int jk=0; jk<SipmHit_pointer->SipmHit_list.size(); jk++) {
	if (isipmid==SipmHit_pointer->SipmHit_list[jk]->GetId()) {
	  SipmHit_pointer->SipmHit_list[jk]->Update(sipmht->GetPulse(), sipmht->GetTime());
	  oldid = jk;
	  break;
	}
      }
      if (oldid<0 && sipmht->GetPulse() > 0.0001) {
	//	sipmht->Print();
	SipmHit_pointer->SipmHit_list.push_back(sipmht);
      }
      //	delete sipmht;	
    }



  
   //    pAnalysis->pEventTree->Fill();


if(inocal1hit_pointer) {inocal1hit_pointer->InoCal1Hit_list.clear(); delete inocal1hit_pointer; inocal1hit_pointer=0;}

   
   if (CmvStrip_pointer) {delete CmvStrip_pointer;CmvStrip_pointer->CmvStrip_list.clear(); CmvStrip_pointer=0;}
  
  if (SipmHit_pointer) { SipmHit_pointer->SipmHit_list.clear();delete SipmHit_pointer; SipmHit_pointer=0;}
  // cout << "Deleting CMVDigiAlg ..." << endl;


   
}








  
