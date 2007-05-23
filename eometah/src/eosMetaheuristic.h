//
// C++ Interface: eosMetaheuristic
//
// Description: 
//
//
// Author: Johann Dréo <nojhan@gmail.com>, (C) 2007
//
// Copyright: See COPYING file that comes with this distribution
//
//

#ifndef _eosMetaheuristic_h
#define _eosMetaheuristic_h

#include <vector>

#include <eo>

using namespace std;

template <class EOT>
class eosMetaheuristic : public eoPopAlgo<EOT>
{
private:
	// Operators types
	typedef eoSelect			< EOT >	t_op_Select;
	typedef eoTransform		< EOT >	t_op_Transform;
	typedef eoPopEvalFunc	< EOT >	t_op_PopEvalFunc;
	typedef eoReplacement	< EOT >	t_op_Replacement;
	typedef eoBreed				< EOT >	t_op_Breed;

	// Operators types ID
	enum t_operators_types{ T_OP_SELECT, T_OP_TRANSFORM, T_OP_POPEVALFUNC, T_OP_REPLACEMENT, T_OP_BREED };

public:


	//! Constructor without arguments
	/*! You have to add operators manually */
	eosMetaheuristic(
		eoContinue<EOT>& _continuator
	) :
		continuator( _continuator )
	{
	}

	//! Add a select operator
	void add( t_op_Select* _operator ) 
	{
		operators.push_back(_operator);
		operators_types.push_back( T_OP_SELECT );
	}
	//! Add a transform operator
	void add( t_op_Transform* _operator ) 
	{
		operators.push_back(_operator);
		operators_types.push_back( T_OP_TRANSFORM );
	}
	//! Add a evaluation operator
	void add( t_op_PopEvalFunc* _operator ) 
	{
		operators.push_back(_operator);
		operators_types.push_back( T_OP_POPEVALFUNC );
	}
	//! Add a replacement operator
	void add( t_op_Replacement* _operator ) 
	{
		operators.push_back(_operator);
		operators_types.push_back( T_OP_REPLACEMENT );
	}
	//! Add a breed operator
	void add( t_op_Breed* _operator ) 
	{
		operators.push_back(_operator);
		operators_types.push_back( T_OP_BREED );
	}

	//! Call the algorithm
	void operator()(eoPop<EOT>& sample) 
	{
		// the sample after the operator
		eoPop<EOT> sample_after, sample_last_generation;

		/********** Iterations loop **********/
		do {

//clog << "#Iteration: " << continuator << endl;

			//sample_last_generation = sample;
			
			/********** Operators loop **********/
			unsigned int s = operators.size();
			for(unsigned int i=0; i<s; ++i) {
				
				//sample_after.clear();
				sample_after = sample;
				
				// Castings depending on the operator type
				switch( operators_types[i] ) {

					// if we have stored a T_OP_SELECT, cast it and call the operator()
					case T_OP_SELECT: {
//clog << "eoSelect() " << endl;
						// cast the void pointer to the asked one
						t_op_Select* op = static_cast<t_op_Select*>( operators[i] );
						
						sample_last_generation = sample;
						// call the binay functor
						(*op)( sample, sample_after ); 
						// update the main sample
						sample = sample_after;
						break;
					} /* Note : the curly braces are necessary to specify the scope of the declared variables.
						It avoids the jump crosses initialization error from the compiler. */

					case T_OP_TRANSFORM: {
//clog << "eoTransform() " << endl;
						t_op_Transform* op = static_cast<t_op_Transform*>( operators[i] );
						(*op)( sample ); // transform is a unary functor 
						break;
					}

					case T_OP_POPEVALFUNC: {
//clog << "eoPopEvalFunc() " << endl;
						t_op_PopEvalFunc* op = static_cast<t_op_PopEvalFunc*>( operators[i] );
						(*op)( sample, sample_after ); 
						sample = sample_after;
						break;
					}

					case T_OP_BREED: {
//clog << "eoBreed() " << endl;
						t_op_Breed* op = static_cast<t_op_Breed*>( operators[i] );
						sample_last_generation = sample;
						(*op)( sample, sample_after ); 
						sample = sample_after;
						break;
					}
					
					case T_OP_REPLACEMENT: {
//clog << "eoReplacement() " << endl;
						t_op_Replacement* op = static_cast<t_op_Replacement*>( operators[i] );
						(*op)( sample_last_generation, sample ); 
						break;
					}

				} // switch operators_types
//clog << "# Sample: " << sample << endl;

			} 
			/********** Operators loop **********/
//clog << "#Sample: " << sample << endl;
		} while ( continuator( sample ) ); 
		/********** Iterations loop **********/

	} // operator()

private:
	//! The stopping criterions
	eoContinue<EOT>& continuator;

	//! A vector of pointers on instances of operators
	vector<void*> operators;

	//! A vector of type identifiers for operators casting
	vector< t_operators_types > operators_types;
};

#endif
