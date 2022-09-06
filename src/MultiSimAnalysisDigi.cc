#include "MultiSimAnalysisDigi.hh"

MultiSimAnalysisDigi *MultiSimAnalysisDigi::AnPointer;

MultiSimAnalysisDigi::MultiSimAnalysisDigi() {
  AnPointer = this;
  paradef = micalDetectorParameterDef::AnPointer;
  CardFile = ParameterMessenger::AnPointer;
  //  DetectorType = CardFile->GetDetectorType();

 h2dDimX = paradef->GetParino(0);
  h2dDimY = paradef->GetParino(1);
   numberInLA = paradef->GetnLayer();
  cout<<"numberInLA "<<numberInLA<<endl;
  nbinxMag2d = (int)(h2dDimX/10);
  nbinyMag2d = (int)(h2dDimY/10);

  magZmax = 2*(paradef->GetParirlay(2) + paradef->GetParlay(2));
  nbinxMagZ = (int)(magZmax);
  CardFile->PrintParameters();
  isInOut = CardFile->GetInputOutput();
  isVisOut = CardFile->GetVisualOutput();
  isXtermOut = CardFile->GetXTermOutput();
  collatedIn = CardFile->GetCollatedIn();

  nmxhit	= 10000;
  ievt		= 0;
  ihist		= 0;
  ievt_wt=0;
  intxn_id=0;

  CorrTimeError = CardFile->GetCorrTimeSmr();
  UnCorrTimeError = CardFile->GetUnCorrTimeSmr();
  TimeToDigiConv = CardFile->GetTimeToDigiConv();
  // cout<<"Hellllllllllll"<<TimeToDigiConv<<endl;
  SignalSpeed = CardFile->GetSignalSpeed();

  H	=0;
  Hp	=0;

  pRootFile	=0;
  inputRootFile=0;
  pVisFile = 0;
  pEventTree	=0;
  inputEventTree=0;
  visTree = 0;

  // double pPosLimXY = 52.5;
  // double pPosLimZ = 2.5;

  double pPosLimXY = 105;
  double pPosLimZ = 20.5;
  cout<<"checck2"<<endl;
  pPosX = new TH1F("deltaX", "#Delta X (cm)", 105, -pPosLimXY, pPosLimXY);
  pPosY = new TH1F("deltaY", "#Delta Y (cm)", 105, -pPosLimXY, pPosLimXY);
  pPosZ = new TH1F("deltaZ", "#Delta Z (cm)", 100, -pPosLimZ, pPosLimZ);

  char title[100];
  for (int ij=0; ij<20; ij++) {
    sprintf(title, "dedz_%i", ij);
      pdedz[ij] = new TH1F(title, title, 4000, -600., 600.);
  }

  pPosXX = new TH2F("deltaXX", "#Delta XX (cm)", 100, -2500, 2500, 100, -pPosLimXY, pPosLimXY);
  pPosYY = new TH2F("deltaYY", "#Delta YY (cm)", 100, -500, 500, 100, -pPosLimXY, pPosLimXY);
  pPosZZ = new TH2F("deltaZZ", "#Delta ZZ (cm)", 100, -1000, 1000, 100, -pPosLimZ, pPosLimZ);

  hitDist  = new TH1F("HitDist","This is the total rawhit distribution across layers",182,0,182);     //asm
  TrkDist  = new TH1F("TrkDist","This is the total track-hit distribution across layers",182,0,182);     //asm
  EffDist  = new TH1F("EffDist","This is the efficiency distribution across layers",182,0,182);     //asm

  InoTrack_listsize =new TH1F("TrksizeDist","Distribution of tracks per event ",10,0,10); //asm

  ShwXw	= new TH1F("ShwXw ","This is a distribution for X position resolution in shower region " ,80 , -3.14 , 3.14);  //asm
  ShwYw	= new TH1F("ShwYw ","This is a distribution for Y position resolution in shower region " ,80 , -3.14 , 3.14);  //asm
  RC		= new TH2F("RCvsff","This is a distribution for raduis of curvature at the edges " ,1000 , 0 , 1000,100,-50,50);  //asm
  DGap	= new TH1F("DetGap"," Plot to see the performance of code at detgap ",10,0,10);  //asm
  //  nhtcal1 = 0;
  //  nhtcal2 = 0;

  cout <<"booooooooooooooooooooooooooooooooooooooooooooooked "<<endl;

  DeadStripX = new TH1F("DeadStripX","Plot to see dead strips on X-plane",64,-0.5,63.5);
  NoisyStripX = new TH1F("NoisyStripX","Plot to see noisy strips on X-Plane",64,-0.5,63.5);
  DeadStripY = new TH1F("DeadStripY","Plot to see dead strips on Y-Plane",64,-0.5,63.5);
  NoisyStripY = new TH1F("NoisyStripY","Plot to see noisy strips on Y-Plane",64,-0.5,63.5);
  DiffTime = new TH1F("DiffTime","DiffTime",100,-100.,100.);

  strpXtime = new TH1F("strpXtime","xsmrtime - xtruetime",100,-40,40.);
  strpYtime = new TH1F("strpYtime","ysmrtime - ytruetime",100,-40,40.);
  strpXtimeCorr = new TH1F("strpXtimeCorr","xsmrtimeCorr - xtruetime",100,-20,20.);
  strpYtimeCorr = new TH1F("strpYtimeCorr","ysmrtimeCorr - ytruetime",100,-20,20.);
  hitXtime = new TH1F("hitXtime","hittime - xtruetime",100,-20,20.);
  hitYtime = new TH1F("hitYtime","hittime - ytruetime",100,-20,20.);

  smagFieldX = new TH1D("smagfieldinx","magfieldinx simu",nbinxMagZ,0,magZmax);
  smagFieldY = new TH1D("smagfieldiny","magfieldiny simu",nbinxMagZ,0,magZmax);
  smag2dX = new TH2D("smag2dinx","magfieldinx 2D simu",nbinxMagZ,0,magZmax,180,-2.0,2.0);
  smag2dY = new TH2D("smag2diny","magfieldiny 2D simu",nbinxMagZ,0,magZmax,180,-2.0,2.0);

  rmagFieldX = new TH1D("rmagfieldinx","magfieldinx in reco",nbinxMagZ,0,magZmax);
  rmagFieldY = new TH1D("rmagfieldiny","magfieldiny in reco",nbinxMagZ,0,magZmax);
  rmag2dX = new TH2D("rmag2dinx","magfieldinx 2D in reco",nbinxMagZ,0,magZmax,180,-2.0,2.0);
  rmag2dY = new TH2D("rmag2diny","magfieldiny 2D in reco",nbinxMagZ,0,magZmax,180,-2.0,2.0);
  // int nbin2d = 1600;
  smag2dXYpixel_iron = new TH2D("smag2dxypixeliron","magfield in xy pixel in sim in iron",nbinxMag2d,-h2dDimX,h2dDimX,nbinyMag2d,-h2dDimY,h2dDimY);
  smag2dXYpixel_air = new TH2D("smag2dxypixelair","magfield in xy pixel in sim in air",nbinxMag2d,-h2dDimX,h2dDimX,nbinyMag2d,-h2dDimY,h2dDimY);
  rmag2dXYpixel_iron = new TH2D("rmag2dxypixeliron","magfield in xy pixel in reco in iron",nbinxMag2d,-h2dDimX,h2dDimX,nbinyMag2d,-h2dDimY,h2dDimY);
  rmag2dXYpixel_air = new TH2D("rmag2dxypixelair","magfield in xy pixel in reco in air",nbinxMag2d,-h2dDimX,h2dDimX,nbinyMag2d,-h2dDimY,h2dDimY);

  xyvsbxin = new TH2D("xyvsbxin", "xyvsbxin", 321, -8025, 8025, 321, -8025, 8025);
  xyvsbyin = new TH2D("xyvsbyin", "xyvsbyin", 321, -8025, 8025, 321, -8025, 8025);
  xyvsbxdiff = new TH2D("xyvsbxdiff", "xyvsbxdiff", 321, -8025, 8025, 321, -8025, 8025);
  xyvsbydiff = new TH2D("xyvsbydiff", "xyvsbydiff", 321, -8025, 8025, 321, -8025, 8025);
  // xyvsbxindiff = new TH2D("xyvsbxindiff", "xyvsbxindiff", 319, -7975, 7975, 319, -7975, 7975);
  // xyvsbyindiff = new TH2D("xyvsbyindiff", "xyvsbyindiff", 319, -7975, 7975, 319, -7975, 7975);

   xyvsbxout = new TH2D("xyvsbxout", "xyvsbxout", 969, -24225, 24225, 321, -8025, 8025);
   xyvsbyout =  new TH2D("xyvsbyout", "xyvsbyout", 969, -24225, 24225, 321, -8025, 8025);

  //G4int hit_wo_ghst;//SSE
  //hprof  = new TProfile("hprof","Profile",2,0,100,0.,1000.);//SSE
 // hh_E 	= new TH2D("hh_E","E_had vs orighits;orighits;E_had (GeV)",25, 0. ,50.,100, 0., 100.);  //SSE
  //hh_woghst_E 	= new TH2D("hh_woghst_E","E_had vs hits;hits;E_had (GeV)",50, 0. ,500.,100, 0., 100.);  //SSE
  //hist_nhits_LargestCluster_E 	= new TH2D("hist_nhits_LargestCluster_E","E_had vs no of hits in largest cluster;hits;E_had (GeV)",50, 0. ,500.,100, 0., 100.);  //SSE
  //hist_orighits_new_E= new TH2D("hist_orighits_new_E","E_had vs orighits (new) ;orighits_new;E_had (GeV)",50, 0. ,100.,100, 0., 100.);//SSE 09/15
 //hist_orighits_mod_E= new TH2D("hist_orighits_mod_E","E_had vs orighits (modified) ;orighits_mod;E_had (GeV)",25, 0. ,50.,100, 0., 100.);//SSE 09/15
 //hist_wogh_orighits_E= new TH2D("hist_wogh_orighits_E","E_had vs wogh_orighits ;Ghosthit_removed_orighit;E_had (GeV)",25, 0. ,50.,100, 0., 100.);//SSE 09/15
   cout<<"checck2"<<endl;


}

MultiSimAnalysisDigi::~MultiSimAnalysisDigi() {
  cout<<"Deleting MultiSimAnalysisDigi class pointer ..."<<endl;
}

void MultiSimAnalysisDigi::OpenInputRootFiles(char* infile) {

  char inRootFile[400];

  cout << " opening root file " << infile << " inout " <<  isInOut << endl;

  if(isInOut==0 ||isInOut==3 ) { ////inout3
    //Input is sim file from MC for sim to reco(3) or sim to digi(0)
    sprintf(inRootFile,"%s",infile);
    inputRootFile = new TFile(inRootFile, "read");
    cout<< "Data is read from simulation output rootfile : "<< inRootFile <<endl;

    inputEventTree = (TTree*)inputRootFile->Get("T3"); //T1
    inputEventTree->SetBranchAddress("irun",&irun);
    inputEventTree->SetBranchAddress("ievt",&ievt);
    inputEventTree->SetBranchAddress("ngent",&ngent);
    inputEventTree->SetBranchAddress("pidin",pidin);
    inputEventTree->SetBranchAddress("momin",momin);
    inputEventTree->SetBranchAddress("thein",thein);
    inputEventTree->SetBranchAddress("phiin",phiin);
    inputEventTree->SetBranchAddress("posxin",posxin);
    inputEventTree->SetBranchAddress("posyin",posyin);
    inputEventTree->SetBranchAddress("poszin",poszin);
    inputEventTree->SetBranchAddress("nsimht", &nsimht);
    inputEventTree->SetBranchAddress("detid", detid);
    inputEventTree->SetBranchAddress("simpdgid", simpdgid);
    inputEventTree->SetBranchAddress("simtime", simtime);
    inputEventTree->SetBranchAddress("simenr", simenr);
    inputEventTree->SetBranchAddress("simvx", simvx);
    inputEventTree->SetBranchAddress("simvy", simvy);
    inputEventTree->SetBranchAddress("simvz", simvz);
    inputEventTree->SetBranchAddress("simpx", simpx);
    inputEventTree->SetBranchAddress("simpy", simpy);
    inputEventTree->SetBranchAddress("simpz", simpz);
    inputEventTree->SetBranchAddress("simlocvx", simlocvx);
    inputEventTree->SetBranchAddress("simlocvy", simlocvy);
    inputEventTree->SetBranchAddress("ngenerated", &ngenerated);
    inputEventTree->SetBranchAddress("naperture", &naperture);
    if(CardFile->GetCMVD()==1){
  //cmv
    inputEventTree->SetBranchAddress("cmv_nsimhit", &cmv_nsimhit);
     inputEventTree->SetBranchAddress("cmv_detid", cmv_detid) ;
    inputEventTree->SetBranchAddress("cmv_simpdgid", cmv_simpdgid);
    inputEventTree->SetBranchAddress("cmv_simtime", cmv_simtime);
    inputEventTree->SetBranchAddress("cmv_simenr", cmv_simenr);
    inputEventTree->SetBranchAddress("cmv_simposx", cmv_simposx);
    inputEventTree->SetBranchAddress("cmv_simposy", cmv_simposy);
    inputEventTree->SetBranchAddress("cmv_simposz", cmv_simposz);
    inputEventTree->SetBranchAddress("cmv_simpx", cmv_simpx);
    inputEventTree->SetBranchAddress("cmv_simpy", cmv_simpy);
    inputEventTree->SetBranchAddress("cmv_simpz", cmv_simpz);
    inputEventTree->SetBranchAddress("cmv_simlocx", cmv_simlocx);
    inputEventTree->SetBranchAddress("cmv_simlocy", cmv_simlocy);
 inputEventTree->SetBranchAddress("cmv_simlocz", cmv_simlocz);
    }// if(CardFile->GetCMVD()==1){
  } else if(isInOut==1) {
    sprintf(inRootFile,"%s",infile);
    inputRootFile = new TFile(inRootFile, "read");
    cout<< "Data is read from digitization output file : "<< inRootFile  <<endl;

    inputEventTree = (TTree*)inputRootFile->Get("T2"); //T2
    inputEventTree->SetBranchAddress("irun",&irun);
    inputEventTree->SetBranchAddress("ievt",&ievt);
    inputEventTree->SetBranchAddress("ngent",&ngent);
    inputEventTree->SetBranchAddress("pidin",pidin);
    inputEventTree->SetBranchAddress("momin",momin);
    inputEventTree->SetBranchAddress("thein",thein);
    inputEventTree->SetBranchAddress("phiin",phiin);
    inputEventTree->SetBranchAddress("posxin",posxin);
    inputEventTree->SetBranchAddress("posyin",posyin);
    inputEventTree->SetBranchAddress("poszin",poszin);
    inputEventTree->SetBranchAddress("ndigiht", &ndigiht);
    inputEventTree->SetBranchAddress("trigx", &trigx);
    inputEventTree->SetBranchAddress("trigy", &trigy);
    inputEventTree->SetBranchAddress("ngenerated", &ngenerated);
    inputEventTree->SetBranchAddress("naperture", &naperture);
    //    inputEventTree->SetBranchAddress("triggeracceptance", &triggeracceptance);
    inputEventTree->SetBranchAddress("stripid", stripid);
    inputEventTree->SetBranchAddress("digipdgid", digipdgid);
    inputEventTree->SetBranchAddress("digitime", digitime);
    inputEventTree->SetBranchAddress("digitruetime", digitruetime);
    inputEventTree->SetBranchAddress("digienr", digienr);
    inputEventTree->SetBranchAddress("digivx", digivx);
    inputEventTree->SetBranchAddress("digivy", digivy);
    inputEventTree->SetBranchAddress("digivz", digivz);
    inputEventTree->SetBranchAddress("digipx", digipx);
    inputEventTree->SetBranchAddress("digipy", digipy);
    inputEventTree->SetBranchAddress("digipz", digipz);
    // inputEventTree->SetBranchAddress("diginoise", diginoise);
       //cmv
     if(CardFile->GetCMVD()==1){
    inputEventTree->SetBranchAddress("cmv_ndigihit", &cmv_ndigihit);
    inputEventTree->SetBranchAddress("cmv_sipmid", cmv_sipmid);
    inputEventTree->SetBranchAddress("cmv_digipdgid", cmv_digipdgid);
    inputEventTree->SetBranchAddress("cmv_digitimpul", cmv_digitimpul);
    inputEventTree->SetBranchAddress("cmv_digiposx", cmv_digiposx);
    inputEventTree->SetBranchAddress("cmv_digiposy", cmv_digiposy);
    inputEventTree->SetBranchAddress("cmv_digiposz", cmv_digiposz);
    inputEventTree->SetBranchAddress("cmv_digimom", cmv_digimom);
    inputEventTree->SetBranchAddress("cmv_digithe", cmv_digithe);
    inputEventTree->SetBranchAddress("cmv_digiphi", cmv_digiphi);
    inputEventTree->SetBranchAddress("cmv_digilocx", cmv_digilocx);
    inputEventTree->SetBranchAddress("cmv_digilocy", cmv_digilocy);
        inputEventTree->SetBranchAddress("cmv_digilocz", cmv_digilocz);
    //cmv
     }// if(CardFile->GetCMVD()==1){
  } else if(isInOut==2) { // datareading
    sprintf(inRootFile,"%s",infile);
    inputRootFile = new TFile(inRootFile, "read");
    cout<< "Data is read from data file : "<< inRootFile  <<endl;

    inputEventTree = (TTree*)inputRootFile->Get("evetree");
    data_event = new RPCEve(inputEventTree);
    // data_event = new evetree(inputEventTree);
    data_event->Loop();

  }

}

