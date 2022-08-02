#include "InoLinearTrackFitAlg.h"


InoLinearTrackFitAlg::InoLinearTrackFitAlg() {
  
  cout<<" InoLinearTrackFitAlg::InoLinearTrackFitAlg() " <<endl;  
  inoHit_pointer = InoHit_Manager::APointer;
  InoCluster_pointer = InoCluster_Manager::APointer;
  pAnalysis = MultiSimAnalysisDigi::AnPointer;
  //  inoTrackCand_pointer = new InoTrackCand_Manager();
  inoTrackCand_pointer =  InoTrackCand_Manager::APointer;
  pinotrack = InoTrack_Manager::APointer;
}

InoLinearTrackFitAlg::~InoLinearTrackFitAlg() {
  cout<<"InoLinearTrackFitAlg::~InoLinearTrackFitAlg() {"<<endl;

  //   for (unsigned int ij=0; ij< inoTrackCand_pointer->InoTrackCand_list.size(); ij++) {
  //   if (inoTrackCand_pointer->InoTrackCand_list[ij]) {
  //     delete inoTrackCand_pointer->InoTrackCand_list[ij];
  //     inoTrackCand_pointer->InoTrackCand_list[ij]=0;
  //   }
  // }
  // inoTrackCand_pointer->InoTrackCand_list.clear();
  // if (inoTrackCand_pointer) {
  //   delete inoTrackCand_pointer;
  //   inoTrackCand_pointer=0;
  // }
  // delete PoissonRn;
  // delete IcalRange;
  
  cout<<"InoLinearTrackFitAlg::~InoLinearTrackFitAlg() }"<<endl;
  
}




void InoLinearTrackFitAlg::Initialise( ) {}

