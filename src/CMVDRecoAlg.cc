#include "CMVDRecoAlg.hh"

CMVDRecoAlg::CMVDRecoAlg(int isInOut) {
  grecoi = GeneralRecoInfo::GnPointer;
  pAnalysis = MultiSimAnalysisDigi::AnPointer;
  MessengerFile = ParameterMessenger::AnPointer;
  paradef = micalDetectorParameterDef::AnPointer;
   SipmHit_pointer = SipmHit_Manager::APointer; 


  
 CmvStrip_pointer = CmvStrip_Manager::APointer;
  
  //  DetectorType = paradef->GetDetectorType();
  isData = (isInOut==2) ? 1 : 0;

 

 for(int ii=0; ii<3; ii++) {
  partopscint[ii]=paradef->GetPartopscint(ii);
}

	
  for(int op=0; op<3;op++) {
    partopscint[op] = paradef->partopscint[op];
  }
	
  AirGapScintTop= paradef->AirGapScintTop;
	
       
  int jmax;
  for(int i =0;i<7;i++){//<4
   
    jmax =  (i==0)? 4:3;
    // if(i==0){
    //   jmax=4;
    // }
    // else{
    //   jmax=3;
    // }
    for(int j=0;j<jmax;j++){
      for(int k=0;k<3;k++){
	PhyVolGlPos[i][j][k] = paradef->ScintLayGPos[i][j][k];
	// cout<< PhyVolGlPos[i][j][k]<<" ";
      }//k
      // cout<<endl;
    }//j
    // cout<<endl<<endl;
  }//i
	
  NoScntStrpTop = paradef->GetNoScntStrpTop();//88
  NoScntStrpSide = paradef->GetNoScntStrpSide();//40
  NoScntStrpSideSmallay= paradef->GetNoScntStrpSideSmallay();	 //8
  SidePlaneHalfLength = paradef->GetSidePlaneHalfLength();
  SideSmallPlaneHalfLength = paradef->GetSideSmallPlaneHalfLength();
  ScntLayShifSide = paradef->GetScntLayShifSide();

  
}
CMVDRecoAlg::~CMVDRecoAlg() {
  SipmHit_pointer->SipmHit_list.clear();
  if (SipmHit_pointer) {delete SipmHit_pointer; SipmHit_pointer=0;}

  for (unsigned ij=0; ij<CmvHit_pointer->CmvHit_list.size(); ij++) {
    if (CmvHit_pointer->CmvHit_list[ij]) {
      cout <<"ij "<< ij<<" "<<CmvHit_pointer->CmvHit_list.size()<<endl;
      delete CmvHit_pointer->CmvHit_list[ij]; CmvHit_pointer->CmvHit_list[ij]=0;
    }
  }

  CmvHit_pointer->CmvHit_list.clear();
  if (CmvHit_pointer) {delete CmvHit_pointer; CmvHit_pointer=0;}




  for (unsigned ij=0; ij<CmvCluster_pointer->CmvCluster_list.size(); ij++) {
    if (CmvCluster_pointer->CmvCluster_list[ij]) {
      cout <<"ij "<< ij<<" "<<CmvCluster_pointer->CmvCluster_list.size()<<endl;
      delete CmvCluster_pointer->CmvCluster_list[ij]; CmvCluster_pointer->CmvCluster_list[ij]=0;
    }
  }

  CmvCluster_pointer->CmvCluster_list.clear();
  if (CmvCluster_pointer) {delete CmvCluster_pointer; CmvCluster_pointer=0;}
  
  cout<<"check cluster pointer"<<endl;

  
  // for (unsigned ij=0; ij<CmvLayExtra_pointer->CmvLayExtra_list.size(); ij++) {
  //   if (CmvLayExtra_pointer->CmvLayExtra_list[ij]) {
  //       cout <<"ij "<< ij<<" "<<CmvLayExtra_pointer->CmvLayExtra_list.size()<<endl;
  //     delete CmvLayExtra_pointer->CmvLayExtra_list[ij]; CmvLayExtra_pointer->CmvLayExtra_list[ij]=0;
  //   }
  // }

  // CmvLayExtra_pointer->CmvLayExtra_list.clear();
  // if (CmvLayExtra_pointer) {delete CmvLayExtra_pointer; CmvLayExtra_pointer=0;}


   cout << "Deleting CMVDRecoAlg ..." << endl;


   
}



void CMVDRecoAlg::ReadCMVDdata(int evt){
  cout<<"void CMVDRecoAlg::ReadCMVDdata(int dataMC){"<<endl;
  SipmHit_pointer = new SipmHit_Manager();

    pAnalysis->inputRootFile->cd();
   pAnalysis->inputEventTree->GetEntry(evt);// Already did while reading mical data


  if(isData){}
  else{
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
      //
  }// for(unsigned ij=0;ij<pAnalysis->cmv_ndigihit;ij++) {


  
  }

    cout<<SipmHit_pointer<<endl;

}



bool CMVDRecoAlg::LinePlaneInt(double* Line, double* Plane, double* Point){
  cout<<"bool CMVDRecoAlg::LinePlaneInt(double* Line, double* Plane, double* Point){"<<endl;
  //	G4double Dist;
  //	G4double a, b;
  bool ok;
	
  double b = Line[3]*Plane[3] + Line[4]*Plane[4] + Line[5]*Plane[5];
  cout<<Line[3]<<" "<<Plane[3]<<" "<<Line[4]<<" "<<Plane[4]<<" "<<Line[5]<<" "<<Plane[5]<<" "<<b<<endl;
  // ok= (fabs(b) > 1e-10) ? 1 : 0;//
  
  ok= ((1.0*b)> 1e-10) ? 1 : 0;//#we only do intersection with those planes which muon passed first..
  if(ok==1) {
    double a=(Plane[0]-Line[0])*Plane[3] +
      (Plane[1]-Line[1])*Plane[4] +
      (Plane[2]-Line[2])*Plane[5];
    cout<<"a "<<a<<endl;
    G4double Dist = a/b;
   cout<<"dist: "<<Dist<<endl;
    
    Point[0] = Line[0] + Line[3]*Dist;
    Point[1] = Line[1] + Line[4]*Dist;
    Point[2] = Line[2] + Line[5]*Dist;
    cout<<"Point "<<Point[0]<<" "<<Point[1]<<" "<<Point[2]<<endl;
  } else {
    cout<<"Setting Point =-100000 "<< endl;
    Point[0]=-1000000; Point[1]=-1000000; Point[2]=-1000000;
  }
  return ok;
}




