#include <iostream>
#include <vector>

#include <eo>
#include <ga.h>


#include "eosExceptions.h"
#include "eosMetaheuristic.h"
#include "eosOperators.h"

using namespace std;

//typedef eoBit<double> point;
typedef eoBit<eoMinimizingFitness> point;
typedef eoEvalFuncPtr< point, double, const vector<bool>& > objectiveFunctionPointer;

int sample_size = 20;
int solution_size = 50;
int max_generations = 1000;
	
	
double of_test(const vector<bool>& tab )
{
	double sum = 0;
	for (unsigned int i = 0; i < tab.size(); i++) {
		sum += tab[i];
	}
	return -1*sum;
}


void sga() 
{


cout << "##### SGA #####" << endl;

	// sample initiation operator
	eoUniformGenerator<bool> uGen;
	eoInitFixedLength<point> random( solution_size, uGen);
	
	// the sample
	eoPop<point> sample( sample_size, random );

	// continuators instanciation
	//eoGenContinue<point> continuator_generations( 10000 );
	//eoSteadyFitContinue<point> continuator_steady( 1000, 10000 );
	// continuators combination
	//eoCombinedContinue<point> continuator(continuator_generations); // TODO : proposer un constructeur sans argument ?
	//continuator.add(continuator_generations);
	//continuator.add(continuator_steady);
	eoGenContinue<point> continuator( max_generations );

	// objective function
	eoEvalFuncPtr<point, double, const vector<bool>& > tempEval(of_test);
	eoPopLoopEval<point> evaluator( tempEval  );

	// first evaluation of the sample
	evaluator( sample, sample );


	// operators
	

	
	// ENGINE
	/////////////////////////////////////
	// selection and replacement
	////////////////////////////////////
	// SELECT
	// The robust tournament selection
	eoDetTournamentSelect<point> selectOne( 50 );       // tSize in [2,POPSIZE]
	// is now encapsulated in a eoSelectPerc (stands for Percentage)
	eoSelectPerc<point> select(selectOne, 0.5);
	

	

	// OPERATORS
	//////////////////////////////////////
	// The variation operators
	//////////////////////////////////////
	// CROSSOVER
	// 1-point crossover for bitstring
	eo1PtBitXover<point> xover1;
	// uniform crossover for bitstring
	eoUBitXover<point> xoverU;
	// 2-pots xover
	eoNPtsBitXover<point> xover2(2);
	// Combine them with relative rates
	eoPropCombinedQuadOp<point> xover(xover1, 0.4);
	xover.add(xoverU, 0.3);
	xover.add(xover2, 0.3, true);


	// MUTATION
	// standard bit-flip mutation for bitstring
	eoBitMutation<point>  mutationBitFlip( 0.1 );
	// mutate exactly 1 bit per individual
	eoDetBitFlip<point> mutationOneBit;
	// Combine them with relative rates
	eoPropCombinedMonOp<point> mutation( mutationOneBit , 0.1 );
	mutation.add(mutationOneBit, 0.1, true);
	
	// The operators are encapsulated into an eoTRansform object
	eoSGATransform<point> transform(xover, 0.4, mutation, 0.4);

	// REPLACE
	// And we now have the full selection/replacement - though with
	// the same generational replacement at the moment :-)
	//eoGenerationalReplacement<point> replace;
	//eoHowMany howMany(0.9,true);
	//eoG3Replacement<point> replace(howMany);
	eoPlusReplacement<point> replace;
	//eoCommaReplacement<point> replace;
	
	// the metaheuristic
	eosMetaheuristic<point> m(continuator);
	
	m.add( &select );
	m.add( &transform );
	m.add( &evaluator );
	m.add( &replace );

	//cout << sample << endl;
	
	// algorithm start
	m( sample );

	cout << sample << endl;
	
}

void eda() 
{
cout << "##### EDA #####" << endl;

	// objective function
	eoEvalFuncPtr<point, double, const vector<bool>& > tempEval(of_test);
	eoPopLoopEval<point> evaluator( tempEval  );

	eoUniformGenerator<bool> uGen2;
	eoInitFixedLength<point> random2( solution_size, uGen2);
	eoPop<point> sample2( sample_size, random2 );
	evaluator( sample2, sample2 );
	
	//cout << sample2 << endl;
	
	
	eoGenContinue<point> continuator2( max_generations );
	
	eosMetaheuristic<point> m2(continuator2);
	

  eoDetTournamentSelect<point> selectOne( 10 ); 
  eoSelectPerc<point> selector(selectOne, 0.5);
  
	m2.add( &selector );
	
	m2.add( &evaluator );
	
	eosSampleFromSample_Binomial<point> edator( sample_size, 0.01 );
	m2.add( &edator );
	
	m2.add( &evaluator );
	
	
	try { 
		m2( sample2 );
	} catch(Exception& e) {
		cerr << e.what() << endl;
	}

	cout << sample2 << endl;

}

int main () 
{
	sga();
	eda();
}