void MultiSimAnalysisDigi::OpenOutputRootFiles(char* outfile) {

  cout << " OpenOutputRootFiles " << outfile << " inout " <<  isInOut << endl;

  char outRootFile[400];
  sprintf(outRootFile,"%s.root",outfile);


    pRootFile = new TFile(outRootFile, "RECREATE");
    if (!pRootFile) {
      cout << "Error opening output root file !" << endl;
      exit(-1);
    } else {
      cout<< "Output stored in root file: "<< outRootFile <<endl;
    }

    if (isInOut==0 ){
      pEventTree = new TTree("T2", "INODIGI");
    }else if (isInOut==1||isInOut==3 ){
      pEventTree = new TTree("T1", "INORECO");
    } else if(isInOut==2) {
      pEventTree = new TTree("T4", "INODATA");
 }

    //These are common to all:

    pEventTree->Branch("irun",&irun,"irun/i"); //VALGRIND
   pEventTree->Branch("ievt",&ievt2,"ievt/i");
  pEventTree->Branch("ngent",&ngent,"ngent/i");
    pEventTree->Branch("pidin",pidin,"pidin[ngent]/I");
    pEventTree->Branch("ievt_wt",&ievt_wt,"ievt_wt/F");
    pEventTree->Branch("intxn_id",&intxn_id,"intxn_id/I");
    pEventTree->Branch("momin",momin,"momin[ngent]/F");
    pEventTree->Branch("thein",thein,"thein[ngent]/F");
    pEventTree->Branch("phiin",phiin,"phiin[ngent]/F");
    pEventTree->Branch("posxin",posxin,"posxin[ngent]/F");
    pEventTree->Branch("posyin",posyin,"posyin[ngent]/F");
    pEventTree->Branch("poszin",poszin,"poszin[ngent]/F");
    pEventTree->Branch("ngenerated",&ngenerated,"ngenerated/i");
    pEventTree->Branch("naperture",&naperture,"naperture/i");


    //DigiOutput
     if (isInOut==0 || isInOut==3  ){
    pEventTree->Branch("ndigiht", &ndigiht, "ndigiht/i");
    pEventTree->Branch("trigx", &trigx, "trigx/I");
    pEventTree->Branch("trigy", &trigy, "trigy/I");

    pEventTree->Branch("triggeracceptance",&triggeracceptance,"triggeracceptance/i");
    pEventTree->Branch("stripid", stripid, "stripid[ndigiht]/i");
    pEventTree->Branch("digipdgid", digipdgid, "digipdgid[ndigiht]/I");
    pEventTree->Branch("digitime", digitime, "digitime[ndigiht]/I");
    pEventTree->Branch("digitruetime", digitruetime, "digitruetime[ndigiht]/I");
    pEventTree->Branch("digienr", digienr, "digienr[ndigiht]/F");
    pEventTree->Branch("digivx", digivx, "digivx[ndigiht]/F");
    pEventTree->Branch("digivy", digivy, "digivy[ndigiht]/F");
    pEventTree->Branch("digivz", digivz, "digivz[ndigiht]/F");
    pEventTree->Branch("digipx", digipx, "digipx[ndigiht]/F");
    pEventTree->Branch("digipy", digipy, "digipy[ndigiht]/F");
    pEventTree->Branch("digipz", digipz, "digipz[ndigiht]/F");

       cout<<"check4"<<endl;
     if(CardFile->GetCMVD()==1){
       //cmv
    pEventTree->Branch("cmv_ndigihit", &cmv_ndigihit, "cmv_ndigihit/i");
    pEventTree->Branch("cmv_sipmid", cmv_sipmid, "cmv_sipmid[cmv_ndigihit]/i") ;
    pEventTree->Branch("cmv_digipdgid", cmv_digipdgid, "cmv_digipdgid[cmv_ndigihit]/I");
    pEventTree->Branch("cmv_digitimpul", cmv_digitimpul, "cmv_digitimpul[cmv_ndigihit]/i");

    pEventTree->Branch("cmv_digiposx", cmv_digiposx, "cmv_digiposx[cmv_ndigihit]/F");
    pEventTree->Branch("cmv_digiposy", cmv_digiposy, "cmv_digiposy[cmv_ndigihit]/F");
    pEventTree->Branch("cmv_digiposz", cmv_digiposz, "cmv_digiposz[cmv_ndigihit]/F");

    pEventTree->Branch("cmv_digimom", cmv_digimom, "cmv_digimom[cmv_ndigihit]/F");
    pEventTree->Branch("cmv_digithe", cmv_digithe, "cmv_digithe[cmv_ndigihit]/F");
    pEventTree->Branch("cmv_digiphi", cmv_digiphi, "cmv_digiphi[cmv_ndigihit]/F");

    pEventTree->Branch("cmv_digilocx", cmv_digilocx, "cmv_digilocx[cmv_ndigihit]/F");
    pEventTree->Branch("cmv_digilocy", cmv_digilocy, "cmv_digilocy[cmv_ndigihit]/F");
    pEventTree->Branch("cmv_digilocz", cmv_digilocz, "cmv_digilocz[cmv_ndigihit]/F");
     }// if(CardFile->GetCMVD()==1){

     }//     if (isInOut==0 || isInOut==3  ){


        if (isInOut){

    pEventTree->Branch("hw_trig",&hw_trig,"hw_trig/I");
    // pEventTree->Branch("hw_trigy",&hw_trigy,"hw_trigy/I");
    pEventTree->Branch("sw_trigx",&sw_trigx,"sw_trigx/I");
    pEventTree->Branch("sw_trigy",&sw_trigy,"sw_trigy/I");
    pEventTree->Branch("ntrkt",&ntrkt,"ntrkt/i");
    pEventTree->Branch("itype",itype,"itype[ntrkt]/I");
    pEventTree->Branch("nLayer",&nLayer,"nLayer/I");
    pEventTree->Branch("nhits",nhits,"nhits[ntrkt]/I");
    pEventTree->Branch("nhits_finder",nhits_finder,"nhits_finder[ntrkt]/I");
    pEventTree->Branch("chisq",chisq,"chisq[ntrkt]/F");
      pEventTree->Branch("chisq2",chisq2,"chisq2[ntrkt]/F");
    pEventTree->Branch("cvalue",cvalue,"cvalue[ntrkt]/F");
    pEventTree->Branch("trkmm",trkmm,"trkmm[ntrkt]/F");
    pEventTree->Branch("trkth",trkth,"trkth[ntrkt]/F");
    pEventTree->Branch("trkph",trkph,"trkph[ntrkt]/F");
    pEventTree->Branch("momvx",momvx,"momvx[ntrkt]/F");
    pEventTree->Branch("thevx",thevx,"thevx[ntrkt]/F");
    pEventTree->Branch("phivx",phivx,"phivx[ntrkt]/F");
    pEventTree->Branch("posxvx",posxvx,"posxvx[ntrkt]/F");
    pEventTree->Branch("posyvx",posyvx,"posyvx[ntrkt]/F");
    pEventTree->Branch("poszvx",poszvx,"poszvx[ntrkt]/F");

    pEventTree->Branch("momrf",momrf,"momrf[ntrkt]/F");
    pEventTree->Branch("therf",therf,"therf[ntrkt]/F");
    pEventTree->Branch("phirf",phirf,"phirf[ntrkt]/F");
    pEventTree->Branch("posxrf",posxrf,"posxrf[ntrkt]/F");
    pEventTree->Branch("posyrf",posyrf,"posyrf[ntrkt]/F");
    pEventTree->Branch("poszrf",poszrf,"poszrf[ntrkt]/F");



    //added for straightline fit:raj
    pEventTree->Branch("XPosdev_exclu",XPosdev_exclu,"XPosdev_exclu[nLayer]/F");
    pEventTree->Branch("YPosdev_exclu",YPosdev_exclu,"YPosdev_exclu[nLayer]/F");

    pEventTree->Branch("XPosdev",XPosdev,"XPosdev[10]/F");
    pEventTree->Branch("YPosdev",YPosdev,"YPosdev[10]/F");



    pEventTree->Branch("momend",momend,"momend[ntrkt]/F");
    pEventTree->Branch("theend",theend,"theend[ntrkt]/F");
    pEventTree->Branch("phiend",phiend,"phiend[ntrkt]/F");
    pEventTree->Branch("posxend",posxend,"posxend[ntrkt]/F");
    pEventTree->Branch("posyend",posyend,"posyend[ntrkt]/F");
    pEventTree->Branch("strpxend",strpxend,"strpxend[ntrkt]/I");
    pEventTree->Branch("strpyend",strpyend,"strpyend[ntrkt]/I");
    pEventTree->Branch("poszend",poszend,"poszend[ntrkt]/F");
    pEventTree->Branch("momds"   ,momds   ,"momds[ntrkt]/F");
    pEventTree->Branch("momrg"   ,momrg   ,"momrg[ntrkt]/F");
    pEventTree->Branch("vtxzplane",vtxzplane,"vtxzplane[ntrkt]/I");
    pEventTree->Branch("endzplane",endzplane,"endzplane[ntrkt]/I");
    pEventTree->Branch("nvisht", &nvisht, "nvisht/i");
    pEventTree->Branch("fitposxx",fitposxx,"fitposxx[nvisht]/F");
    pEventTree->Branch("fitposyy",fitposyy,"fitposyy[nvisht]/F");
    pEventTree->Branch("fitposzz",fitposzz,"fitposzz[nvisht]/F");
    pEventTree->Branch("fitlayzz",fitlayzz,"fitlayzz[nvisht]/F");
    // pEventTree->Branch("fitlayx2",fitlayx2,"fitlayx2[nvisht]/F");
    // pEventTree->Branch("fitlayx3",fitlayx3,"fitlayx3[nvisht]/F");
    // pEventTree->Branch("fitlayx4",fitlayx4,"fitlayx4[nvisht]/F");
    pEventTree->Branch("fitlaymom",fitlaymom,"fitlaymom[nvisht]/F");
    pEventTree->Branch("extrapolxx",extrapolxx,"extrapolxx[nvisht]/F");
    pEventTree->Branch("extrapolyy",extrapolyy,"extrapolyy[nvisht]/F");
    pEventTree->Branch("extrapolmom",extrapolmom,"extrapolmom[nvisht]/F");
    pEventTree->Branch("momdiff1",&momdiff1,"momdiff1/F");
    pEventTree->Branch("radialdiff1",&radialdiff1,"radialdiff1/F");
    // pEventTree->Branch("fitlaythe",fitlaythe,"fitlaythe[nvisht]/F");
    // pEventTree->Branch("fitlayphi",fitlayphi,"fitlayphi[nvisht]/F");
    pEventTree->Branch("nvisclst", &nvisclst, "nvisclst/i");
    pEventTree->Branch("clstposxx",clstposxx,"clstposxx[nvisclst]/F");
    pEventTree->Branch("clstposyy",clstposyy,"clstposyy[nvisclst]/F");
    pEventTree->Branch("clstposzz",clstposzz,"clstposzz[nvisclst]/F");
    pEventTree->Branch("clstposzpln",clstposzpln,"clstposzpln[nvisclst]/I");

    pEventTree->Branch("strtnhitsx",&strtnhitsx,"strtnhitsx/I");
    pEventTree->Branch("strtchisqx",&strtchisqx,"strtchisqx/F");
    pEventTree->Branch("strtintercptx",&strtintercptx,"strtintercptx/F");
    pEventTree->Branch("strtslopex",&strtslopex,"strtslopex/F");
    pEventTree->Branch("strtnhitsy",&strtnhitsy,"strtnhitsy/I");
    pEventTree->Branch("strtchisqy",&strtchisqy,"strtchisqy/F");
    pEventTree->Branch("strtintercpty",&strtintercpty,"strtintercpty/F");
    pEventTree->Branch("strtslopey",&strtslopey,"strtslopey/F");

    pEventTree->Branch("simpleradii",&simpleradii,"simpleradii/F");
    pEventTree->Branch("simplecurv",&simplecurv,"simplecurv/F");
    pEventTree->Branch("simplex0",&simplex0,"simplex0/F");
    pEventTree->Branch("simplez0",&simplez0,"simplez0/F");
    pEventTree->Branch("simplechisqpos",&simplechisqpos,"simplechisqpos/F");
    pEventTree->Branch("simplechisqneg",&simplechisqneg,"simplechisqneg/F");
    pEventTree->Branch("simplechisqcndn",&simplechisqcndn,"simplechisqcndn/F");
    pEventTree->Branch("simpleavgxpos",&simpleavgxpos,"simpleavgxpos/F");
    pEventTree->Branch("simpleavgxneg",&simpleavgxneg,"simpleavgxneg/F");
    pEventTree->Branch("simpleavgxcndn",&simpleavgxcndn,"simpleavgxcndn/F");
    pEventTree->Branch("simpleavgxmeas",&simpleavgxmeas,"simpleavgxmeas/F");
    pEventTree->Branch("simplenhits",&simplenhits,"simplenhits/I");
    pEventTree->Branch("nhits_last",&nhits_last,"nhits_last/I");
    pEventTree->Branch("nhits_last_m1",&nhits_last_m1,"nhits_last_m1/I");
    pEventTree->Branch("nhits_below",&nhits_below,"nhits_below/I");
    pEventTree->Branch("ftime_last",&ftime_last,"ftime_last/F");

    //Extrapolated and measured state vector informations JIM
    pEventTree->Branch("inPosX", inPosX, "inPosX[10]/F");
    pEventTree->Branch("inPosY", inPosY, "inPosY[10]/F");
    pEventTree->Branch("extPosX", extPosX, "extPosX[10]/F");
    pEventTree->Branch("extPosY", extPosY, "extPosY[10]/F");
    pEventTree->Branch("nXStrips", nXStrips, "nXStrips[10]/I");
    pEventTree->Branch("nYStrips", nYStrips, "nYStrips[10]/I");
    pEventTree->Branch("XchisqOccu", XchisqOccu, "XchisqOccu[10]/F");
    pEventTree->Branch("YchisqOccu", YchisqOccu, "YchisqOccu[10]/F");
    pEventTree->Branch("XndfOccu", XndfOccu, "XndfOccu[10]/I");
    pEventTree->Branch("YndfOccu", YndfOccu, "YndfOccu[10]/I");
    pEventTree->Branch("nxFailOccu", nxFailOccu, "nxFailOccu[10]/I");
    pEventTree->Branch("nyFailOccu", nyFailOccu, "nyFailOccu[10]/I");

    //cmvd

 if(CardFile->GetCMVD()==1){
    pEventTree->Branch("extrapolatim00", extrapolatim00, "extrapolatim00[ntrkt]/F");
    pEventTree->Branch("extrapolatim01", extrapolatim01, "extrapolatim01[ntrkt]/F");
    pEventTree->Branch("extrapolatim02", extrapolatim02, "extrapolatim02[ntrkt]/F");
    pEventTree->Branch("extrapolatim03", extrapolatim03, "extrapolatim03[ntrkt]/F");


    pEventTree->Branch("Trig00", Trig00, "Trig00[ntrkt]/I");
    pEventTree->Branch("Trig01", Trig01, "Trig01[ntrkt]/I");
    pEventTree->Branch("Trig02", Trig02, "Trig02[ntrkt]/I");
    pEventTree->Branch("Trig03", Trig03, "Trig03[ntrkt]/I");


    pEventTree->Branch("atim",atim, "atim[ntrkt]/F");

    pEventTree->Branch("cmv_locno00", cmv_locno00, "cmv_locno00[ntrkt]/i");

    pEventTree->Branch("cmv_stripno", cmv_stripno, "cmv_stripno[ntrkt]/i");

    pEventTree->Branch("cmv_lay", cmv_lay, "cmv_lay[ntrkt]/i");

    pEventTree->Branch("clustersize00", clustersize00, "clustersize00[ntrkt]/i");


    pEventTree->Branch("extrapolposx00", extrapolposx00, "extrapolposx00[ntrkt]/F");
    pEventTree->Branch("extrapolposy00", extrapolposy00, "extrapolposy00[ntrkt]/F");
    pEventTree->Branch("extrapolposz00", extrapolposz00, "extrapolposz00[ntrkt]/F");

    pEventTree->Branch("extrapolposxerr00", extrapolposxerr00, "extrapolposxerr00[ntrkt]/F");
    pEventTree->Branch("extrapolposyerr00", extrapolposyerr00, "extrapolposyerr00[ntrkt]/F");
    pEventTree->Branch("extrapolposzerr00", extrapolposzerr00, "extrapolposzerr00[ntrkt]/F");

    pEventTree->Branch("cmvhitrecoposx00", cmvhitrecoposx00, "cmvhitrecoposx00[ntrkt]/F");
    pEventTree->Branch("cmvhitrecoposy00", cmvhitrecoposy00, "cmvhitrecoposy00[ntrkt]/F");
    pEventTree->Branch("cmvhitrecoposz00", cmvhitrecoposz00, "cmvhitrecoposz00[ntrkt]/F");

    pEventTree->Branch("cmvhittrueposx00", cmvhittrueposx00, "cmvhittrueposx00[ntrkt]/F");
    pEventTree->Branch("cmvhittrueposy00", cmvhittrueposy00, "cmvhittrueposy00[ntrkt]/F");
    pEventTree->Branch("cmvhittrueposz00", cmvhittrueposz00, "cmvhittrueposz00[ntrkt]/F");

    pEventTree->Branch("cmvhitrecoposxerr00", cmvhitrecoposxerr00, "cmvhitrecoposxerr00[ntrkt]/F");
    pEventTree->Branch("cmvhitrecoposyerr00", cmvhitrecoposyerr00, "cmvhitrecoposyerr00[ntrkt]/F");
    pEventTree->Branch("cmvhitrecoposzerr00", cmvhitrecoposzerr00, "cmvhitrecoposzerr00[ntrkt]/F");


    pEventTree->Branch("LeTime00", LeTime00, "LeTime00[ntrkt]/F");
  pEventTree->Branch("RiTime00", RiTime00, "RiTime00[ntrkt]/F");

    pEventTree->Branch("LePulse00", LePulse00, "LePulse00[ntrkt]/F");
  pEventTree->Branch("RiPulse00", RiPulse00, "RiPulse00[ntrkt]/F");


       pEventTree->Branch("ellip_diff00", ellip_diff00, "ellip_diff00[ntrkt]/F");


 pEventTree->Branch("clustersize01", clustersize01, "clustersize01[ntrkt]/i");
       pEventTree->Branch("cmv_locno01", cmv_locno01, "cmv_locno01[ntrkt]/i");
  pEventTree->Branch("extrapolposx01", extrapolposx01, "extrapolposx01[ntrkt]/F");
    pEventTree->Branch("extrapolposy01", extrapolposy01, "extrapolposy01[ntrkt]/F");
    pEventTree->Branch("extrapolposz01", extrapolposz01, "extrapolposz01[ntrkt]/F");

    pEventTree->Branch("extrapolposxerr01", extrapolposxerr01, "extrapolposxerr01[ntrkt]/F");
    pEventTree->Branch("extrapolposyerr01", extrapolposyerr01, "extrapolposyerr01[ntrkt]/F");
    pEventTree->Branch("extrapolposzerr01", extrapolposzerr01, "extrapolposzerr01[ntrkt]/F");

    pEventTree->Branch("cmvhitrecoposx01", cmvhitrecoposx01, "cmvhitrecoposx01[ntrkt]/F");
    pEventTree->Branch("cmvhitrecoposy01", cmvhitrecoposy01, "cmvhitrecoposy01[ntrkt]/F");
    pEventTree->Branch("cmvhitrecoposz01", cmvhitrecoposz01, "cmvhitrecoposz01[ntrkt]/F");

    pEventTree->Branch("cmvhittrueposx01", cmvhittrueposx01, "cmvhittrueposx01[ntrkt]/F");
    pEventTree->Branch("cmvhittrueposy01", cmvhittrueposy01, "cmvhittrueposy01[ntrkt]/F");
    pEventTree->Branch("cmvhittrueposz01", cmvhittrueposz01, "cmvhittrueposz01[ntrkt]/F");

    pEventTree->Branch("cmvhitrecoposxerr01", cmvhitrecoposxerr01, "cmvhitrecoposxerr01[ntrkt]/F");
    pEventTree->Branch("cmvhitrecoposyerr01", cmvhitrecoposyerr01, "cmvhitrecoposyerr01[ntrkt]/F");
    pEventTree->Branch("cmvhitrecoposzerr01", cmvhitrecoposzerr01, "cmvhitrecoposzerr01[ntrkt]/F");


    pEventTree->Branch("LeTime01", LeTime01, "LeTime01[ntrkt]/F");
  pEventTree->Branch("RiTime01", RiTime01, "RiTime01[ntrkt]/F");

    pEventTree->Branch("LePulse01", LePulse01, "LePulse01[ntrkt]/F");
  pEventTree->Branch("RiPulse01", RiPulse01, "RiPulse01[ntrkt]/F");







pEventTree->Branch("ellip_diff01", ellip_diff01, "ellip_diff01[ntrkt]/F");

 pEventTree->Branch("clustersize02", clustersize02, "clustersize02[ntrkt]/i");
pEventTree->Branch("cmv_locno02", cmv_locno02, "cmv_locno02[ntrkt]/i");

  pEventTree->Branch("extrapolposx02", extrapolposx02, "extrapolposx02[ntrkt]/F");
    pEventTree->Branch("extrapolposy02", extrapolposy02, "extrapolposy02[ntrkt]/F");
    pEventTree->Branch("extrapolposz02", extrapolposz02, "extrapolposz02[ntrkt]/F");

    pEventTree->Branch("extrapolposxerr02", extrapolposxerr02, "extrapolposxerr02[ntrkt]/F");
    pEventTree->Branch("extrapolposyerr02", extrapolposyerr02, "extrapolposyerr02[ntrkt]/F");
    pEventTree->Branch("extrapolposzerr02", extrapolposzerr02, "extrapolposzerr02[ntrkt]/F");

    pEventTree->Branch("cmvhitrecoposx02", cmvhitrecoposx02, "cmvhitrecoposx02[ntrkt]/F");
    pEventTree->Branch("cmvhitrecoposy02", cmvhitrecoposy02, "cmvhitrecoposy02[ntrkt]/F");
    pEventTree->Branch("cmvhitrecoposz02", cmvhitrecoposz02, "cmvhitrecoposz02[ntrkt]/F");

    pEventTree->Branch("cmvhittrueposx02", cmvhittrueposx02, "cmvhittrueposx02[ntrkt]/F");
    pEventTree->Branch("cmvhittrueposy02", cmvhittrueposy02, "cmvhittrueposy02[ntrkt]/F");
    pEventTree->Branch("cmvhittrueposz02", cmvhittrueposz02, "cmvhittrueposz02[ntrkt]/F");

    pEventTree->Branch("cmvhitrecoposxerr02", cmvhitrecoposxerr02, "cmvhitrecoposxerr02[ntrkt]/F");
    pEventTree->Branch("cmvhitrecoposyerr02", cmvhitrecoposyerr02, "cmvhitrecoposyerr02[ntrkt]/F");
    pEventTree->Branch("cmvhitrecoposzerr02", cmvhitrecoposzerr02, "cmvhitrecoposzerr02[ntrkt]/F");


    pEventTree->Branch("LeTime02", LeTime02, "LeTime02[ntrkt]/F");
  pEventTree->Branch("RiTime02", RiTime02, "RiTime02[ntrkt]/F");

    pEventTree->Branch("LePulse02", LePulse02, "LePulse02[ntrkt]/F");
  pEventTree->Branch("RiPulse02", RiPulse02, "RiPulse02[ntrkt]/F");







pEventTree->Branch("ellip_diff02", ellip_diff02, "ellip_diff02[ntrkt]/F");



 pEventTree->Branch("clustersize03", clustersize03, "clustersize03[ntrkt]/i");
 pEventTree->Branch("cmv_locno03", cmv_locno03, "cmv_locno03[ntrkt]/i");
  pEventTree->Branch("extrapolposx03", extrapolposx03, "extrapolposx03[ntrkt]/F");
    pEventTree->Branch("extrapolposy03", extrapolposy03, "extrapolposy03[ntrkt]/F");
    pEventTree->Branch("extrapolposz03", extrapolposz03, "extrapolposz03[ntrkt]/F");

    pEventTree->Branch("extrapolposxerr03", extrapolposxerr03, "extrapolposxerr03[ntrkt]/F");
    pEventTree->Branch("extrapolposyerr03", extrapolposyerr03, "extrapolposyerr03[ntrkt]/F");
    pEventTree->Branch("extrapolposzerr03", extrapolposzerr03, "extrapolposzerr03[ntrkt]/F");

    pEventTree->Branch("cmvhitrecoposx03", cmvhitrecoposx03, "cmvhitrecoposx03[ntrkt]/F");
    pEventTree->Branch("cmvhitrecoposy03", cmvhitrecoposy03, "cmvhitrecoposy03[ntrkt]/F");
    pEventTree->Branch("cmvhitrecoposz03", cmvhitrecoposz03, "cmvhitrecoposz03[ntrkt]/F");

    pEventTree->Branch("cmvhittrueposx03", cmvhittrueposx03, "cmvhittrueposx03[ntrkt]/F");
    pEventTree->Branch("cmvhittrueposy03", cmvhittrueposy03, "cmvhittrueposy03[ntrkt]/F");
    pEventTree->Branch("cmvhittrueposz03", cmvhittrueposz03, "cmvhittrueposz03[ntrkt]/F");

    pEventTree->Branch("cmvhitrecoposxerr03", cmvhitrecoposxerr03, "cmvhitrecoposxerr03[ntrkt]/F");
    pEventTree->Branch("cmvhitrecoposyerr03", cmvhitrecoposyerr03, "cmvhitrecoposyerr03[ntrkt]/F");
    pEventTree->Branch("cmvhitrecoposzerr03", cmvhitrecoposzerr03, "cmvhitrecoposzerr03[ntrkt]/F");



    pEventTree->Branch("LeTime03", LeTime03, "LeTime03[ntrkt]/F");
  pEventTree->Branch("RiTime03", RiTime03, "RiTime03[ntrkt]/F");

    pEventTree->Branch("LePulse03", LePulse03, "LePulse03[ntrkt]/F");
  pEventTree->Branch("RiPulse03", RiPulse03, "RiPulse03[ntrkt]/F");

pEventTree->Branch("ellip_diff03", ellip_diff03, "ellip_diff03[ntrkt]/F");



     pEventTree->Branch("clustersize10", clustersize10, "clustersize10[ntrkt]/i");
 pEventTree->Branch("cmv_locno10", cmv_locno10, "cmv_locno10[ntrkt]/i");
  pEventTree->Branch("extrapolposx10", extrapolposx10, "extrapolposx10[ntrkt]/F");
    pEventTree->Branch("extrapolposy10", extrapolposy10, "extrapolposy10[ntrkt]/F");
    pEventTree->Branch("extrapolposz10", extrapolposz10, "extrapolposz10[ntrkt]/F");

    pEventTree->Branch("extrapolposxerr10", extrapolposxerr10, "extrapolposxerr10[ntrkt]/F");
    pEventTree->Branch("extrapolposyerr10", extrapolposyerr10, "extrapolposyerr10[ntrkt]/F");
    pEventTree->Branch("extrapolposzerr10", extrapolposzerr10, "extrapolposzerr10[ntrkt]/F");

    pEventTree->Branch("cmvhitrecoposx10", cmvhitrecoposx10, "cmvhitrecoposx10[ntrkt]/F");
    pEventTree->Branch("cmvhitrecoposy10", cmvhitrecoposy10, "cmvhitrecoposy10[ntrkt]/F");
    pEventTree->Branch("cmvhitrecoposz10", cmvhitrecoposz10, "cmvhitrecoposz10[ntrkt]/F");

    pEventTree->Branch("cmvhittrueposx10", cmvhittrueposx10, "cmvhittrueposx10[ntrkt]/F");
    pEventTree->Branch("cmvhittrueposy10", cmvhittrueposy10, "cmvhittrueposy10[ntrkt]/F");
    pEventTree->Branch("cmvhittrueposz10", cmvhittrueposz10, "cmvhittrueposz10[ntrkt]/F");

    pEventTree->Branch("cmvhitrecoposxerr10", cmvhitrecoposxerr10, "cmvhitrecoposxerr10[ntrkt]/F");
    pEventTree->Branch("cmvhitrecoposyerr10", cmvhitrecoposyerr10, "cmvhitrecoposyerr10[ntrkt]/F");
    pEventTree->Branch("cmvhitrecoposzerr10", cmvhitrecoposzerr10, "cmvhitrecoposzerr10[ntrkt]/F");


    pEventTree->Branch("debug", debug, "debug[ntrkt]/F");

    pEventTree->Branch("LeTime10", LeTime10, "LeTime10[ntrkt]/F");
  pEventTree->Branch("RiTime10", RiTime10, "RiTime10[ntrkt]/F");

    pEventTree->Branch("LePulse10", LePulse10, "LePulse10[ntrkt]/F");
  pEventTree->Branch("RiPulse10", RiPulse10, "RiPulse10[ntrkt]/F");


pEventTree->Branch("ellip_diff10", ellip_diff10, "ellip_diff10[ntrkt]/F");



     pEventTree->Branch("clustersize11", clustersize11, "clustersize11[ntrkt]/i");

 pEventTree->Branch("cmv_locno11", cmv_locno11, "cmv_locno11[ntrkt]/i");

  pEventTree->Branch("extrapolposx11", extrapolposx11, "extrapolposx11[ntrkt]/F");
    pEventTree->Branch("extrapolposy11", extrapolposy11, "extrapolposy11[ntrkt]/F");
    pEventTree->Branch("extrapolposz11", extrapolposz11, "extrapolposz11[ntrkt]/F");

    pEventTree->Branch("extrapolposxerr11", extrapolposxerr11, "extrapolposxerr11[ntrkt]/F");
    pEventTree->Branch("extrapolposyerr11", extrapolposyerr11, "extrapolposyerr11[ntrkt]/F");
    pEventTree->Branch("extrapolposzerr11", extrapolposzerr11, "extrapolposzerr11[ntrkt]/F");

    pEventTree->Branch("cmvhitrecoposx11", cmvhitrecoposx11, "cmvhitrecoposx11[ntrkt]/F");
    pEventTree->Branch("cmvhitrecoposy11", cmvhitrecoposy11, "cmvhitrecoposy11[ntrkt]/F");
    pEventTree->Branch("cmvhitrecoposz11", cmvhitrecoposz11, "cmvhitrecoposz11[ntrkt]/F");

    pEventTree->Branch("cmvhittrueposx11", cmvhittrueposx11, "cmvhittrueposx11[ntrkt]/F");
    pEventTree->Branch("cmvhittrueposy11", cmvhittrueposy11, "cmvhittrueposy11[ntrkt]/F");
    pEventTree->Branch("cmvhittrueposz11", cmvhittrueposz11, "cmvhittrueposz11[ntrkt]/F");

    pEventTree->Branch("cmvhitrecoposxerr11", cmvhitrecoposxerr11, "cmvhitrecoposxerr11[ntrkt]/F");
    pEventTree->Branch("cmvhitrecoposyerr11", cmvhitrecoposyerr11, "cmvhitrecoposyerr11[ntrkt]/F");
    pEventTree->Branch("cmvhitrecoposzerr11", cmvhitrecoposzerr11, "cmvhitrecoposzerr11[ntrkt]/F");



    pEventTree->Branch("LeTime11", LeTime11, "LeTime11[ntrkt]/F");
  pEventTree->Branch("RiTime11", RiTime11, "RiTime11[ntrkt]/F");

    pEventTree->Branch("LePulse11", LePulse11, "LePulse11[ntrkt]/F");
  pEventTree->Branch("RiPulse11", RiPulse11, "RiPulse11[ntrkt]/F");


pEventTree->Branch("ellip_diff11", ellip_diff11, "ellip_diff11[ntrkt]/F");


   pEventTree->Branch("clustersize12", clustersize12, "clustersize12[ntrkt]/i");
 pEventTree->Branch("cmv_locno12", cmv_locno12, "cmv_locno12[ntrkt]/i");
  pEventTree->Branch("extrapolposx12", extrapolposx12, "extrapolposx12[ntrkt]/F");
    pEventTree->Branch("extrapolposy12", extrapolposy12, "extrapolposy12[ntrkt]/F");
    pEventTree->Branch("extrapolposz12", extrapolposz12, "extrapolposz12[ntrkt]/F");

    pEventTree->Branch("extrapolposxerr12", extrapolposxerr12, "extrapolposxerr12[ntrkt]/F");
    pEventTree->Branch("extrapolposyerr12", extrapolposyerr12, "extrapolposyerr12[ntrkt]/F");
    pEventTree->Branch("extrapolposzerr12", extrapolposzerr12, "extrapolposzerr12[ntrkt]/F");

    pEventTree->Branch("cmvhitrecoposx12", cmvhitrecoposx12, "cmvhitrecoposx12[ntrkt]/F");
    pEventTree->Branch("cmvhitrecoposy12", cmvhitrecoposy12, "cmvhitrecoposy12[ntrkt]/F");
    pEventTree->Branch("cmvhitrecoposz12", cmvhitrecoposz12, "cmvhitrecoposz12[ntrkt]/F");

    pEventTree->Branch("cmvhittrueposx12", cmvhittrueposx12, "cmvhittrueposx12[ntrkt]/F");
    pEventTree->Branch("cmvhittrueposy12", cmvhittrueposy12, "cmvhittrueposy12[ntrkt]/F");
    pEventTree->Branch("cmvhittrueposz12", cmvhittrueposz12, "cmvhittrueposz12[ntrkt]/F");

    pEventTree->Branch("cmvhitrecoposxerr12", cmvhitrecoposxerr12, "cmvhitrecoposxerr12[ntrkt]/F");
    pEventTree->Branch("cmvhitrecoposyerr12", cmvhitrecoposyerr12, "cmvhitrecoposyerr12[ntrkt]/F");
    pEventTree->Branch("cmvhitrecoposzerr12", cmvhitrecoposzerr12, "cmvhitrecoposzerr12[ntrkt]/F");




    pEventTree->Branch("LeTime12", LeTime12, "LeTime12[ntrkt]/F");
  pEventTree->Branch("RiTime12", RiTime12, "RiTime12[ntrkt]/F");

    pEventTree->Branch("LePulse12", LePulse12, "LePulse12[ntrkt]/F");
  pEventTree->Branch("RiPulse12", RiPulse12, "RiPulse12[ntrkt]/F");





pEventTree->Branch("ellip_diff12", ellip_diff12, "ellip_diff12[ntrkt]/F");


pEventTree->Branch("clustersize20", clustersize20, "clustersize20[ntrkt]/i");
 pEventTree->Branch("cmv_locno20", cmv_locno20, "cmv_locno20[ntrkt]/i");
  pEventTree->Branch("extrapolposx20", extrapolposx20, "extrapolposx20[ntrkt]/F");
    pEventTree->Branch("extrapolposy20", extrapolposy20, "extrapolposy20[ntrkt]/F");
    pEventTree->Branch("extrapolposz20", extrapolposz20, "extrapolposz20[ntrkt]/F");

    pEventTree->Branch("extrapolposxerr20", extrapolposxerr20, "extrapolposxerr20[ntrkt]/F");
    pEventTree->Branch("extrapolposyerr20", extrapolposyerr20, "extrapolposyerr20[ntrkt]/F");
    pEventTree->Branch("extrapolposzerr20", extrapolposzerr20, "extrapolposzerr20[ntrkt]/F");

    pEventTree->Branch("cmvhitrecoposx20", cmvhitrecoposx20, "cmvhitrecoposx20[ntrkt]/F");
    pEventTree->Branch("cmvhitrecoposy20", cmvhitrecoposy20, "cmvhitrecoposy20[ntrkt]/F");
    pEventTree->Branch("cmvhitrecoposz20", cmvhitrecoposz20, "cmvhitrecoposz20[ntrkt]/F");

    pEventTree->Branch("cmvhittrueposx20", cmvhittrueposx20, "cmvhittrueposx20[ntrkt]/F");
    pEventTree->Branch("cmvhittrueposy20", cmvhittrueposy20, "cmvhittrueposy20[ntrkt]/F");
    pEventTree->Branch("cmvhittrueposz20", cmvhittrueposz20, "cmvhittrueposz20[ntrkt]/F");

    pEventTree->Branch("cmvhitrecoposxerr20", cmvhitrecoposxerr20, "cmvhitrecoposxerr20[ntrkt]/F");
    pEventTree->Branch("cmvhitrecoposyerr20", cmvhitrecoposyerr20, "cmvhitrecoposyerr20[ntrkt]/F");
    pEventTree->Branch("cmvhitrecoposzerr20", cmvhitrecoposzerr20, "cmvhitrecoposzerr20[ntrkt]/F");




    pEventTree->Branch("LeTime20", LeTime20, "LeTime20[ntrkt]/F");
  pEventTree->Branch("RiTime20", RiTime20, "RiTime20[ntrkt]/F");

    pEventTree->Branch("LePulse20", LePulse20, "LePulse20[ntrkt]/F");
  pEventTree->Branch("RiPulse20", RiPulse20, "RiPulse20[ntrkt]/F");






pEventTree->Branch("ellip_diff20", ellip_diff20, "ellip_diff20[ntrkt]/F");


    pEventTree->Branch("clustersize21", clustersize21, "clustersize21[ntrkt]/i");

 pEventTree->Branch("cmv_locno21", cmv_locno21, "cmv_locno21[ntrkt]/i");
  pEventTree->Branch("extrapolposx21", extrapolposx21, "extrapolposx21[ntrkt]/F");
    pEventTree->Branch("extrapolposy21", extrapolposy21, "extrapolposy21[ntrkt]/F");
    pEventTree->Branch("extrapolposz21", extrapolposz21, "extrapolposz21[ntrkt]/F");

    pEventTree->Branch("extrapolposxerr21", extrapolposxerr21, "extrapolposxerr21[ntrkt]/F");
    pEventTree->Branch("extrapolposyerr21", extrapolposyerr21, "extrapolposyerr21[ntrkt]/F");
    pEventTree->Branch("extrapolposzerr21", extrapolposzerr21, "extrapolposzerr21[ntrkt]/F");

    pEventTree->Branch("cmvhitrecoposx21", cmvhitrecoposx21, "cmvhitrecoposx21[ntrkt]/F");
    pEventTree->Branch("cmvhitrecoposy21", cmvhitrecoposy21, "cmvhitrecoposy21[ntrkt]/F");
    pEventTree->Branch("cmvhitrecoposz21", cmvhitrecoposz21, "cmvhitrecoposz21[ntrkt]/F");

    pEventTree->Branch("cmvhittrueposx21", cmvhittrueposx21, "cmvhittrueposx21[ntrkt]/F");
    pEventTree->Branch("cmvhittrueposy21", cmvhittrueposy21, "cmvhittrueposy21[ntrkt]/F");
    pEventTree->Branch("cmvhittrueposz21", cmvhittrueposz21, "cmvhittrueposz21[ntrkt]/F");

    pEventTree->Branch("cmvhitrecoposxerr21", cmvhitrecoposxerr21, "cmvhitrecoposxerr21[ntrkt]/F");
    pEventTree->Branch("cmvhitrecoposyerr21", cmvhitrecoposyerr21, "cmvhitrecoposyerr21[ntrkt]/F");
    pEventTree->Branch("cmvhitrecoposzerr21", cmvhitrecoposzerr21, "cmvhitrecoposzerr21[ntrkt]/F");




    pEventTree->Branch("LeTime21", LeTime21, "LeTime21[ntrkt]/F");
  pEventTree->Branch("RiTime21", RiTime21, "RiTime21[ntrkt]/F");

    pEventTree->Branch("LePulse21", LePulse21, "LePulse21[ntrkt]/F");
  pEventTree->Branch("RiPulse21", RiPulse21, "RiPulse21[ntrkt]/F");




pEventTree->Branch("ellip_diff21", ellip_diff21, "ellip_diff21[ntrkt]/F");


pEventTree->Branch("clustersize22", clustersize22, "clustersize22[ntrkt]/i");
       pEventTree->Branch("cmv_locno22", cmv_locno22, "cmv_locno22[ntrkt]/i");
  pEventTree->Branch("extrapolposx22", extrapolposx22, "extrapolposx22[ntrkt]/F");
    pEventTree->Branch("extrapolposy22", extrapolposy22, "extrapolposy22[ntrkt]/F");
    pEventTree->Branch("extrapolposz22", extrapolposz22, "extrapolposz22[ntrkt]/F");

    pEventTree->Branch("extrapolposxerr22", extrapolposxerr22, "extrapolposxerr22[ntrkt]/F");
    pEventTree->Branch("extrapolposyerr22", extrapolposyerr22, "extrapolposyerr22[ntrkt]/F");
    pEventTree->Branch("extrapolposzerr22", extrapolposzerr22, "extrapolposzerr22[ntrkt]/F");

    pEventTree->Branch("cmvhitrecoposx22", cmvhitrecoposx22, "cmvhitrecoposx22[ntrkt]/F");
    pEventTree->Branch("cmvhitrecoposy22", cmvhitrecoposy22, "cmvhitrecoposy22[ntrkt]/F");
    pEventTree->Branch("cmvhitrecoposz22", cmvhitrecoposz22, "cmvhitrecoposz22[ntrkt]/F");

    pEventTree->Branch("cmvhittrueposx22", cmvhittrueposx22, "cmvhittrueposx22[ntrkt]/F");
    pEventTree->Branch("cmvhittrueposy22", cmvhittrueposy22, "cmvhittrueposy22[ntrkt]/F");
    pEventTree->Branch("cmvhittrueposz22", cmvhittrueposz22, "cmvhittrueposz22[ntrkt]/F");

    pEventTree->Branch("cmvhitrecoposxerr22", cmvhitrecoposxerr22, "cmvhitrecoposxerr22[ntrkt]/F");
    pEventTree->Branch("cmvhitrecoposyerr22", cmvhitrecoposyerr22, "cmvhitrecoposyerr22[ntrkt]/F");
    pEventTree->Branch("cmvhitrecoposzerr22", cmvhitrecoposzerr22, "cmvhitrecoposzerr22[ntrkt]/F");



    pEventTree->Branch("LeTime22", LeTime22, "LeTime22[ntrkt]/F");
  pEventTree->Branch("RiTime22", RiTime22, "RiTime22[ntrkt]/F");

    pEventTree->Branch("LePulse22", LePulse22, "LePulse22[ntrkt]/F");
  pEventTree->Branch("RiPulse22", RiPulse22, "RiPulse22[ntrkt]/F");



pEventTree->Branch("ellip_diff22", ellip_diff22, "ellip_diff22[ntrkt]/F");

pEventTree->Branch("clustersize30", clustersize30, "clustersize30[ntrkt]/i");
pEventTree->Branch("cmv_locno30", cmv_locno30, "cmv_locno30[ntrkt]/i");
  pEventTree->Branch("extrapolposx30", extrapolposx30, "extrapolposx30[ntrkt]/F");
    pEventTree->Branch("extrapolposy30", extrapolposy30, "extrapolposy30[ntrkt]/F");
    pEventTree->Branch("extrapolposz30", extrapolposz30, "extrapolposz30[ntrkt]/F");

    pEventTree->Branch("extrapolposxerr30", extrapolposxerr30, "extrapolposxerr30[ntrkt]/F");
    pEventTree->Branch("extrapolposyerr30", extrapolposyerr30, "extrapolposyerr30[ntrkt]/F");
    pEventTree->Branch("extrapolposzerr30", extrapolposzerr30, "extrapolposzerr30[ntrkt]/F");

    pEventTree->Branch("cmvhitrecoposx30", cmvhitrecoposx30, "cmvhitrecoposx30[ntrkt]/F");
    pEventTree->Branch("cmvhitrecoposy30", cmvhitrecoposy30, "cmvhitrecoposy30[ntrkt]/F");
    pEventTree->Branch("cmvhitrecoposz30", cmvhitrecoposz30, "cmvhitrecoposz30[ntrkt]/F");

    pEventTree->Branch("cmvhittrueposx30", cmvhittrueposx30, "cmvhittrueposx30[ntrkt]/F");
    pEventTree->Branch("cmvhittrueposy30", cmvhittrueposy30, "cmvhittrueposy30[ntrkt]/F");
    pEventTree->Branch("cmvhittrueposz30", cmvhittrueposz30, "cmvhittrueposz30[ntrkt]/F");

    pEventTree->Branch("cmvhitrecoposxerr30", cmvhitrecoposxerr30, "cmvhitrecoposxerr30[ntrkt]/F");
    pEventTree->Branch("cmvhitrecoposyerr30", cmvhitrecoposyerr30, "cmvhitrecoposyerr30[ntrkt]/F");
    pEventTree->Branch("cmvhitrecoposzerr30", cmvhitrecoposzerr30, "cmvhitrecoposzerr30[ntrkt]/F");



    pEventTree->Branch("LeTime30", LeTime30, "LeTime30[ntrkt]/F");
  pEventTree->Branch("RiTime30", RiTime30, "RiTime30[ntrkt]/F");

    pEventTree->Branch("LePulse30", LePulse30, "LePulse30[ntrkt]/F");
  pEventTree->Branch("RiPulse30", RiPulse30, "RiPulse30[ntrkt]/F");







pEventTree->Branch("ellip_diff30", ellip_diff30, "ellip_diff30[ntrkt]/F");


 pEventTree->Branch("clustersize31", clustersize31, "clustersize31[ntrkt]/i");
    pEventTree->Branch("cmv_locno31", cmv_locno31, "cmv_locno31[ntrkt]/i");
  pEventTree->Branch("extrapolposx31", extrapolposx31, "extrapolposx31[ntrkt]/F");
    pEventTree->Branch("extrapolposy31", extrapolposy31, "extrapolposy31[ntrkt]/F");
    pEventTree->Branch("extrapolposz31", extrapolposz31, "extrapolposz31[ntrkt]/F");

    pEventTree->Branch("extrapolposxerr31", extrapolposxerr31, "extrapolposxerr31[ntrkt]/F");
    pEventTree->Branch("extrapolposyerr31", extrapolposyerr31, "extrapolposyerr31[ntrkt]/F");
    pEventTree->Branch("extrapolposzerr31", extrapolposzerr31, "extrapolposzerr31[ntrkt]/F");

    pEventTree->Branch("cmvhitrecoposx31", cmvhitrecoposx31, "cmvhitrecoposx31[ntrkt]/F");
    pEventTree->Branch("cmvhitrecoposy31", cmvhitrecoposy31, "cmvhitrecoposy31[ntrkt]/F");
    pEventTree->Branch("cmvhitrecoposz31", cmvhitrecoposz31, "cmvhitrecoposz31[ntrkt]/F");

    pEventTree->Branch("cmvhittrueposx31", cmvhittrueposx31, "cmvhittrueposx31[ntrkt]/F");
    pEventTree->Branch("cmvhittrueposy31", cmvhittrueposy31, "cmvhittrueposy31[ntrkt]/F");
    pEventTree->Branch("cmvhittrueposz31", cmvhittrueposz31, "cmvhittrueposz31[ntrkt]/F");

    pEventTree->Branch("cmvhitrecoposxerr31", cmvhitrecoposxerr31, "cmvhitrecoposxerr31[ntrkt]/F");
    pEventTree->Branch("cmvhitrecoposyerr31", cmvhitrecoposyerr31, "cmvhitrecoposyerr31[ntrkt]/F");
    pEventTree->Branch("cmvhitrecoposzerr31", cmvhitrecoposzerr31, "cmvhitrecoposzerr31[ntrkt]/F");




    pEventTree->Branch("LeTime31", LeTime31, "LeTime31[ntrkt]/F");
  pEventTree->Branch("RiTime31", RiTime31, "RiTime31[ntrkt]/F");

    pEventTree->Branch("LePulse31", LePulse31, "LePulse31[ntrkt]/F");
  pEventTree->Branch("RiPulse31", RiPulse31, "RiPulse31[ntrkt]/F");






    pEventTree->Branch("ellip_diff31", ellip_diff31, "ellip_diff31[ntrkt]/F");
 pEventTree->Branch("clustersize32", clustersize32, "clustersize32[ntrkt]/i");
pEventTree->Branch("cmv_locno32", cmv_locno32, "cmv_locno32[ntrkt]/i");
  pEventTree->Branch("extrapolposx32", extrapolposx32, "extrapolposx32[ntrkt]/F");
    pEventTree->Branch("extrapolposy32", extrapolposy32, "extrapolposy32[ntrkt]/F");
    pEventTree->Branch("extrapolposz32", extrapolposz32, "extrapolposz32[ntrkt]/F");

    pEventTree->Branch("extrapolposxerr32", extrapolposxerr32, "extrapolposxerr32[ntrkt]/F");
    pEventTree->Branch("extrapolposyerr32", extrapolposyerr32, "extrapolposyerr32[ntrkt]/F");
    pEventTree->Branch("extrapolposzerr32", extrapolposzerr32, "extrapolposzerr32[ntrkt]/F");

    pEventTree->Branch("cmvhitrecoposx32", cmvhitrecoposx32, "cmvhitrecoposx32[ntrkt]/F");
    pEventTree->Branch("cmvhitrecoposy32", cmvhitrecoposy32, "cmvhitrecoposy32[ntrkt]/F");
    pEventTree->Branch("cmvhitrecoposz32", cmvhitrecoposz32, "cmvhitrecoposz32[ntrkt]/F");

    pEventTree->Branch("cmvhittrueposx32", cmvhittrueposx32, "cmvhittrueposx32[ntrkt]/F");
    pEventTree->Branch("cmvhittrueposy32", cmvhittrueposy32, "cmvhittrueposy32[ntrkt]/F");
    pEventTree->Branch("cmvhittrueposz32", cmvhittrueposz32, "cmvhittrueposz32[ntrkt]/F");

    pEventTree->Branch("cmvhitrecoposxerr32", cmvhitrecoposxerr32, "cmvhitrecoposxerr32[ntrkt]/F");
    pEventTree->Branch("cmvhitrecoposyerr32", cmvhitrecoposyerr32, "cmvhitrecoposyerr32[ntrkt]/F");
    pEventTree->Branch("cmvhitrecoposzerr32", cmvhitrecoposzerr32, "cmvhitrecoposzerr32[ntrkt]/F");



    pEventTree->Branch("LeTime32", LeTime32, "LeTime32[ntrkt]/F");
  pEventTree->Branch("RiTime32", RiTime32, "RiTime32[ntrkt]/F");

    pEventTree->Branch("LePulse32", LePulse32, "LePulse32[ntrkt]/F");
  pEventTree->Branch("RiPulse32", RiPulse32, "RiPulse32[ntrkt]/F");



    pEventTree->Branch("ellip_diff32", ellip_diff32, "ellip_diff32[ntrkt]/F");



    //11022022


pEventTree->Branch("clustersize40", clustersize40, "clustersize40[ntrkt]/i");
pEventTree->Branch("cmv_locno40", cmv_locno40, "cmv_locno40[ntrkt]/i");
  pEventTree->Branch("extrapolposx40", extrapolposx40, "extrapolposx40[ntrkt]/F");
    pEventTree->Branch("extrapolposy40", extrapolposy40, "extrapolposy40[ntrkt]/F");
    pEventTree->Branch("extrapolposz40", extrapolposz40, "extrapolposz40[ntrkt]/F");

    pEventTree->Branch("extrapolposxerr40", extrapolposxerr40, "extrapolposxerr40[ntrkt]/F");
    pEventTree->Branch("extrapolposyerr40", extrapolposyerr40, "extrapolposyerr40[ntrkt]/F");
    pEventTree->Branch("extrapolposzerr40", extrapolposzerr40, "extrapolposzerr40[ntrkt]/F");

    pEventTree->Branch("cmvhitrecoposx40", cmvhitrecoposx40, "cmvhitrecoposx40[ntrkt]/F");
    pEventTree->Branch("cmvhitrecoposy40", cmvhitrecoposy40, "cmvhitrecoposy40[ntrkt]/F");
    pEventTree->Branch("cmvhitrecoposz40", cmvhitrecoposz40, "cmvhitrecoposz40[ntrkt]/F");

    pEventTree->Branch("cmvhittrueposx40", cmvhittrueposx40, "cmvhittrueposx40[ntrkt]/F");
    pEventTree->Branch("cmvhittrueposy40", cmvhittrueposy40, "cmvhittrueposy40[ntrkt]/F");
    pEventTree->Branch("cmvhittrueposz40", cmvhittrueposz40, "cmvhittrueposz40[ntrkt]/F");

    pEventTree->Branch("cmvhitrecoposxerr40", cmvhitrecoposxerr40, "cmvhitrecoposxerr40[ntrkt]/F");
    pEventTree->Branch("cmvhitrecoposyerr40", cmvhitrecoposyerr40, "cmvhitrecoposyerr40[ntrkt]/F");
    pEventTree->Branch("cmvhitrecoposzerr40", cmvhitrecoposzerr40, "cmvhitrecoposzerr40[ntrkt]/F");



    pEventTree->Branch("LeTime40", LeTime40, "LeTime40[ntrkt]/F");
  pEventTree->Branch("RiTime40", RiTime40, "RiTime40[ntrkt]/F");

    pEventTree->Branch("LePulse40", LePulse40, "LePulse40[ntrkt]/F");
  pEventTree->Branch("RiPulse40", RiPulse40, "RiPulse40[ntrkt]/F");







  //pEventTree->Branch("ellip_diff40", ellip_diff40, "ellip_diff40[ntrkt]/F");


 pEventTree->Branch("clustersize41", clustersize41, "clustersize41[ntrkt]/i");
    pEventTree->Branch("cmv_locno41", cmv_locno41, "cmv_locno41[ntrkt]/i");
  pEventTree->Branch("extrapolposx41", extrapolposx41, "extrapolposx41[ntrkt]/F");
    pEventTree->Branch("extrapolposy41", extrapolposy41, "extrapolposy41[ntrkt]/F");
    pEventTree->Branch("extrapolposz41", extrapolposz41, "extrapolposz41[ntrkt]/F");

    pEventTree->Branch("extrapolposxerr41", extrapolposxerr41, "extrapolposxerr41[ntrkt]/F");
    pEventTree->Branch("extrapolposyerr41", extrapolposyerr41, "extrapolposyerr41[ntrkt]/F");
    pEventTree->Branch("extrapolposzerr41", extrapolposzerr41, "extrapolposzerr41[ntrkt]/F");

    pEventTree->Branch("cmvhitrecoposx41", cmvhitrecoposx41, "cmvhitrecoposx41[ntrkt]/F");
    pEventTree->Branch("cmvhitrecoposy41", cmvhitrecoposy41, "cmvhitrecoposy41[ntrkt]/F");
    pEventTree->Branch("cmvhitrecoposz41", cmvhitrecoposz41, "cmvhitrecoposz41[ntrkt]/F");

    pEventTree->Branch("cmvhittrueposx41", cmvhittrueposx41, "cmvhittrueposx41[ntrkt]/F");
    pEventTree->Branch("cmvhittrueposy41", cmvhittrueposy41, "cmvhittrueposy41[ntrkt]/F");
    pEventTree->Branch("cmvhittrueposz41", cmvhittrueposz41, "cmvhittrueposz41[ntrkt]/F");

    pEventTree->Branch("cmvhitrecoposxerr41", cmvhitrecoposxerr41, "cmvhitrecoposxerr41[ntrkt]/F");
    pEventTree->Branch("cmvhitrecoposyerr41", cmvhitrecoposyerr41, "cmvhitrecoposyerr41[ntrkt]/F");
    pEventTree->Branch("cmvhitrecoposzerr41", cmvhitrecoposzerr41, "cmvhitrecoposzerr41[ntrkt]/F");




    pEventTree->Branch("LeTime41", LeTime41, "LeTime41[ntrkt]/F");
  pEventTree->Branch("RiTime41", RiTime41, "RiTime41[ntrkt]/F");

    pEventTree->Branch("LePulse41", LePulse41, "LePulse41[ntrkt]/F");
  pEventTree->Branch("RiPulse41", RiPulse41, "RiPulse41[ntrkt]/F");






  //    pEventTree->Branch("ellip_diff41", ellip_diff41, "ellip_diff41[ntrkt]/F");
 pEventTree->Branch("clustersize42", clustersize42, "clustersize42[ntrkt]/i");
pEventTree->Branch("cmv_locno42", cmv_locno42, "cmv_locno42[ntrkt]/i");
  pEventTree->Branch("extrapolposx42", extrapolposx42, "extrapolposx42[ntrkt]/F");
    pEventTree->Branch("extrapolposy42", extrapolposy42, "extrapolposy42[ntrkt]/F");
    pEventTree->Branch("extrapolposz42", extrapolposz42, "extrapolposz42[ntrkt]/F");

    pEventTree->Branch("extrapolposxerr42", extrapolposxerr42, "extrapolposxerr42[ntrkt]/F");
    pEventTree->Branch("extrapolposyerr42", extrapolposyerr42, "extrapolposyerr42[ntrkt]/F");
    pEventTree->Branch("extrapolposzerr42", extrapolposzerr42, "extrapolposzerr42[ntrkt]/F");

    pEventTree->Branch("cmvhitrecoposx42", cmvhitrecoposx42, "cmvhitrecoposx42[ntrkt]/F");
    pEventTree->Branch("cmvhitrecoposy42", cmvhitrecoposy42, "cmvhitrecoposy42[ntrkt]/F");
    pEventTree->Branch("cmvhitrecoposz42", cmvhitrecoposz42, "cmvhitrecoposz42[ntrkt]/F");

    pEventTree->Branch("cmvhittrueposx42", cmvhittrueposx42, "cmvhittrueposx42[ntrkt]/F");
    pEventTree->Branch("cmvhittrueposy42", cmvhittrueposy42, "cmvhittrueposy42[ntrkt]/F");
    pEventTree->Branch("cmvhittrueposz42", cmvhittrueposz42, "cmvhittrueposz42[ntrkt]/F");

    pEventTree->Branch("cmvhitrecoposxerr42", cmvhitrecoposxerr42, "cmvhitrecoposxerr42[ntrkt]/F");
    pEventTree->Branch("cmvhitrecoposyerr42", cmvhitrecoposyerr42, "cmvhitrecoposyerr42[ntrkt]/F");
    pEventTree->Branch("cmvhitrecoposzerr42", cmvhitrecoposzerr42, "cmvhitrecoposzerr42[ntrkt]/F");



    pEventTree->Branch("LeTime42", LeTime42, "LeTime42[ntrkt]/F");
  pEventTree->Branch("RiTime42", RiTime42, "RiTime42[ntrkt]/F");

    pEventTree->Branch("LePulse42", LePulse42, "LePulse42[ntrkt]/F");
  pEventTree->Branch("RiPulse42", RiPulse42, "RiPulse42[ntrkt]/F");



  //    pEventTree->Branch("ellip_diff42", ellip_diff42, "ellip_diff42[ntrkt]/F");


pEventTree->Branch("clustersize50", clustersize50, "clustersize50[ntrkt]/i");
pEventTree->Branch("cmv_locno50", cmv_locno50, "cmv_locno50[ntrkt]/i");
  pEventTree->Branch("extrapolposx50", extrapolposx50, "extrapolposx50[ntrkt]/F");
    pEventTree->Branch("extrapolposy50", extrapolposy50, "extrapolposy50[ntrkt]/F");
    pEventTree->Branch("extrapolposz50", extrapolposz50, "extrapolposz50[ntrkt]/F");

    pEventTree->Branch("extrapolposxerr50", extrapolposxerr50, "extrapolposxerr50[ntrkt]/F");
    pEventTree->Branch("extrapolposyerr50", extrapolposyerr50, "extrapolposyerr50[ntrkt]/F");
    pEventTree->Branch("extrapolposzerr50", extrapolposzerr50, "extrapolposzerr50[ntrkt]/F");

    pEventTree->Branch("cmvhitrecoposx50", cmvhitrecoposx50, "cmvhitrecoposx50[ntrkt]/F");
    pEventTree->Branch("cmvhitrecoposy50", cmvhitrecoposy50, "cmvhitrecoposy50[ntrkt]/F");
    pEventTree->Branch("cmvhitrecoposz50", cmvhitrecoposz50, "cmvhitrecoposz50[ntrkt]/F");

    pEventTree->Branch("cmvhittrueposx50", cmvhittrueposx50, "cmvhittrueposx50[ntrkt]/F");
    pEventTree->Branch("cmvhittrueposy50", cmvhittrueposy50, "cmvhittrueposy50[ntrkt]/F");
    pEventTree->Branch("cmvhittrueposz50", cmvhittrueposz50, "cmvhittrueposz50[ntrkt]/F");

    pEventTree->Branch("cmvhitrecoposxerr50", cmvhitrecoposxerr50, "cmvhitrecoposxerr50[ntrkt]/F");
    pEventTree->Branch("cmvhitrecoposyerr50", cmvhitrecoposyerr50, "cmvhitrecoposyerr50[ntrkt]/F");
    pEventTree->Branch("cmvhitrecoposzerr50", cmvhitrecoposzerr50, "cmvhitrecoposzerr50[ntrkt]/F");



    pEventTree->Branch("LeTime50", LeTime50, "LeTime50[ntrkt]/F");
  pEventTree->Branch("RiTime50", RiTime50, "RiTime50[ntrkt]/F");

    pEventTree->Branch("LePulse50", LePulse50, "LePulse50[ntrkt]/F");
  pEventTree->Branch("RiPulse50", RiPulse50, "RiPulse50[ntrkt]/F");







  //pEventTree->Branch("ellip_diff50", ellip_diff50, "ellip_diff50[ntrkt]/F");


 pEventTree->Branch("clustersize51", clustersize51, "clustersize51[ntrkt]/i");
    pEventTree->Branch("cmv_locno51", cmv_locno51, "cmv_locno51[ntrkt]/i");
  pEventTree->Branch("extrapolposx51", extrapolposx51, "extrapolposx51[ntrkt]/F");
    pEventTree->Branch("extrapolposy51", extrapolposy51, "extrapolposy51[ntrkt]/F");
    pEventTree->Branch("extrapolposz51", extrapolposz51, "extrapolposz51[ntrkt]/F");

    pEventTree->Branch("extrapolposxerr51", extrapolposxerr51, "extrapolposxerr51[ntrkt]/F");
    pEventTree->Branch("extrapolposyerr51", extrapolposyerr51, "extrapolposyerr51[ntrkt]/F");
    pEventTree->Branch("extrapolposzerr51", extrapolposzerr51, "extrapolposzerr51[ntrkt]/F");

    pEventTree->Branch("cmvhitrecoposx51", cmvhitrecoposx51, "cmvhitrecoposx51[ntrkt]/F");
    pEventTree->Branch("cmvhitrecoposy51", cmvhitrecoposy51, "cmvhitrecoposy51[ntrkt]/F");
    pEventTree->Branch("cmvhitrecoposz51", cmvhitrecoposz51, "cmvhitrecoposz51[ntrkt]/F");

    pEventTree->Branch("cmvhittrueposx51", cmvhittrueposx51, "cmvhittrueposx51[ntrkt]/F");
    pEventTree->Branch("cmvhittrueposy51", cmvhittrueposy51, "cmvhittrueposy51[ntrkt]/F");
    pEventTree->Branch("cmvhittrueposz51", cmvhittrueposz51, "cmvhittrueposz51[ntrkt]/F");

    pEventTree->Branch("cmvhitrecoposxerr51", cmvhitrecoposxerr51, "cmvhitrecoposxerr51[ntrkt]/F");
    pEventTree->Branch("cmvhitrecoposyerr51", cmvhitrecoposyerr51, "cmvhitrecoposyerr51[ntrkt]/F");
    pEventTree->Branch("cmvhitrecoposzerr51", cmvhitrecoposzerr51, "cmvhitrecoposzerr51[ntrkt]/F");




    pEventTree->Branch("LeTime51", LeTime51, "LeTime51[ntrkt]/F");
  pEventTree->Branch("RiTime51", RiTime51, "RiTime51[ntrkt]/F");

    pEventTree->Branch("LePulse51", LePulse51, "LePulse51[ntrkt]/F");
  pEventTree->Branch("RiPulse51", RiPulse51, "RiPulse51[ntrkt]/F");






  //    pEventTree->Branch("ellip_diff51", ellip_diff51, "ellip_diff51[ntrkt]/F");
 pEventTree->Branch("clustersize52", clustersize52, "clustersize52[ntrkt]/i");
pEventTree->Branch("cmv_locno52", cmv_locno52, "cmv_locno52[ntrkt]/i");
  pEventTree->Branch("extrapolposx52", extrapolposx52, "extrapolposx52[ntrkt]/F");
    pEventTree->Branch("extrapolposy52", extrapolposy52, "extrapolposy52[ntrkt]/F");
    pEventTree->Branch("extrapolposz52", extrapolposz52, "extrapolposz52[ntrkt]/F");

    pEventTree->Branch("extrapolposxerr52", extrapolposxerr52, "extrapolposxerr52[ntrkt]/F");
    pEventTree->Branch("extrapolposyerr52", extrapolposyerr52, "extrapolposyerr52[ntrkt]/F");
    pEventTree->Branch("extrapolposzerr52", extrapolposzerr52, "extrapolposzerr52[ntrkt]/F");

    pEventTree->Branch("cmvhitrecoposx52", cmvhitrecoposx52, "cmvhitrecoposx52[ntrkt]/F");
    pEventTree->Branch("cmvhitrecoposy52", cmvhitrecoposy52, "cmvhitrecoposy52[ntrkt]/F");
    pEventTree->Branch("cmvhitrecoposz52", cmvhitrecoposz52, "cmvhitrecoposz52[ntrkt]/F");

    pEventTree->Branch("cmvhittrueposx52", cmvhittrueposx52, "cmvhittrueposx52[ntrkt]/F");
    pEventTree->Branch("cmvhittrueposy52", cmvhittrueposy52, "cmvhittrueposy52[ntrkt]/F");
    pEventTree->Branch("cmvhittrueposz52", cmvhittrueposz52, "cmvhittrueposz52[ntrkt]/F");

    pEventTree->Branch("cmvhitrecoposxerr52", cmvhitrecoposxerr52, "cmvhitrecoposxerr52[ntrkt]/F");
    pEventTree->Branch("cmvhitrecoposyerr52", cmvhitrecoposyerr52, "cmvhitrecoposyerr52[ntrkt]/F");
    pEventTree->Branch("cmvhitrecoposzerr52", cmvhitrecoposzerr52, "cmvhitrecoposzerr52[ntrkt]/F");



    pEventTree->Branch("LeTime52", LeTime52, "LeTime52[ntrkt]/F");
  pEventTree->Branch("RiTime52", RiTime52, "RiTime52[ntrkt]/F");

    pEventTree->Branch("LePulse52", LePulse52, "LePulse52[ntrkt]/F");
  pEventTree->Branch("RiPulse52", RiPulse52, "RiPulse52[ntrkt]/F");



  //    pEventTree->Branch("ellip_diff52", ellip_diff52, "ellip_diff52[ntrkt]/F");

pEventTree->Branch("clustersize60", clustersize60, "clustersize60[ntrkt]/i");
pEventTree->Branch("cmv_locno60", cmv_locno60, "cmv_locno60[ntrkt]/i");
  pEventTree->Branch("extrapolposx60", extrapolposx60, "extrapolposx60[ntrkt]/F");
    pEventTree->Branch("extrapolposy60", extrapolposy60, "extrapolposy60[ntrkt]/F");
    pEventTree->Branch("extrapolposz60", extrapolposz60, "extrapolposz60[ntrkt]/F");

    pEventTree->Branch("extrapolposxerr60", extrapolposxerr60, "extrapolposxerr60[ntrkt]/F");
    pEventTree->Branch("extrapolposyerr60", extrapolposyerr60, "extrapolposyerr60[ntrkt]/F");
    pEventTree->Branch("extrapolposzerr60", extrapolposzerr60, "extrapolposzerr60[ntrkt]/F");

    pEventTree->Branch("cmvhitrecoposx60", cmvhitrecoposx60, "cmvhitrecoposx60[ntrkt]/F");
    pEventTree->Branch("cmvhitrecoposy60", cmvhitrecoposy60, "cmvhitrecoposy60[ntrkt]/F");
    pEventTree->Branch("cmvhitrecoposz60", cmvhitrecoposz60, "cmvhitrecoposz60[ntrkt]/F");

    pEventTree->Branch("cmvhittrueposx60", cmvhittrueposx60, "cmvhittrueposx60[ntrkt]/F");
    pEventTree->Branch("cmvhittrueposy60", cmvhittrueposy60, "cmvhittrueposy60[ntrkt]/F");
    pEventTree->Branch("cmvhittrueposz60", cmvhittrueposz60, "cmvhittrueposz60[ntrkt]/F");

    pEventTree->Branch("cmvhitrecoposxerr60", cmvhitrecoposxerr60, "cmvhitrecoposxerr60[ntrkt]/F");
    pEventTree->Branch("cmvhitrecoposyerr60", cmvhitrecoposyerr60, "cmvhitrecoposyerr60[ntrkt]/F");
    pEventTree->Branch("cmvhitrecoposzerr60", cmvhitrecoposzerr60, "cmvhitrecoposzerr60[ntrkt]/F");



    pEventTree->Branch("LeTime60", LeTime60, "LeTime60[ntrkt]/F");
  pEventTree->Branch("RiTime60", RiTime60, "RiTime60[ntrkt]/F");

    pEventTree->Branch("LePulse60", LePulse60, "LePulse60[ntrkt]/F");
  pEventTree->Branch("RiPulse60", RiPulse60, "RiPulse60[ntrkt]/F");







  //pEventTree->Branch("ellip_diff60", ellip_diff60, "ellip_diff60[ntrkt]/F");


 pEventTree->Branch("clustersize61", clustersize61, "clustersize61[ntrkt]/i");
    pEventTree->Branch("cmv_locno61", cmv_locno61, "cmv_locno61[ntrkt]/i");
  pEventTree->Branch("extrapolposx61", extrapolposx61, "extrapolposx61[ntrkt]/F");
    pEventTree->Branch("extrapolposy61", extrapolposy61, "extrapolposy61[ntrkt]/F");
    pEventTree->Branch("extrapolposz61", extrapolposz61, "extrapolposz61[ntrkt]/F");

    pEventTree->Branch("extrapolposxerr61", extrapolposxerr61, "extrapolposxerr61[ntrkt]/F");
    pEventTree->Branch("extrapolposyerr61", extrapolposyerr61, "extrapolposyerr61[ntrkt]/F");
    pEventTree->Branch("extrapolposzerr61", extrapolposzerr61, "extrapolposzerr61[ntrkt]/F");

    pEventTree->Branch("cmvhitrecoposx61", cmvhitrecoposx61, "cmvhitrecoposx61[ntrkt]/F");
    pEventTree->Branch("cmvhitrecoposy61", cmvhitrecoposy61, "cmvhitrecoposy61[ntrkt]/F");
    pEventTree->Branch("cmvhitrecoposz61", cmvhitrecoposz61, "cmvhitrecoposz61[ntrkt]/F");

    pEventTree->Branch("cmvhittrueposx61", cmvhittrueposx61, "cmvhittrueposx61[ntrkt]/F");
    pEventTree->Branch("cmvhittrueposy61", cmvhittrueposy61, "cmvhittrueposy61[ntrkt]/F");
    pEventTree->Branch("cmvhittrueposz61", cmvhittrueposz61, "cmvhittrueposz61[ntrkt]/F");

    pEventTree->Branch("cmvhitrecoposxerr61", cmvhitrecoposxerr61, "cmvhitrecoposxerr61[ntrkt]/F");
    pEventTree->Branch("cmvhitrecoposyerr61", cmvhitrecoposyerr61, "cmvhitrecoposyerr61[ntrkt]/F");
    pEventTree->Branch("cmvhitrecoposzerr61", cmvhitrecoposzerr61, "cmvhitrecoposzerr61[ntrkt]/F");




    pEventTree->Branch("LeTime61", LeTime61, "LeTime61[ntrkt]/F");
  pEventTree->Branch("RiTime61", RiTime61, "RiTime61[ntrkt]/F");

    pEventTree->Branch("LePulse61", LePulse61, "LePulse61[ntrkt]/F");
  pEventTree->Branch("RiPulse61", RiPulse61, "RiPulse61[ntrkt]/F");






  //    pEventTree->Branch("ellip_diff61", ellip_diff61, "ellip_diff61[ntrkt]/F");
 pEventTree->Branch("clustersize62", clustersize62, "clustersize62[ntrkt]/i");
pEventTree->Branch("cmv_locno62", cmv_locno62, "cmv_locno62[ntrkt]/i");
  pEventTree->Branch("extrapolposx62", extrapolposx62, "extrapolposx62[ntrkt]/F");
    pEventTree->Branch("extrapolposy62", extrapolposy62, "extrapolposy62[ntrkt]/F");
    pEventTree->Branch("extrapolposz62", extrapolposz62, "extrapolposz62[ntrkt]/F");

    pEventTree->Branch("extrapolposxerr62", extrapolposxerr62, "extrapolposxerr62[ntrkt]/F");
    pEventTree->Branch("extrapolposyerr62", extrapolposyerr62, "extrapolposyerr62[ntrkt]/F");
    pEventTree->Branch("extrapolposzerr62", extrapolposzerr62, "extrapolposzerr62[ntrkt]/F");

    pEventTree->Branch("cmvhitrecoposx62", cmvhitrecoposx62, "cmvhitrecoposx62[ntrkt]/F");
    pEventTree->Branch("cmvhitrecoposy62", cmvhitrecoposy62, "cmvhitrecoposy62[ntrkt]/F");
    pEventTree->Branch("cmvhitrecoposz62", cmvhitrecoposz62, "cmvhitrecoposz62[ntrkt]/F");

    pEventTree->Branch("cmvhittrueposx62", cmvhittrueposx62, "cmvhittrueposx62[ntrkt]/F");
    pEventTree->Branch("cmvhittrueposy62", cmvhittrueposy62, "cmvhittrueposy62[ntrkt]/F");
    pEventTree->Branch("cmvhittrueposz62", cmvhittrueposz62, "cmvhittrueposz62[ntrkt]/F");

    pEventTree->Branch("cmvhitrecoposxerr62", cmvhitrecoposxerr62, "cmvhitrecoposxerr62[ntrkt]/F");
    pEventTree->Branch("cmvhitrecoposyerr62", cmvhitrecoposyerr62, "cmvhitrecoposyerr62[ntrkt]/F");
    pEventTree->Branch("cmvhitrecoposzerr62", cmvhitrecoposzerr62, "cmvhitrecoposzerr62[ntrkt]/F");



    pEventTree->Branch("LeTime62", LeTime62, "LeTime62[ntrkt]/F");
  pEventTree->Branch("RiTime62", RiTime62, "RiTime62[ntrkt]/F");

    pEventTree->Branch("LePulse62", LePulse62, "LePulse62[ntrkt]/F");
  pEventTree->Branch("RiPulse62", RiPulse62, "RiPulse62[ntrkt]/F");



  //    pEventTree->Branch("ellip_diff62", ellip_diff62, "ellip_diff62[ntrkt]/F");



    pEventTree->Branch("extra_diff1", extra_diff1, "extra_diff1[ntrkt]/F");
    pEventTree->Branch("extra_diff2", extra_diff2, "extra_diff2[ntrkt]/F");
    pEventTree->Branch("extra_diff3", extra_diff3, "extra_diff3[ntrkt]/F");

  // pEventTree->Branch("ellip_diff0", ellip_diff0, "ellip_diff0[ntrkt]/F");
  // pEventTree->Branch("ellip_diff1", ellip_diff1, "ellip_diff1[ntrkt]/F");
  //   pEventTree->Branch("ellip_diff2", ellip_diff2, "ellip_diff2[ntrkt]/F");
  //   pEventTree->Branch("ellip_diff3", ellip_diff3, "ellip_diff3[ntrkt]/F");


    pEventTree->Branch("cmv_nhit", &cmv_nhit, "cmv_nhit/i");
    pEventTree->Branch("cmv_hitid", cmv_hitid, "cmv_hitid[cmv_nhit]/I") ;
    pEventTree->Branch("cmv_hitpdgid", cmv_hitpdgid, "cmv_hitpdgid[cmv_nhit]/I");
    pEventTree->Branch("cmv_hitLeTim", cmv_hitLeTim, "cmv_hitLeTim[cmv_nhit]/F");
    pEventTree->Branch("cmv_hitRiTim", cmv_hitLeTim, "cmv_hitRiTim[cmv_nhit]/F");
    pEventTree->Branch("cmv_hitLePul", cmv_hitLePul, "cmv_hitLePul[cmv_nhit]/F");
    pEventTree->Branch("cmv_hitRiPul", cmv_hitLePul, "cmv_hitRiPul[cmv_nhit]/F");
    pEventTree->Branch("cmv_hitTrueposx", cmv_hitTrueposx, "cmv_hitTrueposx[cmv_nhit]/F");
    pEventTree->Branch("cmv_hitTrueposy", cmv_hitTrueposy, "cmv_hitTrueposy[cmv_nhit]/F");
    pEventTree->Branch("cmv_hitTrueposz", cmv_hitTrueposz, "cmv_hitTrueposz[cmv_nhit]/F");
    // pEventTree->Branch("cmv_hitmom", cmv_hitmom, "cmv_hitmom[cmv_nhit]/F");
    // pEventTree->Branch("cmv_hitthe", cmv_hitthe, "cmv_hitthe[cmv_nhit]/F");
    // pEventTree->Branch("cmv_hitphi", cmv_hitphi, "cmv_hitphi[cmv_nhit]/F");
    pEventTree->Branch("cmv_hitRecoposx", cmv_hitRecoposx, "cmv_hitRecoposx[cmv_nhit]/F");
    pEventTree->Branch("cmv_hitRecoposy", cmv_hitRecoposy, "cmv_hitRecoposy[cmv_nhit]/F");
    pEventTree->Branch("cmv_hitRecoposz", cmv_hitRecoposz, "cmv_hitRecoposz[cmv_nhit]/F");




    //cmv clust branches
    pEventTree->Branch("cmv_nclust", &cmv_nclust, "cmv_nclust/i");
    pEventTree->Branch("cmv_clustid", cmv_clustid, "cmv_clustid[cmv_nclust]/I") ;
    pEventTree->Branch("cmv_clustpdgid", cmv_clustpdgid, "cmv_clustpdgid[cmv_nclust]/I");
    pEventTree->Branch("cmv_clustLeTim", cmv_clustLeTim, "cmv_clustLeTim[cmv_nclust]/F");
    pEventTree->Branch("cmv_clustRiTim", cmv_clustLeTim, "cmv_clustRiTim[cmv_nclust]/F");
    pEventTree->Branch("cmv_clustLePul", cmv_clustLePul, "cmv_clustLePul[cmv_nclust]/F");
    pEventTree->Branch("cmv_clustRiPul", cmv_clustLePul, "cmv_clustRiPul[cmv_nclust]/F");
    pEventTree->Branch("cmv_clustTrueposx", cmv_clustTrueposx, "cmv_clustTrueposx[cmv_nclust]/F");
    pEventTree->Branch("cmv_clustTrueposy", cmv_clustTrueposy, "cmv_clustTrueposy[cmv_nclust]/F");
    pEventTree->Branch("cmv_clustTrueposz", cmv_clustTrueposz, "cmv_clustTrueposz[cmv_nclust]/F");
    // pEventTree->Branch("cmv_clustmom", cmv_clustmom, "cmv_clustmom[cmv_nclust]/F");
    // pEventTree->Branch("cmv_clustthe", cmv_clustthe, "cmv_clustthe[cmv_nclust]/F");
    // pEventTree->Branch("cmv_clustphi", cmv_clustphi, "cmv_clustphi[cmv_nclust]/F");
    pEventTree->Branch("cmv_clustRecoposx", cmv_clustRecoposx, "cmv_clustRecoposx[cmv_nclust]/F");
    pEventTree->Branch("cmv_clustRecoposy", cmv_clustRecoposy, "cmv_clustRecoposy[cmv_nclust]/F");
    pEventTree->Branch("cmv_clustRecoposz", cmv_clustRecoposz, "cmv_clustRecoposz[cmv_nclust]/F");
    pEventTree->Branch("cmv_clustsiz", cmv_clustsiz, "cmv_clustsiz[cmv_nclust]/I") ;








    //extrapol brances
    pEventTree->Branch("cmv_nexphit", &cmv_nexphit, "cmv_nexphit/i");
    pEventTree->Branch("cmv_expid", cmv_expid, "cmv_expid[cmv_nexphit]/I") ;
    pEventTree->Branch("cmv_Expposx", cmv_Expposx, "cmv_Expposx[cmv_nexphit]/F");
    pEventTree->Branch("cmv_Expposy", cmv_Expposy, "cmv_Expposy[cmv_nexphit]/F");
    pEventTree->Branch("cmv_Expposz", cmv_Expposz, "cmv_Expposz[cmv_nexphit]/F");
     pEventTree->Branch("cmv_DCAposx", cmv_DCAposx, "cmv_DCAposx[cmv_nexphit]/F");
    pEventTree->Branch("cmv_DCAposy", cmv_DCAposy, "cmv_DCAposy[cmv_nexphit]/F");
    pEventTree->Branch("cmv_DCAposz", cmv_DCAposz, "cmv_DCAposz[cmv_nexphit]/F");


    pEventTree->Branch("distofclosapp", distofclosapp, "distofclosapp[cmv_nexphit]/F");
    pEventTree->Branch("planeedge", planeedge, "planeedge[cmv_nexphit]/I") ;


 }// if(CardFile->GetCMVD()==1){

    pEventTree->Branch("momend",momend,"momend[ntrkt]/F");
    pEventTree->Branch("theend",theend,"theend[ntrkt]/F");
    pEventTree->Branch("phiend",phiend,"phiend[ntrkt]/F");
    pEventTree->Branch("posxend",posxend,"posxend[ntrkt]/F");
    pEventTree->Branch("posyend",posyend,"posyend[ntrkt]/F");
    pEventTree->Branch("poszend",poszend,"poszend[ntrkt]/F");
    pEventTree->Branch("tx_end",tx_end,"tx_end[ntrkt]/F");
    pEventTree->Branch("ty_end",ty_end,"ty_end[ntrkt]/F");

    pEventTree->Branch("momds"   ,momds   ,"momds[ntrkt]/F");
    pEventTree->Branch("momrg"   ,momrg   ,"momrg[ntrkt]/F");

    pEventTree->Branch("mcxgnvx" ,mcxgnvx ,"mcxgnvx[ntrkt]/F");
    pEventTree->Branch("mcygnvx" ,mcygnvx ,"mcygnvx[ntrkt]/F");
    pEventTree->Branch("momgnvx" ,momgnvx ,"momgnvx[ntrkt]/F");
    pEventTree->Branch("thegnvx" ,thegnvx ,"thegnvx[ntrkt]/F");
    pEventTree->Branch("phignvx" ,phignvx ,"phignvx[ntrkt]/F");
    pEventTree->Branch("momgnend",momgnend,"momgnend[ntrkt]/F");
    pEventTree->Branch("thegnend",thegnend,"thegnend[ntrkt]/F");
    pEventTree->Branch("phignend",phignend,"phignend[ntrkt]/F");

    pEventTree->Branch("vtxzplane",vtxzplane,"vtxzplane[ntrkt]/I");
    pEventTree->Branch("endzplane",endzplane,"endzplane[ntrkt]/I");
    pEventTree->Branch("ntrkcl",ntrkcl,"ntrkcl[ntrkt]/I");
    pEventTree->Branch("ntrkst",ntrkst,"ntrkst[ntrkt]/I");
    pEventTree->Branch("range", &range, "range/F");

    pEventTree->Branch("tx", &tx, "tx/F");
    pEventTree->Branch("ty", &ty, "ty/F");
    pEventTree->Branch("xxin", &xxin, "xxin/F");
    pEventTree->Branch("yyin", &yyin, "yyin/F");
    pEventTree->Branch("txin", &txin, "txin/F");
    pEventTree->Branch("tyin", &tyin, "tyin/F");

    pEventTree->Branch("therr", &therr, "therr/F");
    pEventTree->Branch("pherr", &pherr, "pherr/F");

    pEventTree->Branch("atimslope", &atimslope, "atimslope/F");
    pEventTree->Branch("atiminter", &atiminter, "atiminter/F");


    pEventTree->Branch("xxerr", &xxerr, "xxerr/F");
    pEventTree->Branch("yyerr", &yyerr, "yyerr/F");
    pEventTree->Branch("txerr", &txerr, "txerr/F");
    pEventTree->Branch("tyerr", &tyerr, "tyerr/F");
    pEventTree->Branch("qperr", &qperr, "qperr/F");


   pEventTree->Branch("xxtxerr", &xxtxerr, "xxtxerr/F");
   pEventTree->Branch("xxtyerr", &xxtyerr, "xxtyerr/F");
    pEventTree->Branch("yytyerr", &yytyerr, "yytyerr/F");
    pEventTree->Branch("yytxerr", &yytxerr, "yytxerr/F");
    pEventTree->Branch("txtyerr", &txtyerr, "txtyerr/F");

    pEventTree->Branch("xxenderr", &xxenderr, "xxenderr/F");
    pEventTree->Branch("yyenderr", &yyenderr, "yyenderr/F");
    pEventTree->Branch("txenderr", &txenderr, "txenderr/F");
    pEventTree->Branch("tyenderr", &tyenderr, "tyenderr/F");
    pEventTree->Branch("qpenderr", &qpenderr, "qpenderr/F");

   }




    if(isInOut==2) {
      pEventTree->Branch("ntdc1x",&ntdc1x,"ntdc1x/i");
      pEventTree->Branch("tdcID1x",tdcID1x,"tdcID1x[ntdc1x]/I");
      pEventTree->Branch("TDCval1x",TDCval1x,"TDCval1x[ntdc1x]/F");
      pEventTree->Branch("ntstrp1x",&ntstrp1x,"ntstrp1x/i");
      pEventTree->Branch("StrpID1x",StrpID1x,"StrpID1x[ntstrp1x]/I");
      pEventTree->Branch("ntdc2x",&ntdc2x,"ntdc2x/i");
      pEventTree->Branch("tdcID2x",tdcID2x,"tdcID2x[ntdc2x]/I");
      pEventTree->Branch("TDCval2x",TDCval2x,"TDCval2x[ntdc2x]/F");
      pEventTree->Branch("ntstrp2x",&ntstrp2x,"ntstrp2x/i");
      pEventTree->Branch("StrpID2x",StrpID2x,"StrpID2x[ntstrp2x]/I");

      pEventTree->Branch("ntdc1y",&ntdc1y,"ntdc1y/i");
      pEventTree->Branch("tdcID1y",tdcID1y,"tdcID1y[ntdc1y]/I");
      pEventTree->Branch("TDCval1y",TDCval1y,"TDCval1y[ntdc1y]/F");
      pEventTree->Branch("ntstrp1y",&ntstrp1y,"ntstrp1y/i");
      pEventTree->Branch("StrpID1y",StrpID1y,"StrpID1y[ntstrp1y]/I");
      pEventTree->Branch("ntdc2y",&ntdc2y,"ntdc2y/i");
      pEventTree->Branch("tdcID2y",tdcID2y,"tdcID2y[ntdc2y]/I");
      pEventTree->Branch("TDCval2y",TDCval2y,"TDCval2y[ntdc2y]/F");
      pEventTree->Branch("ntstrp2y",&ntstrp2y,"ntstrp2y/i");
      pEventTree->Branch("StrpID2y",StrpID2y,"StrpID2y[ntstrp2y]/I");
      pEventTree->Branch("ntrecord1x",&ntrecord1x,"ntrecord1x/i");
      pEventTree->Branch("striprec1x",striprec1x,"striprec1x[ntrecord1x]/I");
      pEventTree->Branch("tdcrec1x",tdcrec1x,"tdcrec1x[ntrecord1x]/F");
      pEventTree->Branch("ntrecord1y",&ntrecord1y,"ntrecord1y/i");
      pEventTree->Branch("striprec1y",striprec1y,"striprec1y[ntrecord1y]/I");
      pEventTree->Branch("tdcrec1y",tdcrec1y,"tdcrec1y[ntrecord1y]/F");
      pEventTree->Branch("ntrecord2x",&ntrecord2x,"ntrecord2x/i");
      pEventTree->Branch("striprec2x",striprec2x,"striprec2x[ntrecord2x]/I");
      pEventTree->Branch("tdcrec2x",tdcrec2x,"tdcrec2x[ntrecord2x]/F");
      pEventTree->Branch("ntrecord2y",&ntrecord2y,"ntrecord2y/i");
      pEventTree->Branch("striprec2y",striprec2y,"striprec2y[ntrecord2y]/I");
      pEventTree->Branch("tdcrec2y",tdcrec2y,"tdcrec2y[ntrecord2y]/F");
    }

    if(isVisOut==1) {
      char outVisFile[300];
      sprintf(outVisFile,"%s.inh",outfile);
      pVisFile = new TFile(outVisFile, "RECREATE"); //VALGRIND
      if (!pVisFile) {
	cout << "Error opening .inh file !" << endl;
	exit(-1);
      } else {
	cout << "Vis Hits stored in: " << outVisFile << endl;
      }
      pVisFile->cd();
      EveCnt=0;//Event Counter
            if(!H) H = new Hits(); //VALGRIND
       if(!Hp) Hp= new HitPos();
      nloops=0;// number of tree fills
      if(!visTree){visTree = new TTree("Hitstree","Geant Hits File");}
      visTree->Branch("Hits_Branch","Hits",&H,1600000,2);
      H->Clear();
      Hp->Clear();
      H->ClearTracks();
      H->ENum=0;
    }


    char namex[200];
    for(int iaj=0; iaj<numberInLA; iaj++) {
      sprintf(namex,"hdifftime1_xy_%i",iaj);
      hdifftime1[iaj] = new TH1D(namex,namex,120,-25.,25.);
      sprintf(namex,"hdifftime2_xy_%i",iaj);
      hdifftime2[iaj] = new TH1D(namex,namex,120,-5.,5.);

      sprintf(namex,"hxtime_ext_%i",iaj);
      hxtime_ext[iaj] = new TH1D(namex,namex,120,-25.,25.);
      sprintf(namex,"hytime_ext_%i",iaj);
      hytime_ext[iaj] = new TH1D(namex,namex,120,-25.,25.);

      sprintf(namex,"hxpos_ext_%i",iaj);
      hxpos_ext[iaj] = new TH1D(namex,namex,120,-6.,6.);
      sprintf(namex,"hypos_ext_%i",iaj);
      hypos_ext[iaj] = new TH1D(namex,namex,120,-6.,6.);
      sprintf(namex,"hxpos_ext_kalman_%i",iaj);
      hxpos_ext_kalman[iaj] = new TH1D(namex,namex,120,-6.,6.);
      sprintf(namex,"hypos_ext_kalman_%i",iaj);
      hypos_ext_kalman[iaj] = new TH1D(namex,namex,120,-6.,6.);

      sprintf(namex,"h_hit_time_ext_%i",iaj);
      h_hit_time_ext[iaj] = new TH1D(namex,namex,120,-25.,25.);
      // for(int jak=0; jak<8; jak++) {
      // 	sprintf(namex,"xtdc_minus_ref_l%i_%i",iaj,jak);
      // 	xtdc_minus_ref[iaj][jak] = new TH1D(namex,namex,120,-20000.,200000);
      // 	sprintf(namex,"ytdc_minus_ref_l%i_%i",iaj,jak);
      // 	ytdc_minus_ref[iaj][jak] = new TH1D(namex,namex,120,-20000.,200000);
      // 	sprintf(namex,"tshift_xtdc_minus_ref%i_%i",iaj,jak);
      // 	tshift_xtdc_minus_ref[iaj][jak] = new TH1D(namex,namex,120,-20000.,200000);
      // 	sprintf(namex,"tshift_ytdc_minus_ref%i_%i",iaj,jak);
      // 	tshift_ytdc_minus_ref[iaj][jak] = new TH1D(namex,namex,120,-20000.,200000);
      // }
    }
    // DGap = new TH1D("DetGap"," Plot to see the performance of code at detgap ",10,0,10);  //asm
    // ShwXw	= new TH1D("ShwXw ","This is a distribution for X position resolution in shower region " ,80 , -3.14 , 3.14);  //asm
    // ShwYw	= new TH1D("ShwYw ","This is a distribution for Y position resolution in shower region " ,80 , -3.14 , 3.14);  //asm
    trk_edge = new TH2D("theta_dmn at trk_edge","theta_dmn at trk_edge" ,20, 0 ,20,200,0,3);  //asm
    trk_gap = new TH1D("difference in energy lost in the  dead space and the tracklength - )"," dpp-m*dxx + c" ,100, -5 ,5);  //asm
    // if(!pPosX)pPosX = new TH1D("deltaX", "#Delta X (cm)", 100, -52.5, 52.5);
    // if(!pPosY)pPosY = new TH1D("deltaY", "#Delta Y (cm)", 100, -52.5, 52.5);
    // if(!pPosZ)pPosZ = new TH1D("deltaZ", "#Delta Z (cm)", 100, -2.5, 2.5);
    // if(!pPosXX)pPosXX = new TH2D("deltaXX", "#Delta XX (cm)", 100, -2500, 2500, 100, -52.5, 52.5);
    // if(!pPosYY)pPosYY = new TH2D("deltaYY", "#Delta YY (cm)", 100, -500, 500, 100, -52.5, 52.5);
    // if(!pPosZZ)pPosZZ = new TH2D("deltaZZ", "#Delta ZZ (cm)", 100, -1000, 1000, 100, -2.5, 2.5);
  }