void CMVDRecoAlg::CMVD_Extrapolation(){   
      pAnalysis->pRootFile->cd();
  cout<<"......................CMVD straight line extrapolation........................."<<endl;
  CmvLayExtra_pointer = new CmvLayExtra_Manager();
  CmvLayExtra_pointer->CmvLayExtra_list.clear();


  int counter=0;
   

  int ijmax=0;
  cout<<"check ab "<<pAnalysis->ntrkt<<endl;
  for (unsigned jk=0; jk<pAnalysis->ntrkt ; jk++) {
    cout<<"ntrkt "<<pAnalysis->ntrkt<<endl;
  
    //		double    momvx = pAnalysis->momvx[jk];
    //This theta phi represents a track going downward. The dirvector using this theta phi matched with generated direction vector (in PGA after...)
    double theta =pAnalysis->thevx[jk];
    double phi = pAnalysis->phivx[jk];
    double posx = pAnalysis->posxvx[jk]*m; //these were stored in metre and convert in mm unit
    double posy =pAnalysis->posyvx[jk]*m;
    double posz= pAnalysis->poszvx[jk]*mm;//already stored in mm only


    double atimslope = pAnalysis->atimslope[jk];
    double atiminter = pAnalysis->atiminter[jk];


    cout<<"atimslope "<<atimslope<<"atiminter "<<atiminter<<endl;
    cout<<"...jk..... "<<jk<<" "<<posx<<" "<<posy<<" "<<posz<<" "<<theta<<" "<<phi<<endl;
    //err calculation:
    
    double therr = pAnalysis->therr[jk];
    double pherr = pAnalysis->pherr[jk];
    // these are errors i.e. just sigma and are stored in meters..
    double xxerr = pAnalysis->xxerr[jk]*m;
    double yyerr = pAnalysis->yyerr[jk]*m;
    double txerr = pAnalysis->txerr[jk];// x = a + b z , err in b (no units)
    double tyerr = pAnalysis->tyerr[jk];// y = a + b z , err in b (no units)
    cout<<"error in positions and angles  "<<xxerr<<" "<<yyerr<<" "<<txerr<<" "<<tyerr<<" "<<therr<<" "<<pherr<<endl;
    
    double xxtxerr = pAnalysis->xxtxerr[jk]*m;
    double xxtyerr = pAnalysis->xxtyerr[jk]*m;
    double yytyerr = pAnalysis->yytyerr[jk]*m;
    double yytxerr = pAnalysis->yytxerr[jk]*m;
    double txtyerr = pAnalysis->txtyerr[jk];
    cout<<"xxtxerr "<<xxtxerr<<"yytyerr "<<yytyerr<<endl;
    //err cal.

    //    pAnalysis->chisq[jk]=-1.0; pAnalysis->chisq2[jk]=-1.0;
    //    pAnalysis->posxvx[jk]=0.0;  pAnalysis->posyvx[jk]=0.0;  pAnalysis->poszvx[jk]=0.0;
   
    pAnalysis->cmv_lay[jk]=-1;
    pAnalysis->extra_diff1[jk]=pAnalysis->extra_diff2[jk]=pAnalysis->extra_diff3[jk]= 1000000;

    
    pAnalysis->cmv_locno00[jk]=0; pAnalysis->cmv_locno01[jk]=0; pAnalysis->cmv_locno02[jk]=0; pAnalysis->cmv_locno03[jk]=0;
    pAnalysis->cmv_locno10[jk]=0; pAnalysis->cmv_locno11[jk]=0; pAnalysis->cmv_locno12[jk]=0;
    pAnalysis->cmv_locno20[jk]=0; pAnalysis->cmv_locno21[jk]=0; pAnalysis->cmv_locno22[jk]=0;
    pAnalysis->cmv_locno30[jk]=0; pAnalysis->cmv_locno31[jk]=0; pAnalysis->cmv_locno32[jk]=0;    
    pAnalysis->cmv_locno40[jk]=0; pAnalysis->cmv_locno41[jk]=0; pAnalysis->cmv_locno42[jk]=0;
    pAnalysis->cmv_locno50[jk]=0; pAnalysis->cmv_locno51[jk]=0; pAnalysis->cmv_locno52[jk]=0;
    pAnalysis->cmv_locno60[jk]=0; pAnalysis->cmv_locno61[jk]=0; pAnalysis->cmv_locno62[jk]=0;


   
    pAnalysis->distofclosapp[jk]=-1000000;
    pAnalysis->planeedge[jk]=-1000000;
    pAnalysis->cmv_Expposx[jk] = -1000000; pAnalysis->cmv_Expposy[jk] = -1000000; pAnalysis->cmv_Expposz[jk] = -1000000;
    pAnalysis->cmv_DCAposx[jk]; pAnalysis->cmv_DCAposy[jk]; pAnalysis->cmv_DCAposz[jk];
    
    pAnalysis->extrapolatim00[jk]=-1000000; pAnalysis->extrapolatim01[jk]=-1000000; pAnalysis->extrapolatim02[jk]=-1000000; pAnalysis->extrapolatim03[jk]=-1000000;
    pAnalysis->Trig00[jk]=-1000000; pAnalysis->Trig01[jk]=-1000000; pAnalysis->Trig02[jk]=-1000000; pAnalysis->Trig03[jk]=-1000000;
 
    pAnalysis->clustersize00[jk]=100; pAnalysis->clustersize01[jk]=100; pAnalysis->clustersize02[jk]=100; pAnalysis->clustersize03[jk]=100;
    pAnalysis->clustersize10[jk]=100; pAnalysis->clustersize11[jk]=100; pAnalysis->clustersize12[jk]=100;
    pAnalysis->clustersize20[jk]=100; pAnalysis->clustersize21[jk]=100; pAnalysis->clustersize22[jk]=100;
    pAnalysis->clustersize30[jk]=100; pAnalysis->clustersize31[jk]=100; pAnalysis->clustersize32[jk]=100; 
    pAnalysis->clustersize40[jk]=100; pAnalysis->clustersize41[jk]=100; pAnalysis->clustersize42[jk]=100; 
    pAnalysis->clustersize50[jk]=100; pAnalysis->clustersize51[jk]=100; pAnalysis->clustersize52[jk]=100; 
    pAnalysis->clustersize60[jk]=100; pAnalysis->clustersize61[jk]=100; pAnalysis->clustersize62[jk]=100; 
    
    pAnalysis->extrapolposx00[jk]=-1000000; pAnalysis->extrapolposy00[jk]=-1000000; pAnalysis->extrapolposz00[jk]=-1000000; pAnalysis->cmvhitrecoposx00[jk]=-1000000;   pAnalysis->cmvhitrecoposy00[jk]=-1000000; pAnalysis->cmvhitrecoposz00[jk]=-1000000; pAnalysis->cmvhittrueposx00[jk]=-1000000; pAnalysis->cmvhittrueposy00[jk]=-1000000; pAnalysis->cmvhittrueposz00[jk]=-1000000; pAnalysis->cmvhitrecoposxerr00[jk]=-1000000; pAnalysis->cmvhitrecoposyerr00[jk]=-1000000; pAnalysis->cmvhitrecoposzerr00[jk]=-1000000;         pAnalysis->extrapolposxerr00[jk]=-1000000; pAnalysis->extrapolposyerr00[jk]=-1000000; pAnalysis->extrapolposzerr00[jk]=-1000000;      pAnalysis->extrapolposx01[jk]=-1000000; pAnalysis->extrapolposy01[jk]=-1000000; pAnalysis->extrapolposz01[jk]=-1000000;      pAnalysis->cmvhitrecoposx01[jk]=-1000000; pAnalysis->cmvhitrecoposy01[jk]=-1000000; pAnalysis->cmvhitrecoposz01[jk]=-1000000;   pAnalysis->cmvhittrueposx01[jk]=-1000000; pAnalysis->cmvhittrueposy01[jk]=-1000000; pAnalysis->cmvhittrueposz01[jk]=-1000000; pAnalysis->cmvhitrecoposxerr01[jk]=-1000000; pAnalysis->cmvhitrecoposyerr01[jk]=-1000000; pAnalysis->cmvhitrecoposzerr01[jk]=-1000000;         pAnalysis->extrapolposxerr01[jk]=-1000000; pAnalysis->extrapolposyerr01[jk]=-1000000; pAnalysis->extrapolposzerr01[jk]=-1000000;                   pAnalysis->extrapolposx02[jk]=-1000000; pAnalysis->extrapolposy02[jk]=-1000000; pAnalysis->extrapolposz02[jk]=-1000000;      pAnalysis->cmvhitrecoposx02[jk]=-1000000; pAnalysis->cmvhitrecoposy02[jk]=-1000000; pAnalysis->cmvhitrecoposz02[jk]=-1000000; pAnalysis->cmvhittrueposx02[jk]=-1000000; pAnalysis->cmvhittrueposy02[jk]=-1000000; pAnalysis->cmvhittrueposz02[jk]=-1000000; pAnalysis->cmvhitrecoposxerr02[jk]=-1000000; pAnalysis->cmvhitrecoposyerr02[jk]=-1000000; pAnalysis->cmvhitrecoposzerr02[jk]=-1000000; pAnalysis->extrapolposxerr02[jk]=-1000000; pAnalysis->extrapolposyerr02[jk]=-1000000; pAnalysis->extrapolposzerr02[jk]=-1000000; pAnalysis->extrapolposx03[jk]=-1000000; pAnalysis->extrapolposy03[jk]=-1000000; pAnalysis->extrapolposz03[jk]=-1000000; pAnalysis->cmvhitrecoposx03[jk]=-1000000; pAnalysis->cmvhitrecoposy03[jk]=-1000000; pAnalysis->cmvhitrecoposz03[jk]=-1000000; pAnalysis->cmvhittrueposx03[jk]=-1000000; pAnalysis->cmvhittrueposy03[jk]=-1000000; pAnalysis->cmvhittrueposz03[jk]=-1000000; pAnalysis->cmvhitrecoposxerr03[jk]=-1000000; pAnalysis->cmvhitrecoposyerr03[jk]=-1000000; pAnalysis->cmvhitrecoposzerr03[jk]=-1000000;         pAnalysis->extrapolposxerr03[jk]=-1000000; pAnalysis->extrapolposyerr03[jk]=-1000000; pAnalysis->extrapolposzerr03[jk]=-1000000;                 pAnalysis->extrapolposx10[jk]=-1000000; pAnalysis->extrapolposy10[jk]=-1000000; pAnalysis->extrapolposz10[jk]=-1000000;      pAnalysis->cmvhitrecoposx10[jk]=-1000000; pAnalysis->cmvhitrecoposy10[jk]=-1000000; pAnalysis->cmvhitrecoposz10[jk]=-1000000;   pAnalysis->cmvhittrueposx10[jk]=-1000000; pAnalysis->cmvhittrueposy10[jk]=-1000000; pAnalysis->cmvhittrueposz10[jk]=-1000000;      pAnalysis->cmvhitrecoposxerr10[jk]=-1000000; pAnalysis->cmvhitrecoposyerr10[jk]=-1000000; pAnalysis->cmvhitrecoposzerr10[jk]=-1000000;         pAnalysis->extrapolposxerr10[jk]=-1000000; pAnalysis->extrapolposyerr10[jk]=-1000000; pAnalysis->extrapolposzerr10[jk]=-1000000;                pAnalysis->extrapolposx11[jk]=-1000000; pAnalysis->extrapolposy11[jk]=-1000000; pAnalysis->extrapolposz11[jk]=-1000000;      pAnalysis->cmvhitrecoposx11[jk]=-1000000; pAnalysis->cmvhitrecoposy11[jk]=-1000000; pAnalysis->cmvhitrecoposz11[jk]=-1000000;   pAnalysis->cmvhittrueposx11[jk]=-1000000; pAnalysis->cmvhittrueposy11[jk]=-1000000; pAnalysis->cmvhittrueposz11[jk]=-1000000;     pAnalysis->cmvhitrecoposxerr11[jk]=-1000000; pAnalysis->cmvhitrecoposyerr11[jk]=-1000000; pAnalysis->cmvhitrecoposzerr11[jk]=-1000000;         pAnalysis->extrapolposxerr11[jk]=-1000000; pAnalysis->extrapolposyerr11[jk]=-1000000; pAnalysis->extrapolposzerr11[jk]=-1000000;                pAnalysis->extrapolposx12[jk]=-1000000; pAnalysis->extrapolposy12[jk]=-1000000; pAnalysis->extrapolposz12[jk]=-1000000;      pAnalysis->cmvhitrecoposx12[jk]=-1000000; pAnalysis->cmvhitrecoposy12[jk]=-1000000; pAnalysis->cmvhitrecoposz12[jk]=-1000000;   pAnalysis->cmvhittrueposx12[jk]=-1000000; pAnalysis->cmvhittrueposy12[jk]=-1000000; pAnalysis->cmvhittrueposz12[jk]=-1000000;      pAnalysis->cmvhitrecoposxerr12[jk]=-1000000; pAnalysis->cmvhitrecoposyerr12[jk]=-1000000; pAnalysis->cmvhitrecoposzerr12[jk]=-1000000;         pAnalysis->extrapolposxerr12[jk]=-1000000; pAnalysis->extrapolposyerr12[jk]=-1000000; pAnalysis->extrapolposzerr12[jk]=-1000000;
    pAnalysis->extrapolposx20[jk]=-1000000; pAnalysis->extrapolposy20[jk]=-1000000; pAnalysis->extrapolposz20[jk]=-1000000;      pAnalysis->cmvhitrecoposx20[jk]=-1000000; pAnalysis->cmvhitrecoposy20[jk]=-1000000; pAnalysis->cmvhitrecoposz20[jk]=-1000000;   pAnalysis->cmvhittrueposx20[jk]=-1000000; pAnalysis->cmvhittrueposy20[jk]=-1000000; pAnalysis->cmvhittrueposz20[jk]=-1000000;      pAnalysis->cmvhitrecoposxerr20[jk]=-1000000; pAnalysis->cmvhitrecoposyerr20[jk]=-1000000; pAnalysis->cmvhitrecoposzerr20[jk]=-1000000;         pAnalysis->extrapolposxerr20[jk]=-1000000; pAnalysis->extrapolposyerr20[jk]=-1000000; pAnalysis->extrapolposzerr20[jk]=-1000000;



    //   int laymax;
    //   for(int loc = 0;loc<4;loc++){
    //     if(loc==0){laymax==4;}
    //     else{laymax==3;}
    //         for(int lay = 0;lay<laymax;lay++){
    // 	    pAnalysis->TString::Format("extrapolposx%d%d",loc,lay)[jk]=-1000000;
    // 	    pAnalysis->TString::Format("extrapolposy%d%d",loc,lay)[jk]=-1000000;
    // pAnalysis->TString::Format("extrapolposz%d%d",loc,lay)[jk]=-1000000;

    //   pAnalysis->TString::Format("cmvhittrueposx%d%d",loc,lay)[jk]=-1000000;
    // pAnalysis->TString::Format("cmvhittrueposy%d%d",loc,lay)[jk]=-1000000;
    // pAnalysis->TString::Format("cmvhittrueposz%d%d",loc,lay)[jk]=-1000000;


    //   pAnalysis->TString::Format("cmvhitrecoposx%d%d",loc,lay)[jk]=-1000000;
    // pAnalysis->TString::Format("cmvhitrecoposy%d%d",loc,lay)[jk]=-1000000;
    // pAnalysis->TString::Format("cmvhitrecoposz%d%d",loc,lay)[jk]=-1000000;


    //   pAnalysis->TString::Format("cmvhitrecoposxerr%d%d",loc,lay)[jk]=-1000000;
    // pAnalysis->TString::Format("cmvhitrecoposyerr%d%d",loc,lay)[jk]=-1000000;
    // pAnalysis->TString::Format("cmvhitrecoposzerr%d%d",loc,lay)[jk]=-1000000;



    //   pAnalysis->TString::Format("extrapolposxerr%d%d",loc,lay)[jk]=-1000000;
    // pAnalysis->TString::Format("extrapolposyerr%d%d",loc,lay)[jk]=-1000000;
    // pAnalysis->TString::Format("extrapolposzerr%d%d",loc,lay)[jk]=-1000000;



    //   }
    //   }


    pAnalysis->extrapolposx20[jk]=-1000000; pAnalysis->extrapolposy21[jk]=-1000000; pAnalysis->extrapolposz21[jk]=-1000000;      pAnalysis->cmvhitrecoposx21[jk]=-1000000; pAnalysis->cmvhitrecoposy21[jk]=-1000000; pAnalysis->cmvhitrecoposz21[jk]=-1000000;   pAnalysis->cmvhittrueposx21[jk]=-1000000; pAnalysis->cmvhittrueposy21[jk]=-1000000; pAnalysis->cmvhittrueposz21[jk]=-1000000;      pAnalysis->cmvhitrecoposxerr21[jk]=-1000000; pAnalysis->cmvhitrecoposyerr21[jk]=-1000000; pAnalysis->cmvhitrecoposzerr21[jk]=-1000000;         pAnalysis->extrapolposxerr21[jk]=-1000000; pAnalysis->extrapolposyerr21[jk]=-1000000; pAnalysis->extrapolposzerr21[jk]=-1000000;
    
    pAnalysis->extrapolposx22[jk]=-1000000; pAnalysis->extrapolposy22[jk]=-1000000; pAnalysis->extrapolposz22[jk]=-1000000;      pAnalysis->cmvhitrecoposx22[jk]=-1000000; pAnalysis->cmvhitrecoposy22[jk]=-1000000; pAnalysis->cmvhitrecoposz22[jk]=-1000000;   pAnalysis->cmvhittrueposx22[jk]=-1000000; pAnalysis->cmvhittrueposy22[jk]=-1000000; pAnalysis->cmvhittrueposz22[jk]=-1000000;      pAnalysis->cmvhitrecoposxerr22[jk]=-1000000; pAnalysis->cmvhitrecoposyerr22[jk]=-1000000; pAnalysis->cmvhitrecoposzerr22[jk]=-1000000;         pAnalysis->extrapolposxerr22[jk]=-1000000; pAnalysis->extrapolposyerr22[jk]=-1000000; pAnalysis->extrapolposzerr22[jk]=-1000000;          pAnalysis->extrapolposx30[jk]=-1000000; pAnalysis->extrapolposy30[jk]=-1000000; pAnalysis->extrapolposz30[jk]=-1000000;      pAnalysis->cmvhitrecoposx30[jk]=-1000000; pAnalysis->cmvhitrecoposy30[jk]=-1000000; pAnalysis->cmvhitrecoposz30[jk]=-1000000;   pAnalysis->cmvhittrueposx30[jk]=-1000000; pAnalysis->cmvhittrueposy30[jk]=-1000000; pAnalysis->cmvhittrueposz30[jk]=-1000000;      pAnalysis->cmvhitrecoposxerr30[jk]=-1000000; pAnalysis->cmvhitrecoposyerr30[jk]=-1000000; pAnalysis->cmvhitrecoposzerr30[jk]=-1000000;         pAnalysis->extrapolposxerr30[jk]=-1000000; pAnalysis->extrapolposyerr30[jk]=-1000000; pAnalysis->extrapolposzerr30[jk]=-1000000;           pAnalysis->extrapolposx31[jk]=-1000000; pAnalysis->extrapolposy31[jk]=-1000000; pAnalysis->extrapolposz31[jk]=-1000000;      pAnalysis->cmvhitrecoposx31[jk]=-1000000; pAnalysis->cmvhitrecoposy31[jk]=-1000000; pAnalysis->cmvhitrecoposz31[jk]=-1000000;   pAnalysis->cmvhittrueposx31[jk]=-1000000; pAnalysis->cmvhittrueposy31[jk]=-1000000; pAnalysis->cmvhittrueposz31[jk]=-1000000;      pAnalysis->cmvhitrecoposxerr31[jk]=-1000000; pAnalysis->cmvhitrecoposyerr31[jk]=-1000000; pAnalysis->cmvhitrecoposzerr31[jk]=-1000000;         pAnalysis->extrapolposxerr31[jk]=-1000000; pAnalysis->extrapolposyerr31[jk]=-1000000; pAnalysis->extrapolposzerr31[jk]=-1000000;                pAnalysis->extrapolposx32[jk]=-1000000; pAnalysis->extrapolposy32[jk]=-1000000; pAnalysis->extrapolposz32[jk]=-1000000;      pAnalysis->cmvhitrecoposx32[jk]=-1000000; pAnalysis->cmvhitrecoposy32[jk]=-1000000; pAnalysis->cmvhitrecoposz32[jk]=-1000000;   pAnalysis->cmvhittrueposx32[jk]=-1000000; pAnalysis->cmvhittrueposy32[jk]=-1000000; pAnalysis->cmvhittrueposz32[jk]=-1000000;      pAnalysis->cmvhitrecoposxerr32[jk]=-1000000; pAnalysis->cmvhitrecoposyerr32[jk]=-1000000; pAnalysis->cmvhitrecoposzerr32[jk]=-1000000;         pAnalysis->extrapolposxerr32[jk]=-1000000; pAnalysis->extrapolposyerr32[jk]=-1000000; pAnalysis->extrapolposzerr32[jk]=-1000000;  
 


    pAnalysis->extrapolposx40[jk]=-1000000; pAnalysis->extrapolposy40[jk]=-1000000; pAnalysis->extrapolposz40[jk]=-1000000;      pAnalysis->cmvhitrecoposx40[jk]=-1000000; pAnalysis->cmvhitrecoposy40[jk]=-1000000; pAnalysis->cmvhitrecoposz40[jk]=-1000000;   pAnalysis->cmvhittrueposx40[jk]=-1000000; pAnalysis->cmvhittrueposy40[jk]=-1000000; pAnalysis->cmvhittrueposz40[jk]=-1000000;      pAnalysis->cmvhitrecoposxerr40[jk]=-1000000; pAnalysis->cmvhitrecoposyerr40[jk]=-1000000; pAnalysis->cmvhitrecoposzerr40[jk]=-1000000;         pAnalysis->extrapolposxerr40[jk]=-1000000; pAnalysis->extrapolposyerr40[jk]=-1000000; pAnalysis->extrapolposzerr40[jk]=-1000000;           pAnalysis->extrapolposx41[jk]=-1000000; pAnalysis->extrapolposy41[jk]=-1000000; pAnalysis->extrapolposz41[jk]=-1000000;      pAnalysis->cmvhitrecoposx41[jk]=-1000000; pAnalysis->cmvhitrecoposy41[jk]=-1000000; pAnalysis->cmvhitrecoposz41[jk]=-1000000;   pAnalysis->cmvhittrueposx41[jk]=-1000000; pAnalysis->cmvhittrueposy41[jk]=-1000000; pAnalysis->cmvhittrueposz41[jk]=-1000000;      pAnalysis->cmvhitrecoposxerr41[jk]=-1000000; pAnalysis->cmvhitrecoposyerr41[jk]=-1000000; pAnalysis->cmvhitrecoposzerr41[jk]=-1000000;         pAnalysis->extrapolposxerr41[jk]=-1000000; pAnalysis->extrapolposyerr41[jk]=-1000000; pAnalysis->extrapolposzerr41[jk]=-1000000;                pAnalysis->extrapolposx42[jk]=-1000000; pAnalysis->extrapolposy42[jk]=-1000000; pAnalysis->extrapolposz42[jk]=-1000000;      pAnalysis->cmvhitrecoposx42[jk]=-1000000; pAnalysis->cmvhitrecoposy42[jk]=-1000000; pAnalysis->cmvhitrecoposz42[jk]=-1000000;   pAnalysis->cmvhittrueposx42[jk]=-1000000; pAnalysis->cmvhittrueposy42[jk]=-1000000; pAnalysis->cmvhittrueposz42[jk]=-1000000;      pAnalysis->cmvhitrecoposxerr42[jk]=-1000000; pAnalysis->cmvhitrecoposyerr42[jk]=-1000000; pAnalysis->cmvhitrecoposzerr42[jk]=-1000000;         pAnalysis->extrapolposxerr42[jk]=-1000000; pAnalysis->extrapolposyerr42[jk]=-1000000; pAnalysis->extrapolposzerr42[jk]=-1000000;





    pAnalysis->extrapolposx50[jk]=-1000000; pAnalysis->extrapolposy50[jk]=-1000000; pAnalysis->extrapolposz50[jk]=-1000000;      pAnalysis->cmvhitrecoposx50[jk]=-1000000; pAnalysis->cmvhitrecoposy50[jk]=-1000000; pAnalysis->cmvhitrecoposz50[jk]=-1000000;   pAnalysis->cmvhittrueposx50[jk]=-1000000; pAnalysis->cmvhittrueposy50[jk]=-1000000; pAnalysis->cmvhittrueposz50[jk]=-1000000;      pAnalysis->cmvhitrecoposxerr50[jk]=-1000000; pAnalysis->cmvhitrecoposyerr50[jk]=-1000000; pAnalysis->cmvhitrecoposzerr50[jk]=-1000000;         pAnalysis->extrapolposxerr50[jk]=-1000000; pAnalysis->extrapolposyerr50[jk]=-1000000; pAnalysis->extrapolposzerr50[jk]=-1000000;           pAnalysis->extrapolposx51[jk]=-1000000; pAnalysis->extrapolposy51[jk]=-1000000; pAnalysis->extrapolposz51[jk]=-1000000;      pAnalysis->cmvhitrecoposx51[jk]=-1000000; pAnalysis->cmvhitrecoposy51[jk]=-1000000; pAnalysis->cmvhitrecoposz51[jk]=-1000000;   pAnalysis->cmvhittrueposx51[jk]=-1000000; pAnalysis->cmvhittrueposy51[jk]=-1000000; pAnalysis->cmvhittrueposz51[jk]=-1000000;      pAnalysis->cmvhitrecoposxerr51[jk]=-1000000; pAnalysis->cmvhitrecoposyerr51[jk]=-1000000; pAnalysis->cmvhitrecoposzerr51[jk]=-1000000;         pAnalysis->extrapolposxerr51[jk]=-1000000; pAnalysis->extrapolposyerr51[jk]=-1000000; pAnalysis->extrapolposzerr51[jk]=-1000000;                pAnalysis->extrapolposx52[jk]=-1000000; pAnalysis->extrapolposy52[jk]=-1000000; pAnalysis->extrapolposz52[jk]=-1000000;      pAnalysis->cmvhitrecoposx52[jk]=-1000000; pAnalysis->cmvhitrecoposy52[jk]=-1000000; pAnalysis->cmvhitrecoposz52[jk]=-1000000;   pAnalysis->cmvhittrueposx52[jk]=-1000000; pAnalysis->cmvhittrueposy52[jk]=-1000000; pAnalysis->cmvhittrueposz52[jk]=-1000000;      pAnalysis->cmvhitrecoposxerr52[jk]=-1000000; pAnalysis->cmvhitrecoposyerr52[jk]=-1000000; pAnalysis->cmvhitrecoposzerr52[jk]=-1000000;         pAnalysis->extrapolposxerr52[jk]=-1000000; pAnalysis->extrapolposyerr52[jk]=-1000000; pAnalysis->extrapolposzerr52[jk]=-1000000;  


    pAnalysis->extrapolposx60[jk]=-1000000; pAnalysis->extrapolposy60[jk]=-1000000; pAnalysis->extrapolposz60[jk]=-1000000;      pAnalysis->cmvhitrecoposx60[jk]=-1000000; pAnalysis->cmvhitrecoposy60[jk]=-1000000; pAnalysis->cmvhitrecoposz60[jk]=-1000000;   pAnalysis->cmvhittrueposx60[jk]=-1000000; pAnalysis->cmvhittrueposy60[jk]=-1000000; pAnalysis->cmvhittrueposz60[jk]=-1000000;      pAnalysis->cmvhitrecoposxerr60[jk]=-1000000; pAnalysis->cmvhitrecoposyerr60[jk]=-1000000; pAnalysis->cmvhitrecoposzerr60[jk]=-1000000;         pAnalysis->extrapolposxerr60[jk]=-1000000; pAnalysis->extrapolposyerr60[jk]=-1000000; pAnalysis->extrapolposzerr60[jk]=-1000000;           pAnalysis->extrapolposx61[jk]=-1000000; pAnalysis->extrapolposy61[jk]=-1000000; pAnalysis->extrapolposz61[jk]=-1000000;      pAnalysis->cmvhitrecoposx61[jk]=-1000000; pAnalysis->cmvhitrecoposy61[jk]=-1000000; pAnalysis->cmvhitrecoposz61[jk]=-1000000;   pAnalysis->cmvhittrueposx61[jk]=-1000000; pAnalysis->cmvhittrueposy61[jk]=-1000000; pAnalysis->cmvhittrueposz61[jk]=-1000000;      pAnalysis->cmvhitrecoposxerr61[jk]=-1000000; pAnalysis->cmvhitrecoposyerr61[jk]=-1000000; pAnalysis->cmvhitrecoposzerr61[jk]=-1000000;         pAnalysis->extrapolposxerr61[jk]=-1000000; pAnalysis->extrapolposyerr61[jk]=-1000000; pAnalysis->extrapolposzerr61[jk]=-1000000;                pAnalysis->extrapolposx62[jk]=-1000000; pAnalysis->extrapolposy62[jk]=-1000000; pAnalysis->extrapolposz62[jk]=-1000000;      pAnalysis->cmvhitrecoposx62[jk]=-1000000; pAnalysis->cmvhitrecoposy62[jk]=-1000000; pAnalysis->cmvhitrecoposz62[jk]=-1000000;   pAnalysis->cmvhittrueposx62[jk]=-1000000; pAnalysis->cmvhittrueposy62[jk]=-1000000; pAnalysis->cmvhittrueposz62[jk]=-1000000;      pAnalysis->cmvhitrecoposxerr62[jk]=-1000000; pAnalysis->cmvhitrecoposyerr62[jk]=-1000000; pAnalysis->cmvhitrecoposzerr62[jk]=-1000000;         pAnalysis->extrapolposxerr62[jk]=-1000000; pAnalysis->extrapolposyerr62[jk]=-1000000; pAnalysis->extrapolposzerr62[jk]=-1000000;  
    
    pAnalysis->LeTime00[jk] = -1000000;
    pAnalysis->RiTime00[jk] = -1000000;
    pAnalysis->LePulse00[jk] = -1000000;
    pAnalysis->RiPulse00[jk] = -1000000;
     
    pAnalysis->LeTime01[jk] = -1000000;
    pAnalysis->RiTime01[jk] = -1000000;
    pAnalysis->LePulse01[jk] = -1000000;
    pAnalysis->RiPulse01[jk] = -1000000;

    pAnalysis->LeTime02[jk] = -1000000;
    pAnalysis->RiTime02[jk] = -1000000;
    pAnalysis->LePulse02[jk] = -1000000;
    pAnalysis->RiPulse02[jk] = -1000000;

    pAnalysis->LeTime03[jk] = -1000000;
    pAnalysis->RiTime03[jk] = -1000000;
    pAnalysis->LePulse03[jk] = -1000000;
    pAnalysis->RiPulse03[jk] = -1000000;




    pAnalysis->LeTime10[jk] = -1000000;
    pAnalysis->RiTime10[jk] = -1000000;
    pAnalysis->LePulse10[jk] = -1000000;
    pAnalysis->RiPulse10[jk] = -1000000;
     
    pAnalysis->LeTime11[jk] = -1000000;
    pAnalysis->RiTime11[jk] = -1000000;
    pAnalysis->LePulse11[jk] = -1000000;
    pAnalysis->RiPulse11[jk] = -1000000;

    pAnalysis->LeTime12[jk] = -1000000;
    pAnalysis->RiTime12[jk] = -1000000;
    pAnalysis->LePulse12[jk] = -1000000;
    pAnalysis->RiPulse12[jk] = -1000000;


    pAnalysis->LeTime20[jk] = -1000000;
    pAnalysis->RiTime20[jk] = -1000000;
    pAnalysis->LePulse20[jk] = -1000000;
    pAnalysis->RiPulse20[jk] = -1000000;
     
    pAnalysis->LeTime21[jk] = -1000000;
    pAnalysis->RiTime21[jk] = -1000000;
    pAnalysis->LePulse21[jk] = -1000000;
    pAnalysis->RiPulse21[jk] = -1000000;

    pAnalysis->LeTime22[jk] = -1000000;
    pAnalysis->RiTime22[jk] = -1000000;
    pAnalysis->LePulse22[jk] = -1000000;
    pAnalysis->RiPulse22[jk] = -1000000;




    pAnalysis->LeTime30[jk] = -1000000;
    pAnalysis->RiTime30[jk] = -1000000;
    pAnalysis->LePulse30[jk] = -1000000;
    pAnalysis->RiPulse30[jk] = -1000000;
     
    pAnalysis->LeTime31[jk] = -1000000;
    pAnalysis->RiTime31[jk] = -1000000;
    pAnalysis->LePulse31[jk] = -1000000;
    pAnalysis->RiPulse31[jk] = -1000000;

    pAnalysis->LeTime32[jk] = -1000000;
    pAnalysis->RiTime32[jk] = -1000000;
    pAnalysis->LePulse32[jk] = -1000000;
    pAnalysis->RiPulse32[jk] = -1000000;


    pAnalysis->LeTime40[jk] = -1000000;
    pAnalysis->RiTime40[jk] = -1000000;
    pAnalysis->LePulse40[jk] = -1000000;
    pAnalysis->RiPulse40[jk] = -1000000;
     
    pAnalysis->LeTime41[jk] = -1000000;
    pAnalysis->RiTime41[jk] = -1000000;
    pAnalysis->LePulse41[jk] = -1000000;
    pAnalysis->RiPulse41[jk] = -1000000;

    pAnalysis->LeTime42[jk] = -1000000;
    pAnalysis->RiTime42[jk] = -1000000;
    pAnalysis->LePulse42[jk] = -1000000;
    pAnalysis->RiPulse42[jk] = -1000000;


    pAnalysis->LeTime50[jk] = -1000000;
    pAnalysis->RiTime50[jk] = -1000000;
    pAnalysis->LePulse50[jk] = -1000000;
    pAnalysis->RiPulse50[jk] = -1000000;
     
    pAnalysis->LeTime51[jk] = -1000000;
    pAnalysis->RiTime51[jk] = -1000000;
    pAnalysis->LePulse51[jk] = -1000000;
    pAnalysis->RiPulse51[jk] = -1000000;

    pAnalysis->LeTime52[jk] = -1000000;
    pAnalysis->RiTime52[jk] = -1000000;
    pAnalysis->LePulse52[jk] = -1000000;
    pAnalysis->RiPulse52[jk] = -1000000;


    pAnalysis->LeTime60[jk] = -1000000;
    pAnalysis->RiTime60[jk] = -1000000;
    pAnalysis->LePulse60[jk] = -1000000;
    pAnalysis->RiPulse60[jk] = -1000000;
     
    pAnalysis->LeTime61[jk] = -1000000;
    pAnalysis->RiTime61[jk] = -1000000;
    pAnalysis->LePulse61[jk] = -1000000;
    pAnalysis->RiPulse61[jk] = -1000000;

    pAnalysis->LeTime62[jk] = -1000000;
    pAnalysis->RiTime62[jk] = -1000000;
    pAnalysis->LePulse62[jk] = -1000000;
    pAnalysis->RiPulse62[jk] = -1000000;



    cout<<"clustersize initialization: " <<pAnalysis->clustersize00[jk]<<" "<< pAnalysis->cmv_locno00[jk]<<" "<<endl ;
    cout<<pAnalysis->clustersize01[jk]<<" "<< pAnalysis->cmv_locno01[jk]<<" "<<endl;
    cout<<  pAnalysis->clustersize02[jk]<<" "<< pAnalysis->cmv_locno02[jk]<<" "<<endl;
    cout<<  pAnalysis->clustersize03[jk]<<" "<< pAnalysis->cmv_locno03[jk]<<" "<<endl;
    cout<<  pAnalysis->clustersize10[jk]<<" "<< pAnalysis->cmv_locno10[jk]<<" "<<endl;
    cout<<  pAnalysis->clustersize11[jk]<<" "<< pAnalysis->cmv_locno11[jk]<<" "<<endl;
    cout<<pAnalysis->clustersize12[jk]<<" "<< pAnalysis->cmv_locno12[jk]<<" "<<endl;
    cout<<pAnalysis->clustersize20[jk]<<" "<< pAnalysis->cmv_locno20[jk]<<" "<<endl;
    cout<<pAnalysis->clustersize21[jk]<<" "<< pAnalysis->cmv_locno21[jk]<<" "<<endl;
    cout<<  pAnalysis->clustersize22[jk]<<" "<< pAnalysis->cmv_locno22[jk]<<" "<<endl;
    cout<<pAnalysis->clustersize30[jk]<<" "<< pAnalysis->cmv_locno30[jk]<<" "<<endl;
    cout<<  pAnalysis->clustersize31[jk]<<" "<< pAnalysis->cmv_locno31[jk]<<" "<<endl;
    cout<<  pAnalysis->clustersize32[jk]<<" "<< pAnalysis->cmv_locno32[jk]<<" "<<endl;
    cout<<  pAnalysis->clustersize42[jk]<<" "<< pAnalysis->cmv_locno42[jk]<<" "<<endl;
    cout<<  pAnalysis->clustersize52[jk]<<" "<< pAnalysis->cmv_locno52[jk]<<" "<<endl;
    cout<<  pAnalysis->clustersize62[jk]<<" "<< pAnalysis->cmv_locno62[jk]<<" "<<endl;
    //      cout<<  pAanlysis->chisq[jk]<<" "<< pAnalysis->chisq2[jk]<<endl;
    //      cout<<  pAnalysis->posxvx[jk]<<" "<<  pAnalysis->poszvx[jk]<<" "<<  pAnalysis->posyvx[jk] <<endl;



  
    // pAnalysis->ellip_diff0[jk] = -10000000;  
    // pAnalysis->ellip_diff1[jk] = -1000000;  
    // pAnalysis->ellip_diff2[jk] = -1000000;  
    // pAnalysis->ellip_diff3[jk] = -1000000; 
    

    //convert theta and phi to dxdz and dydz
    
    double dxdz = tan(theta)*cos(phi);   
    double dydz = tan(theta)*sin(phi);
    
    cout<< "Downward theta "<< theta<<" phi  "<< phi<< " dxdz "<< dxdz<<" dydz "<<dydz<<endl;


    double PI = acos(-1.0);
	
    //This gives a downward going track
    // G4ThreeVector dirVector1(0,0,1);       
    // dirVector1.setTheta(theta);
    // dirVector1.setPhi(phi);
    //We need to do extrapolation for those cmvd plane which a muon have passed before entering RPC stack. Thus we need to reverse sign of dirvector(parity) thus we add Pi to phi and do theta-pi..


    double dxdz_upward = tan(PI-theta)*cos(phi+PI);   //modified on 10.01.2022
    double dydz_upward = tan(PI-theta)*sin(phi+PI);
    //slope doesnt change on inverting all xyz to negative
    cout<< "Upward  theta "<< PI-theta<<" phi  "<< phi+PI<< " dxdz "<< dxdz_upward<<" dydz "<<dydz_upward<<endl;



    
    G4ThreeVector dirVector(0,0,1);  //     
    dirVector.setTheta(PI-theta);// this works atleast for SL
    dirVector.setPhi(phi+PI);

    //   dirVector.setTheta(theta);
    //  dirVector.setPhi(phi);
    

    cout<<"dirvector "<<dirVector<<" "<<endl;
    //thus this dirvector represents a track going backward towards the vertex/gen point.
    G4double G_Point[3]={0};
    G4double Ptxerr, Ptyerr, Ptzerr =0;
    G4double Line[6]={posx,posy,posz,dirVector.x(),dirVector.y(),dirVector.z()};
    
    cout<<"Line: "<<Line[0]<<" "<<Line[1]<<" "<<Line[2]<<" "<<Line[3]<<" "<<Line[4]<<" "<<Line[5]<<endl;



    // for (unsigned int ij=0; ij<CmvHit_pointer->CmvHit_list.size(); ij++) {

      
    //   CmvHit_pointer->CmvHit_list[ij]->Print();
    // }
    // //...
 

    unsigned int layid = 0;
    double ellip_diff[4][4];
    double erralgstrplen[4][4];
    double  xhat=0,yhat=0,zhat=0; //area unit vector of planes

    double layhalflength;
    for(int ijk=0;ijk<7;ijk++){//loc_no loop 0:Top, 1:Left 2:Right 3:Back 4:Front 5: miniLeft 6:miniRight //4
      if(ijk==4){continue;} //Reserved for Front Wall
      cout<<"locno: "<<ijk<<endl;
      if(ijk==0){ijmax=4;xhat=0;yhat=0;zhat=1;layhalflength = paradef->GetTopPlaneHalfLength();}//top
      else if(ijk==1 ){ijmax=3;xhat=-1;yhat=0;zhat=0; layhalflength = paradef->GetSidePlaneHalfLength(); }//left
      else if(ijk==2){ijmax=3;xhat=1;yhat=0;zhat=0;layhalflength = paradef->GetSidePlaneHalfLength(); }//right
      else if(ijk==3){ijmax=3;xhat=0;yhat=1;zhat=0;layhalflength = paradef->GetSidePlaneHalfLength();}//back
      else if(ijk==4){ijmax=3;xhat=0;yhat=-1;zhat=0;layhalflength = paradef->GetSidePlaneHalfLength();}//Front
      else if(ijk==5){ijmax=3;xhat=-1;yhat=0;zhat=0;layhalflength = paradef->GetSideSmallPlaneHalfLength();}//miniLeft
      else if(ijk==6){ijmax=3;xhat=1;yhat=0;zhat=0;layhalflength = paradef->GetSideSmallPlaneHalfLength();}//miniRight

      for(int ij=0;ij<ijmax;ij++){//layer loop
	double diffmx=1000000;
	cout<<" Layer No. "<<ij<<endl;
	cout<<ijk<<" "<<ij<<endl;
	G4double Plane[6]={PhyVolGlPos[ijk][ij][0], PhyVolGlPos[ijk][ij][1], PhyVolGlPos[ijk][ij][2],xhat,yhat,zhat};

        cout<<"Plane: "<<Plane[0]<<" "<<Plane[1]<<" "<<Plane[2]<<" "<<Plane[3]<<" "<<Plane[4]<<" "<<Plane[5]<<endl;
	

        bool pl2 = LinePlaneInt (Line, Plane, G_Point);

	double layhalfbreadth = paradef->partopscint[1];     //2300 for left right
	if(ijk==3){
	  layhalfbreadth = paradef->partopscint[1]+50;//4.7 m    

	}
	if(ijk==0){
	  layhalfbreadth = paradef->partopscint[1]-50;//4.5 m    

	}

	if(ijk==5 || ijk==6){
	  layhalfbreadth = 1000;

	}



		
	cout<<"Loc_no: "<<ijk<<" layhalflength "<<layhalflength<<"layhalfbreadth  "<<layhalfbreadth<<endl;

        cout<<"Point: "<<G_Point[0]<<" "<<G_Point[1]<<" "<<G_Point[2]<<endl;
	vector <double> edge[4];
    
	if(ijk==0){

	  edge[0] =  {PhyVolGlPos[ijk][ij][0], PhyVolGlPos[ijk][ij][1]+layhalfbreadth, PhyVolGlPos[ijk][ij][2],-1,0,0}; //backside edge
	  edge[1] =	      {PhyVolGlPos[ijk][ij][0]-layhalflength, PhyVolGlPos[ijk][ij][1], PhyVolGlPos[ijk][ij][2],0,-1,0}; //leftside edge
	  edge[2] =	      {PhyVolGlPos[ijk][ij][0], PhyVolGlPos[ijk][ij][1]-layhalfbreadth, PhyVolGlPos[ijk][ij][2],1,0,0}; //frontside edge
	  edge[3] =	    {PhyVolGlPos[ijk][ij][0]+layhalflength, PhyVolGlPos[ijk][ij][1], PhyVolGlPos[ijk][ij][2],0,1,0}; //rightside edge


	}
	      
	if(ijk==1){

	  edge[0]    =  {PhyVolGlPos[ijk][ij][0], PhyVolGlPos[ijk][ij][1], PhyVolGlPos[ijk][ij][2]+layhalflength,0,1,0}; //topside edge
	  edge[1] =	   {PhyVolGlPos[ijk][ij][0], PhyVolGlPos[ijk][ij][1]+layhalfbreadth, PhyVolGlPos[ijk][ij][2],0,0,-1}; //backside edge
	  edge[2] =	   {PhyVolGlPos[ijk][ij][0], PhyVolGlPos[ijk][ij][1], PhyVolGlPos[ijk][ij][2]-layhalflength,0,-1,0}; //bottomside edge
	  edge[3] =	   {PhyVolGlPos[ijk][ij][0], PhyVolGlPos[ijk][ij][1]-layhalfbreadth, PhyVolGlPos[ijk][ij][2],0,0,1} ; //frontside edge
		
		
	}
	      
	if(ijk==2){
		
	  edge[0]  =  {PhyVolGlPos[ijk][ij][0], PhyVolGlPos[ijk][ij][1], PhyVolGlPos[ijk][ij][2]+layhalflength,0,-1,0}; //topside edge
	  edge[1] =     {PhyVolGlPos[ijk][ij][0], PhyVolGlPos[ijk][ij][1]+layhalfbreadth, PhyVolGlPos[ijk][ij][2],0,0,-1}; //backside edge
	  edge[2] =    {PhyVolGlPos[ijk][ij][0], PhyVolGlPos[ijk][ij][1], PhyVolGlPos[ijk][ij][2]-layhalflength,0,1,0}; //bottomside edge
	  edge[3] =   {PhyVolGlPos[ijk][ij][0], PhyVolGlPos[ijk][ij][1]-layhalfbreadth, PhyVolGlPos[ijk][ij][2],0,0,1}; //frontside edge
		
		  
	}
	      
	if(ijk==3){
		
	  edge[0]   =  {PhyVolGlPos[ijk][ij][0], PhyVolGlPos[ijk][ij][1], PhyVolGlPos[ijk][ij][2]+layhalflength,1,0,0}; //topside edge
	  edge[1] =	{PhyVolGlPos[ijk][ij][0]+layhalfbreadth, PhyVolGlPos[ijk][ij][1], PhyVolGlPos[ijk][ij][2],0,0,-1}; //rightside edge
	  edge[2] =	  {PhyVolGlPos[ijk][ij][0], PhyVolGlPos[ijk][ij][1], PhyVolGlPos[ijk][ij][2]-layhalflength,-1,0,0}; //bottomside edge
	  edge[3] =    {PhyVolGlPos[ijk][ij][0]-layhalfbreadth, PhyVolGlPos[ijk][ij][1],PhyVolGlPos[ijk][ij][2],0,0,1}; //leftside edge
		
	       
	}
	//miniLeft
	if(ijk==5){

	  edge[0]    =  {PhyVolGlPos[ijk][ij][0], PhyVolGlPos[ijk][ij][1], PhyVolGlPos[ijk][ij][2]+layhalfbreadth,0,1,0}; //topside edge
	  edge[1] =	   {PhyVolGlPos[ijk][ij][0], PhyVolGlPos[ijk][ij][1]+layhalflength, PhyVolGlPos[ijk][ij][2],0,0,-1}; //backside edge
	  edge[2] =	   {PhyVolGlPos[ijk][ij][0], PhyVolGlPos[ijk][ij][1], PhyVolGlPos[ijk][ij][2]-layhalfbreadth,0,-1,0}; //bottomside edge
	  edge[3] =	   {PhyVolGlPos[ijk][ij][0], PhyVolGlPos[ijk][ij][1]-layhalflength, PhyVolGlPos[ijk][ij][2],0,0,1} ; //frontside edge
		
		
	}
	      
	if(ijk==6){

	  edge[0]  =  {PhyVolGlPos[ijk][ij][0], PhyVolGlPos[ijk][ij][1], PhyVolGlPos[ijk][ij][2]+layhalfbreadth,0,-1,0}; //topside edge
	  edge[1] =     {PhyVolGlPos[ijk][ij][0], PhyVolGlPos[ijk][ij][1]+layhalflength, PhyVolGlPos[ijk][ij][2],0,0,-1}; //backside edge
	  edge[2] =    {PhyVolGlPos[ijk][ij][0], PhyVolGlPos[ijk][ij][1], PhyVolGlPos[ijk][ij][2]-layhalfbreadth,0,1,0}; //bottomside edge
	  edge[3] =   {PhyVolGlPos[ijk][ij][0], PhyVolGlPos[ijk][ij][1]-layhalflength, PhyVolGlPos[ijk][ij][2],0,0,1}; //frontside edge
		
		  
	}
	// for (unsigned int ix=0; ix<CmvStrip_pointer->CmvStrip_list.size(); ix++) {
	//   CmvStrip* strip = CmvStrip_pointer->CmvStrip_list[ix];//#
	//   int plane = strip->GetPlane();
	//   int layer = strip->GetLayer();
	  
	//   cout<<"eventaction time "<<plane<<" "<<layer<<" "<<strip->GetTime()<<endl;
	//   if (plane==2 && layer==2){ // 2,4 && 2
	   
	//     pAnalysis->atim[jk]=strip->GetTime();
	//   }

	// }
	// cout<<  pAnalysis->atim[jk]<<endl;
	//	pAnalysis->hist55->Fill(pAnalysis->atim[jk]);

	//
	
        if(pl2){
	  CmvLayExtra* layexp = new CmvLayExtra(); //GMA Memory leakages ??

	  cout<<"---Intersection with Plane found---"<<endl;
	 
	  bool isInside= false;
	  int delta = 0;// 100cm
	  cout<<"checkkk: "<<G_Point[2]<<" "<<PhyVolGlPos[ijk][ij][2]<<endl;
	    
	  switch (ijk){
	    //  cout<<"checkkk: "<<G_Point[2]<<PhyVolGlPos[ijk][ij][2]<<endl;
          case 0: isInside = ( (G_Point[0])<  (PhyVolGlPos[ijk][ij][0]+layhalflength+delta) &&  (G_Point[0])>  (PhyVolGlPos[ijk][ij][0]-layhalflength-delta) &&  (G_Point[1])<  (PhyVolGlPos[ijk][ij][1]+layhalfbreadth+delta) &&  (G_Point[1])>  (PhyVolGlPos[ijk][ij][1]-layhalfbreadth-delta)    && abs(G_Point[2]-PhyVolGlPos[ijk][ij][2])<1.e-10 );
	    cout<<"case 0"<<endl;
	    //	    cout<<fixed<<G_Point[2]<<" "<<fixed<<PhyVolGlPos[ijk][ij][2]<<endl;
	    cout<<"0"<<PhyVolGlPos[ijk][ij][0]<<" "<<PhyVolGlPos[ijk][ij][1]<<" "<<PhyVolGlPos[ijk][ij][2]<<" "<<layhalflength<<" "<<layhalfbreadth<<endl;
	    
	    cout<<"   (PhyVolGlPos[ijk][ij][1])+layhalfbreadth    "<<    (PhyVolGlPos[ijk][ij][1])+layhalfbreadth    <<endl;
	    cout<<"    (PhyVolGlPos[ijk][ij][1])-layhalfbreadth   "<<     (PhyVolGlPos[ijk][ij][1])-layhalfbreadth   <<endl;
	    cout<<"   (PhyVolGlPos[ijk][ij][0])+layhalflength   "<<    (PhyVolGlPos[ijk][ij][0])+layhalflength   <<endl;
	    cout<<"    (PhyVolGlPos[ijk][ij][0])-layhalflength     "<<     (PhyVolGlPos[ijk][ij][0])-layhalflength   <<endl;

   
	    
            break;
	    
          case 1: isInside = (  (G_Point[1])<  (PhyVolGlPos[ijk][ij][1]+layhalfbreadth+delta) &&  (G_Point[1])>  (PhyVolGlPos[ijk][ij][1]-layhalfbreadth-delta) &&  (G_Point[2])<  (PhyVolGlPos[ijk][ij][2]+layhalflength+delta) &&  (G_Point[2])>  (PhyVolGlPos[ijk][ij][2]-layhalflength-delta)       &&     abs(G_Point[0]-PhyVolGlPos[ijk][ij][0])<1.e-10  );
	    cout<<"case 1"<<endl;
	    
	    cout<<"1"<<PhyVolGlPos[ijk][ij][1]<<" "<<PhyVolGlPos[ijk][ij][2]<<" "<<layhalflength<<" "<<layhalfbreadth<<endl;
	    
	    cout<<"   (PhyVolGlPos[ijk][ij][1])+layhalfbreadth    "<<    (PhyVolGlPos[ijk][ij][1])+layhalfbreadth    <<endl;
	    cout<<"    (PhyVolGlPos[ijk][ij][1])-layhalfbreadth   "<<     (PhyVolGlPos[ijk][ij][1])-layhalfbreadth   <<endl;
	    cout<<"   (PhyVolGlPos[ijk][ij][2])+layhalflength   "<<    (PhyVolGlPos[ijk][ij][2])+layhalflength   <<endl;
	    cout<<"    (PhyVolGlPos[ijk][ij][2])-layhalflength     "<<     (PhyVolGlPos[ijk][ij][2])-layhalflength   <<endl;

   
	    
	    break;
	    
	  case 2: isInside = ( (G_Point[1])<  (PhyVolGlPos[ijk][ij][1]+layhalfbreadth+delta) &&  (G_Point[1])>  (PhyVolGlPos[ijk][ij][1]-layhalfbreadth-delta) &&  (G_Point[2])<  (PhyVolGlPos[ijk][ij][2]+layhalflength+delta) &&  (G_Point[2])>  (PhyVolGlPos[ijk][ij][2]-layhalflength-delta)   &&   abs(G_Point[0] -  PhyVolGlPos[ijk][ij][0])<1.e-10  );
	    cout<<"case 2"<<endl;

	    cout<<"2 "<<PhyVolGlPos[ijk][ij][1]<<" "<<PhyVolGlPos[ijk][ij][2]<<" "<<layhalflength<<" "<<layhalfbreadth<<endl;
	    
	    cout<<"   (PhyVolGlPos[ijk][ij][1])+layhalfbreadth    "<<    (PhyVolGlPos[ijk][ij][1])+layhalfbreadth    <<endl;
	    cout<<"    (PhyVolGlPos[ijk][ij][1])-layhalfbreadth   "<<     (PhyVolGlPos[ijk][ij][1])-layhalfbreadth   <<endl;
	    cout<<"   (PhyVolGlPos[ijk][ij][2])+layhalflength   "<<    (PhyVolGlPos[ijk][ij][2])+layhalflength   <<endl;
	    cout<<"    (PhyVolGlPos[ijk][ij][2])-layhalflength     "<<     (PhyVolGlPos[ijk][ij][2])-layhalflength   <<endl;

	    break;
	    
	  case 3: isInside = ( (G_Point[0])<  (PhyVolGlPos[ijk][ij][0]+layhalfbreadth+delta) &&  (G_Point[0])>  (PhyVolGlPos[ijk][ij][0]-layhalfbreadth-delta) &&  (G_Point[2])<  (PhyVolGlPos[ijk][ij][2]+layhalflength+delta) &&  (G_Point[2])> (PhyVolGlPos[ijk][ij][2]-layhalflength-delta)      &&  abs(G_Point[1] -PhyVolGlPos[ijk][ij][1])<1.e-10 );
	    cout<<"case 3"<<endl;


	    cout<<"3"<<PhyVolGlPos[ijk][ij][0]<<" "<<PhyVolGlPos[ijk][ij][1]<<" "<<PhyVolGlPos[ijk][ij][2]<<" "<<layhalflength<<" "<<layhalfbreadth<<endl;
	    
	    cout<<"   (PhyVolGlPos[ijk][ij][0])+layhalfbreadth    "<<    (PhyVolGlPos[ijk][ij][0])+layhalfbreadth    <<endl;
	    cout<<"    (PhyVolGlPos[ijk][ij][0])-layhalfbreadth   "<<     (PhyVolGlPos[ijk][ij][0])-layhalfbreadth   <<endl;
	    cout<<"   (PhyVolGlPos[ijk][ij][2])+layhalflength   "<<    (PhyVolGlPos[ijk][ij][2])+layhalflength   <<endl;
	    cout<<"    (PhyVolGlPos[ijk][ij][2])-layhalflength     "<<     (PhyVolGlPos[ijk][ij][2])-layhalflength   <<endl;





	    
	    break;



	  case 5: isInside = (  (G_Point[1])<  (PhyVolGlPos[ijk][ij][1]+layhalflength+delta) &&  (G_Point[1])>  (PhyVolGlPos[ijk][ij][1]-layhalflength-delta) &&  (G_Point[2])<  (PhyVolGlPos[ijk][ij][2]+layhalfbreadth+delta) &&  (G_Point[2])>  (PhyVolGlPos[ijk][ij][2]-layhalfbreadth-delta)  &&     abs(G_Point[0]-  PhyVolGlPos[ijk][ij][0])<1.e-10  );
	    cout<<"case 1"<<endl;
	    
	    cout<<"1"<<PhyVolGlPos[ijk][ij][1]<<" "<<PhyVolGlPos[ijk][ij][2]<<" "<<layhalflength<<" "<<layhalfbreadth<<endl;
	    
	    cout<<"   (PhyVolGlPos[ijk][ij][1])+layhalflength    "<<    (PhyVolGlPos[ijk][ij][1])+layhalflength    <<endl;
	    cout<<"    (PhyVolGlPos[ijk][ij][1])-layhalflength   "<<     (PhyVolGlPos[ijk][ij][1])-layhalflength   <<endl;
	    cout<<"   (PhyVolGlPos[ijk][ij][2])+layhalfbreadth   "<<    (PhyVolGlPos[ijk][ij][2])+layhalfbreadth   <<endl;
	    cout<<"    (PhyVolGlPos[ijk][ij][2])-layhalfbreadth     "<<     (PhyVolGlPos[ijk][ij][2])-layhalfbreadth   <<endl;

   
	    
	    break;



	  case 6: isInside = (  (G_Point[1])<  (PhyVolGlPos[ijk][ij][1]+layhalflength+delta) &&  (G_Point[1])>  (PhyVolGlPos[ijk][ij][1]-layhalflength-delta) &&  (G_Point[2])<  (PhyVolGlPos[ijk][ij][2]+layhalfbreadth+delta) &&  (G_Point[2])>  (PhyVolGlPos[ijk][ij][2]-layhalfbreadth-delta)       &&     abs(G_Point[0]-  PhyVolGlPos[ijk][ij][0])<1.e-10  );
	    cout<<"case 1"<<endl;
	    
	    cout<<"1"<<PhyVolGlPos[ijk][ij][1]<<" "<<PhyVolGlPos[ijk][ij][2]<<" "<<layhalflength<<" "<<layhalfbreadth<<endl;
	    
	    cout<<"   (PhyVolGlPos[ijk][ij][1])+layhalflength    "<<    (PhyVolGlPos[ijk][ij][1])+layhalflength    <<endl;
	    cout<<"    (PhyVolGlPos[ijk][ij][1])-layhalflength   "<<     (PhyVolGlPos[ijk][ij][1])-layhalflength   <<endl;
	    cout<<"   (PhyVolGlPos[ijk][ij][2])+layhalfbreadth   "<<    (PhyVolGlPos[ijk][ij][2])+layhalfbreadth   <<endl;
	    cout<<"    (PhyVolGlPos[ijk][ij][2])-layhalfbreadth     "<<     (PhyVolGlPos[ijk][ij][2])-layhalfbreadth   <<endl;

   
	    
	    break;

	    
          default :isInside=false;
	    cout<<"default case"<<endl;
	    break;
          }

	  double trg =0;
	  cout<<"isinside: "<<isInside<<endl;

	
	  bool hitpresent = false;

	    layid = ijk+1;
	    layid<<=2;
	    layid+=ij;

	    cout<<"layid: "<<layid<<endl;
	    layexp->SetId(layid);
	    layid = ijk+1;
	    layid<<=2;
	    layid+=ij;

	    cout<<"layid: "<<layid<<endl;
	    layexp->SetId(layid);

	  
          if(isInside) {

	    cout<<"Extrapolation inside the detector boundary +1m"<<endl;
	    //
	    if(ijk==0 && ij==0){
	      pAnalysis->Trig00[jk] = 1;
	    }

	    else 	  if(ijk==0 && ij==1){
	      pAnalysis->Trig01[jk] = 1;
	    }
		  
	    else 	  if(ijk==0 && ij==2){
	      pAnalysis->Trig02[jk] = 1;
	    }
	    else 	  if(ijk==0 && ij==3){
	      pAnalysis->Trig03[jk] = 1;
	    }

	    //
		  
	    // // time extrapolation
	    // //slope and intersept are calulated in meters and nanosec.
	    // double extrapolatim = (G_Point[2]*0.001*atimslope)+atiminter;
	    // cout<<"extrapolatim"<<atimslope<<" "<<atiminter<<" "<<extrapolatim<<" " << PhyVolGlPos[ijk][ij][2]<<" "<<G_Point[2]     <<endl;
	    // pAnalysis->extrapolatim[jk]=extrapolatim;
	  
	  
	    // pAnalysis->hist44->Fill(extrapolatim);

	    //error calculation
	    if(ijk==0){ //top wall
	      Ptxerr = sqrt((xxerr*xxerr)+pow((G_Point[2]-posz),2)*txerr*txerr+2*(G_Point[2]-posz)*xxtxerr);
	      Ptyerr = sqrt((yyerr*yyerr)+pow((G_Point[2]-posz),2)*tyerr*tyerr+ 2*(G_Point[2]-posz)*yytyerr);
	      Ptzerr = 0;
		    
	    }
	    else if(ijk==3) {//for back wall
	      Ptxerr =sqrt((xxerr*xxerr)+pow((G_Point[1]-posy)*txerr/dydz,2)+ pow(((G_Point[1]-posy)*dxdz*tyerr)/(dydz*dydz),2)+       pow((dxdz*yyerr)/dydz,2)            + 2*((G_Point[1]-posy)*xxtxerr)/dydz-  2*pow((G_Point[1]-posy),2)*dxdz*txtyerr/pow(dydz,3) - 2*(G_Point[1]-posy)*dxdz*xxtyerr/pow(dydz,2)    - 2*(dxdz*xxtyerr)/dydz - 2*   (G_Point[1]-posy)*dxdz*yytxerr/pow(dydz,2) +2*dxdz*dxdz* (G_Point[1]-posy)  *yytyerr/pow(dydz,3)  );
	      Ptyerr =0 ;
	      Ptzerr =sqrt(  pow((G_Point[1]-posy)*tyerr/(dydz*dydz),2) + pow((yyerr/dydz),2) + 2*(G_Point[1]-posy)*yytyerr/pow(dydz,3)   ) ;
	    }
		  
	    else { //for left right walls
	      Ptxerr =0 ;
	      Ptyerr =sqrt((yyerr*yyerr)+pow((G_Point[0]-posx)*tyerr/dxdz,2)+ pow(((G_Point[0]-posx)*dydz*txerr)/(dxdz*dxdz),2)+pow((dydz*xxerr)/dxdz,2)          + 2*((G_Point[0]-posx)*yytyerr)/dxdz- 2*(dydz*xxtyerr)/dxdz -  2*pow((G_Point[0]-posx),2)*dydz*txtyerr/pow(dxdz,3)-2*((G_Point[0]-posx)*dydz*xxtyerr)/(dxdz*dxdz)+2*(G_Point[0]-posx)*dydz*dydz*xxtxerr/pow(dxdz,3) - 2*(G_Point[0]-posx)*dydz*yytxerr/pow(dxdz,2));
	      Ptzerr =sqrt(pow((G_Point[0]-posx)*txerr/(dxdz*dxdz),2) +pow((xxerr/dxdz),2) +2* ((G_Point[0]-posx)*xxtxerr)/pow(dxdz,3)) ;
	    }
	    cout<<"Ptxerr "<<Ptxerr<<" Ptyerr "<<Ptyerr<<"Ptzerr "<<Ptzerr<<endl;

		  

	   
	    cout<<"Point inside the detector boundary"<<endl;
	    
            G4ThreeVector extposvec(G_Point[0], G_Point[1], G_Point[2]);
	    cout<<"Point:"<<extposvec<<endl;
	    G4ThreeVector tmphtvec;
	    G4ThreeVector truehtvec;
	    int clustersize;
	    double ellip_max[4][4];

	    for(int ab=0;ab<4;ab++){
	      for(int cd=0;cd<4;cd++){
		ellip_max[ab][cd]  = 1000000;

	      }
	    }
	    //  double ellip_max1 = 1000000;
	    //  double ellip_max2 = 1000000;
	    //   double  ellip_max3 = 1000000;
	    //cluster----->hit

	    //23032022
	    //   CmvLayExtra* layexp = new CmvLayExtra(); //GMA Memory leakages ??
	    // layid = ijk+1;moved before isinside 28072022
	    // layid<<=2;
	    // layid+=ij;

	    // cout<<"layid: "<<layid<<endl;
	    // layexp->SetId(layid);
	    // layid = ijk+1;
	    // layid<<=2;
	    // layid+=ij;

	    // cout<<"layid: "<<layid<<endl;
	    // layexp->SetId(layid);
	    layexp->SetExtXPos(G_Point[0]);
	    layexp->SetExtYPos(G_Point[1]);
	    layexp->SetExtZPos(G_Point[2]);
	    layexp->SetUsed(true);
	    layexp->Print();
	    //	CmvLayExtra_pointer->CmvLayExtra_list.push_back(layexp);
	    //

	    //200522
	
            for (unsigned int ix=0; ix<CmvCluster_pointer->CmvCluster_list.size(); ix++) {

	      CmvCluster* tmpcluster = CmvCluster_pointer->CmvCluster_list[ix];    //#
	      //  tmpcluster->Print();
	      cout<<"## "<<tmpcluster->GetPlane()-1<<" "<<tmpcluster->GetLayer()<<endl;
	      if (tmpcluster->GetPlane()-1==ijk && tmpcluster->GetLayer()==ij) {//#plane  we have stored from 1 and in loop it is from 0
	        
		//	clustersize = tmpcluster->GetClustersize();

		tmpcluster->Print();


	     	
		cout<<"tmpcluster inside loop "<<tmpcluster->GetPlane()<<" "<<tmpcluster->GetLayer()<<endl;
		tmphtvec.setX(tmpcluster->GetRecoPosX());
		tmphtvec.setY(tmpcluster->GetRecoPosY());
		tmphtvec.setZ(tmpcluster->GetRecoPosZ());

		cout<<"recohtvec: "<<tmphtvec<<endl;
        	double difx = (extposvec-tmphtvec).mag();

		truehtvec.setX(tmpcluster->GetTruePosX());
		truehtvec.setY(tmpcluster->GetTruePosY());
		truehtvec.setZ(tmpcluster->GetTruePosZ());
		cout<<" truehtvvec: "<<truehtvec<<endl;
		//Store the differences in three

		//	double difx = (extposvec-truehtvec).mag();

		
	       
		/*
		//ellip_diff	

		  
		// bool largerr = true;
		bool largerr = false;

		   
		//  double ellip_diff = pow( (G_Point[0]-tmphtvec.x()),2)/(Ptxerr*Ptxerr+tmpcluster->GetPosXErr()*tmpcluster->GetPosXErr()) + pow( (G_Point[1]-tmphtvec.y()),2)/(Ptyerr*Ptyerr+tmpcluster->GetPosYErr()*tmpcluster->GetPosYErr());



		if(ijk==0){
		ellip_diff[ijk][ij] = pow( (G_Point[0]-tmphtvec.x()),2)/(Ptxerr*Ptxerr+tmpcluster->GetPosXErr()*tmpcluster->GetPosXErr());


		erralgstrplen[ijk][ij] =  pow( (G_Point[1]-tmphtvec.y()),2)/(Ptyerr*Ptyerr+tmpcluster->GetPosYErr()*tmpcluster->GetPosYErr());

		}
		else{

		ellip_diff[ijk][ij] = pow( (G_Point[2]-tmphtvec.z()),2)/(Ptzerr*Ptzerr+tmpcluster->GetPosZErr()*tmpcluster->GetPosZErr());

		if(ijk==2 || ijk==3){
		erralgstrplen[ijk][ij] =  pow( (G_Point[0]-tmphtvec.x()),2)/(Ptxerr*Ptxerr+tmpcluster->GetPosXErr()*tmpcluster->GetPosXErr());

		}
		else
		erralgstrplen[ijk][ij] =  pow( (G_Point[1]-tmphtvec.y()),2)/(Ptyerr*Ptyerr+tmpcluster->GetPosYErr()*tmpcluster->GetPosYErr());

		}



		 

		if(largerr==true){ellip_diff[ijk][ij]+=erralgstrplen[ijk][ij];}
		cout<<"ellip_diff "<<ijk<<" "<<ij<<" "<<ellip_diff[ijk][ij]<<" "<<ellip_max[ijk][ij]<< endl;
		cout<<"layer ellip: "<<ij<<endl;
			
		if( ellip_diff[ijk][ij]<ellip_max[ijk][ij]){
		cout<<ellip_max[ijk][ij]<<endl;
		ellip_max[ijk][ij] = ellip_diff[ijk][ij];
		cout<<ellip_max[ijk][ij]<<endl;
		}
			
		// else if(ij == 1 && ellip_diff<ellip_max1){
		//   cout<<ellip_max1<<endl;
		//   ellip_max1 = ellip_diff;
		//   cout<<ellip_max1<<endl;

		// }
			
		// else if(ij == 2 && ellip_diff<ellip_max2){
		//   cout<<ellip_max2<<endl;
		//   ellip_max2 = ellip_diff;
		//   cout<<ellip_max2<<endl;
		// }
			
			
		// else if(ij == 3 && ellip_diff<ellip_max3){
		//   cout<<ellip_max3<<endl;
		//   ellip_max3 = ellip_diff;
		//   cout<<ellip_max3<<endl;
		// }


		// pAnalysis->hist_ellip0 ->Fill(ellip_max0);
		// pAnalysis->hist_ellip1 ->Fill(ellip_max1);
		// pAnalysis->hist_ellip2 ->Fill(ellip_max2);
		// pAnalysis->hist_ellip3 ->Fill(ellip_max3);
		cout<<"elliii: "<<ellip_max[ijk][ij]<<endl;
	
		// if(ij==0){	pAnalysis->ellip_diff0[jk] = ellip_max[ijk][ij];}
		// else if (ij==1){
		//   pAnalysis->ellip_diff1[jk] = ellip_max[ijk][ij];}
		// else if(ij==2){
		//   pAnalysis->ellip_diff2[jk] = ellip_max[ijk][ij]; } 
		// else {
		//   pAnalysis->ellip_diff3[jk] = ellip_max[ijk][ij];  
		// }


		// cout<<"	pAnalysis->ellip_diff0[jk]"<<	pAnalysis->ellip_diff0[jk]<<endl;
		// cout<<"	pAnalysis->ellip_diff1[jk]"<<	pAnalysis->ellip_diff1[jk]<<endl;
		// cout<<"	pAnalysis->ellip_diff2[jk]"<<	pAnalysis->ellip_diff2[jk]<<endl;
		// cout<<"	pAnalysis->ellip_diff3[jk]"<<	pAnalysis->ellip_diff3[jk]<<endl;


		
		//	}//	if(ijk==0){

		//

		*/
		
		// if (difx < pAnalysis->extra_diff1[jk]) {
		//   pAnalysis->extra_diff3[jk] = pAnalysis->extra_diff2[jk];
		//   pAnalysis->extra_diff2[jk] = pAnalysis->extra_diff1[jk];
		//   pAnalysis->extra_diff1[jk] = difx;
		// } else if (difx < pAnalysis->extra_diff2[jk]) {
		//   pAnalysis->extra_diff3[jk] = pAnalysis->extra_diff2[jk];
		//   pAnalysis->extra_diff2[jk] = difx;
		// } else if (difx < pAnalysis->extra_diff3[jk]) {
		//   pAnalysis->extra_diff3[jk] = difx;
		// }
								
		//	cout<<"check3"<<endl;
		if (difx < diffmx) {


		  pAnalysis->cmv_lay[jk]=ij;
		  cout<<"Loc_no "<<ijk<<" layer no: "<<ij<<endl;
		
		  pAnalysis->cmv_stripno[jk]=tmpcluster->GetStrip();


		  diffmx = difx;
		  cout<<" Diff "<<difx<<endl;
                
	
     		

		  cout<<" Extrapolated position "<<G_Point[0]<<" "<<G_Point[1]<<" "<<G_Point[2]<<endl;
		  cout<<" Reconstructed position "<<tmphtvec.x()<<" "<<tmphtvec.y()<<" "<<tmphtvec.z()<<endl;
		  cout<<" True Position "<<truehtvec.x()<<" "<<truehtvec.x()<<" "<<truehtvec.y()<<" "<<truehtvec.z()<<endl;

		  cout<<"Reco-Extrap: "<<  tmphtvec.x()-G_Point[0]<<" "<<  tmphtvec.y()-G_Point[1]<<" "    <<tmphtvec.z()-G_Point[2]<<endl;
		  cout<<"True-Extra: "<<truehtvec.x()-G_Point[0]<<" " << truehtvec.y()-G_Point[1]<<" " << truehtvec.z()-G_Point[2] <<endl;
    

		    
        
		  // time extrapolation
		  //slope and intersept are calulated in meters and nanosec.
		  
		  //		  double extrapolatim;
		  //		  extrapolatim = (G_Point[2]*0.001*dZdT)+ztinter;
		  //		  cout<<"extrapolatim"<<atimslope<<" "<<atiminter<<" "<<extrapolatim<<" " << PhyVolGlPos[ijk][ij][2]<<" "<<G_Point[2]     <<endl;
		  //		    if(extrapolatim<0) extrapolatim=0;
		  //		  cout<<"extrapolatim"<<atimslope<<" "<<atiminter<<" "<<extrapolatim<<" " << PhyVolGlPos[ijk][ij][2]<<" "<<G_Point[2]     <<endl;

		  //13 differernt branches to store the data for all 13 walls of cmvd
		  //  if(abs(truehtvec.x()-G_Point[0])<100 && abs(truehtvec.y()-G_Point[1])<100 ){
		  //		  if(abs(tmphtvec.x()-G_Point[0])<100 /* && abs(tmphtvec.y()-G_Point[1])<100*/ ){
		    
		  if(ijk==0 && ij==0){

		 
		    //		    pAnalysis->extrapolatim00[jk]=extrapolatim;
	  
		    pAnalysis->cmv_locno00[jk]=1;//locno starts from 1
		      
		    pAnalysis->clustersize00[jk]=tmpcluster->GetClustersize();
		    pAnalysis->extrapolposx00[jk]=G_Point[0];
		    pAnalysis->extrapolposy00[jk]=G_Point[1];
		    pAnalysis->extrapolposz00[jk]=G_Point[2];
		  
		    pAnalysis->cmvhitrecoposx00[jk]=tmphtvec.x();
		    pAnalysis->cmvhitrecoposy00[jk]=tmphtvec.y();
		    pAnalysis->cmvhitrecoposz00[jk]=tmphtvec.z();


		    pAnalysis->cmvhittrueposx00[jk]=truehtvec.x();
		    pAnalysis->cmvhittrueposy00[jk]=truehtvec.y();
		    pAnalysis->cmvhittrueposz00[jk]=truehtvec.z();
		  
		    pAnalysis->cmvhitrecoposxerr00[jk]=tmpcluster->GetPosXErr();
		    pAnalysis->cmvhitrecoposyerr00[jk]=tmpcluster->GetPosYErr();
		    pAnalysis->cmvhitrecoposzerr00[jk]=tmpcluster->GetPosZErr();

	
		  
		    pAnalysis->extrapolposxerr00[jk]=Ptxerr;
		    pAnalysis->extrapolposyerr00[jk]=Ptyerr;
		    pAnalysis->extrapolposzerr00[jk]=Ptzerr;



		    pAnalysis->LeTime00[jk] = tmpcluster->GetLeTime();
		    pAnalysis->RiTime00[jk] = tmpcluster->GetRiTime();

		    pAnalysis->LePulse00[jk] = tmpcluster->GetLePulse();
		    pAnalysis->RiPulse00[jk] = tmpcluster->GetRiPulse();
		   
 
		    pAnalysis->ellip_diff00[jk]= ellip_max[ijk][ij];
		  
		  }


		  else	  if(ijk==0 && ij==1){
		    pAnalysis->cmv_locno01[jk]=1;//locno starts from 1
		    
		    pAnalysis->clustersize01[jk]=tmpcluster->GetClustersize();

		    //		    pAnalysis->extrapolatim01[jk]=extrapolatim;
		    
		    pAnalysis->extrapolposx01[jk]=G_Point[0];
		    pAnalysis->extrapolposy01[jk]=G_Point[1];
		    pAnalysis->extrapolposz01[jk]=G_Point[2];
		  
		    pAnalysis->cmvhitrecoposx01[jk]=tmphtvec.x();
		    pAnalysis->cmvhitrecoposy01[jk]=tmphtvec.y();
		    pAnalysis->cmvhitrecoposz01[jk]=tmphtvec.z();


		    pAnalysis->cmvhittrueposx01[jk]=truehtvec.x();
		    pAnalysis->cmvhittrueposy01[jk]=truehtvec.y();
		    pAnalysis->cmvhittrueposz01[jk]=truehtvec.z();
		  
		    pAnalysis->cmvhitrecoposxerr01[jk]=tmpcluster->GetPosXErr();
		    pAnalysis->cmvhitrecoposyerr01[jk]=tmpcluster->GetPosYErr();
		    pAnalysis->cmvhitrecoposzerr01[jk]=tmpcluster->GetPosZErr();

	
		  
		    pAnalysis->extrapolposxerr01[jk]=Ptxerr;
		    pAnalysis->extrapolposyerr01[jk]=Ptyerr;
		    pAnalysis->extrapolposzerr01[jk]=Ptzerr;


		    pAnalysis->ellip_diff01[jk]= ellip_max[ijk][ij];


		    pAnalysis->LeTime01[jk] = tmpcluster->GetLeTime();
		    pAnalysis->RiTime01[jk] = tmpcluster->GetRiTime();

		    pAnalysis->LePulse01[jk] = tmpcluster->GetLePulse();
		    pAnalysis->RiPulse01[jk] = tmpcluster->GetRiPulse();
		    
		    
		  }



		  else	  if(ijk==0 && ij==2){


		    //		    pAnalysis->extrapolatim02[jk]=extrapolatim;
		    pAnalysis->cmv_locno02[jk]=1;//locno starts from 1
		    pAnalysis->clustersize02[jk]=tmpcluster->GetClustersize();
		    pAnalysis->extrapolposx02[jk]=G_Point[0];
		    pAnalysis->extrapolposy02[jk]=G_Point[1];
		    pAnalysis->extrapolposz02[jk]=G_Point[2];
		  
		    pAnalysis->cmvhitrecoposx02[jk]=tmphtvec.x();
		    pAnalysis->cmvhitrecoposy02[jk]=tmphtvec.y();
		    pAnalysis->cmvhitrecoposz02[jk]=tmphtvec.z();


		    pAnalysis->cmvhittrueposx02[jk]=truehtvec.x();
		    pAnalysis->cmvhittrueposy02[jk]=truehtvec.y();
		    pAnalysis->cmvhittrueposz02[jk]=truehtvec.z();
		  
		    pAnalysis->cmvhitrecoposxerr02[jk]=tmpcluster->GetPosXErr();
		    pAnalysis->cmvhitrecoposyerr02[jk]=tmpcluster->GetPosYErr();
		    pAnalysis->cmvhitrecoposzerr02[jk]=tmpcluster->GetPosZErr();

	
		  
		    pAnalysis->extrapolposxerr02[jk]=Ptxerr;
		    pAnalysis->extrapolposyerr02[jk]=Ptyerr;
		    pAnalysis->extrapolposzerr02[jk]=Ptzerr;


		    pAnalysis->ellip_diff02[jk]= ellip_max[ijk][ij];



		    pAnalysis->LeTime02[jk] = tmpcluster->GetLeTime();
		    pAnalysis->RiTime02[jk] = tmpcluster->GetRiTime();

		    pAnalysis->LePulse02[jk] = tmpcluster->GetLePulse();
		    pAnalysis->RiPulse02[jk] = tmpcluster->GetRiPulse();
		    


		    
		  }


		  else	  if(ijk==0 && ij==3){


		    pAnalysis->cmv_locno03[jk]=1;//locno starts from 1

		    pAnalysis->clustersize03[jk]=tmpcluster->GetClustersize();
		    //		    pAnalysis->extrapolatim03[jk]=extrapolatim;
		    pAnalysis->extrapolposx03[jk]=G_Point[0];
		    pAnalysis->extrapolposy03[jk]=G_Point[1];
		    pAnalysis->extrapolposz03[jk]=G_Point[2];
		  
		    pAnalysis->cmvhitrecoposx03[jk]=tmphtvec.x();
		    pAnalysis->cmvhitrecoposy03[jk]=tmphtvec.y();
		    pAnalysis->cmvhitrecoposz03[jk]=tmphtvec.z();


		    pAnalysis->cmvhittrueposx03[jk]=truehtvec.x();
		    pAnalysis->cmvhittrueposy03[jk]=truehtvec.y();
		    pAnalysis->cmvhittrueposz03[jk]=truehtvec.z();
		  
		    pAnalysis->cmvhitrecoposxerr03[jk]=tmpcluster->GetPosXErr();
		    pAnalysis->cmvhitrecoposyerr03[jk]=tmpcluster->GetPosYErr();
		    pAnalysis->cmvhitrecoposzerr03[jk]=tmpcluster->GetPosZErr();

	
		  
		    pAnalysis->extrapolposxerr03[jk]=Ptxerr;
		    pAnalysis->extrapolposyerr03[jk]=Ptyerr;
		    pAnalysis->extrapolposzerr03[jk]=Ptzerr;


		    pAnalysis->ellip_diff03[jk]= ellip_max[ijk][ij];



		    pAnalysis->LeTime03[jk] = tmpcluster->GetLeTime();
		    pAnalysis->RiTime03[jk] = tmpcluster->GetRiTime();

		    pAnalysis->LePulse03[jk] = tmpcluster->GetLePulse();
		    pAnalysis->RiPulse03[jk] = tmpcluster->GetRiPulse();
		    





		    
		  }


		  

        
        	  if(ijk==1 && ij==0){

		    pAnalysis->cmv_locno10[jk]=1;//locno starts from 1
		    pAnalysis->clustersize10[jk]=tmpcluster->GetClustersize();
       
	    
		    pAnalysis->cmvhittrueposx10[jk]=tmpcluster->GetTruePosX() ;
		    pAnalysis->cmvhittrueposy10[jk]=truehtvec.y();
		    pAnalysis->cmvhittrueposz10[jk]=   tmpcluster->GetTruePosZ()       ;

		    //  pAnalysis->debug[jk]= tmpcluster->GetTruePosX();

		    
		    pAnalysis->extrapolposx10[jk]=G_Point[0];
		    pAnalysis->extrapolposy10[jk]=G_Point[1];
		    pAnalysis->extrapolposz10[jk]=G_Point[2];
		  
		    pAnalysis->cmvhitrecoposx10[jk]=tmphtvec.x();
		    pAnalysis->cmvhitrecoposy10[jk]=tmphtvec.y();
		    pAnalysis->cmvhitrecoposz10[jk]=tmphtvec.z();
		    
	   
		    pAnalysis->cmvhitrecoposxerr10[jk]=tmpcluster->GetPosXErr();
		    pAnalysis->cmvhitrecoposyerr10[jk]=tmpcluster->GetPosYErr();
		    pAnalysis->cmvhitrecoposzerr10[jk]=tmpcluster->GetPosZErr();

	
		  
		    pAnalysis->extrapolposxerr10[jk]=Ptxerr;
		    pAnalysis->extrapolposyerr10[jk]=Ptyerr;
		    pAnalysis->extrapolposzerr10[jk]=Ptzerr;

		    pAnalysis->ellip_diff10[jk]= ellip_max[ijk][ij];



		    pAnalysis->LeTime10[jk] = tmpcluster->GetLeTime();
		    pAnalysis->RiTime10[jk] = tmpcluster->GetRiTime();

		    pAnalysis->LePulse10[jk] = tmpcluster->GetLePulse();
		    pAnalysis->RiPulse10[jk] = tmpcluster->GetRiPulse();





		    
		  }


		  if(ijk==1 && ij==1){

		    pAnalysis->cmv_locno11[jk]=1;//locno starts from 1
		    pAnalysis->clustersize11[jk]=tmpcluster->GetClustersize();
			    
		    pAnalysis->extrapolposx11[jk]=G_Point[0];
		    pAnalysis->extrapolposy11[jk]=G_Point[1];
		    pAnalysis->extrapolposz11[jk]=G_Point[2];
		  
		    pAnalysis->cmvhitrecoposx11[jk]=tmphtvec.x();
		    pAnalysis->cmvhitrecoposy11[jk]=tmphtvec.y();
		    pAnalysis->cmvhitrecoposz11[jk]=tmphtvec.z();


		    pAnalysis->cmvhittrueposx11[jk]=truehtvec.x();
		    pAnalysis->cmvhittrueposy11[jk]=truehtvec.y();
		    pAnalysis->cmvhittrueposz11[jk]=truehtvec.z();
		  
		    pAnalysis->cmvhitrecoposxerr11[jk]=tmpcluster->GetPosXErr();
		    pAnalysis->cmvhitrecoposyerr11[jk]=tmpcluster->GetPosYErr();
		    pAnalysis->cmvhitrecoposzerr11[jk]=tmpcluster->GetPosZErr();

	
		  
		    pAnalysis->extrapolposxerr11[jk]=Ptxerr;
		    pAnalysis->extrapolposyerr11[jk]=Ptyerr;
		    pAnalysis->extrapolposzerr11[jk]=Ptzerr;



		    pAnalysis->ellip_diff11[jk]= ellip_max[ijk][ij];


		    pAnalysis->LeTime11[jk] = tmpcluster->GetLeTime();
		    pAnalysis->RiTime11[jk] = tmpcluster->GetRiTime();

		    pAnalysis->LePulse11[jk] = tmpcluster->GetLePulse();
		    pAnalysis->RiPulse11[jk] = tmpcluster->GetRiPulse();


		    
		  }


		  if(ijk==1 && ij==2){

		    pAnalysis->cmv_locno12[jk]=1;//locno starts from 1
		    pAnalysis->clustersize12[jk]=tmpcluster->GetClustersize();
			    
		    pAnalysis->extrapolposx12[jk]=G_Point[0];
		    pAnalysis->extrapolposy12[jk]=G_Point[1];
		    pAnalysis->extrapolposz12[jk]=G_Point[2];
		  
		    pAnalysis->cmvhitrecoposx12[jk]=tmphtvec.x();
		    pAnalysis->cmvhitrecoposy12[jk]=tmphtvec.y();
		    pAnalysis->cmvhitrecoposz12[jk]=tmphtvec.z();


		    pAnalysis->cmvhittrueposx12[jk]=truehtvec.x();
		    pAnalysis->cmvhittrueposy12[jk]=truehtvec.y();
		    pAnalysis->cmvhittrueposz12[jk]=truehtvec.z();
		  
		    pAnalysis->cmvhitrecoposxerr12[jk]=tmpcluster->GetPosXErr();
		    pAnalysis->cmvhitrecoposyerr12[jk]=tmpcluster->GetPosYErr();
		    pAnalysis->cmvhitrecoposzerr12[jk]=tmpcluster->GetPosZErr();

	
		  
		    pAnalysis->extrapolposxerr12[jk]=Ptxerr;
		    pAnalysis->extrapolposyerr12[jk]=Ptyerr;
		    pAnalysis->extrapolposzerr12[jk]=Ptzerr;


		    pAnalysis->ellip_diff12[jk]= ellip_max[ijk][ij];


		    pAnalysis->LeTime12[jk] = tmpcluster->GetLeTime();
		    pAnalysis->RiTime12[jk] = tmpcluster->GetRiTime();

		    pAnalysis->LePulse12[jk] = tmpcluster->GetLePulse();
		    pAnalysis->RiPulse12[jk] = tmpcluster->GetRiPulse();
		    
		  }
	     
        	  if(ijk==2 && ij==0){

		    pAnalysis->cmv_locno20[jk]=1;//locno starts from 1
		    pAnalysis->clustersize20[jk]=tmpcluster->GetClustersize();
		    pAnalysis->extrapolposx20[jk]=G_Point[0];
		    pAnalysis->extrapolposy20[jk]=G_Point[1];
		    pAnalysis->extrapolposz20[jk]=G_Point[2];
		  
		    pAnalysis->cmvhitrecoposx20[jk]=tmphtvec.x();
		    pAnalysis->cmvhitrecoposy20[jk]=tmphtvec.y();
		    pAnalysis->cmvhitrecoposz20[jk]=tmphtvec.z();


		    pAnalysis->cmvhittrueposx20[jk]=truehtvec.x();
		    pAnalysis->cmvhittrueposy20[jk]=truehtvec.y();
		    pAnalysis->cmvhittrueposz20[jk]=truehtvec.z();
		  
		    pAnalysis->cmvhitrecoposxerr20[jk]=tmpcluster->GetPosXErr();
		    pAnalysis->cmvhitrecoposyerr20[jk]=tmpcluster->GetPosYErr();
		    pAnalysis->cmvhitrecoposzerr20[jk]=tmpcluster->GetPosZErr();

	
		  
		    pAnalysis->extrapolposxerr20[jk]=Ptxerr;
		    pAnalysis->extrapolposyerr20[jk]=Ptyerr;
		    pAnalysis->extrapolposzerr20[jk]=Ptzerr;



		    pAnalysis->ellip_diff20[jk]= ellip_max[ijk][ij];

		    pAnalysis->LeTime20[jk] = tmpcluster->GetLeTime();
		    pAnalysis->RiTime20[jk] = tmpcluster->GetRiTime();

		    pAnalysis->LePulse20[jk] = tmpcluster->GetLePulse();
		    pAnalysis->RiPulse20[jk] = tmpcluster->GetRiPulse();
		    
		  }

		  if(ijk==2 && ij==1){

		    pAnalysis->cmv_locno21[jk]=1;//locno starts from 1
		    pAnalysis->clustersize21[jk]=tmpcluster->GetClustersize();
		    pAnalysis->extrapolposx21[jk]=G_Point[0];
		    pAnalysis->extrapolposy21[jk]=G_Point[1];
		    pAnalysis->extrapolposz21[jk]=G_Point[2];
		  
		    pAnalysis->cmvhitrecoposx21[jk]=tmphtvec.x();
		    pAnalysis->cmvhitrecoposy21[jk]=tmphtvec.y();
		    pAnalysis->cmvhitrecoposz21[jk]=tmphtvec.z();


		    pAnalysis->cmvhittrueposx21[jk]=truehtvec.x();
		    pAnalysis->cmvhittrueposy21[jk]=truehtvec.y();
		    pAnalysis->cmvhittrueposz21[jk]=truehtvec.z();
		  
		    pAnalysis->cmvhitrecoposxerr21[jk]=tmpcluster->GetPosXErr();
		    pAnalysis->cmvhitrecoposyerr21[jk]=tmpcluster->GetPosYErr();
		    pAnalysis->cmvhitrecoposzerr21[jk]=tmpcluster->GetPosZErr();

	
		  
		    pAnalysis->extrapolposxerr21[jk]=Ptxerr;
		    pAnalysis->extrapolposyerr21[jk]=Ptyerr;
		    pAnalysis->extrapolposzerr21[jk]=Ptzerr;

		    pAnalysis->ellip_diff21[jk]= ellip_max[ijk][ij];



		    pAnalysis->LeTime21[jk] = tmpcluster->GetLeTime();
		    pAnalysis->RiTime21[jk] = tmpcluster->GetRiTime();

		    pAnalysis->LePulse21[jk] = tmpcluster->GetLePulse();
		    pAnalysis->RiPulse21[jk] = tmpcluster->GetRiPulse();
		    



		    
		  }


        	  if(ijk==2 && ij==2){

		    pAnalysis->cmv_locno22[jk]=1;//locno starts from 1
		    pAnalysis->clustersize22[jk]=tmpcluster->GetClustersize();
		    
		    pAnalysis->extrapolposx22[jk]=G_Point[0];
		    pAnalysis->extrapolposy22[jk]=G_Point[1];
		    pAnalysis->extrapolposz22[jk]=G_Point[2];
		  
		    pAnalysis->cmvhitrecoposx22[jk]=tmphtvec.x();
		    pAnalysis->cmvhitrecoposy22[jk]=tmphtvec.y();
		    pAnalysis->cmvhitrecoposz22[jk]=tmphtvec.z();


		    pAnalysis->cmvhittrueposx22[jk]=truehtvec.x();
		    pAnalysis->cmvhittrueposy22[jk]=truehtvec.y();
		    pAnalysis->cmvhittrueposz22[jk]=truehtvec.z();
		  
		    pAnalysis->cmvhitrecoposxerr22[jk]=tmpcluster->GetPosXErr();
		    pAnalysis->cmvhitrecoposyerr22[jk]=tmpcluster->GetPosYErr();
		    pAnalysis->cmvhitrecoposzerr22[jk]=tmpcluster->GetPosZErr();

	
		  
		    pAnalysis->extrapolposxerr22[jk]=Ptxerr;
		    pAnalysis->extrapolposyerr22[jk]=Ptyerr;
		    pAnalysis->extrapolposzerr22[jk]=Ptzerr;


		    pAnalysis->ellip_diff22[jk]= ellip_max[ijk][ij];


		    pAnalysis->LeTime22[jk] = tmpcluster->GetLeTime();
		    pAnalysis->RiTime22[jk] = tmpcluster->GetRiTime();

		    pAnalysis->LePulse22[jk] = tmpcluster->GetLePulse();
		    pAnalysis->RiPulse22[jk] = tmpcluster->GetRiPulse();

		    
		    
		  }


		  if(ijk==3 && ij==0){

		    pAnalysis->cmv_locno30[jk]=1;//locno starts from 1
		    pAnalysis->clustersize30[jk]=tmpcluster->GetClustersize();
	    
		    pAnalysis->extrapolposx30[jk]=G_Point[0];
		    pAnalysis->extrapolposy30[jk]=G_Point[1];
		    pAnalysis->extrapolposz30[jk]=G_Point[2];
		  
		    pAnalysis->cmvhitrecoposx30[jk]=tmphtvec.x();
		    pAnalysis->cmvhitrecoposy30[jk]=tmphtvec.y();
		    pAnalysis->cmvhitrecoposz30[jk]=tmphtvec.z();


		    pAnalysis->cmvhittrueposx30[jk]=truehtvec.x();
		    pAnalysis->cmvhittrueposy30[jk]=truehtvec.y();
		    pAnalysis->cmvhittrueposz30[jk]=truehtvec.z();
		  
		    pAnalysis->cmvhitrecoposxerr30[jk]=tmpcluster->GetPosXErr();
		    pAnalysis->cmvhitrecoposyerr30[jk]=tmpcluster->GetPosYErr();
		    pAnalysis->cmvhitrecoposzerr30[jk]=tmpcluster->GetPosZErr();

	
		  
		    pAnalysis->extrapolposxerr30[jk]=Ptxerr;
		    pAnalysis->extrapolposyerr30[jk]=Ptyerr;
		    pAnalysis->extrapolposzerr30[jk]=Ptzerr;

		    pAnalysis->ellip_diff30[jk]= ellip_max[ijk][ij];



		    pAnalysis->LeTime30[jk] = tmpcluster->GetLeTime();
		    pAnalysis->RiTime30[jk] = tmpcluster->GetRiTime();

		    pAnalysis->LePulse30[jk] = tmpcluster->GetLePulse();
		    pAnalysis->RiPulse30[jk] = tmpcluster->GetRiPulse();





		    
		  }




		  if(ijk==3 && ij==1){

		    pAnalysis->cmv_locno31[jk]=1;//locno starts from 1
		    pAnalysis->clustersize31[jk]=tmpcluster->GetClustersize();
			    
		    pAnalysis->extrapolposx31[jk]=G_Point[0];
		    pAnalysis->extrapolposy31[jk]=G_Point[1];
		    pAnalysis->extrapolposz31[jk]=G_Point[2];
		  
		    pAnalysis->cmvhitrecoposx31[jk]=tmphtvec.x();
		    pAnalysis->cmvhitrecoposy31[jk]=tmphtvec.y();
		    pAnalysis->cmvhitrecoposz31[jk]=tmphtvec.z();


		    pAnalysis->cmvhittrueposx31[jk]=truehtvec.x();
		    pAnalysis->cmvhittrueposy31[jk]=truehtvec.y();
		    pAnalysis->cmvhittrueposz31[jk]=truehtvec.z();
		  
		    pAnalysis->cmvhitrecoposxerr31[jk]=tmpcluster->GetPosXErr();
		    pAnalysis->cmvhitrecoposyerr31[jk]=tmpcluster->GetPosYErr();
		    pAnalysis->cmvhitrecoposzerr31[jk]=tmpcluster->GetPosZErr();

	
		  
		    pAnalysis->extrapolposxerr31[jk]=Ptxerr;
		    pAnalysis->extrapolposyerr31[jk]=Ptyerr;
		    pAnalysis->extrapolposzerr31[jk]=Ptzerr;


		    pAnalysis->ellip_diff31[jk]= ellip_max[ijk][ij];


		    pAnalysis->LeTime31[jk] = tmpcluster->GetLeTime();
		    pAnalysis->RiTime31[jk] = tmpcluster->GetRiTime();

		    pAnalysis->LePulse31[jk] = tmpcluster->GetLePulse();
		    pAnalysis->RiPulse31[jk] = tmpcluster->GetRiPulse();





		    
		  }



        	  if(ijk==3 && ij==2){
		    pAnalysis->cmv_locno32[jk]=1;//locno starts from 1
		    pAnalysis->clustersize32[jk]=tmpcluster->GetClustersize();


		    
		    pAnalysis->extrapolposx32[jk]=G_Point[0];
		    pAnalysis->extrapolposy32[jk]=G_Point[1];
		    pAnalysis->extrapolposz32[jk]=G_Point[2];
		  
		    pAnalysis->cmvhitrecoposx32[jk]=tmphtvec.x();
		    pAnalysis->cmvhitrecoposy32[jk]=tmphtvec.y();
		    pAnalysis->cmvhitrecoposz32[jk]=tmphtvec.z();


		    pAnalysis->cmvhittrueposx32[jk]=truehtvec.x();
		    pAnalysis->cmvhittrueposy32[jk]=truehtvec.y();
		    pAnalysis->cmvhittrueposz32[jk]=truehtvec.z();
		  
		    pAnalysis->cmvhitrecoposxerr32[jk]=tmpcluster->GetPosXErr();
		    pAnalysis->cmvhitrecoposyerr32[jk]=tmpcluster->GetPosYErr();
		    pAnalysis->cmvhitrecoposzerr32[jk]=tmpcluster->GetPosZErr();

	
		  
		    pAnalysis->extrapolposxerr32[jk]=Ptxerr;
		    pAnalysis->extrapolposyerr32[jk]=Ptyerr;
		    pAnalysis->extrapolposzerr32[jk]=Ptzerr;



		    pAnalysis->ellip_diff32[jk]= ellip_max[ijk][ij]; 

		    pAnalysis->LeTime32[jk] = tmpcluster->GetLeTime();
		    pAnalysis->RiTime32[jk] = tmpcluster->GetRiTime(); 
		    pAnalysis->LePulse32[jk] = tmpcluster->GetLePulse();
		    pAnalysis->RiPulse32[jk] = tmpcluster->GetRiPulse();



		  }
		  //110222


		  if(ijk==4 && ij==0){

		    pAnalysis->cmv_locno40[jk]=1;//locno starts from 1
		    pAnalysis->clustersize40[jk]=tmpcluster->GetClustersize();
	    
		    pAnalysis->extrapolposx40[jk]=G_Point[0];
		    pAnalysis->extrapolposy40[jk]=G_Point[1];
		    pAnalysis->extrapolposz40[jk]=G_Point[2];
		  
		    pAnalysis->cmvhitrecoposx40[jk]=tmphtvec.x();
		    pAnalysis->cmvhitrecoposy40[jk]=tmphtvec.y();
		    pAnalysis->cmvhitrecoposz40[jk]=tmphtvec.z();


		    pAnalysis->cmvhittrueposx40[jk]=truehtvec.x();
		    pAnalysis->cmvhittrueposy40[jk]=truehtvec.y();
		    pAnalysis->cmvhittrueposz40[jk]=truehtvec.z();
		  
		    pAnalysis->cmvhitrecoposxerr40[jk]=tmpcluster->GetPosXErr();
		    pAnalysis->cmvhitrecoposyerr40[jk]=tmpcluster->GetPosYErr();
		    pAnalysis->cmvhitrecoposzerr40[jk]=tmpcluster->GetPosZErr();

	
		  
		    pAnalysis->extrapolposxerr40[jk]=Ptxerr;
		    pAnalysis->extrapolposyerr40[jk]=Ptyerr;
		    pAnalysis->extrapolposzerr40[jk]=Ptzerr;

		    // pAnalysis->ellip_diff40[jk]= ellip_max[ijk][ij];



		    pAnalysis->LeTime40[jk] = tmpcluster->GetLeTime();
		    pAnalysis->RiTime40[jk] = tmpcluster->GetRiTime();

		    pAnalysis->LePulse40[jk] = tmpcluster->GetLePulse();
		    pAnalysis->RiPulse40[jk] = tmpcluster->GetRiPulse();





		    
		  }




		  if(ijk==4 && ij==1){

		    pAnalysis->cmv_locno41[jk]=1;//locno starts from 1
		    pAnalysis->clustersize41[jk]=tmpcluster->GetClustersize();
			    
		    pAnalysis->extrapolposx41[jk]=G_Point[0];
		    pAnalysis->extrapolposy41[jk]=G_Point[1];
		    pAnalysis->extrapolposz41[jk]=G_Point[2];
		  
		    pAnalysis->cmvhitrecoposx41[jk]=tmphtvec.x();
		    pAnalysis->cmvhitrecoposy41[jk]=tmphtvec.y();
		    pAnalysis->cmvhitrecoposz41[jk]=tmphtvec.z();


		    pAnalysis->cmvhittrueposx41[jk]=truehtvec.x();
		    pAnalysis->cmvhittrueposy41[jk]=truehtvec.y();
		    pAnalysis->cmvhittrueposz41[jk]=truehtvec.z();
		  
		    pAnalysis->cmvhitrecoposxerr41[jk]=tmpcluster->GetPosXErr();
		    pAnalysis->cmvhitrecoposyerr41[jk]=tmpcluster->GetPosYErr();
		    pAnalysis->cmvhitrecoposzerr41[jk]=tmpcluster->GetPosZErr();

	
		  
		    pAnalysis->extrapolposxerr41[jk]=Ptxerr;
		    pAnalysis->extrapolposyerr41[jk]=Ptyerr;
		    pAnalysis->extrapolposzerr41[jk]=Ptzerr;


		    //  pAnalysis->ellip_diff41[jk]= ellip_max[ijk][ij];


		    pAnalysis->LeTime41[jk] = tmpcluster->GetLeTime();
		    pAnalysis->RiTime41[jk] = tmpcluster->GetRiTime();

		    pAnalysis->LePulse41[jk] = tmpcluster->GetLePulse();
		    pAnalysis->RiPulse41[jk] = tmpcluster->GetRiPulse();





		    
		  }



        	  if(ijk==4 && ij==2){
		    pAnalysis->cmv_locno42[jk]=1;//locno starts from 1
		    pAnalysis->clustersize42[jk]=tmpcluster->GetClustersize();


		    
		    pAnalysis->extrapolposx42[jk]=G_Point[0];
		    pAnalysis->extrapolposy42[jk]=G_Point[1];
		    pAnalysis->extrapolposz42[jk]=G_Point[2];
		  
		    pAnalysis->cmvhitrecoposx42[jk]=tmphtvec.x();
		    pAnalysis->cmvhitrecoposy42[jk]=tmphtvec.y();
		    pAnalysis->cmvhitrecoposz42[jk]=tmphtvec.z();


		    pAnalysis->cmvhittrueposx42[jk]=truehtvec.x();
		    pAnalysis->cmvhittrueposy42[jk]=truehtvec.y();
		    pAnalysis->cmvhittrueposz42[jk]=truehtvec.z();
		  
		    pAnalysis->cmvhitrecoposxerr42[jk]=tmpcluster->GetPosXErr();
		    pAnalysis->cmvhitrecoposyerr42[jk]=tmpcluster->GetPosYErr();
		    pAnalysis->cmvhitrecoposzerr42[jk]=tmpcluster->GetPosZErr();

	
		  
		    pAnalysis->extrapolposxerr42[jk]=Ptxerr;
		    pAnalysis->extrapolposyerr42[jk]=Ptyerr;
		    pAnalysis->extrapolposzerr42[jk]=Ptzerr;



		    // pAnalysis->ellip_diff42[jk]= ellip_max[ijk][ij]; 

		    pAnalysis->LeTime42[jk] = tmpcluster->GetLeTime();
		    pAnalysis->RiTime42[jk] = tmpcluster->GetRiTime(); 
		    pAnalysis->LePulse42[jk] = tmpcluster->GetLePulse();
		    pAnalysis->RiPulse42[jk] = tmpcluster->GetRiPulse();



		  }
		  
		  if(ijk==5 && ij==0){

		    pAnalysis->cmv_locno50[jk]=1;//locno starts from 1
		    pAnalysis->clustersize50[jk]=tmpcluster->GetClustersize();
	    
		    pAnalysis->extrapolposx50[jk]=G_Point[0];
		    pAnalysis->extrapolposy50[jk]=G_Point[1];
		    pAnalysis->extrapolposz50[jk]=G_Point[2];
		  
		    pAnalysis->cmvhitrecoposx50[jk]=tmphtvec.x();
		    pAnalysis->cmvhitrecoposy50[jk]=tmphtvec.y();
		    pAnalysis->cmvhitrecoposz50[jk]=tmphtvec.z();


		    pAnalysis->cmvhittrueposx50[jk]=truehtvec.x();
		    pAnalysis->cmvhittrueposy50[jk]=truehtvec.y();
		    pAnalysis->cmvhittrueposz50[jk]=truehtvec.z();
		  
		    pAnalysis->cmvhitrecoposxerr50[jk]=tmpcluster->GetPosXErr();
		    pAnalysis->cmvhitrecoposyerr50[jk]=tmpcluster->GetPosYErr();
		    pAnalysis->cmvhitrecoposzerr50[jk]=tmpcluster->GetPosZErr();

	
		  
		    pAnalysis->extrapolposxerr50[jk]=Ptxerr;
		    pAnalysis->extrapolposyerr50[jk]=Ptyerr;
		    pAnalysis->extrapolposzerr50[jk]=Ptzerr;

		    // pAnalysis->ellip_diff50[jk]= ellip_max[ijk][ij];



		    pAnalysis->LeTime50[jk] = tmpcluster->GetLeTime();
		    pAnalysis->RiTime50[jk] = tmpcluster->GetRiTime();

		    pAnalysis->LePulse50[jk] = tmpcluster->GetLePulse();
		    pAnalysis->RiPulse50[jk] = tmpcluster->GetRiPulse();





		    
		  }




		  if(ijk==5 && ij==1){

		    pAnalysis->cmv_locno51[jk]=1;//locno starts from 1
		    pAnalysis->clustersize51[jk]=tmpcluster->GetClustersize();
			    
		    pAnalysis->extrapolposx51[jk]=G_Point[0];
		    pAnalysis->extrapolposy51[jk]=G_Point[1];
		    pAnalysis->extrapolposz51[jk]=G_Point[2];
		  
		    pAnalysis->cmvhitrecoposx51[jk]=tmphtvec.x();
		    pAnalysis->cmvhitrecoposy51[jk]=tmphtvec.y();
		    pAnalysis->cmvhitrecoposz51[jk]=tmphtvec.z();


		    pAnalysis->cmvhittrueposx51[jk]=truehtvec.x();
		    pAnalysis->cmvhittrueposy51[jk]=truehtvec.y();
		    pAnalysis->cmvhittrueposz51[jk]=truehtvec.z();
		  
		    pAnalysis->cmvhitrecoposxerr51[jk]=tmpcluster->GetPosXErr();
		    pAnalysis->cmvhitrecoposyerr51[jk]=tmpcluster->GetPosYErr();
		    pAnalysis->cmvhitrecoposzerr51[jk]=tmpcluster->GetPosZErr();

	
		  
		    pAnalysis->extrapolposxerr51[jk]=Ptxerr;
		    pAnalysis->extrapolposyerr51[jk]=Ptyerr;
		    pAnalysis->extrapolposzerr51[jk]=Ptzerr;


		    // pAnalysis->ellip_diff51[jk]= ellip_max[ijk][ij];


		    pAnalysis->LeTime51[jk] = tmpcluster->GetLeTime();
		    pAnalysis->RiTime51[jk] = tmpcluster->GetRiTime();

		    pAnalysis->LePulse51[jk] = tmpcluster->GetLePulse();
		    pAnalysis->RiPulse51[jk] = tmpcluster->GetRiPulse();





		    
		  }



        	  if(ijk==5 && ij==2){
		    pAnalysis->cmv_locno52[jk]=1;//locno starts from 1
		    pAnalysis->clustersize52[jk]=tmpcluster->GetClustersize();


		    
		    pAnalysis->extrapolposx52[jk]=G_Point[0];
		    pAnalysis->extrapolposy52[jk]=G_Point[1];
		    pAnalysis->extrapolposz52[jk]=G_Point[2];
		  
		    pAnalysis->cmvhitrecoposx52[jk]=tmphtvec.x();
		    pAnalysis->cmvhitrecoposy52[jk]=tmphtvec.y();
		    pAnalysis->cmvhitrecoposz52[jk]=tmphtvec.z();


		    pAnalysis->cmvhittrueposx52[jk]=truehtvec.x();
		    pAnalysis->cmvhittrueposy52[jk]=truehtvec.y();
		    pAnalysis->cmvhittrueposz52[jk]=truehtvec.z();
		  
		    pAnalysis->cmvhitrecoposxerr52[jk]=tmpcluster->GetPosXErr();
		    pAnalysis->cmvhitrecoposyerr52[jk]=tmpcluster->GetPosYErr();
		    pAnalysis->cmvhitrecoposzerr52[jk]=tmpcluster->GetPosZErr();

	
		  
		    pAnalysis->extrapolposxerr52[jk]=Ptxerr;
		    pAnalysis->extrapolposyerr52[jk]=Ptyerr;
		    pAnalysis->extrapolposzerr52[jk]=Ptzerr;



		    //	    pAnalysis->ellip_diff52[jk]= ellip_max[ijk][ij]; 

		    pAnalysis->LeTime52[jk] = tmpcluster->GetLeTime();
		    pAnalysis->RiTime52[jk] = tmpcluster->GetRiTime(); 
		    pAnalysis->LePulse52[jk] = tmpcluster->GetLePulse();
		    pAnalysis->RiPulse52[jk] = tmpcluster->GetRiPulse();



		  }
		  
		  if(ijk==6 && ij==0){

		    pAnalysis->cmv_locno60[jk]=1;//locno starts from 1
		    pAnalysis->clustersize60[jk]=tmpcluster->GetClustersize();
	    
		    pAnalysis->extrapolposx60[jk]=G_Point[0];
		    pAnalysis->extrapolposy60[jk]=G_Point[1];
		    pAnalysis->extrapolposz60[jk]=G_Point[2];
		  
		    pAnalysis->cmvhitrecoposx60[jk]=tmphtvec.x();
		    pAnalysis->cmvhitrecoposy60[jk]=tmphtvec.y();
		    pAnalysis->cmvhitrecoposz60[jk]=tmphtvec.z();


		    pAnalysis->cmvhittrueposx60[jk]=truehtvec.x();
		    pAnalysis->cmvhittrueposy60[jk]=truehtvec.y();
		    pAnalysis->cmvhittrueposz60[jk]=truehtvec.z();
		  
		    pAnalysis->cmvhitrecoposxerr60[jk]=tmpcluster->GetPosXErr();
		    pAnalysis->cmvhitrecoposyerr60[jk]=tmpcluster->GetPosYErr();
		    pAnalysis->cmvhitrecoposzerr60[jk]=tmpcluster->GetPosZErr();

	
		  
		    pAnalysis->extrapolposxerr60[jk]=Ptxerr;
		    pAnalysis->extrapolposyerr60[jk]=Ptyerr;
		    pAnalysis->extrapolposzerr60[jk]=Ptzerr;

		    //  pAnalysis->ellip_diff60[jk]= ellip_max[ijk][ij];



		    pAnalysis->LeTime60[jk] = tmpcluster->GetLeTime();
		    pAnalysis->RiTime60[jk] = tmpcluster->GetRiTime();

		    pAnalysis->LePulse60[jk] = tmpcluster->GetLePulse();
		    pAnalysis->RiPulse60[jk] = tmpcluster->GetRiPulse();





		    
		  }




		  if(ijk==6 && ij==1){

		    pAnalysis->cmv_locno61[jk]=1;//locno starts from 1
		    pAnalysis->clustersize61[jk]=tmpcluster->GetClustersize();
			    
		    pAnalysis->extrapolposx61[jk]=G_Point[0];
		    pAnalysis->extrapolposy61[jk]=G_Point[1];
		    pAnalysis->extrapolposz61[jk]=G_Point[2];
		  
		    pAnalysis->cmvhitrecoposx61[jk]=tmphtvec.x();
		    pAnalysis->cmvhitrecoposy61[jk]=tmphtvec.y();
		    pAnalysis->cmvhitrecoposz61[jk]=tmphtvec.z();


		    pAnalysis->cmvhittrueposx61[jk]=truehtvec.x();
		    pAnalysis->cmvhittrueposy61[jk]=truehtvec.y();
		    pAnalysis->cmvhittrueposz61[jk]=truehtvec.z();
		  
		    pAnalysis->cmvhitrecoposxerr61[jk]=tmpcluster->GetPosXErr();
		    pAnalysis->cmvhitrecoposyerr61[jk]=tmpcluster->GetPosYErr();
		    pAnalysis->cmvhitrecoposzerr61[jk]=tmpcluster->GetPosZErr();

	
		  
		    pAnalysis->extrapolposxerr61[jk]=Ptxerr;
		    pAnalysis->extrapolposyerr61[jk]=Ptyerr;
		    pAnalysis->extrapolposzerr61[jk]=Ptzerr;


		    //   pAnalysis->ellip_diff61[jk]= ellip_max[ijk][ij];


		    pAnalysis->LeTime61[jk] = tmpcluster->GetLeTime();
		    pAnalysis->RiTime61[jk] = tmpcluster->GetRiTime();

		    pAnalysis->LePulse61[jk] = tmpcluster->GetLePulse();
		    pAnalysis->RiPulse61[jk] = tmpcluster->GetRiPulse();





		    
		  }



        	  if(ijk==6 && ij==2){
		    pAnalysis->cmv_locno62[jk]=1;//locno starts from 1
		    pAnalysis->clustersize62[jk]=tmpcluster->GetClustersize();


		    
		    pAnalysis->extrapolposx62[jk]=G_Point[0];
		    pAnalysis->extrapolposy62[jk]=G_Point[1];
		    pAnalysis->extrapolposz62[jk]=G_Point[2];
		  
		    pAnalysis->cmvhitrecoposx62[jk]=tmphtvec.x();
		    pAnalysis->cmvhitrecoposy62[jk]=tmphtvec.y();
		    pAnalysis->cmvhitrecoposz62[jk]=tmphtvec.z();


		    pAnalysis->cmvhittrueposx62[jk]=truehtvec.x();
		    pAnalysis->cmvhittrueposy62[jk]=truehtvec.y();
		    pAnalysis->cmvhittrueposz62[jk]=truehtvec.z();
		  
		    pAnalysis->cmvhitrecoposxerr62[jk]=tmpcluster->GetPosXErr();
		    pAnalysis->cmvhitrecoposyerr62[jk]=tmpcluster->GetPosYErr();
		    pAnalysis->cmvhitrecoposzerr62[jk]=tmpcluster->GetPosZErr();

	
		  
		    pAnalysis->extrapolposxerr62[jk]=Ptxerr;
		    pAnalysis->extrapolposyerr62[jk]=Ptyerr;
		    pAnalysis->extrapolposzerr62[jk]=Ptzerr;



		    //		    pAnalysis->ellip_diff62[jk]= ellip_max[ijk][ij]; 

		    pAnalysis->LeTime62[jk] = tmpcluster->GetLeTime();
		    pAnalysis->RiTime62[jk] = tmpcluster->GetRiTime(); 
		    pAnalysis->LePulse62[jk] = tmpcluster->GetLePulse();
		    pAnalysis->RiPulse62[jk] = tmpcluster->GetRiPulse();



		  }
		  


		  //


		  // }
		  // else{
		  //   cout<<"deltaX and deltaY >=20"<<endl;
		  // }




		  
		 	  
		  //	  
		}//difx
		//
	      }


	      else{

		cout<<"Extrapolated in Side "<<ijk<< " and Layer no  " <<ij<<" but no hits found in records."<<endl;
			 

	      }






		      
            } //for (unsigned int ix=0; ix<CmvHit_pointer->CmvHit_list.size(); ix++)

	 
	    

	    
          } //isInside
	  else {
	    //  else if(hitpresent == true){ //200522
	    // layid = ijk+1;
	    // 	    layid<<=2;
	    // 	    layid+=ij;

	    // 	    cout<<"layid: "<<layid<<endl;
	    // 	layexp->SetId(layid);

	    
	    //    for (unsigned iji=0; iji<CmvCluster_pointer->CmvCluster_list.size(); iji++) {
	    //     CmvCluster* tmpcluster = CmvCluster_pointer->CmvCluster_list[iji];
	    //   if (tmpcluster->GetPlane()-1==ijk && tmpcluster->GetLayer()==ij) {
	    // find distance between  line and edges:

	    //each layer has 4 edges
	    // Find minium distance of line from all the 4 edges
	    // consider minimum of all these..
		
 	    //23032022
	    //cout<<"No extrapolation but hit present in: "<<ijk<<" "<<ij<<endl;
	    cout<<".......Finding distance of closest approach...."<<endl;
	       
	    
	    //   double edge[4][6];
	     
	    double closdist[4];

	    for(int zx=0;zx<4;zx++){
	      for(int ik=0;ik<6;ik++){	
		//	  cout<<edge[zx][ik]<<" " <<endl;
	      }
	      //	double a1a2 = Line[3]*edge[zx][3] + Line[4]*edge[zx][4] + Line[5]*edge[zx][5];
	 
	      double rvec[3] ={ -Line[0]+edge[zx][0],-Line[1]+edge[zx][1],-Line[2]+edge[zx][2] };
	      cout<<"rvec: "<<rvec[0]<<" "<<rvec[1]<<" "<<rvec[2]<<endl;
	      // double ra1 =  Line[3]*rvec[0] + Line[4]*rvec[1] + Line[5]*rvec[2];
	      // double ra2 =  edge[zx][3]*rvec[0] + edge[zx][4]*rvec[1] + edge[zx][5]*rvec[2];
	 
	      // double t1 = (-ra2-(ra1*a1a2))/(1-pow(a1a2,2));
	      // double t2 = ((ra1*a1a2)-ra2)/(1-pow(a1a2,2));

	      // cout<<"t1 t2 "<<t1<<" "<<t2<<endl;
	      // double closdistvec[3];
	 
	      // closdistvec[0] = rvec[0]+edge[zx][3]*t2-Line[3]*t1;
	      // closdistvec[1] = rvec[1]+edge[zx][4]*t2-Line[4]*t1;
	      // closdistvec[2] = rvec[2]+edge[zx][5]*t2-Line[5]*t1;

	      // double closdist = sqrt(pow(closdistvec[0],2)+pow(closdistvec[1],2)+pow(closdistvec[2],2));
	      //	 cout<<"dist of closest approach to the edge"<<zx<<" is: "<<closdist<<endl;
	      //23032022

	      //formula: d = (a1 x a2). (r1-r2)/|a1 x a2|
	      double a1xa2[3] = {edge[zx][4]*Line[5]-edge[zx][5]*Line[4], -edge[zx][3]*Line[5]+edge[zx][5]*Line[3],  edge[zx][3]*Line[4]-edge[zx][4]*Line[3] }; 

	      double maga1xa2 = sqrt( pow(a1xa2[0],2) + pow(a1xa2[1],2)+ pow(a1xa2[2],2) );
	      closdist[zx] = abs(rvec[0]*a1xa2[0]+rvec[1]*a1xa2[1]+rvec[2]*a1xa2[2])/(maga1xa2);


	      cout<<"dist of closest approach to the edge"<<zx<<" is: "<<closdist[zx]<<endl;
	 
	    }//   for(int zx=0;zx<4;zx++){


	      
	    double minclosdist=100000;
	    int minedge;
	    for(int zxy=0;zxy<4;zxy++){
	      if(	 closdist[zxy]<minclosdist){
   
		minclosdist = 	 closdist[zxy];
		minedge=zxy;
	      }
	    }

	    cout<<"minimum closest dis from the 4 edges is: "<<minclosdist<< " miniedge: "<< minedge<<endl;

	    const int miniedge = minedge;


	    cout<<"Now Finding the points on Line And Edge where min d exists "<<minedge<<endl;
	    //.. To find points on line and edge where this dist of clos app is found. //23072022
	    //A1 Line, A2 Edge
	    //Line: Lamda = x-x1/a1 = y-y1/b1 = z-z1/c1; x is point on Line where u get min clos app
	    //Edge: mu = xx-x2/a2 = yy-y2/b2 = zz-z2/c2; xx is point on ege where you get min clos dist
	    // X-XX is vector joining X and XX whose length is mini and is perpendicular to but Line and edge.
	    // (X-XX).A1 = 0 && (X-XX).A2=0
	    //Solving this we get Lamda and Mu..
 
	    double rveccloseedge[3] ={ -Line[0]+edge[miniedge][0],-Line[1]+edge[miniedge][1],-Line[2]+edge[miniedge][2] };
	    cout<<"rveccloseedge: "<<rveccloseedge[0]<<" "<<rveccloseedge[1]<<" "<<rveccloseedge[2]<<endl;
 
	    double A1dotA2 = Line[3]*edge[miniedge][3]+Line[4]*edge[miniedge][4]+Line[5]*edge[miniedge][5];
	    double A1dotrvec =Line[3]*rveccloseedge[0]+Line[4]*rveccloseedge[1]+Line[5]*rveccloseedge[2];
	    double A2dotrvec = edge[miniedge][3]*rveccloseedge[0]+edge[miniedge][4]*rveccloseedge[1]+edge[miniedge][5]*rveccloseedge[2];  
 
	    double lambda = (A1dotrvec - A2dotrvec*(A1dotA2))/(1-pow(A1dotA2,2));
	    double mu = lambda*A1dotA2 - A2dotrvec;
	       
	    //	    cout<<mu<<" "<<lambda<<endl;
	    double GOnLine[3] = {Line[0]+lambda*Line[3],Line[1]+lambda*Line[4],Line[2]+lambda*Line[5]};
 
	    // cout<<edge[miniedge][0]<<" "<<mu<<" "<<edge[miniedge][3]<<" "<<miniedge<<" "<<edge[miniedge][0]+mu*edge[miniedge][3]<<endl;
	    double GOnedge[3] = {edge[miniedge][0]+mu*edge[miniedge][3],edge[miniedge][1]+mu*edge[miniedge][4],edge[miniedge][2]+mu*edge[miniedge][5]};
 
	    for(int qw=0;qw<3;qw++){
	       cout<<"GOnLine["<<qw<<"] "<<GOnLine[qw] <<" GOnedge["<<qw<<"] "<<GOnedge[qw]<<endl;
	    }
	     cout<<"mindist Re-check"<<sqrt( pow(GOnLine[0]-GOnedge[0],2) + pow(GOnLine[1]-GOnedge[1],2)+ pow(GOnLine[2]-GOnedge[2],2) ) <<endl;
 
	     bool Onedge = false;
	     if(ijk==0){ //top wall
	       switch(minedge){
	       case 0: Onedge =  ( (GOnedge[0])<  (PhyVolGlPos[ijk][ij][0]+layhalflength+delta) &&  (GOnedge[0])>  (PhyVolGlPos[ijk][ij][0]-layhalflength-delta) && abs(GOnedge[1]-PhyVolGlPos[ijk][ij][1]-layhalfbreadth)<1.e-10  && abs(GOnedge[2]-PhyVolGlPos[ijk][ij][2])<1.e-10 );//backside edge
		 break;
	       case 1: Onedge =  ( (GOnedge[1])<  (PhyVolGlPos[ijk][ij][1]+layhalfbreadth+delta) &&  (GOnedge[1])>  (PhyVolGlPos[ijk][ij][1]-layhalfbreadth-delta) && abs(GOnedge[0]-PhyVolGlPos[ijk][ij][0]+layhalflength)<1.e-10  && abs(GOnedge[2]-PhyVolGlPos[ijk][ij][2])<1.e-10 );//leftside edge
		 	 break;
	       case 2: Onedge =  ( (GOnedge[0])<  (PhyVolGlPos[ijk][ij][0]+layhalflength+delta) &&  (GOnedge[0])>  (PhyVolGlPos[ijk][ij][0]-layhalflength-delta) && abs(GOnedge[1]-PhyVolGlPos[ijk][ij][1]+layhalfbreadth)<1.e-10  && abs(GOnedge[2]-PhyVolGlPos[ijk][ij][2])<1.e-10 );//frontside edge
		 	 break;

	       case 3: Onedge =  ( (GOnedge[1])<  (PhyVolGlPos[ijk][ij][1]+layhalfbreadth+delta) &&  (GOnedge[1])>  (PhyVolGlPos[ijk][ij][1]-layhalfbreadth-delta) && abs(GOnedge[0]-PhyVolGlPos[ijk][ij][0]-layhalflength)<1.e-10  && abs(GOnedge[2]-PhyVolGlPos[ijk][ij][2])<1.e-10 );//rightside edge
		 	 break;
	        
	   
	       }
	       
	     }
	     else if(ijk==1){ //left wall
	       switch(minedge){
	          case 0: Onedge =  ( (GOnedge[1])<  (PhyVolGlPos[ijk][ij][1]+layhalfbreadth+delta) &&  (GOnedge[1])>  (PhyVolGlPos[ijk][ij][1]-layhalfbreadth-delta) && abs(GOnedge[2]-PhyVolGlPos[ijk][ij][2]-layhalflength)<1.e-10  && abs(GOnedge[0]-PhyVolGlPos[ijk][ij][0])<1.e-10 );//topside edge
	 break;
	       
	          case 1: Onedge =  ( (GOnedge[2])<  (PhyVolGlPos[ijk][ij][2]+layhalflength+delta) &&  (GOnedge[2])>  (PhyVolGlPos[ijk][ij][2]-layhalflength-delta) && abs(GOnedge[1]-PhyVolGlPos[ijk][ij][1]-layhalfbreadth)<1.e-10  && abs(GOnedge[0]-PhyVolGlPos[ijk][ij][0])<1.e-10 );//backside edge
	 break;
		    
	          case 2: Onedge =  ( (GOnedge[1])<  (PhyVolGlPos[ijk][ij][1]+layhalfbreadth+delta) &&  (GOnedge[1])>  (PhyVolGlPos[ijk][ij][1]-layhalfbreadth-delta) && abs(GOnedge[2]-PhyVolGlPos[ijk][ij][2]+layhalflength)<1.e-10  && abs(GOnedge[0]-PhyVolGlPos[ijk][ij][0])<1.e-10 );//bottomside edge
	 break;
	 
  case 3: Onedge =  ( (GOnedge[2])<  (PhyVolGlPos[ijk][ij][2]+layhalflength+delta) &&  (GOnedge[2])>  (PhyVolGlPos[ijk][ij][2]-layhalflength-delta) && abs(GOnedge[1]-PhyVolGlPos[ijk][ij][1]+layhalfbreadth)<1.e-10  && abs(GOnedge[0]-PhyVolGlPos[ijk][ij][0])<1.e-10 );//frontside edge
	 break;
	       }
	       
	     }

	     else if(ijk==2){ //right wall

  switch(minedge){
	          case 0: Onedge =  ( (GOnedge[1])<  (PhyVolGlPos[ijk][ij][1]+layhalfbreadth+delta) &&  (GOnedge[1])>  (PhyVolGlPos[ijk][ij][1]-layhalfbreadth-delta) && abs(GOnedge[2]-PhyVolGlPos[ijk][ij][2]-layhalflength)<1.e-10  && abs(GOnedge[0]-PhyVolGlPos[ijk][ij][0])<1.e-10 );//topside edge
	 break;
	       
	          case 1: Onedge =  ( (GOnedge[2])<  (PhyVolGlPos[ijk][ij][2]+layhalflength+delta) &&  (GOnedge[2])>  (PhyVolGlPos[ijk][ij][2]-layhalflength-delta) && abs(GOnedge[1]-PhyVolGlPos[ijk][ij][1]+layhalfbreadth)<1.e-10  && abs(GOnedge[0]-PhyVolGlPos[ijk][ij][0])<1.e-10 );//frontside edge
	 break;
		    
	          case 2: Onedge =  ( (GOnedge[1])<  (PhyVolGlPos[ijk][ij][1]+layhalfbreadth+delta) &&  (GOnedge[1])>  (PhyVolGlPos[ijk][ij][1]-layhalfbreadth-delta) && abs(GOnedge[2]-PhyVolGlPos[ijk][ij][2]+layhalflength)<1.e-10  && abs(GOnedge[0]-PhyVolGlPos[ijk][ij][0])<1.e-10 );//bottomside edge
	 break;
	 
  case 3: Onedge =  ( (GOnedge[2])<  (PhyVolGlPos[ijk][ij][2]+layhalflength+delta) &&  (GOnedge[2])>  (PhyVolGlPos[ijk][ij][2]-layhalflength-delta) && abs(GOnedge[1]-PhyVolGlPos[ijk][ij][1]-layhalfbreadth)<1.e-10  && abs(GOnedge[0]-PhyVolGlPos[ijk][ij][0])<1.e-10 );//backside edge
	 break;
	 
	       }

	       

	     }

	     else if(ijk==3){ //back
	       switch(minedge){
 case 0: Onedge =  ( (GOnedge[0])<  (PhyVolGlPos[ijk][ij][0]+layhalfbreadth+delta) &&  (GOnedge[0])>  (PhyVolGlPos[ijk][ij][0]-layhalfbreadth-delta) && abs(GOnedge[2]-PhyVolGlPos[ijk][ij][2]-layhalflength)<1.e-10  && abs(GOnedge[1]-PhyVolGlPos[ijk][ij][1])<1.e-10 );//topside edge
	 break;
	 
 case 1: Onedge =  ( (GOnedge[2])<  (PhyVolGlPos[ijk][ij][2]+layhalflength+delta) &&  (GOnedge[2])>  (PhyVolGlPos[ijk][ij][2]-layhalflength-delta) && abs(GOnedge[0]-PhyVolGlPos[ijk][ij][0]-layhalfbreadth)<1.e-10  && abs(GOnedge[1]-PhyVolGlPos[ijk][ij][1])<1.e-10 );//rightside edge
	 break;
	 

 case 2: Onedge =  ( (GOnedge[0])<  (PhyVolGlPos[ijk][ij][0]+layhalfbreadth+delta) &&  (GOnedge[0])>  (PhyVolGlPos[ijk][ij][0]-layhalfbreadth-delta) && abs(GOnedge[2]-PhyVolGlPos[ijk][ij][2]+layhalflength)<1.e-10  && abs(GOnedge[1]-PhyVolGlPos[ijk][ij][1])<1.e-10 );//bottomside edge
	 break;
	 
 case 3: Onedge =  ( (GOnedge[2])<  (PhyVolGlPos[ijk][ij][2]+layhalflength+delta) &&  (GOnedge[2])>  (PhyVolGlPos[ijk][ij][2]-layhalflength-delta) && abs(GOnedge[0]-PhyVolGlPos[ijk][ij][0]+layhalfbreadth)<1.e-10  && abs(GOnedge[1]-PhyVolGlPos[ijk][ij][1])<1.e-10 );//leftside edge
	 break;
	 


	       }


	       

	     }

	     else if(ijk==5){//miniLeft

	           switch(minedge){
	          case 0: Onedge =  ( (GOnedge[1])<  (PhyVolGlPos[ijk][ij][1]+layhalflength+delta) &&  (GOnedge[1])>  (PhyVolGlPos[ijk][ij][1]-layhalflength-delta) && abs(GOnedge[2]-PhyVolGlPos[ijk][ij][2]-layhalfbreadth)<1.e-10  && abs(GOnedge[0]-PhyVolGlPos[ijk][ij][0])<1.e-10 );//topside edge
	 break;
	 
	       
	          case 1: Onedge =  ( (GOnedge[2])<  (PhyVolGlPos[ijk][ij][2]+layhalfbreadth+delta) &&  (GOnedge[2])>  (PhyVolGlPos[ijk][ij][2]-layhalfbreadth-delta) && abs(GOnedge[1]-PhyVolGlPos[ijk][ij][1]-layhalflength)<1.e-10  && abs(GOnedge[0]-PhyVolGlPos[ijk][ij][0])<1.e-10 );//backside edge
	 break;
	 
		    
	          case 2: Onedge =  ( (GOnedge[1])<  (PhyVolGlPos[ijk][ij][1]+layhalflength+delta) &&  (GOnedge[1])>  (PhyVolGlPos[ijk][ij][1]-layhalflength-delta) && abs(GOnedge[2]-PhyVolGlPos[ijk][ij][2]+layhalfbreadth)<1.e-10  && abs(GOnedge[0]-PhyVolGlPos[ijk][ij][0])<1.e-10 );//bottomside edge
	 break;
	 
  case 3: Onedge =  ( (GOnedge[2])<  (PhyVolGlPos[ijk][ij][2]+layhalfbreadth+delta) &&  (GOnedge[2])>  (PhyVolGlPos[ijk][ij][2]-layhalfbreadth-delta) && abs(GOnedge[1]-PhyVolGlPos[ijk][ij][1]+layhalflength)<1.e-10  && abs(GOnedge[0]-PhyVolGlPos[ijk][ij][0])<1.e-10 );//frontside edge
	 break;
	 
	       }



	     }	     
	     else if(ijk==6) {
     switch(minedge){
	          case 0: Onedge =  ( (GOnedge[1])<  (PhyVolGlPos[ijk][ij][1]+layhalflength+delta) &&  (GOnedge[1])>  (PhyVolGlPos[ijk][ij][1]-layhalflength-delta) && abs(GOnedge[2]-PhyVolGlPos[ijk][ij][2]-layhalfbreadth)<1.e-10  && abs(GOnedge[0]-PhyVolGlPos[ijk][ij][0])<1.e-10 );//topside edge
	 break;
	 
	       
	          case 1: Onedge =  ( (GOnedge[2])<  (PhyVolGlPos[ijk][ij][2]+layhalfbreadth+delta) &&  (GOnedge[2])>  (PhyVolGlPos[ijk][ij][2]-layhalfbreadth-delta) && abs(GOnedge[1]-PhyVolGlPos[ijk][ij][1]+layhalflength)<1.e-10  && abs(GOnedge[0]-PhyVolGlPos[ijk][ij][0])<1.e-10 );//backside edge
	 break;
	 
		    
	          case 2: Onedge =  ( (GOnedge[1])<  (PhyVolGlPos[ijk][ij][1]+layhalflength+delta) &&  (GOnedge[1])>  (PhyVolGlPos[ijk][ij][1]-layhalflength-delta) && abs(GOnedge[2]-PhyVolGlPos[ijk][ij][2]+layhalfbreadth)<1.e-10  && abs(GOnedge[0]-PhyVolGlPos[ijk][ij][0])<1.e-10 );//bottomside edge
	 break;
	 
  case 3: Onedge =  ( (GOnedge[2])<  (PhyVolGlPos[ijk][ij][2]+layhalfbreadth+delta) &&  (GOnedge[2])>  (PhyVolGlPos[ijk][ij][2]-layhalfbreadth-delta) && abs(GOnedge[1]-PhyVolGlPos[ijk][ij][1]-layhalflength)<1.e-10  && abs(GOnedge[0]-PhyVolGlPos[ijk][ij][0])<1.e-10 );//frontside edge
	 break;
	 
	       }


	     }
	     
 
	    // layid = ijk+1;//28072022
	    // layid<<=2;
	    // layid+=ij;
 
	    // cout<<"layid: "<<layid<<endl;
	    // layexp->SetId(layid);



	     if(Onedge){
	    layexp->SetClosDist(minclosdist);
	    layexp->SetEdge(minedge);
	    layexp->SetDCAXPos(GOnedge[0]);//DCA: dist of closest app
	    layexp->SetDCAYPos(GOnedge[1]);
	    layexp->SetDCAZPos(GOnedge[2]);
	    layexp->Print();
	     }
 
	    //	 pAnalysis->distofclosapp[jk]=minclosdist;
	    //   }//  if (tmpcluster->GetPlane()-1==ijk && tmpcluster->GetLayer()==ij) {
	    //	 }// for (unsigned iji=0; iji<CmvCluster_pointer->CmvCluster_list.size(); iji++) {
	    cout<<".....Point outside the boundary...."<<endl;
	    cout<<"MinCloseDist:  "<<minclosdist<<" "<<hitpresent<<endl;
	    for (unsigned int ix=0; ix<CmvCluster_pointer->CmvCluster_list.size(); ix++) {
   
	      CmvCluster* tmpcluster = CmvCluster_pointer->CmvCluster_list[ix];    //#
	      //  tmpcluster->Print();
	      cout<<"## "<<tmpcluster->GetPlane()-1<<" "<<tmpcluster->GetLayer()<<endl;
	      if (tmpcluster->GetPlane()-1==ijk && tmpcluster->GetLayer()==ij) {//#plane  we have stored from 1 and in loop it is from 0
		hitpresent =true;
		cout<<"Extrapolation outside but hit present"<<endl;
     
	      }
   
	    }
	    cout<<"MinCloseDist:  "<<minclosdist<<" "<<hitpresent<<endl;
	    // 	       if(hitpresent==true && abs(minclosdist)<300){//300                 
	    // cout<<"hithit "<<hitpresent<<endl;
	    // if(ijk==0 && ij==0){pAnalysis->cmv_locno00[jk]=1; }                                                                       
	    // if(ijk==0 && ij==1){pAnalysis->cmv_locno01[jk]=1; }                                                                       
	    // if(ijk==0 && ij==2){pAnalysis->cmv_locno02[jk]=1; }        
	    // if(ijk==0 && ij==3){pAnalysis->cmv_locno03[jk]=1; }        
	    // if(ijk==1 && ij==0){pAnalysis->cmv_locno10[jk]=1; }        
	    // if(ijk==1 && ij==1){pAnalysis->cmv_locno11[jk]=1; }        
	    // if(ijk==1 && ij==2){pAnalysis->cmv_locno12[jk]=1; }        
	    // if(ijk==2 && ij==0){pAnalysis->cmv_locno20[jk]=1; }        
	    // if(ijk==2 && ij==1){pAnalysis->cmv_locno21[jk]=1; }        
	    // if(ijk==2 && ij==2){pAnalysis->cmv_locno22[jk]=1; }        
	    // if(ijk==3 && ij==0){pAnalysis->cmv_locno30[jk]=1; }        
	    // if(ijk==3 && ij==1){pAnalysis->cmv_locno31[jk]=1; }        
	    // if(ijk==3 && ij==2){pAnalysis->cmv_locno32[jk]=1; }        
	    // if(ijk==5 && ij==0){pAnalysis->cmv_locno50[jk]=1; }        
	    // if(ijk==5 && ij==1){pAnalysis->cmv_locno51[jk]=1; }        
	    // if(ijk==5 && ij==2){pAnalysis->cmv_locno52[jk]=1; }        
	    // if(ijk==6 && ij==0){pAnalysis->cmv_locno60[jk]=1; }        
	    // if(ijk==6 && ij==1){pAnalysis->cmv_locno61[jk]=1; }        
	    // if(ijk==6 && ij==2){pAnalysis->cmv_locno62[jk]=1; }        







	    // 	       }//               if(hitpresent==true && (minclosdist)<100){  
	       
	  }//	  else{

	  CmvLayExtra_pointer->CmvLayExtra_list.push_back(layexp);     
	  
        }//pl2

	else{

	  cout<<"..............Line not intersecting the plane......."<<endl;
	}
	
      }//layer loop
  
  
    }//loc_no loop

    cout<<"clustersize initialization last " <<pAnalysis->clustersize00[jk]<<" "<< pAnalysis->cmv_locno00[jk]<<" "<<endl ;
    cout<<pAnalysis->clustersize01[jk]<<" "<< pAnalysis->cmv_locno01[jk]<<" "<<endl;
    cout<<  pAnalysis->clustersize02[jk]<<" "<< pAnalysis->cmv_locno02[jk]<<" "<<endl;
    cout<<  pAnalysis->clustersize03[jk]<<" "<< pAnalysis->cmv_locno03[jk]<<" "<<endl;
    cout<<  pAnalysis->clustersize10[jk]<<" "<< pAnalysis->cmv_locno10[jk]<<" "<<endl;
    cout<<  pAnalysis->clustersize11[jk]<<" "<< pAnalysis->cmv_locno11[jk]<<" "<<endl;
    cout<<pAnalysis->clustersize12[jk]<<" "<< pAnalysis->cmv_locno12[jk]<<" "<<endl;
    cout<<pAnalysis->clustersize20[jk]<<" "<< pAnalysis->cmv_locno20[jk]<<" "<<endl;
    cout<<pAnalysis->clustersize21[jk]<<" "<< pAnalysis->cmv_locno21[jk]<<" "<<endl;
    cout<<  pAnalysis->clustersize22[jk]<<" "<< pAnalysis->cmv_locno22[jk]<<" "<<endl;
    cout<<pAnalysis->clustersize30[jk]<<" "<< pAnalysis->cmv_locno30[jk]<<" "<<endl;
    cout<<  pAnalysis->clustersize31[jk]<<" "<< pAnalysis->cmv_locno31[jk]<<" "<<endl;
    cout<<  pAnalysis->clustersize32[jk]<<" "<< pAnalysis->cmv_locno32[jk]<<" "<<endl;
    cout<<pAnalysis->clustersize50[jk]<<" "<< pAnalysis->cmv_locno50[jk]<<" "<<endl;
    cout<<  pAnalysis->clustersize51[jk]<<" "<< pAnalysis->cmv_locno51[jk]<<" "<<endl;
    cout<<  pAnalysis->clustersize52[jk]<<" "<< pAnalysis->cmv_locno52[jk]<<" "<<endl;
    cout<<pAnalysis->clustersize60[jk]<<" "<< pAnalysis->cmv_locno60[jk]<<" "<<endl;
    cout<<  pAnalysis->clustersize61[jk]<<" "<< pAnalysis->cmv_locno61[jk]<<" "<<endl;
    cout<<  pAnalysis->clustersize62[jk]<<" "<< pAnalysis->cmv_locno62[jk]<<" "<<endl;
  }

	
}










