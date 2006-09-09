
#include "itsEOInterface.hpp"

#include <ometah/problems.h>
#include <ometah/communications.h>
#include <ometah/commons.h>

#include <eo>
#include <es.h>

using namespace std;
using namespace ometah;
using namespace eo;

typedef eoReal<eoMinimizingFitness> Indi;

int main(int argc, char ** argv)
{

    /********** EO **********/
    eoState * stator = new eoState();

    eoPop< Indi > * pop = new eoPop<Indi>;
  
    
    eoDetTournamentSelect< Indi > * select = new eoDetTournamentSelect< Indi >(3);
      //stator->storeFunctor( select );
    eoSegmentCrossover< Indi > * xover = new eoSegmentCrossover< Indi >();
      //stator->storeFunctor( xover );
    eoUniformMutation< Indi > * mutation = new eoUniformMutation< Indi >(0.01);
      //stator->storeFunctor( mutation );

    itsEOInterface  metaheuristic( pop, select, xover, 0.8, mutation, 0.5);
cerr << "itsEOInterface.getLogLevel():" << metaheuristic.getLogLevel() << endl;


    itsSphere problem;
    itsCommunicationClient_embedded client;
    itsCommunicationServer_embedded server;
    metaheuristic.setProblem( & client );
    client.problem = & server;
    server.problem = & problem;
cerr << "linking c/s" << endl;
    
    //problem.setDimension(1); cerr << "dim ";
    
    metaheuristic.setSampleSize(10); cerr << "sampleSize ";
    metaheuristic.setEvaluationsMaxNumber(100); cerr << "evalNb ";
    metaheuristic.setIterationsMaxNumber(10); cerr << "iterNb ";
    metaheuristic.setValueMin(0); cerr << "valMin ";
    metaheuristic.initRandom(0); cerr << "initRnd ";
cerr << endl;



    try {
cerr << "itsEOInterface.start()" << endl;
        metaheuristic.start();
    } catch( exception & e ) {
        cerr << "Erreur EO : " << e.what()  << endl;
        delete stator;
        exit(1);
    } catch( Exception_oMetah & e ) {
        cerr << "Erreur oMetah : " << e.what() << endl;
        delete stator;
        exit(1);
    }
}
