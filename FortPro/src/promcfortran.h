//----------------------------------------------------------------------
//                             ProMCFortran
//  Author:  Kyle Strand 
//  Date:    3 July 2013
//
//  This declares the ProMCFortran class.
//
//----------------------------------------------------------------------

#ifndef promcfortran_H
#define promcfortran_H

// Standard C++ libraries
#include <iostream>
#include <string>
#include <vector>
#include <cmath>
#include"../src_promc/ProMCHeader.pb.h"
#include"../src_promc/ProMC.pb.h"
#include"../src_promc/ProMCBook.h"

using namespace std;
using namespace promc;

    const double kEV=1000*100;
    const double uL=1000.;

class PromcFortran{
public:
    
    // Constructors and destructors
    PromcFortran();
    ~PromcFortran();

    // Public methods
    void Initialise();
    void AddEvent(int proid, double scale);
//    void Write();
    void Close();
    void AddParticle(int barcode, double px, double py, double pz, double e, double mass, int status, int mother1, int mother2, int daughter1, int daughter2, double x, double y, double z, double t);
    void SetWeight(double weight);
    void SetStats(double cross, double crosserr, double lumi, int events);

private:
    // Private methods
    void ClearVectors();

    // ProMC object
    ProMCBook *epbook; 
    
    // ProMC Variables
    double m_weight; // from Double_t
    std::vector<int> m_barcode, m_status, m_mother1, m_mother2, m_daughter1, m_daughter2;
    std::vector<double> m_Px, m_Py, m_Pz, m_E, m_mass, m_X, m_Y, m_Z, m_T;
};

#endif
