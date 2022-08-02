// Reconstruction Code of DIGI output
//argv[0]: 
//argv[1]:input file name containing root files
//argv[2]:Input options: 0: SIM -> DIFI, 1 : DIGI -> RECO (MC), 2: DIGI->RECO (Data), 3: SIM-> RECO (MC) //inout3
//argv[3]:Least Count of TDC- 0.1
//argv[4]: filename Alignement and offset correction for data
//argv[5]: Print Level- 1000
//argv[6]:CMVD Flag: 0: dont include CMVD files, 1: Use it
//argv[7]:Mag Field: 0: Use Straightline Fit; 1: Field is there
//argv[8]: Trackfit Flag: 0:(Old) SwimSwimmer,1: (New)Kohlahal
//argv[9]:Gdml Option: 0:- One stack center & No CMVD, 1:- Two Stacks & No CMVD, 2:  Two Stacks with CMVD




#include <iostream>
#include <iomanip>
#include <stdlib.h>
#include <fstream>
#include <time.h>
#include <map>
#include <iomanip>
#include <utility>
#include "ParameterMessenger.hh"
#include "micalDetectorParameterDef.hh"
#include "micalFieldPropagator.hh"
#include "InoDigiAlg.hh"
#include "InoRecoAlg.hh"
#include "CMVDRecoAlg.hh"
#include "CMVDigiAlg.hh"
#include "GeneralRecoInfo.hh"
#include "MultiSimAnalysisDigi.hh"
#include "vect_manager.h"