void MultiSimAnalysisDigi::OpenCollatedRootFile() {
  cout<<"Reading Collated Root Input file for digitization."<<endl;
  cout<<"collatedin "<<collatedIn<<endl;
  if(collatedIn) {
    collatedRootFile = new TFile("Collated_evtraw_20181120_20210813ai_5of8.root","read");
    if(!collatedRootFile) {
      cout << "Error opening collated file !" << endl;
      exit(-1);
    } else{
      cout<<"Collated file open successful..."<<endl;
      // char namex[200];
      // for(int iki=0; iki<numberInLA; iki++) {
      // 	sprintf(namex,"inefficiency_corx_l%i",iki);
      // 	inefficiency_corx[iki] = (TH2D*)collatedRootFile->Get(namex);
      // 	sprintf(namex,"inefficiency_uncx_l%i",iki);
      // 	inefficiency_uncx[iki] = (TH2D*)collatedRootFile->Get(namex);
      // 	// cout<<"inefficiency_uncx["<<iki<<"] "<<inefficiency_uncx[iki]<<endl;
      // 	sprintf(namex,"inefficiency_uncy_l%i",iki);
      // 	inefficiency_uncy[iki] = (TH2D*)collatedRootFile->Get(namex);
      // 	// cout<<"inefficiency_uncy["<<iki<<"] "<<inefficiency_uncy[iki]<<endl;
      // 	sprintf(namex,"triggereffi_xevt_l%i",iki);
      // 	triggereffi_xevt[iki] = (TH2D*)collatedRootFile->Get(namex);
      // 	sprintf(namex,"triggereffi_yevt_l%i",iki);
      // 	triggereffi_yevt[iki] = (TH2D*)collatedRootFile->Get(namex);
      // 	sprintf(namex,"strp_xmulsim_cor_l%i",iki);
      // 	strp_xmulsim_cor[iki] = (TH2D*)collatedRootFile->Get(namex);
      // 	sprintf(namex,"strp_ymulsim_cor_l%i",iki);
      // 	strp_ymulsim_cor[iki] = (TH2D*)collatedRootFile->Get(namex);
      // }

 char namex[200];
    for(int iki=0; iki<numberInLA; iki++) {
      sprintf(namex,"inefficiency_cor_m0_xr0_yr0_l%i",iki);
      inefficiency_corx[iki] = (TH2D*)collatedRootFile->Get(namex);
      sprintf(namex,"inefficiency_unc_m0_xr0_yr0_x%i",iki);
      inefficiency_uncx[iki] = (TH2D*)collatedRootFile->Get(namex);
      cout<<"inefficiency_uncx["<<iki<<"] "<<inefficiency_uncx[iki]<<endl;
      sprintf(namex,"inefficiency_unc_m0_xr0_yr0_y%i",iki);
      inefficiency_uncy[iki] = (TH2D*)collatedRootFile->Get(namex);
      cout<<"inefficiency_uncy["<<iki<<"] "<<inefficiency_uncy[iki]<<endl;
      sprintf(namex,"triggereffi_evt_m0_xr0_yr0_x%i",iki);
      triggereffi_xevt[iki] = (TH2D*)collatedRootFile->Get(namex);
      sprintf(namex,"triggereffi_evt_m0_xr0_yr0_y%i",iki);
      triggereffi_yevt[iki] = (TH2D*)collatedRootFile->Get(namex);
      sprintf(namex,"strp_xmulsim_cor_l%i",iki);
      strp_xmulsim_cor[iki] = (TH2D*)collatedRootFile->Get(namex);
      sprintf(namex,"strp_ymulsim_cor_l%i",iki);
      strp_ymulsim_cor[iki] = (TH2D*)collatedRootFile->Get(namex);
    }
    for(int iki=0; iki<numberInLA; iki++) {
      for(int ikj=0; ikj<16; ikj++) {
        for(int ikk=0; ikk<16; ikk++) {
          sprintf(namex,"blk_xmullaysim_m0_xr0_yr0_l%i_%i_%i",iki,ikj,ikk);
          block_xmulsim[iki][ikj][ikk] = (TH2D*)collatedRootFile->Get(namex);
          sprintf(namex,"blk_ymullaysim_m0_xr0_yr0_l%i_%i_%i",iki,ikj,ikk);
          block_ymulsim[iki][ikj][ikk] = (TH2D*)collatedRootFile->Get(namex);
        }
      }
    } // for(int iki=0; iki<numberInLA; iki++) {





    }
  }
}

