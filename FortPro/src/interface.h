//----------------------------------------------------------------------
//                             ProMCFortran
//  Author:  Kyle Strand
//  Date:    3 July 2013
//
//  This is the interface declaration. It uses a wrapper from the
//  implementation to the symbols that the FORTRAN compiler will
//  find. It uses macros found in FC_namemangling.h to get the name
//  mangling correct for the user's compiler. These macros are output
//  by CMake during build, but the user could define them if needed.
//
//  Based on RootTuple interface
//----------------------------------------------------------------------

#ifndef interface_H
#define interface_H

#include <string>
#include "FC_namemangling.h"

void promcinit();
void promcclose();

void promcaddparticle(int barcode, double px, double py, double pz, double e, double mass, int status, int mother1, int mother2, int daughter1, int daughter2, double x, double y, double z, double t);
void promcaddevent(double weight, int proid, double scale);
void promcaddstats(double cross, double crosserr, double lumi, int events);

std::string strFtoC(const char *str, int len);

// Wrappers from FORTRAN subroutines to C++ functions
// Name mangling is accounted for by FortranCInterface_GLOBAL macro
// Problems could occur with character array argument from FORTRAN subroutine
#ifdef __cplusplus
extern "C" {
#endif
    void FortranCInterface_GLOBAL(promcinit,PROMCINIT)()
    {promcinit();};
    void FortranCInterface_GLOBAL(promcclose,PROMCCLOSE)()
    {promcclose();};

    void FortranCInterface_GLOBAL(promcaddparticle,PROMCADDPARTICLE)(int *barcode, double *px, double *py, double *pz, double *e, double *mass, int *status, int *mother1, int *mother2, int *daughter1, int *daughter2, double *x, double *y, double *z, double *t)
    {promcaddparticle(*barcode, *px, *py, *pz, *e, *mass, *status, *mother1, *mother2, *daughter1, *daughter2, *x, *y, *z, *t);};
    void FortranCInterface_GLOBAL(promcaddevent,PROMCADDEVENT)(double *weight, int *proid, double *scale)
    {promcaddevent(*weight, *proid, *scale);};

    void FortranCInterface_GLOBAL(promcaddstats,PROMCADDSTATS)(double *cross, double *crosserr, double *lumi, int *events)
    {promcaddstats(*cross, *crosserr, *lumi, *events);};


#ifdef __cplusplus
}
#endif

#endif
