// Swimming through a particle, forward or backward
//
// Units are: position  : meters
//            momentum  : GeV/c
//            mass      : GeV/c^2
//            stepmax   : meters
//            stepmin   : meters
//
#ifndef SWIMSWIMMER_H
#define SWIMSWIMMER_H
#include "TVector3.h"
#include <cassert>
#include <cmath>
#include "micalFieldPropagator.hh"
#include "micalDetectorParameterDef.hh" //AAR:these variables added to include variable airgap
#include "TGeoManager.h"
//class BField;
//class VldContext;
class SwimParticle;

//......................................................................

class SwimSwimmer
{
public:
    // this constructor should be used for all reco purposes!!!!!!
    SwimSwimmer( double dist, double halfgap);
    SwimSwimmer( int plane, double dist, double halfgap,double NewPlane);// const VldContext& vldc);//NewPlane added RSA
    SwimSwimmer( double dist, double halfgap , double NewPlane);
    // Swimmer to use some user defined BField. Don't use them for real reconstruction!!!!!
    //  SwimSwimmer(BField* magField);
    //  SwimSwimmer(const VldContext& vldc, BField* magField);

    ~SwimSwimmer();
    void         SetNmaxStep(int n) { assert(n>0); fNmaxStep = n; }
    bool         SetStepper(const char* name = 0);
void SetBPlane(int n) {BPlane=n;};
    bool         SwimForward(SwimParticle& particle, int& nextplane, double& b_ave);//, SwimCondition& c);
    bool         SwimBackward(SwimParticle& particle, int& nextplane, double& b_ave); //, SwimCondition& c);
    double         Swim(SwimParticle& particle, int& nextplane); //, SwimCondition& c);
    bool         SwimForward(SwimParticle& particle, double& b_ave);//, SwimCondition& c);
    bool         SwimBackward(SwimParticle& particle,double& b_ave); //, SwimCondition& c);
    double         Swim(SwimParticle& particle); //, SwimCondition& c);
   double GetEnergyLoss(double* istate, double dz, double &axi, double &aT_max, double &aI, TGeoMaterial* material);
    inline void SetIsForward(bool isForward)
    { fIsForward = isForward; }

    inline void SetStepSize(double stepSize)
    { fStepSize = stepSize; }

    inline void SetSPI(int n)
    { fSPI = n; }

    TVector3 getCrossingShift() {return lastCrossingShift;}

 private:
    micalFieldPropagator *pFieldMap;
      TGeoManager* icalGeometry;
      TGeoMaterial* localmat;
    void anal_getnrot(double*, double*);
    void anal_getarot(double, double, double*);
    void trace_track_planef(double*, double*, double, double*, double&);//AAR: 1st argument added to pass the Field array.
    void anal_rotme(double*, double*, double*);
    void anal_rotmet(double*, double*, double*);
    void track_move_pt_align(double*,double*, double, double*, double*, double&); //AAR: 1st argument added to pass the Field array.

    double          fStepMax;    // maximum step size
    double          fStepMin;    // minimum step size
    double          fAcc;        // Requested fractional accuracy
    int             fNmaxStep;   // Maximum number of steps to allow
    bool            fOwnBfield;  // Own magnetic field
    bool            fOwnSwimGeo; // Own Swim geometry
    int BPlane;
    bool                    fIsForward;    // Swim direction in time
    double                  fStepSize;     // Stepsize
    int                     fSPI;          // Next SwimPlaneInterface to be crossed
    double          fDistance;  // Distance to travel in Z-direction
    double          fHalfLayerThickness;
    double fHalfAirGap;                //AAR:these variables added to include variable airgap
    micalDetectorParameterDef* paradef;//AAR:these variables added to include variable airgap
    TVector3    lastCrossingShift;
    TVector3    startposXYZ;
    int        Plane;
    int nbfield;
    double fNewPlane;
    //    double b_ave;
   double ds;
    double drange;
    double I;
    double xi;
    double T_max;
    double BetheBloch;
};

#endif // SWIMSWIMMER_H