void InoLinearTrackFitAlg::RunAlg( ) {

cout<<".......Entered RPC  straight line fit.........."<<endl;
  
// InoTrack_Manager *pinotrack = InoTrack_Manager::APointer;
  // InoTrackCand_Manager* inoTrackCand_pointer;
// inoTrackCand_pointer = new InoTrackCand_Manager();//deleted at the end of event
  inoTrackCand_pointer->InoTrackCand_list.clear();
  double xvtx_parameter[6]={0};
  //  InoTrackCand* fLinearTrackCand;
  cout<<"fTackCand pointer "<<fLinearTrackCand<<endl; //valgrind RSA
   
    occulyr=-1;
	  
  cout<<"inotrack list size"<<pinotrack->InoTrack_list.size()<<endl;
  
  for (unsigned int iji=0; iji<pinotrack->InoTrack_list.size() ; iji++) {
    double zcor;
    int cluster_size=pinotrack->InoTrack_list[iji]->ClustsInTrack.size();
    

    int nhits1 = pinotrack->InoTrack_list[iji]->GetEntries();
    cout<<"nhits1: "<<nhits1<<endl;


    
     Xpos[nlayer]={0.0}; 
     Xusedpos[nlayer]={0};
     Xdev[nlayer]; for (int ij1=0; ij1<nlayer; ij1++) { Xdev[ij1] = 100; Xpos[ij1]=-5000;}
    
    
     Ypos[nlayer]={0.0};
     Yusedpos[nlayer]={0};//=new float[nlayer];
     Ydev[nlayer]; for (int ij2=0; ij2<nlayer; ij2++) { Ydev[ij2] = 100; Ypos[ij2]=-5000.0;}
    
    
     CorrTimeError = pAnalysis->GetCorrTimeError();
      UnCorrTimeError = pAnalysis->GetUnCorrTimeError();
      timeerr=pow((pow(CorrTimeError,2.) + pow(UnCorrTimeError,2.)),0.5);
    
   
      //   double errxsq[nlayer], errysq[nlayer];
   
    
  
    Nx=0;
    int nxfail = 0;
    xchi2 = 1000;
    double xresol = 0;
    double zval[nlayer], xext[nlayer], xexter[nlayer], xposinstr[nlayer], valz[nlayer];
    for (int ij3=0; ij3<nlayer; ij3++) { zval[ij3] = 0;}
    
	Ny=0;
	int nyfail = 0;
	ychi2 = 1000;
	double yresol = 0;
	double yext[nlayer], yexter[nlayer], yposinstr[nlayer];
	
	
	
	
	for (int ixx=0; ixx<nlayer; ixx++) { xext[ixx]= xexter[ixx] =xposinstr[ixx] =  100;}
	for (int iyy=0; iyy<nlayer; iyy++) { yext[iyy]= yexter[iyy] =yposinstr[iyy] =  100;}

	//Finding Topmost Layer hit:

	double zposmx = 10000;

	for (int jk =0; jk<cluster_size;jk++) {
	  topmostlay=pinotrack->InoTrack_list[iji]->ClustsInTrack[jk]->GetZPos();
	  
	  if(topmostlay<1000){
   zposmx = topmostlay;
	  }
	}
	
	cout<<"TopMost layer having hit is: "<<zposmx<<endl;

	
	cout<<"cluster size: "<<cluster_size<<endl;
	  for (int jk =0; jk<cluster_size;jk++) {
	  
		
	    zval[jk]=pinotrack->InoTrack_list[iji]->ClustsInTrack[jk]->GetZPos();
	    Xpos[jk]=pinotrack->InoTrack_list[iji]->ClustsInTrack[jk]->GetXPos();
	    Ypos[jk]=pinotrack->InoTrack_list[iji]->ClustsInTrack[jk]->GetYPos();
	    errxsq[jk]=pinotrack->InoTrack_list[iji]->ClustsInTrack[jk]->GetXPosErr() * pinotrack->InoTrack_list[iji]->ClustsInTrack[jk]->GetXPosErr() ;
	    errysq[jk]=pinotrack->InoTrack_list[iji]->ClustsInTrack[jk]->GetYPosErr() * pinotrack->InoTrack_list[iji]->ClustsInTrack[jk]->GetYPosErr() ;

      
	    Xusedpos[jk]=true;
	    Yusedpos[jk]=true;
	    cout<<jk<<" "<<Xpos[jk]<<" "<<Ypos[jk]<<" "<<zval[jk]<<" "<<Xusedpos[jk]<<" "<<Yusedpos[jk]<<" "<<errxsq[jk]<<" "<<errysq[jk]<<endl;
        
	 
	      
 
	      
	  }// for ( unsigned int jk =0; jk<nlayer;jk++) {
	    
   
	    
     
	    
	  //............RPC Straight line fit in X-Z plane.............
       
	  StraightLineFit xposfit(1, zval, Xpos,  errxsq, Xusedpos, occulyr, occulyr, layfirst, laylast, xyPosDev);
	 
	    
	    
	  xposfit.GetParameters(nxfail, xinters, xslope);
	  cout<<"Slope and intercept X  "<<xslope<<" "<<xinters<<endl;
	  xposfit.GetError(xerrcst, xerrlin, xerrcov);
	  cout<<"Error in slope and intercept X "<<xerrcst<<" "<<xerrlin<<endl;
	  xposfit.GetChisqure(Nx, xchi2);
	  cout<<"NDOF and chi-square X "<<Nx<<" "<<xchi2<<endl;
       
	  xposfit.GetFitValues(xext,valz, Xdev, xexter);
	  for(int jk = 0;jk<nlayer;jk++){
	    cout<<"get xz fit values "<<xext[jk]<<" "<<Xpos[jk]<<" "<<valz[jk]<<" "<<Xdev[jk]<<endl;
	  }


	
	  //	.............RPC Straight line fit in y-zplane....................

	    
	  StraightLineFit yposfit(1, zval, Ypos,  errysq, Yusedpos, occulyr, occulyr, layfirst, laylast, xyPosDev);
	  yposfit.GetParameters(nyfail, yinters, yslope);
	  cout<<"Slope and intercept Y  "<<yslope<<" "<<yinters<<endl;
	  yposfit.GetError(yerrcst, yerrlin, yerrcov);
	  cout<<"Error in slope and intercept Y "<<yerrcst<<" "<<yerrlin<<endl;
	  yposfit.GetChisqure(Ny, ychi2);
	  cout<<"NDOF and chi-square Y "<<Ny<<" "<<ychi2<<endl;
	  yposfit.GetFitValues(yext,valz, Ydev, yexter);
	  for(int jk = 0;jk<nlayer;jk++){
	    cout<<"get yz fit values "<<yext[jk]<<" "<<Ypos[jk]<<" "<<valz[jk]<<" "<<Ydev[jk]<<endl;
	  }
       
	  // // dz by dt calculation:
       
	  // double szxy=0, sz=0, sxy=0, sn=0, sz2=0;
	  // double errsq=timeerr*timeerr;
	  // double dZdT = 0.0;

          // double ztinter = 0;
 
	  // for (int jk =0; jk<cluster_size;jk++) {

	  //   szxy +=pinotrack->InoTrack_list[iji]->ClustsInTrack[jk]->GetZPos()*pinotrack->InoTrack_list[iji]->ClustsInTrack[jk]->GetTime()/errsq;
	  //   sz +=pinotrack->InoTrack_list[iji]->ClustsInTrack[jk]->GetZPos()/errsq;
	  //   sz2 +=pinotrack->InoTrack_list[iji]->ClustsInTrack[jk]->GetZPos()*pinotrack->InoTrack_list[iji]->ClustsInTrack[jk]->GetZPos()/errsq;
	  //   sxy +=pinotrack->InoTrack_list[iji]->ClustsInTrack[jk]->GetTime()/errsq;
	  //   sn +=1/errsq;
    
	  // }// for ( unsigned int jk =0; jk<cluster_size;jk++) {

   
	  // if ((sz2*sn - sz*sz) !=0) { 
	  //   dZdT = (szxy*sn - sz*sxy)/(sz2*sn - sz*sz);

	  //   ztinter  = sxy/sn - dZdT*sz/sn;

	 
	  //   cout<<"dZdT: "<<dZdT<<endl;
	  // }
	  // cout<<"dZdT: "<<dZdT<<endl;
  
	  // if(dZdT>0){
	  //   ZIncreasesWithTime= true;
	  // }

	  // else{
	  //   ZIncreasesWithTime=false;

	  // }
  
	  cout<<"Zincreaseswith time: "<<ZIncreasesWithTime<<endl; //valgrind RSA
	  //

	  // store events only when they satisfy:
       
	  // if (Ny>=nmnhits/*-ntcor*/ && ychi2/(Ny-2)<mxchisq && nyfail==0) {
	  //   if (Nx>=nmnhits/*-ntcor*/ && xchi2/(Nx-2)<mxchisq && nxfail==0) {

	  if (Ny>=nmnhits/*-ntcor*/  && nyfail==0) {
	    if (Nx>=nmnhits/*-ntcor*/  && nxfail==0) {

	      
	      cout<<"Slope and intercept X  "<<xslope<<" "<<xinters<<endl;
	      cout<<"Error in slope and intercept X "<<xerrcst<<" "<<xerrlin<<endl;
	      cout<<"NDOF and chi-square X "<<Nx<<" "<<xchi2<<endl;
	      for(int jk = 0;jk<nlayer;jk++){
		cout<<"get x/y fit values "<<xext[jk]<<" "<<Xpos[jk]<<" "<<valz[jk]<<" "<<yext[jk]<<" "<<Ypos[jk]<<" "<<valz[jk]<<" "<<endl;
	      }


	      cout<<"Slope and intercept Y  "<<yslope<<" "<<yinters<<endl;
	      cout<<"Error in slope and intercept Y "<<yerrcst<<" "<<yerrlin<<endl;
	      cout<<"NDOF and chi-square Y "<<Ny<<" "<<ychi2<<endl;


	      //storing the data :


       
	      // theta and phi from slopes in x-z and y-z plane
       
	      phi = atan2(yslope,xslope);
     
	      //since z is decreasing with time we have minus sign
	      ZIncreasesWithTime=false;
	      if(ZIncreasesWithTime==false){
	  
		theta = acos(-1.0/sqrt(1+pow(xslope,2)+pow(yslope,2)));
	   
		double PI = acos(-1.0);
		phi +=PI;
		if (phi > PI) { phi -=2*PI;}
        
	      }

	      else{
		theta= acos(1.0/sqrt(1+pow(xslope,2)+pow(yslope,2)));
	      }
     
	      cout<<iji<<" theta: "<<theta<<" phi: "<<phi<<" xslope "<<xslope<<" yslope  "<<yslope<<endl;//:
	 

    G4ThreeVector dirVector0(0,0,1);       
    dirVector0.setTheta(theta);
    dirVector0.setPhi(phi);
    cout<<"dirVector0 "<<dirVector0<<endl;
	      
	  //:

	  
	  
	      fLinearTrackCand = new InoTrackCand(pinotrack->InoTrack_list[iji], ZIncreasesWithTime); 
	  
	      fLinearTrackCand->SetTheta(theta);
	      fLinearTrackCand->SetPhi(phi);

     
	       therr =sqrt(pow((xslope*xerrcst),2)+pow((yslope*yerrcst),2))/(1+pow(xslope,2)+pow(yslope,2))*(pow(xslope,2)+pow(yslope,2)) ;
	       pherr = sqrt(pow(yslope*xerrcst,2)+pow(xslope*yerrcst,2))/(pow(xslope,2)+pow(yslope,2));
	      cout<<" therr: "<<therr<<" pherr: "<<pherr<<endl;
	   
	      fLinearTrackCand->SetThErr(therr);
	      fLinearTrackCand->SetPhErr(pherr);

	      xvtx_parameter[0] =0;//momen
	      xvtx_parameter[1] =theta;
	      xvtx_parameter[2] =phi;
	      xvtx_parameter[3] =xslope*(zposmx)+xinters;//in meters
	      xvtx_parameter[4] =yslope*(zposmx)+yinters;//in meters
	      xvtx_parameter[5]=zposmx*1000;//top layer z pos in mm

	      fLinearTrackCand->SetExtPara(xvtx_parameter);
	  
	      for(int zz=0;zz<6;zz++){
		cout<<"xvtx_parameter "<<xvtx_parameter[zz]<<endl;	    
	      }
   xexterr = xerrcst + 2*xerrcov*(zposmx) + xerrlin*(zposmx)*(zposmx);
   yexterr = yerrcst + 2*yerrcov*(zposmx) + yerrlin*(zposmx)*(zposmx);
  
  
	      
     
	      cout<<"error in extrapolated x and y point: "<<xexterr<<" "<<yexterr<<endl;
     
	      fLinearTrackCand->SetVtxUError(pow(xexterr,0.5));//err in xpos
	      fLinearTrackCand->SetVtxVError(pow(yexterr,0.5));//err in y pos	   
	      fLinearTrackCand->SetVtxdUError(pow(xerrlin,0.5));// error in  dxdz
	      fLinearTrackCand->SetVtxdVError(pow(yerrlin,0.5));// error in dydz
	      fLinearTrackCand->SetVtxQPError(0);
	      fLinearTrackCand->SetVtxUdUError(xerrcov);//store cov
	      fLinearTrackCand->SetVtxUdVError(0);
	      fLinearTrackCand->SetVtxVdVError(yerrcov);//
	      fLinearTrackCand->SetVtxVdUError(0);
	      fLinearTrackCand->SetVtxdUdVError(0);
	      cout<<"Pointer exists "<<fLinearTrackCand<<endl;
	      inoTrackCand_pointer->InoTrackCand_list.push_back(fLinearTrackCand);

	      fLinearTrackCand->SetChi2(xchi2);
	      fLinearTrackCand->SetChi22(ychi2);

	    //   fLinearTrackCand->SetXdevLay1(Xdev[0]);
	    //   fLinearTrackCand->SetYdevLay1(Ydev[0]);

	    //   //fLinearTrackCand->SetXStrip(     ( (1000*xpos)-ShiftInX+pargas[0])/Xstrwd)-0.5    );


	    // //double shift2y = (paradef->GetnStack()>1) ? (2*nInCH-1)*parchm[1] : 0;
 

	    // // fLinearTrackCand->SetYStrip(  ((1000*ypos-ShiftInY-shift2y+pargas[1])/Ystrwd)-0.5           );
	      
	    //   fLinearTrackCand->SetXdevLay2(Xdev[1]);
	    //   fLinearTrackCand->SetYdevLay2(Ydev[1]);

	    //   fLinearTrackCand->SetXdevLay3(Xdev[2]);
	    //   fLinearTrackCand->SetYdevLay3(Ydev[2]);


	    //   fLinearTrackCand->SetXdevLay4(Xdev[3]);
	    //   fLinearTrackCand->SetYdevLay4(Ydev[3]);



	    //   fLinearTrackCand->SetXdevLay5(Xdev[4]);
	    //   fLinearTrackCand->SetYdevLay5(Ydev[4]);



	    //   fLinearTrackCand->SetXdevLay6(Xdev[5]);
	    //   fLinearTrackCand->SetYdevLay6(Ydev[5]);

	    //   fLinearTrackCand->SetXdevLay7(Xdev[6]);
	    //   fLinearTrackCand->SetYdevLay7(Ydev[6]);



	    //   fLinearTrackCand->SetXdevLay8(Xdev[7]);
	    //   fLinearTrackCand->SetYdevLay8(Ydev[7]);

	      
	    //   fLinearTrackCand->SetXdevLay9(Xdev[8]);
	    //   fLinearTrackCand->SetYdevLay9(Ydev[8]);


	    //   fLinearTrackCand->SetXdevLay10(Xdev[9]);
	    //   fLinearTrackCand->SetYdevLay10(Ydev[9]);


	    //   fLinearTrackCand->SetXdevLay11(Xdev[10]);
	    //   fLinearTrackCand->SetYdevLay11(Ydev[10]);

	    //   fLinearTrackCand->SetXdevLay12(Xdev[11]);
	    //   fLinearTrackCand->SetYdevLay12(Ydev[11]);

	      
 	    }//    if (Nx>=nmnhits/*-ntcor*/ && xchi2/(Nx-2)<mxchisq && nxfail==0) {
	      
 	  }//  if (Ny>=nmnhits/*-ntcor*/ && ychi2/(Ny-2)<mxchisq && nyfail==0) {

	  else{

	    fLinearTrackCand=0;
	    cout<<"Pointer is Null "<<fLinearTrackCand <<endl;
	  }
       
	}//for (unsigned iji=0; iji<pinotrack->InoTrack_list.size() ; iji++) {


	//
 






  
}

