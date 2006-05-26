
#include <cmath>
#include <vector>
#include <windows.h>

#include "itsCatsProblem.hpp"

#include <cstdlib>
#include <iostream>
#include <ometah/common/string.hpp>
#include <ometah/common/random.hpp>

using namespace std;

typedef double (__stdcall *DB_READ)(vector<double> parametres);
typedef int (__stdcall *INT_READ) (void);

//int Chargement();
//double Calcul (vector<double> parametres);

DB_READ Calcul;
INT_READ Chargement;

//extern int __stdcall _declspec(dllimport) Chargement ();
//extern double __stdcall _declspec(dllimport) Calcul (vector<double> parametres);

/*
int Chargement ()
{
  return 13;
}

double Calcul (vector<double> parametres)
{
 return (double) randomNormal(1.0,1.0);
}
*/

itsCatsProblem::itsCatsProblem()
{
    int res;
    HINSTANCE module;

    module = LoadLibrary ("DLLCoutOmetah.dll");
    if (module)
    {
     Calcul = (DB_READ) GetProcAddress (module, "Calcul");
     if (! Calcul) cout << "Arrive pas a charger la DLL";
     Chargement = (INT_READ) GetProcAddress (module, "Chargement");
     if (! Chargement) cout << "Arrive pas a charger la DLL";
    }
    else cout << "Arrive pas a charger la DLL";

    res=Chargement();

    // informations
    setName("Reconstruction CATS");
    setKey("Cats");
    setDescription("The aim of the problem is to find the best parameters for cats reconstruction");
    setCitation("Unknown");    
    setFormula("Unknow");

    setDimensionFixed(Chargement());
    //cout << "dimension = " ; cout << getDimension();

    setBoundsMinimaAll(-0.03);
    setBoundsMaximaAll(0.03);

    setAccuracy(0.000001);

    vector<itsPoint> optim;
    itsPoint pt;

    vector<double> sol(getDimension(),0);
    pt.setSolution(sol); // a vector of 0

    vector<double> val(1,0);
    pt.setValues(val); // a value of 0

    optim.push_back(pt);
    setOptima(optim);
    
}

itsPoint itsCatsProblem::objectiveFunction(itsPoint point)
{
    vector<double> x=point.getSolution();

    vector<double> val(1,Calcul(x));
    point.setValues( val );

    return point;
}


itsProblem * itsCatsProblemFactory::create()
{
    return new itsCatsProblem();
}
