#include "InoLinearTrackFitAlg.h"


InoLinearTrackFitAlg::InoLinearTrackFitAlg() {

  cout<<" InoLinearTrackFitAlg::InoLinearTrackFitAlg() " <<endl;
  inoHit_pointer = InoHit_Manager::APointer;
  InoCluster_pointer = InoCluster_Manager::APointer;
  pAnalysis = MultiSimAnalysisDigi::AnPointer;
  //  inoTrackCand_pointer = new InoTrackCand_Manager();
  inoTrackCand_pointer =  InoTrackCand_Manager::APointer;
  pinotrack = InoTrack_Manager::APointer;
  grecoi = GeneralRecoInfo::GnPointer;
  paradef = micalDetectorParameterDef::AnPointer;

  StripXWidth = (1/m)*paradef->GetXStrwd();
  StripYWidth = (1/m)*paradef->GetYStrwd();

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

void InoLinearTrackFitAlg::RunAlg() {

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

  for (unsigned int iji=0; iji<pinotrack->InoTrack_list.size(); iji++) {
    cout<<"ntrkmx: "<<pAnalysis->ntrkmx<<endl;
    if(iji<pAnalysis->ntrkmx ){
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


	cout<<"cluster size: "<<cluster_size<<endl;

	for (unsigned int ix=0; ix<pinotrack->InoTrack_list[iji]->ClustsInTrack.size(); ix++) {
	  InoCluster* clust = pinotrack->InoTrack_list[iji]->ClustsInTrack[ix];
	  clust->Print();
	  ClustsInTrackBank[iji][clust->GetZPlane()].push_back(clust);
    cout<<"Filling Cluster"<< "\tZplane="<<clust->GetZPlane()<<endl;
	}// for (unsigned int ix=0; ix<Cluster_pointer->InoStripX_list.size(); ix++) {

  float errxco[12]={0.288675,0.288675,0.288675,0.288675,0.288675,0.288675,0.288675,0.288675,0.288675,0.288675,0.288675,0.288675};
  float erryco[12]={0.288675,0.288675,0.288675,0.288675,0.288675,0.288675,0.288675,0.288675,0.288675,0.288675,0.288675,0.288675};

	  for (int nlay =0; nlay<10;nlay++) {
	    //Only one clust per layer and that cluster should have less than 5 multiplicity:
     if(ClustsInTrackBank[iji][nlay].size()>=1) {
      cout<<"ClustsInTrackBank Size -----"<<ClustsInTrackBank[iji][nlay].size()<<endl;
	    cout<<"nxstrip nystrip  "<< ClustsInTrackBank[iji][nlay][0]->GetNXStripsInClust() << " "<< ClustsInTrackBank[iji][nlay][0]->GetNYStripsInClust() <<endl;

	    if (ClustsInTrackBank[iji][nlay].size()==1 &&  ClustsInTrackBank[iji][nlay][0]->GetNXStripsInClust()<5 && ClustsInTrackBank[iji][nlay][0]->GetNYStripsInClust()<5 ){

	      zval[nlay]=ClustsInTrackBank[iji][nlay][0]->GetZPos();
	      Xpos[nlay]=ClustsInTrackBank[iji][nlay][0]->GetXPos();
	      Ypos[nlay]=ClustsInTrackBank[iji][nlay][0]->GetYPos();
	      if(ClustsInTrackBank[iji][nlay][0]->GetNXStripsInClust()>0) {
		errxsq[nlay] = grecoi->xposerrsq[ClustsInTrackBank[iji][nlay][0]->GetNXStripsInClust()-1][nlay]*0.03*0.03;
		
	      } else {
		errxsq[nlay]=errxco[nlay]*errxco[nlay]*0.03*0.03;
		
	      }
	      if(ClustsInTrackBank[iji][nlay][0]->GetNYStripsInClust()>0) {
		errysq[nlay] = grecoi->yposerrsq[ClustsInTrackBank[iji][nlay][0]->GetNYStripsInClust()-1][nlay]*0.03*0.03;
	        
	      } else {
		errysq[nlay]=erryco[nlay]*erryco[nlay]*0.03*0.03;
	    
	      }
	      //errxsq[nlay]=ClustsInTrackBank[iji][nlay][0]->GetXPosErr() * ClustsInTrackBank[iji][nlay][0]->GetXPosErr() ;
	      //errysq[nlay]=ClustsInTrackBank[iji][nlay][0]->GetYPosErr() * ClustsInTrackBank[iji][nlay][0]->GetYPosErr() ;

	    Xusedpos[nlay]=true;
	    Yusedpos[nlay]=true;
	   
	    }
	    else{

	      zval[nlay]=-5000.0;
	      Xpos[nlay]=-5000.0;
	      Ypos[nlay]=-5000.0;
	      errxsq[nlay]=errxco[nlay]*errxco[nlay]*0.03*0.03;
	      errysq[nlay]=erryco[nlay]*erryco[nlay]*0.03*0.03;
	      Xusedpos[nlay]=false;
	      Yusedpos[nlay]=false;
	    }
     }

      cout<<nlay<<" "<<Xpos[nlay]<<" "<<Ypos[nlay]<<" "<<zval[nlay]<<" "<<Xusedpos[nlay]<<" "<<Yusedpos[nlay]<<" "<<errxsq[nlay]<<" "<<errysq[nlay]<<endl;

	  }


	  cout<<"cluster size: "<<cluster_size<<endl;
	  // for (int jk =0; jk<cluster_size;jk++) {

	  //   zval[jk]=pinotrack->InoTrack_list[iji]->ClustsInTrack[jk]->GetZPos();
	  //   Xpos[jk]=pinotrack->InoTrack_list[iji]->ClustsInTrack[jk]->GetXPos();
	  //   Ypos[jk]=pinotrack->InoTrack_list[iji]->ClustsInTrack[jk]->GetYPos();
	  //   errxsq[jk]=pinotrack->InoTrack_list[iji]->ClustsInTrack[jk]->GetXPosErr() * pinotrack->InoTrack_list[iji]->ClustsInTrack[jk]->GetXPosErr() ;
	  //   errysq[jk]=pinotrack->InoTrack_list[iji]->ClustsInTrack[jk]->GetYPosErr() * pinotrack->InoTrack_list[iji]->ClustsInTrack[jk]->GetYPosErr() ;

	  //   Xusedpos[jk]=true;
	  //   Yusedpos[jk]=true;
	  //   cout<<jk<<" "<<Xpos[jk]<<" "<<Ypos[jk]<<" "<<zval[jk]<<" "<<Xusedpos[jk]<<" "<<Yusedpos[jk]<<" "<<errxsq[jk]<<" "<<errysq[jk]<<endl;



	  // }// for ( unsigned int jk =0; jk<nlayer;jk++) {




	//Finding Topmost Layer hit:

	  double zposmx = 10000;
	  double topmostlay;
	  for (int jk =0; jk<10;jk++) {
	    if (Xusedpos[jk]==false || Yusedpos[jk]==false) continue;
	    if(ClustsInTrackBank[iji][jk].size()>=1) {
	      topmostlay=ClustsInTrackBank[iji][jk][0]->GetZPos();
	    }
	    if(topmostlay<1000){
	      zposmx = topmostlay;
	    }
	  }
	  
	  cout<<"TopMost layer having hit is: "<<zposmx<<endl;







	  double posresol =true;
	  if(posresol){
	    cout<<"...Position resolution..."<<endl;
	    for(int jki=0;jki<10;jki++){
	      occulyr = jki;
	      cout<<"Fitting Excluding Layer:"<<occulyr<<endl;

	      //Position Alignment correction
	      double inpar1[3];
	      double inpar2[3];
	      double inpar3[3];
	      double inpar4[3];
	      for(int prx=0; prx<3; prx++) {
		inpar1[prx] = grecoi->align_xstr_xdev[jki][prx];
		inpar2[prx] = grecoi->align_ystr_ydev[jki][prx];
		inpar3[prx] = grecoi->align_xstr_ydev[jki][prx];
		inpar4[prx] = grecoi->align_ystr_xdev[jki][prx];
	      }
	      double tmp_poffxx = StripXWidth*cal_slope2(Xpos[jki]/StripXWidth, inpar1) ;
	      double tmp_poffyy = StripYWidth*cal_slope2(Ypos[jki]/StripYWidth, inpar2) ;

	      cout<<"tmp_poffxx "<<tmp_poffxx<<" tmp_poffyy "<<tmp_poffyy<<endl;
	      Xpos[jki] -= tmp_poffxx;
	      Ypos[jki] -= tmp_poffyy;

	      StraightLineFit xposresolfit(1, zval, Xpos,  errxsq, Xusedpos, occulyr, occulyr, layfirst, laylast, xyPosDev);
	      xposresolfit.GetParameters(nxfail, xinters, xslope);
	      xposresolfit.GetError(xerrcst, xerrlin, xerrcov);
	      xposresolfit.GetChisqure(Nx, xchi2);
	      xposresolfit.GetFitValues(xext,valz, Xdev, xexter);

	      StraightLineFit yposresolfit(1, zval, Ypos,  errysq, Yusedpos, occulyr, occulyr, layfirst, laylast, xyPosDev);
	      yposresolfit.GetParameters(nyfail, yinters, yslope);
	      yposresolfit.GetError(yerrcst, yerrlin, yerrcov);
	      yposresolfit.GetChisqure(Ny, ychi2);
	      yposresolfit.GetFitValues(yext,valz, Ydev, yexter);


	      double tmp_poffyx = StripXWidth*cal_slope2(yext[jki]/StripYWidth, inpar4) ;
	      double tmp_poffxy = StripYWidth*cal_slope2(xext[jki]/StripXWidth, inpar3) ;

	      if(nxfail==0) {Xpos[jki] -= tmp_poffyx;}
	      if(nyfail==0) {Ypos[jki] -= tmp_poffxy;}

	      xposresolfit = StraightLineFit(1, zval, Xpos,  errxsq, Xusedpos, occulyr, occulyr, layfirst, laylast, xyPosDev);
	      xposresolfit.GetParameters(nxfail, xinters, xslope);
	      cout<<"Slope and intercept X  "<<xslope<<" "<<xinters<<endl;
	      xposresolfit.GetError(xerrcst, xerrlin, xerrcov);
	      cout<<"Error in slope and intercept X "<<xerrcst<<" "<<xerrlin<<endl;
	      xposresolfit.GetChisqure(Nx, xchi2);
	      cout<<"NDOF and chi-square X "<<Nx<<" "<<xchi2<<endl;
	      xposresolfit.GetFitValues(xext,valz, Xdev, xexter);
	      for(int jk = 0;jk<nlayer;jk++){
		cout<<"get xz fit values "<<xext[jk]<<" "<<Xpos[jk]<<" "<<valz[jk]<<" "<<Xdev[jk]<<endl;
	      }

	      yposresolfit = StraightLineFit(1, zval, Ypos,  errysq, Yusedpos, occulyr, occulyr, layfirst, laylast, xyPosDev);
	      yposresolfit.GetParameters(nyfail, yinters, yslope);
	      cout<<"Slope and intercept Y  "<<yslope<<" "<<yinters<<endl;
	      yposresolfit.GetError(yerrcst, yerrlin, yerrcov);
	      cout<<"Error in slope and intercept Y "<<yerrcst<<" "<<yerrlin<<endl;
	      yposresolfit.GetChisqure(Ny, ychi2);
	      cout<<"NDOF and chi-square Y "<<Ny<<" "<<ychi2<<endl;
	      yposresolfit.GetFitValues(yext,valz, Ydev, yexter);
	      for(int jk = 0;jk<nlayer;jk++){
		cout<<"get yz fit values "<<yext[jk]<<" "<<Ypos[jk]<<" "<<valz[jk]<<" "<<Ydev[jk]<<endl;
	      }



	      if(ClustsInTrackBank[iji][jki].size()>=1) {
		pAnalysis->XPosdev_exclu[jki] = Xdev[jki];
		pAnalysis->YPosdev_exclu[jki] = Ydev[jki];
		pAnalysis->inPosX[jki] = Xpos[jki];
		pAnalysis->extPosX[jki] = xext[jki];
		pAnalysis->inPosY[jki] = Ypos[jki];
		pAnalysis->extPosY[jki] = yext[jki];
		pAnalysis->nXStrips[jki] = ClustsInTrackBank[iji][jki][0]->GetNXStripsInClust();
		pAnalysis->nYStrips[jki] = ClustsInTrackBank[iji][jki][0]->GetNYStripsInClust();
		pAnalysis->XchisqOccu[jki] = xchi2;
		pAnalysis->YchisqOccu[jki] = ychi2;
		pAnalysis->XndfOccu[jki] = Nx;
		pAnalysis->YndfOccu[jki] = Ny;
		pAnalysis->nxFailOccu[jki] = nxfail;
		pAnalysis->nyFailOccu[jki] = nyfail;
        }

        if((pAnalysis->inPosY[9] - pAnalysis->extPosY[9] > 1) && pAnalysis->nYStrips[9]==2) {
          cout<<"Check the event jim anna eldo"<<endl;
        }
	      cout<<"pAnalysis->XPosdev_exclu[jki] "<<pAnalysis->XPosdev_exclu[jki]<<" pAnalysis->YPosdev_xclu[jki] "<<pAnalysis->YPosdev_exclu[jki]<<endl;



	    }//  for(int jki=0;jki<nLayer;jki++){

	  } // if(posresol){


	  occulyr = -1;
	  cout<<"occulyr: "<<occulyr<<endl;

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


   for (int jk =0; jk<cluster_size;jk++) {

		pAnalysis->XPosdev[jk] = Xdev[jk];
		pAnalysis->YPosdev[jk] = Ydev[jk];
	      }


 	    }//    if (Nx>=nmnhits/*-ntcor*/ && xchi2/(Nx-2)<mxchisq && nxfail==0) {

 	  }//  if (Ny>=nmnhits/*-ntcor*/ && ychi2/(Ny-2)<mxchisq && nyfail==0) {

	  else{

	    fLinearTrackCand=0;
	    cout<<"Pointer is Null "<<fLinearTrackCand <<endl;
	  }
    }//   if(iji<pAnalysis->ntrkmx ){
  }//for (unsigned iji=0; iji<pinotrack->InoTrack_list.size() ; iji++) {
  
  
  //
}


double InoLinearTrackFitAlg::cal_slope2(double x, double* par) {
  int nstrips = 64; // need to transfer from card file
  if (x<nstrips/2.-0.5) {
    return par[0] + par[1]*(x - nstrips/4. +0.5);
  } else {
    double par3 = (par[2]-par[0]-par[1]*nstrips/4.)/(nstrips/4.);
    return par[2] + par3*(x - 3*nstrips/4. +0.5);
  }
}