void CMVDRecoAlg::CreateCmvHit() {
  //   SipmHit_pointer = SipmHit_Manager::APointer; 
  CmvHit_pointer = new CmvHit_Manager();

  CmvHit_pointer->CmvHit_list.clear(); 

  // Convert SiPM hit to Cmv Hit
  // SipmHit* foursipm[4]={0}; //GMA memory leakage ?
  cout<<SipmHit_pointer<<endl;
  cout <<"micalEventAction::CreateCmvHit() "<<pAnalysis->isInOut<<" "<<SipmHit_pointer->SipmHit_list.size()<<endl;
  //  if (pAnalysis->isInOut==0 || pAnalysis->isInOut ==3 || pAnalysis->isInOut==5) {//0:GEN->RECO, 1:GEN->DIGI, 2:GEN->SIM, 3: SIM -> RECO, 4: SIM -> DIFI, 5 : DIGI -> RECO




    for (unsigned int ij=0; ij<SipmHit_pointer->SipmHit_list.size(); ij++) {
      //  if(SipmHit_pointer->SipmHit_list[ij]->GetPulse()<160) continue;
      SipmHit* foursipm[4]={0}; //GMA memory leakage ?

      int isfoursipm[4]={0};
      int tmpstripid = -1;
      int tmpside = -1; //Used this to find global position of the layer
      int tmplayer=-1;
      if (!(SipmHit_pointer->SipmHit_list[ij]->GetUsed())) {
	tmpstripid = SipmHit_pointer->SipmHit_list[ij]->GetStripId();
	cout <<"tmp "<< tmpstripid<<" "<<int(SipmHit_pointer->SipmHit_list[ij]->GetUsed())<<" "<<SipmHit_pointer->SipmHit_list[ij]->GetSiPM()<<endl;
        int isipm = SipmHit_pointer->SipmHit_list[ij]->GetSiPM();
        isfoursipm[isipm]=1;
	foursipm[isipm] = SipmHit_pointer->SipmHit_list[ij];
	SipmHit_pointer->SipmHit_list[ij]->SetUsed(true);
				
	tmpside = SipmHit_pointer->SipmHit_list[ij]->GetPlane()-1; //We had added 1 while storing it.
	tmplayer = SipmHit_pointer->SipmHit_list[ij]->GetLayer();

	cout<<tmpside+1<<" "<<tmplayer<<" "<<isipm<<endl;
	
	//Look for all other SiPM of same strip
	for (unsigned int jk=ij+1; jk<SipmHit_pointer->SipmHit_list.size(); jk++) {
	  // if(SipmHit_pointer->SipmHit_list[jk]->GetPulse()<160) continue;
	  if (!(SipmHit_pointer->SipmHit_list[jk]->GetUsed())) {	
	    int tmpstripid2 = SipmHit_pointer->SipmHit_list[jk]->GetStripId();
	    cout <<"tmp2 "<< tmpstripid2 <<endl;
	        int isipm2 = SipmHit_pointer->SipmHit_list[jk]->GetSiPM();
	    int tmpside2 = SipmHit_pointer->SipmHit_list[jk]->GetPlane()-1;
	    int tmplayer2 = SipmHit_pointer->SipmHit_list[jk]->GetLayer();

	cout<<tmpside2+1<<" "<<tmplayer2<<" "<<isipm2<<endl;
	if (tmpstripid !=tmpstripid2) continue; //this continues to next iteration
	
	////	if(tmpside !=tmpside2 || tmplayer !=tmplayer2) continue;
	int isipmm = SipmHit_pointer->SipmHit_list[jk]->GetSiPM();
	    cout<<"isipmm "<<isipm<<endl;
            isfoursipm[isipmm]=1;
	    foursipm[isipmm] = SipmHit_pointer->SipmHit_list[jk];
	    SipmHit_pointer->SipmHit_list[jk]->SetUsed(true);
	  }
	}
      }
      cout<<foursipm[0]<<" "<< foursipm[1]<< " "<< foursipm[2]<<" "<< foursipm[3]<<endl;
      cout<<"isfoursipm: "<< isfoursipm[0]<<" "<<isfoursipm[1]<<" "<<isfoursipm[2]<<" "<<isfoursipm[3]<<endl;
      if (tmpside>=0 && isfoursipm[0]+isfoursipm[1]+isfoursipm[2]+isfoursipm[3]>1) { //# atleast 2 sipms must have hit
	double pos[3];
	cout<<"posvec: ";
	for (int ix=0; ix<3; ix++) {pos[ix] = PhyVolGlPos[tmpside][tmplayer][ix];cout<<pos[ix]<<" ";}
	cout<<endl;

	cout<<"creating cmv hit "<<endl;
	if(foursipm[0]){	cout<<foursipm[0]->GetStripId()<<" "<<foursipm[0]->GetSiPM()<< endl;}
	if(foursipm[1]){	cout<<foursipm[1]->GetStripId()<<" "<<foursipm[1]->GetSiPM()<< endl;}
	if(foursipm[2]){	cout<<foursipm[2]->GetStripId()<<" "<<foursipm[2]->GetSiPM()<< endl;}
	if(foursipm[3]){	cout<<foursipm[3]->GetStripId()<<" "<<foursipm[3]->GetSiPM()<< endl;}
	CmvHit* tmpcmvHit = new CmvHit(foursipm[0], foursipm[1], foursipm[2], foursipm[3], pos); //valgrind RSA
	tmpcmvHit->Print();
	cout<<endl;		     
	CmvHit_pointer->CmvHit_list.push_back(tmpcmvHit);
      }
    }
  

  //Calling Save cmv hit data here
  //  if (pAnalysis->cmv_nhit >pAnalysis->cmv_nhtmx) pAnalysis->cmv_nhit =pAnalysis->cmv_nhtmx;

    CmvHit_Manager *pCmvHit = CmvHit_Manager::APointer;
  if(pCmvHit){
    
    if(pCmvHit->CmvHit_list.size()) {
      
      pAnalysis->pRootFile->cd();
      pAnalysis->cmv_nhit = pCmvHit->CmvHit_list.size();
      for (unsigned int ij=0; ij<pCmvHit->CmvHit_list.size() && ij<pAnalysis->cmv_nhit; ij++) {
	
	SaveCMVHitData(ij,pCmvHit->CmvHit_list[ij]);
	
      }
    }
  }

  
}  