int main(int argc, char** argv) {
  
  cout<<"---------------------------------------"<<endl;
  G4cout <<"argc "<<argc<<" "<<argv[0]<<" "<<argv[1]<<" "<<argv[2]<<" "<<argv[3]<<" "<<argv[4]<<" "<<argv[5]<<" "<<argv[6]<<" "<<argv[7]<<" "<<argv[8]<<" "<<argv[9]<<G4endl;
  bool runcode = true;

  ParameterMessenger* detectorConfig = new ParameterMessenger();

  int InputOutput = atoi(argv[2]);
  double TimeToDigi = atof(argv[3]);
  int printModulo = atoi(argv[5]);
  int CMVDFlag = atoi(argv[6]); 
  int MagFlag =  atoi(argv[7]);
  int TrackFitFlag =  atoi(argv[8]);
  int gdmlOption = atoi(argv[9]);
  int collatedIn;


  if(InputOutput==0 || InputOutput==3) {
    collatedIn = 1;//atoi(argv[4]);
  } else {
    collatedIn = 0;
  }
  
  detectorConfig->SetInputOutput(InputOutput);
  detectorConfig->SetTimeToDigiConv(TimeToDigi);
  detectorConfig->SetCollatedIn(collatedIn);
  detectorConfig->SetCMVD(CMVDFlag);
  detectorConfig->SetMag(MagFlag);
  detectorConfig->SetTrackFit(TrackFitFlag);
  detectorConfig->SetgdmlOption(gdmlOption);

  
  detectorConfig->PrintParameters();
  
  cout<<"InputOutput: "<<detectorConfig->GetInputOutput();
  cout<<"TimeToDigi: "<<detectorConfig->GetTimeToDigiConv();
  cout<<"CollatedIn: "<< detectorConfig->GetCollatedIn();
  cout<<"CMVD: "<<  detectorConfig->GetCMVD();
  cout<<"Mag: "<< detectorConfig->GetMag();
  cout<<"gdmlOption: "<< detectorConfig->GetgdmlOption();
  
  micalDetectorParameterDef* paradef = new micalDetectorParameterDef;
  InoGeometry_Manager* geoManager;
  GeneralRecoInfo* greco;
  micalFieldPropagator *pfield;
  MultiSimAnalysisDigi *pAnalysis;
  
  char fileCorrName[300];
  if(InputOutput) {
    if(InputOutput==1 ||InputOutput==3 ) {
      greco = new GeneralRecoInfo();
    } else if(InputOutput==2) {
      sprintf(fileCorrName,"%s",argv[4]); 
      greco = new GeneralRecoInfo(fileCorrName);
    }
    
    
    
    if(gdmlOption==0){
      geoManager = new InoGeometry_Manager("OneStack_NoCMVD.gdml");
    }
    else if(gdmlOption==1){
      geoManager = new InoGeometry_Manager("TwoStacks_NoCMVD.gdml");
    }
    else {
      geoManager = new InoGeometry_Manager("CMVD_mical.gdml");
    } //cmvd
    
 
   
    
  }
  
  char rootfiles[200] = {};
  char ffoutname[200] = {};
  
  sprintf(rootfiles, "%s",argv[1]);//q_mical_test_300k_m1.log"); //logFiles/w_test_sim1.log");//mical_sim.log");
  // cout<<"Enter filename: ";
  // cin >> rootfiles;

  cout << " rootfiles " << rootfiles << endl;
  
  int len1 = strlen(rootfiles);
  cout<<"len1 "<<len1<<endl;
  strncpy(ffoutname,rootfiles,len1-4);
  ffoutname[len1-4] = '\0';
  cout << " ffoutname " << ffoutname << endl;
  
  if(InputOutput) {// ??
    greco->OpenRootFiles(ffoutname);
    // pfield->PrintFieldMap();
  }
  ffoutname[len1-4] = '\0';
  cout << " ffoutname " << ffoutname << endl;

  
  char outfile[300] = {};
  cout<<"outfile "<<outfile<<endl;
  if(runcode) {
    pAnalysis = new MultiSimAnalysisDigi();
    cout<<"pAnalysis "<<pAnalysis <<endl;
    pAnalysis->SetTimeToDigiConvVal(TimeToDigi);//detectorConfig->GetTimeToDigiConv());

    std::string tempName(ffoutname);
    std::string outfileName  = tempName.substr(9,31);

    if(InputOutput) {
      
      if(InputOutput==2) {//data Reco //inout3
	sprintf(outfile,"./fileOut/%s_data",ffoutname);
      } else {  //inout3
	sprintf(outfile,"./fileOut/%s_reco",ffoutname);
      }
      
      
    } else {
      sprintf(outfile,"./fileOut/%s_digi",ffoutname);
    }
    
  
    
    cout<<InputOutput<<" outfile "<<outfile<<endl;
    pAnalysis->OpenOutputRootFiles(outfile);
    cout<<"OpenOutputRootFiles : complete "<<endl;
    cout<<"pAnalysis "<<pAnalysis <<endl;

    if(InputOutput) {
      pfield = new  micalFieldPropagator();cout<<"pfiled:"<<pfield<<endl;   cout<<"pAnalysis "<<pAnalysis <<endl; /* pfield->PrintFieldMap();*/
    }
    
    UInt_t numEntry_old = 0;
    UInt_t nfileRead = 0;
    ifstream file_db;
    file_db.open(rootfiles);  
    while(!(file_db.eof())) {
      
      char indatafile[300];
      char outfilx[300];
      char infile[300];
      int nentrymx = 0;
      int ini_ievt = 0; //205474;
      if(file_db.eof()) break;
      file_db >> indatafile>>nentrymx>>ini_ievt;
      if (strstr(indatafile,"#")) continue;
      // if(file_db.eof()) break;
      
      if(InputOutput==1) {
	sprintf(infile, ".%s", indatafile);
	cout<<"infile is "<<infile<<endl;
      } else if(InputOutput==2) {
	sprintf(infile, "%s", indatafile);
	cout<<"infile is  "<<infile<<" " <<InputOutput<<endl;
      } else {
	cout<<"infile is  "<<infile<<" " <<InputOutput<<endl;
	sprintf(infile, "%s", indatafile);
      }
      
      cout<<"infile is "<<indatafile<<endl;
      cout<<"outfile is "<<outfile<<endl;
      pAnalysis->OpenInputRootFiles(infile);
      cout<<"main pAnalysis "<<pAnalysis<<endl;
      if(InputOutput==0  || InputOutput==3 ) { //inout3
	pAnalysis->OpenCollatedRootFile();
      }
      pAnalysis->inputRootFile->cd();
      int numEntry = pAnalysis->inputEventTree->GetEntries();
      cout<<"Input file "<<infile<<" has "<<numEntry<<" entries....."<<endl;
      numEntry = min(numEntry,nentrymx);
      cout<<endl;
      cout<<"ini "<<ini_ievt<<" "<<numEntry<<" "<<numEntry+ini_ievt<<endl;
      // numEntry = 10;
      //  for(int ievt1=ini_ievt; ievt1<numEntry+ini_ievt; ievt1++) {
      for(int ievt1=ini_ievt; ievt1<numEntry; ievt1++) {
	//	 cout<<"numEntry "<<numEntry<<" "<<numEntry_old<<" "<<nfileRead<<endl;
	cout<<"EVENT No.- "<<ievt1<<endl;
	cout<<printModulo<<endl;
	if(ievt1%printModulo==0) {
	  //	  	 cout<<"checkk1"<<endl;
	  cout<<"---> Processing Event # "<<ievt1<<"..."<<endl;
	}
	//	 cout<<"checkk2"<<endl;
	 
	if(InputOutput==0 || InputOutput==3) {//inout3
	   
	  InoDigiAlg DigiAlgINO;
	   
	  DigiAlgINO.ReadEvent(ievt1);
	   
	  pAnalysis->ievt2 = (numEntry_old*nfileRead) + ievt1;
	  // cout<<"panal "<<pAnalysis->ievt2<<endl;
	  DigiAlgINO.DigitiseSimData();	        
	  // DigiAlgINO.NoiseGenLoop();
	   
	  DigiAlgINO.CalculateTrigger();
	  // cout<<"panal "<<pAnalysis->ievt2<<endl;
	  
	  DigiAlgINO.SaveDigiData();
	  
	  if(CMVDFlag==1 && gdmlOption==2){
	    CMVDigiAlg CMVDigiAlgINO;
	    CMVDigiAlgINO.ReadEvent(ievt1);
	    CMVDigiAlgINO.DigitiseSimData();
	    CMVDigiAlgINO.SaveDigiData();
	    
	  }// if(CMVDFlag==1){
	  
	}
	 
	//	else if(InputOutput) {
	if(InputOutput>0){//inout3
	   
	  InoRecoAlg RecoAlgINO(InputOutput);
	  RecoAlgINO.ReadEvent(ievt1);
	   
	  pAnalysis->ievt2 = (numEntry_old*nfileRead) + ievt1;
	   
	  cout<<"panal "<<pAnalysis->ievt2<<endl;
	  
	  RecoAlgINO.PerformTrackReconstruction();

	  cout<<"check abc "<<pAnalysis->ntrkt<<endl;
	  //add cmvd extrapolation here
	  if(CMVDFlag==1 && gdmlOption==2){ 
	    CMVDRecoAlg RecoAlgCMVD(InputOutput);
	    RecoAlgCMVD.ReadCMVDdata(ievt1);
	    RecoAlgCMVD.CreateCmvHit();
	    RecoAlgCMVD.FormCmvCluster();
	    RecoAlgCMVD.CMVD_Extrapolation();
	    cout<<"check aborted"<<endl;

	  }//CMVDFlag
	  pAnalysis->SaveGenVisFile();	  
	  cout<<"!!check aborted!!"<<endl;
	}


	cout<<pAnalysis<<endl;
	pAnalysis->pEventTree->Fill();
	cout<<"check aborted....."<<endl;
	//  cout<<"---> Event # "<<" processed."<<endl;
	// cout<<endl;
      } // for(int ievt1=0;ievt1<numEntry;ievt1++) {
      numEntry_old = numEntry;
      nfileRead++;
      pAnalysis->CloseInputRootFiles();
    } // while(!(file_db.eof())) {
    pAnalysis->CloseOutputRootFiles();

    delete pAnalysis; pAnalysis = 0;
  } else {
    cout<<"No code run..."<<endl;
  }

  if(InputOutput==1) {
    if(pfield) {delete pfield; pfield=0;}
    if(geoManager) {delete geoManager; geoManager = 0;}
    if(greco) {
      greco->CloseRootFiles();
      delete greco; greco =0;
    }
  }
  
  delete paradef; paradef=0;
  delete detectorConfig; detectorConfig=0;
    
  cout<<"Bye.. Bye.."<<endl;
  cout<<endl;
  cout<<endl;
  
  
  return 1;
}

	