void MultiSimAnalysisDigi::CloseInputRootFiles() {
  if (inputRootFile) {
    inputRootFile->cd();
    inputRootFile->Close(); //valgrind RSA
    // delete inputEventTree; inputEventTree=0;
    delete inputRootFile; inputRootFile=0;
    cout << "Input root file close !" << endl;
  } else {
    cout << "No input file to close.... !" << endl;
  }

}

void MultiSimAnalysisDigi::CloseOutputRootFiles() {
  if (pRootFile) {
    pRootFile->cd();
    if (pEventTree) { pEventTree->Write(); delete pEventTree; pEventTree=0;}
    pRootFile->Write();

    if (pPosX) {pPosX->Write(); delete pPosX; pPosX=0;}
    if (pPosY) {pPosY->Write(); delete pPosY; pPosY=0;}
    if (pPosZ) {pPosZ->Write(); delete pPosZ; pPosZ=0;}

    if (pPosXX) {pPosXX->Write(); delete pPosXX; pPosXX=0;}
    if (pPosYY) {pPosYY->Write(); delete pPosYY; pPosYY=0;}
    if (pPosZZ) {pPosZZ->Write(); delete pPosZZ; pPosZZ=0;}

    for (int i=0; i<20; i++) {if (pdedz[i]) {pdedz[i]->Write();  delete pdedz[i]; pdedz[i]=0;}};

    if (hitDist) {hitDist->Write(); delete hitDist; hitDist=0;}   //asm
    if (TrkDist) {TrkDist->Write(); delete TrkDist; TrkDist=0;}   //asm
    if (EffDist) {EffDist->Write(); delete EffDist; EffDist=0;}   //asm
    if (InoTrack_listsize) {InoTrack_listsize->Write(); delete InoTrack_listsize;  InoTrack_listsize=0;} //asm

    if (ShwXw) {ShwXw->Write(); delete ShwXw; ShwXw=0;}
    if (ShwYw) {ShwYw->Write(); delete ShwYw; ShwYw=0;}
    if (RC) {RC->Write(); delete RC; RC=0;}
    if (DGap) {DGap->Write(); delete DGap; DGap=0;}

    if (DeadStripX) {DeadStripX->Write(); delete DeadStripX; DeadStripX=0;}
    if (DeadStripY) {DeadStripY->Write(); delete DeadStripY; DeadStripY=0;}
    if (NoisyStripX) {NoisyStripX->Write(); delete NoisyStripX; NoisyStripX=0;}
    if (NoisyStripY) {NoisyStripY->Write(); delete NoisyStripY; NoisyStripY=0;}
    if (DiffTime) {DiffTime->Write(); delete DiffTime; DiffTime=0;}
    if (strpXtime) {strpXtime->Write(); delete strpXtime; strpXtime=0;}
    if (strpYtime) {strpYtime->Write(); delete strpYtime; strpYtime=0;}
    if (strpXtimeCorr) {strpXtimeCorr->Write(); delete strpXtimeCorr; strpXtimeCorr=0;}
    if (strpYtimeCorr) {strpYtimeCorr->Write(); delete strpYtimeCorr; strpYtimeCorr=0;}
    if (hitXtime) {hitXtime->Write(); delete hitXtime; hitXtime=0;}
    if (hitYtime) {hitYtime->Write(); delete hitYtime; hitYtime=0;}
    if (smagFieldX) {smagFieldX->Write(); delete smagFieldX; smagFieldX=0;}
    if (smagFieldY) {smagFieldY->Write(); delete smagFieldY; smagFieldY=0;}
    if (smag2dX) {smag2dX->Write(); delete smag2dX; smag2dX=0;}
    if (smag2dY) {smag2dY->Write(); delete smag2dY; smag2dY=0;}
    if (rmagFieldX) {rmagFieldX->Write(); delete rmagFieldX; rmagFieldX=0;}
    if (rmagFieldY) {rmagFieldY->Write(); delete rmagFieldY; rmagFieldY=0;}
    if (rmag2dX) {rmag2dX->Write(); delete rmag2dX; rmag2dX=0;}
    if (rmag2dY) {rmag2dY->Write(); delete rmag2dY; rmag2dY=0;}
    if (smag2dXYpixel_iron) {smag2dXYpixel_iron->Write(); delete smag2dXYpixel_iron; smag2dXYpixel_iron=0;}
    if (smag2dXYpixel_air) {smag2dXYpixel_air->Write(); delete smag2dXYpixel_air; smag2dXYpixel_air=0;}
    if (rmag2dXYpixel_iron) {rmag2dXYpixel_iron->Write(); delete rmag2dXYpixel_iron; rmag2dXYpixel_iron=0;}
    if (rmag2dXYpixel_air) {rmag2dXYpixel_air->Write(); delete rmag2dXYpixel_air; rmag2dXYpixel_air=0;}
    if (xyvsbxin) {xyvsbxin->Write(); delete xyvsbxin; xyvsbxin=0;}
    if (xyvsbyin) {xyvsbyin->Write(); delete xyvsbyin; xyvsbyin=0;}
    if (xyvsbxdiff) {xyvsbxdiff->Write(); delete xyvsbxdiff; xyvsbxdiff=0;}
    if (xyvsbydiff) {xyvsbydiff->Write(); delete xyvsbydiff; xyvsbydiff=0;}
    // if (xyvsbxindiff) {xyvsbxindiff->Write(); delete xyvsbxindiff; xyvsbxindiff=0;}
    // if (xyvsbyindiff) {xyvsbyindiff->Write(); delete xyvsbyindiff; xyvsbyindiff=0;}
    if (xyvsbxout) {xyvsbxout->Write(); delete xyvsbxout; xyvsbxout=0;}
    if (xyvsbyout) {xyvsbyout->Write(); delete xyvsbyout; xyvsbyout=0;}

    // for(int iai=0; iai<numberInLA; iai++) {
    //   if(hdifftime1[iai]) {hdifftime1[iai]->Write(); delete hdifftime1[iai]; hdifftime1[iai]=0;}
    //   if(hdifftime2[iai]) {hdifftime2[iai]->Write(); delete hdifftime2[iai]; hdifftime2[iai]=0;}
    //   if(hdifftime1[iai]) {hdifftime1[iai]->Write(); delete hdifftime1[iai]; hdifftime1[iai]=0;}
    pRootFile->Close();
    delete pRootFile; pRootFile=0;
    cout << "Output root file  close !" << endl;
  }  else {
    cout << "Output root file cannot close !" << endl;
  } // if (pRootFile) {

  if (pVisFile) {
    pVisFile->cd();
    visTree->Fill(); // fill tree
    H->Clear();
    Hp->Clear();
    H->ClearTracks();
    pVisFile->Write(); //VALGRIND
    cout<< "Write to .inh file  done"<<endl;
    if (visTree) {delete visTree; visTree=0;}
    pVisFile->Close();
    if (Hp) {delete Hp; Hp=0;}
    if (H) {delete H; H=0;}
    delete pVisFile; pVisFile=0;
    cout << "Visualization file  close !" << endl;
  } else {
    cout << "No output Hit Display Tree !" << endl;
  }

  if (collatedRootFile) {
    collatedRootFile->cd();
    for(int iki=0; iki<numberInLA; iki++) {
      delete inefficiency_corx[iki];
      delete inefficiency_uncx[iki];
      delete inefficiency_uncy[iki];
      delete triggereffi_xevt[iki];
      delete triggereffi_yevt[iki];
      delete strp_xmulsim_cor[iki];
      delete strp_ymulsim_cor[iki];
    }
    collatedRootFile->Close();
    delete collatedRootFile; collatedRootFile=0;
    cout<<"Collated root file closed."<<endl;
  } else {
    cout << "No collated histograms !" << endl;
  }

}