// cmv hit



void CMVDRecoAlg::FormCmvCluster() {

  cout<<"Form Cmv_Cluster"<<endl;
 CmvCluster_pointer = new CmvCluster_Manager();
  CmvCluster_pointer->CmvCluster_list.clear(); 

  // Form cmv cluster

  cout <<"micalEventAction::CreateCmvHit() "<<pAnalysis->isInOut<<" "<<CmvHit_pointer->CmvHit_list.size()<<endl<<endl;
  // if (pAnalysis->isInOut==0 || pAnalysis->isInOut ==3 || pAnalysis->isInOut==5) {//0:GEN->RECO, 1:GEN->DIGI, 2:GEN->SIM, 3: SIM -> RECO, 4: SIM -> DIFI, 5 : DIGI -> RECO
    cout<<"CmvHit_list size: "<<CmvHit_pointer->CmvHit_list.size()<<endl;

    cout<<"dfdh"<<endl;

    for (unsigned int ijj=0; ijj<CmvHit_pointer->CmvHit_list.size(); ijj++) {

      CmvHit_pointer->CmvHit_list[ijj]->Print();
      cout<<endl;
    }


    for (unsigned int ij=0; ij<CmvHit_pointer->CmvHit_list.size(); ij++) {
      CmvHit* clust[2]={0}; //GMA memory leakage ?
          int tmpstrip = -1;
      int tmpside = -1; //Used this to find global position of the layer
      int tmplayer=-1;
       int isclust[2]={0};

       
      if (!(CmvHit_pointer->CmvHit_list[ij]->GetUsed())) {
	//   CmvHit* clust[2]={0}; //GMA memory leakage ?
  
    tmpside = CmvHit_pointer->CmvHit_list[ij]->GetPlane()-1; //Used this to find global position of the layer
       tmplayer=CmvHit_pointer->CmvHit_list[ij]->GetLayer();
       tmpstrip = CmvHit_pointer->CmvHit_list[ij]->GetStrip();
      cout<<ij<<" GetUsed  "<< CmvHit_pointer->CmvHit_list[ij]->GetUsed()<<endl;
      //   if (!(CmvHit_pointer->CmvHit_list[ij]->GetUsed())) {

      //	tmpside = CmvHit_pointer->CmvHit_list[ij]->GetPlane()-1;
      //	tmplayer = CmvHit_pointer->CmvHit_list[ij]->GetLayer();
      //	tmpstrip = CmvHit_pointer->CmvHit_list[ij]->GetStrip();
	cout<<"tmpside: "<<tmpside<<" tmplayer: "<<tmplayer<<" tmpstrip: "<<tmpstrip<<endl;
	clust[0]  = CmvHit_pointer->CmvHit_list[ij];
	 isclust[0] = 1;
	CmvHit_pointer->CmvHit_list[ij]->SetUsed(true);
	cout<<endl;
	//Look for neighboring strips in same layer and in same plane
	for (unsigned int jk=ij+1; jk<CmvHit_pointer->CmvHit_list.size(); jk++) {
	  if (!(CmvHit_pointer->CmvHit_list[jk]->GetUsed()) ) {
	      cout<<jk<<" GetUsed2  "<< CmvHit_pointer->CmvHit_list[jk]->GetUsed()<<endl;
	    int	tmpside2 = CmvHit_pointer->CmvHit_list[jk]->GetPlane()-1;
	int    tmplayer2 = CmvHit_pointer->CmvHit_list[jk]->GetLayer();
	    int tmpstrip2 = CmvHit_pointer->CmvHit_list[jk]->GetStrip();

	cout<<" tmpside2: "<<tmpside2<<" tmplayer2: "<<tmplayer2<<" tmpstrip2: "<<tmpstrip2<<endl;
	    
	 
	    
	if (tmpside != tmpside2 || tmplayer!=tmplayer2 || abs(tmpstrip-tmpstrip2)>1 )continue;


 clust[1] =  CmvHit_pointer->CmvHit_list[jk];
 isclust[1] = 1;
  CmvHit_pointer->CmvHit_list[jk]->SetUsed(true);

	  }
	}
      }
      
  if (tmpside>=0 && isclust[0]+isclust[1]>=1) { //#
    
    // cout<<"nearest neighbour found "<<tmpstrip<<" "<<tmpstrip2<<endl;
     
	      CmvCluster* tmpcmvCluster = new CmvCluster(clust[0],clust[1]);
	      tmpcmvCluster->Print();
	      cout<<endl;
	      //   if(tmplayer==0){
	      //  tmpcmvCluster->SetClustersize(2);//}
	      //   else if(tmplayer==1){  tmpcmvCluster->SetClustersizeL1(2);
  //  }
	      //    else if(tmplayer==2){  tmpcmvCluster->SetClustersizeL2(2);  }
	      CmvCluster_pointer->CmvCluster_list.push_back(tmpcmvCluster);


  }//  if (tmpside>=0 && isclust[0]+isclust[1]>=1) { //#
  

    } //  for (unsigned int ij=0; ij<CmvHit_pointer->CmvHit_list.size(); ij++) {
    
    cout<<endl;

    //... 

    
  
    cout<<"CmvCluster_list size"<<CmvCluster_pointer->CmvCluster_list.size()<<endl;
	for (unsigned int ab=0; ab<CmvCluster_pointer->CmvCluster_list.size(); ab++) {
	CmvCluster_pointer->CmvCluster_list[ab]->SetUsed(false);
CmvCluster_pointer->CmvCluster_list[ab]->Print();
	  cout<<endl;
	}


  //looping over clusters to combine 3 conscutive strip hits
 cout<<"........looping over cluster to combine 3 conscutive strip hits........"<<endl;
    for (unsigned int cd=0; cd<CmvCluster_pointer->CmvCluster_list.size(); cd++) {
     if (!(CmvCluster_pointer->CmvCluster_list[cd]->GetUsed())) {
      CmvCluster* clust2[2]={0}; //GMA memory leakage ?
   int isclust2[2]={0};
 
      cout<<cd<<" GetUsed  "<< CmvCluster_pointer->CmvCluster_list[cd]->GetUsed()<<endl;
     

int tmpside = CmvCluster_pointer->CmvCluster_list[cd]->GetPlane()-1;
int tmplayer = CmvCluster_pointer->CmvCluster_list[cd]->GetLayer();
int tmpstrip = CmvCluster_pointer->CmvCluster_list[cd]->GetStrip();
	cout<<"tmpside: "<<tmpside<<" tmplayer: "<<tmplayer<<" tmpstrip: "<<tmpstrip<<endl;
	clust2[0]  = CmvCluster_pointer->CmvCluster_list[cd];
	 isclust2[0] = 1;
	CmvCluster_pointer->CmvCluster_list[cd]->SetUsed(true);
	
	cout<<endl;

	//Look for neighboring strips in same layer and in same plane
	for (unsigned int dc=cd+1; dc<CmvCluster_pointer->CmvCluster_list.size(); dc++) {
	  if (!(CmvCluster_pointer->CmvCluster_list[dc]->GetUsed()) ) {
	      cout<<dc<<" GetUsed2  "<< CmvCluster_pointer->CmvCluster_list[dc]->GetUsed()<<endl;
	    int	tmpside2 = CmvCluster_pointer->CmvCluster_list[dc]->GetPlane()-1;
	int    tmplayer2 = CmvCluster_pointer->CmvCluster_list[dc]->GetLayer();
	    int tmpstrip2 = CmvCluster_pointer->CmvCluster_list[dc]->GetStrip();

	cout<<" tmpside2: "<<tmpside2<<" tmplayer2: "<<tmplayer2<<" tmpstrip2: "<<tmpstrip2<<endl;
	    
	 
	    
	if (tmpside == tmpside2 && tmplayer==tmplayer2 && abs(tmpstrip-tmpstrip2)==2 ){

	  cout<<"Found 3 consecutive hits.."<<endl;
 clust2[1] =  CmvCluster_pointer->CmvCluster_list[dc];

	  CmvCluster_pointer->CmvCluster_list[cd]->CombineClusts(clust2[1]);

	CmvCluster_pointer->CmvCluster_list.erase(CmvCluster_pointer->CmvCluster_list.begin()+dc);
	//	CmvCluster_pointer->CmvCluster_list.erase(CmvCluster_pointer->CmvCluster_list.begin()+cd);
	dc--;
	////	cd--;

	
	}
	  }
	}
      }

    } //  for (unsigned int cd=0; cd<CmvCluster_pointer->CmvCluster_list.size(); cd++) {
    
    cout<<endl;

    cout<<"CmvCluster_list size"<<CmvCluster_pointer->CmvCluster_list.size()<<endl;
    for (unsigned int ab=0; ab<CmvCluster_pointer->CmvCluster_list.size(); ab++) {
      CmvCluster_pointer->CmvCluster_list[ab]->SetUsed(false);
      CmvCluster_pointer->CmvCluster_list[ab]->Print();
      cout<<endl;
    }
    
    
    

    // } //  if (pAnalysis->isInOut==0 || pAnalysis->isInOut ==3 || pAnalysis->isInOut==5) {	



  //Calling Save cluster  data here
   CmvCluster_Manager *pCmvClust = CmvCluster_Manager::APointer;
  if(pCmvClust){
    
    if(pCmvClust->CmvCluster_list.size()) {
      
      pAnalysis->pRootFile->cd();
      pAnalysis->cmv_nclust = pCmvClust->CmvCluster_list.size();
      for (unsigned int ij=0; ij<pCmvClust->CmvCluster_list.size(); ij++) {
	
	SaveCMVClusterData(ij,pCmvClust->CmvCluster_list[ij]);
	
      }
    }
  }



}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void CMVDRecoAlg::SaveCMVHitData(unsigned int hits,CmvHit* pCmvHit ) {

 
  //  cout<<"  pCmvHit->CmvHit_list.size():  "<<  pCmvHit->CmvHit_list.size()<<endl;
  //  cout <<"  pAnalysis->cmv_nclusthit "<<  pAnalysis->cmv_nhit<<endl;
  
  unsigned int hitId =0;
  
    hitId = pCmvHit->GetPlane();
    hitId<<=2;
    hitId+=pCmvHit->GetLayer();
    hitId<<=7;
    hitId+=pCmvHit->GetStrip();
    hitId<<=2; //just shifted by 2 as we did this while forming strip	
    //pAnalysis->cmv_clustpdgid[hits] =pCmvHit->CmvHit_list[hits]->GetpdgId();
    
    cout<<"clustid "<<hitId<<" "<<endl;
    
    pAnalysis->cmv_hitid[hits] =hitId;
    
    cout<< pAnalysis->cmv_hitid[hits]<<endl;
    
    pAnalysis->cmv_hitLeTim[hits] =pCmvHit->GetLeTime();
    pAnalysis->cmv_hitRiTim[hits] =pCmvHit->GetRiTime();
    
    pAnalysis->cmv_hitLePul[hits] =pCmvHit->GetLePulse();
    pAnalysis->cmv_hitRiPul[hits] =pCmvHit->GetRiPulse();
    
    
    pAnalysis->cmv_hitTrueposx[hits] =pCmvHit->GetTruePosX();
    pAnalysis->cmv_hitTrueposy[hits] =pCmvHit->GetTruePosY();
    pAnalysis->cmv_hitTrueposz[hits] =pCmvHit->GetTruePosZ();
    
    pAnalysis->cmv_hitRecoposx[hits] =pCmvHit->GetRecoPosX();
    pAnalysis->cmv_hitRecoposy[hits] =pCmvHit->GetRecoPosY();
    pAnalysis->cmv_hitRecoposz[hits] =pCmvHit->GetRecoPosZ();
    
    //	pAnalysis->cmv_clustsiz[hits] =pCmvHit->GetHitsize();
    // pAnalysis->cmv_clustmom[hits] =pCmvHit->GetSimMom();
    // pAnalysis->cmv_clustthe[hits] =pCmvHit->GetSimThe();
    // pAnalysis->cmv_clustphi[hits] =pCmvHit->GetSimPhi();
      
  //			pAnalysis->pEventTree->Fill();
    
}


