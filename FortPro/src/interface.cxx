//----------------------------------------------------------------------
//                             ProMCFortran
//  Author:  Kyle Strand
//  Date:    29th August 2012
//
//  This is the interface implementation. It converts the Fortran MC
//  simulation and implements it using the PromcFortran class. It then
//  converts the Fortran using c++ to create and write the compact 
//  ProMC file in one quick shot
//
//----------------------------------------------------------------------

#include "interface.h"
#include "promcfortran.h"

// Declare global pointer
PromcFortran *eventHandler;

void promcinit()
{
    eventHandler = new PromcFortran();
    eventHandler->Initialise();
}//Promcinit

void promcclose()
{
    eventHandler->Close();
    delete eventHandler;
}//Promcclose

void promcaddparticle(int barcode, double px, double py, double pz, double e, double mass, int status, int mother1, int mother2, int daughter1, int daughter2, double x, double y, double z, double t)
{
    eventHandler->AddParticle(barcode, px, py, pz, e, mass, status, mother1, mother2, daughter1, daughter2, x, y, z, t);
}//Promcaddparticle

void promcaddevent(double weight, int proid, double scale)
{
    eventHandler->SetWeight(weight);
    eventHandler->AddEvent(proid, scale);
}//Promcaddevent

void promcaddstats(double cross, double crosserr, double lumi, int events)
{
        eventHandler->SetStats(cross, crosserr, lumi, events);
}

// Converts a FORTRAN string to a C++ string
std::string strFtoC(const char *str, int len)
{
    int tlen = 0;
    char tem;

    // Counts non-blank characters in a string str until a first blank character or the end of the string is met
    while (str[tlen] != ' ' && (tlen < len) && (tem = str[tlen++], tem));

    char *tstr = new char[tlen+1];
    strncpy(tstr, str, tlen);
    tstr[tlen] = '\0';

    return std::string(tstr);
}//strFtoC
