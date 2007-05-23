//
// C++ Interface: eosOperators
//
// Description: 
//
//
// Author: Johann Dréo <nojhan@gmail.com>, (C) 2007
//
// Copyright: See COPYING file that comes with this distribution
//
//

#ifndef eosOperators_H
#define eosOperators_H

#include <sstream>
#include <vector>

#include <eo>

#include "eosExceptions.h"

using namespace std;

template<class EOT>
//class eosSampleFromSample_Binomial : public eoBF<eoPop<EOT> & , eoPop<EOT> &, void> 
class eosSampleFromSample_Binomial : public eoBreed<EOT>
{
public:
	eosSampleFromSample_Binomial(unsigned int _size, double _min, double _max) : 
		size_wanted(_size), proba_min(_min), proba_max(_max)
	{};
	eosSampleFromSample_Binomial(unsigned int _size, double _proba_e) : 
		size_wanted(_size), proba_min(_proba_e), proba_max(1-_proba_e)
	{};
	eosSampleFromSample_Binomial(unsigned int _size) : 
		size_wanted(_size), proba_min(0.01), proba_max(0.99)
	{};
		
	void operator() (const eoPop<EOT> & _parents, eoPop<EOT> & _offspring)
	{
		if( _parents.size() <= 0 ) {
   				throw Exception_Empty( "The parents sample is empty", EXCEPTION_INFOS ); 
   	}
		if( _parents[0].size() <= 0 ) {
   				throw Exception_Empty( "The size of the first point is (0)", EXCEPTION_INFOS ); 
   	}
/*
clog << "#Sample before" << endl;
clog << _parents << endl;
*/
		unsigned int s = _parents[0].size(); // size of a solution
		unsigned int p = _parents.size(); // size of the sample
		
		vector<double> distribution;
		
		/***** Step 1: extract the parameters from the sample *****/
		
		// dimensions
		for(unsigned int i=0; i < s; ++i) {
		
			double sum = 0.0;
			
			// points
			for (unsigned int j=0; j < p; ++j) {
				sum += _parents[j][i];	
			}
			
			// prbability to found a 1 in this variable
			double proba = sum / p;
			
			// check the proba
			if( proba > proba_max ) {proba = proba_max;}
			if( proba < proba_min ) {proba = proba_min;}
			
			// add the probability to the distribution parameters list
			distribution.push_back( proba ); 
			
		} // for dimensions
		
		if( distribution.size() != s ) {
   		ostringstream msg;
   		msg << "The size of the vector of parameters is (" << distribution.size() << ")"
   				<< " it should be (" << s << ")";
			throw Exception_Length( msg.str(), EXCEPTION_INFOS );
		}
/*
clog << "# Distribution" << endl;
for(unsigned int v=0; v<distribution.size(); v++) {
	clog << distribution[v] << " ";
}
clog << endl;
*/
		
		/***** Step 2: draw a new sample *****/
		
		// erase the vector
		_offspring.clear();
		
		for (unsigned int j=0; j < size_wanted; ++j) { // sample
			EOT apoint;
				
			for(unsigned int i=0; i < s; ++i) { // dimensions
			
				double r = (double)rand()/RAND_MAX; // draw a number in [0,1[
				
				// distribution[i] is the probability to draw a 1
//clog << "r=" << r << ", dj=" << distribution[j] << endl;
				if( r <= distribution[i] ) {  
					apoint.push_back( 1 );
				} else {
					apoint.push_back( 0 );
				}
			} // for dimensions
			
			_offspring.push_back( apoint );
		} // for sample
		
		if( _offspring.size() != size_wanted ) {
   		ostringstream msg;
   		msg << "The generated population size is (" << _offspring.size() << ")"
   				<< " it should be (" << size_wanted << ")";
			throw Exception_Length( msg.str(), EXCEPTION_INFOS );
		}
/*
clog << "#Sample after" << endl;
for(unsigned int i=0; i<_offspring.size(); ++i) {
	for(unsigned int j=0; j<_offspring[i].size(); ++j) {
		clog << _offspring[i][j];
	}
	clog << endl;
}
*/
	} // operator()
	
private:
	unsigned int size_wanted;
	double proba_min;
	double proba_max;
};

#endif