//..................

void CMVDRecoAlg::SaveCMVClusterData(unsigned int clusthits,CmvCluster* pCmvCluster ) {
 
  //  cout<<"  pCmvCluster->CmvCluster_list.size():  "<<  pCmvCluster->CmvCluster_list.size()<<endl;
  //  cout <<"  pAnalysis->cmv_nclusthit "<<  pAnalysis->cmv_nclust<<endl;
  
  unsigned int clustId =0;
  
    clustId = pCmvCluster->GetPlane();

    clustId<<=2;
    clustId+=pCmvCluster->GetLayer();
    clustId<<=7;
    clustId+=pCmvCluster->GetStrip();
    clustId<<=2; //just shifted by 2 as we did this while forming strip	
    //pAnalysis->cmv_clustpdgid[clusthits] =pCmvCluster->GetpdgId();
    
    cout<<"clustid "<<clustId<<" "<<endl;
    
    pAnalysis->cmv_clustid[clusthits] =clustId;
    
    cout<< pAnalysis->cmv_clustid[clusthits]<<endl;
    
    pAnalysis->cmv_clustLeTim[clusthits] =pCmvCluster->GetLeTime();
    pAnalysis->cmv_clustRiTim[clusthits] =pCmvCluster->GetRiTime();
    
    pAnalysis->cmv_clustLePul[clusthits] =pCmvCluster->GetLePulse();
    pAnalysis->cmv_clustRiPul[clusthits] =pCmvCluster->GetRiPulse();
    
    
    pAnalysis->cmv_clustTrueposx[clusthits] =pCmvCluster->GetTruePosX();
    pAnalysis->cmv_clustTrueposy[clusthits] =pCmvCluster->GetTruePosY();
    pAnalysis->cmv_clustTrueposz[clusthits] =pCmvCluster->GetTruePosZ();
    
    pAnalysis->cmv_clustRecoposx[clusthits] =pCmvCluster->GetRecoPosX();
    pAnalysis->cmv_clustRecoposy[clusthits] =pCmvCluster->GetRecoPosY();
    pAnalysis->cmv_clustRecoposz[clusthits] =pCmvCluster->GetRecoPosZ();


       
    	pAnalysis->cmv_clustsiz[clusthits] =pCmvCluster->GetClustersize();
    // pAnalysis->cmv_clustmom[clusthits] =pCmvCluster->GetSimMom();
    // pAnalysis->cmv_clustthe[clusthits] =pCmvCluster->GetSimThe();
    // pAnalysis->cmv_clustphi[clusthits] =pCmvCluster->GetSimPhi();
    
    
   
  
  //			pAnalysis->pEventTree->Fill();
  


  
}

void CMVDRecoAlg::SaveCMVExtrapData(unsigned int extrap , CmvLayExtra* pCmvLayExtra){


  //	pCmvLayExtra->CmvLayExtra_list[extrap]->Print();
		
	//pAnalysis->cmv_clustpdgid[extrap] =pCmvLayExtra->GetpdgId();


        pAnalysis->cmv_expid[extrap] =pCmvLayExtra->GetId();

	cout<< pAnalysis->cmv_hitid[extrap]<<endl;
	pAnalysis->cmv_Expposx[extrap] =pCmvLayExtra->GetExtXPos();
	pAnalysis->cmv_Expposy[extrap] =pCmvLayExtra->GetExtYPos();
	pAnalysis->cmv_Expposz[extrap] =pCmvLayExtra->GetExtZPos();
	pAnalysis->distofclosapp[extrap]=pCmvLayExtra->GetClosDist();

	
	//	if (extrap >=pAnalysis->cmv_nexphtmx) break; //redundant
      
      //			pAnalysis->pEventTree->Fill();


}


//