void MultiSimAnalysisDigi::SetCorrTimeError(G4double val) {
  cout<<"void MultiSimAnalysisDigi::SetCorrTimeError(G4double "<<val<<")"<<endl;
  CorrTimeError = val;
}

void MultiSimAnalysisDigi::SetUnCorrTimeError(G4double val) {
  cout<<"void MultiSimAnalysisDigi::SetUnCorrTimeError(G4double "<<val<<")"<<endl;
  UnCorrTimeError = val;
}

void MultiSimAnalysisDigi::SetTimeToDigiConvVal(G4double val) {
  cout<<"void MultiSimAnalysisDigi::SetTimeToDigiConvVal(G4double "<<val<<")"<<endl;
  TimeToDigiConv = val;
}

void MultiSimAnalysisDigi::SetSignalSpeedVal(G4double val) {
  cout<<"void MultiSimAnalysisDigi::SetSignalSpeedVal(G4double "<<val<<")"<<endl;
  SignalSpeed = val;
}


void MultiSimAnalysisDigi::SetPhotonSpeedVal(G4double val) {
  cout<<"void MultiSimAnalysis::SetPhotonSpeedVal(G4double "<<val<<")"<<endl;
  PhotonSpeed = val;
}

void MultiSimAnalysisDigi::SetCMVadctons(G4double val) {
  cout<<"void MultiSimAnalysis::SetCMVadctons(G4double "<<val<<")"<<endl;
  CMVadctons = val;
}









void MultiSimAnalysisDigi::SaveGenVisFile() {

  cout<<"void MultiSimAnalysisDigi::SaveGenVisFile() {"<<endl;
  if(isVisOut==1) {
    for(unsigned ij=0;ij<ngent;ij++) {
      H->NParticles++;
      Hp=  H->AddHits(0,0);
      Hp->TrackType=-14;
      Hp->ParCode= pidin[ij];
      Hp->ZZ= (((numberInLA*(parirlay[2]+parlay[2])*cm/m-parlay[2])- poszin[ij]*cm/m))/((parirlay[2]+parlay[2])*2*(1/m));
      Hp->XX= posxin[ij]*cm/m;
      Hp->YY= posyin[ij]*cm/m;
      Hp->pmag= momin[ij];
      Hp->pt= thein[ij];
      Hp->pp= phiin[ij];
    } // for(unsigned ij=0;ij< ngent;ij++) {
  } // if(isVisOut==1) {
}
