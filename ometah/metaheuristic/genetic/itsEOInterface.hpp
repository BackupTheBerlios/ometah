/***************************************************************************
 *  $Id: itsEOInterface.hpp,v 1.2 2006/05/13 10:05:55 nojhan Exp $
 *  Copyright : Free Software Foundation
 *  Author : Johann Dréo <nojhan@gmail.com>
 ****************************************************************************/

/*  Open Metaheuristic is a Library aimed at the conception of metaheuristics 
 *  for difficult optimization.
 *  
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU Lesser General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU Lesser General Public License for more details.
 *
 *  You should have received a copy of the GNU Lesser General Public License
 *  along with this program; if not, write to the Free Software
 *  Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.
 */

#ifndef ITSEOINTERFACE
#define ITSEOINTERFACE
 
#include <sstream>
 
#include <eo>
#include <es.h>

#include "../../common/Exception_oMetah.hpp"
#include "../../common/itsPoint.hpp"
#include "../itsMetaheuristic.hpp"

using namespace std;

namespace ometah {

//! An interface for adapting metaheuristics made with EO to oMetah
/*! 
    Please, use eoMinimizingFitness as a GeneType,
    it encapsulate a double (see eoScalarFitness.h) and 
    allow minimization, which is the rule in oMetah.
    For example :
        itsEOInterface< eoReal<eoMinimizingFitness> > yourMetaheuristic;
*/
//template< class EOType >
typedef eoReal<eoMinimizingFitness> EOType;

class itsEOInterface : public itsMetaheuristic
{
protected:
    //! A temporary population
    /*!
        Necessary for some operators
    */
    eoPop<EOType> * eoOffspring;

protected:
    //! The EO population used
    eoPop<EOType> * eoPopulation;

    //! The selection operator
    eoSelectOne<EOType> * eoSelect;

    //! The crossing over operator
    eoQuadOp<EOType> * eoCross;
    float eoCrossRate;

    //! The mutation operator
    eoMonOp<EOType> * eoMutate;
    float eoMutateRate;

protected:
    
    //! the initialization
    /*void initialization() 
    {
        itsMetaheuristic::initialization();
    };*/
    





    //! the learning : selection, crossover
    void learning() 
    {cerr << " L " << endl;
        sampleToPop();
    
        // use the selection operator
        (*eoSelect)( (*eoPopulation) );
    
        for (unsigned int i=0; i < getSampleSizeCurrent()/2; i++) {
            if ( rng.flip( eoCrossRate ) ) {
                // this crossover generates 2 offspring from two parents
                if ( (*eoCross)( (*eoOffspring)[2*i], (*eoOffspring)[2*i+1] ) ) {
                    (*eoOffspring)[2*i].invalidate();
                    (*eoOffspring)[2*i+1].invalidate();
                }
            }
        }
    
        popToSample();
    }
      
    //! the diversification : mutation
    void diversification()
    {cerr << " D " << endl;
        sampleToPop();
    

        for( unsigned int i=0; i < getSampleSizeCurrent(); i++ ) {
            if( rng.flip( eoMutateRate ) ) {
                if( (*eoMutate)( (*eoOffspring)[i] ) ) {
                    (*eoOffspring)[i].invalidate();
                }
            }
        }

    
        popToSample();
    }
    
    //! the intensification : replacement
    void intensification()
    {cerr << " I " << endl;
        sampleToPop();
    
        (*eoPopulation).swap((*eoOffspring));
    
        popToSample();
        evaluate();
    }




public:
    

    //! Call to the objective function
    //static eoMinimizingFitness evaluation_mask(const EOType& point ) {return 0.0;};

    //! The constructor
    itsEOInterface( eoPop< EOType > * _pop,
                    eoSelectOne<EOType> * _select, 
                    eoQuadOp<EOType> * _cross, float _cross_rate, 
                    eoMonOp<EOType> * _mutate, float _mutate_rate
                  ) 
        : itsMetaheuristic(),
          eoPopulation( _pop ), 
          eoSelect( _select ), 
          eoCross( _cross ), eoCrossRate( _cross_rate), 
          eoMutate( _mutate ), eoMutateRate( _mutate_rate )
    {
        clog << "itsEOInterface::itsEOInterface - getLogLevel:" << getLogLevel() << " - logLevel:" << logLevel << endl;
        clog << "itsEOInterface::itsEOInterface - getLogKeys().size:" << getLogKeys().size() << " - logKeys.size:" << logKeys.size() << endl;
    }
    


    
    //! Convert the eoPop of EOType individuals to the oMetah sample
    void popToSample() 
    {
        unsigned int pop_size = (*eoPopulation).size();
        
        if( pop_size != getSampleSizeCurrent() ) {
            ostringstream msg;
            msg << "(*eoPopulation) size (" << pop_size << ") is different from the sample size (" << getSampleSizeCurrent() << ")";
            throw Exception_Size_Match( msg.str(), EXCEPTION_INFOS );
        }
    
        for(unsigned int i=0; i < pop_size; i++ ) { 
            // get the fitness of the individual in the (*eoPopulation)
            sample[i].setValue( (*eoPopulation)[i].fitness() );
            // get the vector<double> hidden behind the indivudal (for example eoReal<eoMinimizingFitness> is based on vector)
            vector<double> s = (*eoPopulation)[i];
            //sample[i].setSolution( s );
            sample[i].solution = s;
        }
    }

    //! Convert the oMetah sample to the eoPop of EOType
    void sampleToPop() 
    {
        unsigned int pop_size = (*eoPopulation).size();
        
        if( pop_size != getSampleSizeCurrent() ) {
            ostringstream msg;
            msg << "eoPopulation size (" << pop_size << ") is different from the sample size (" << getSampleSizeCurrent() << ")";
            throw Exception_Size_Match( msg.str(), EXCEPTION_INFOS );
        }
    
        for(unsigned int i=0; i < pop_size; i++ ) {
            if( sample[i].getValues().size() == 0 ) {
                throw Exception_Size("the sample has not been evaluated", EXCEPTION_INFOS );
            }
            // set the fitness
            (*eoPopulation)[i].fitness( sample[i].getValues()[0] );
        
            // EO doesn't implement the [] operator so that we can put a vector<double> in the EOType
            // fortunatly, we can use the constructor and the [] operator for items
            unsigned int i_size;
            EOType s( i_size, 0.0 ); // build a EOType vector of i_size items filled with 0.0
            for( unsigned int j=0; j<i_size; j++) {
                s[j] = sample[i].getSolution()[j]; // use the [] operator to change its items
            }
            (*eoPopulation)[i] = s; // put an EOType in the eoPop<EOType>
        }
    }
    
    
};

}//ometah

#endif
