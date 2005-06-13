/***************************************************************************
 *  $Id: matrix.hpp,v 1.1 2005/06/13 09:05:00 nojhan Exp $
 *  Copyright : Université Paris 12 Val-de-Marne
 *              (61 avenue du Général de Gaulle, 94010, Créteil, France)
 *  Author : Johann Dréo <nojhan@gmail.com>
 ****************************************************************************/

/*
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
 
#ifndef MATRIX
#define MATRIX

#include <vector>

using namespace std;

//! Test if a vector is comprised in bounds
template<class T>
bool isInBounds( T aVector, T mins, T maxs)
{
  unsigned int i;
  for(i=0; i<aVector.size(); i++ ){
    // too low
    if( aVector[i] < mins[i] ){
      return false;
    // too high
    }else if( aVector[i] > maxs[i] ){
      return true;
    }
  }
  return true;
}


//! Force a vector to be in bounds
template<class T>
T forceBounds( T aVector, T mins, T maxs)
{
  T CastedVector=aVector;
    
  unsigned int i;
  for(i=0; i<aVector.size(); i++ ){
    
    if( aVector[i] < mins[i] ){
      CastedVector[i]=mins[i];
            
    }else if( aVector[i] > maxs[i] ){
      CastedVector[i]=maxs[i];
    }
  }
  return CastedVector;
}

//! Create a 2D matrix filled with values
/* 
    if we want a vector<vector<double> > :
    T stand for double
    V stand for vector<vector<double> >
*/
template <class T, class U>
U matrixFilled( unsigned int dimL, unsigned int dimC, T fillValue )
{
  unsigned int i;

  // make the vector<double> possible at this step
  typename U::value_type vec(dimC, fillValue);

  U mat;
  for(i=0; i<dimL; i++){
    mat.push_back(vec);
  }

  return mat;
}


template <class T>
vector<vector< T > > matrixFilled( unsigned int dimL, unsigned int dimC, T fillValue )
{
  unsigned int i;

  // make the vector<double> possible at this step
  vector< T > vec(dimC, fillValue);

  vector<vector< T > >  mat;
  for(i=0; i<dimL; i++){
    mat.push_back(vec);
  }

  return mat;
}


//! Multipliate two 2D matrix
template<class T>
T multiply( T matA, T matB)
{

  T newMat;

  unsigned int Al=matA.size();
  unsigned int Ac=matA[0].size();
  unsigned int Bl=matB.size();
  unsigned int Bc=matB[0].size();

  newMat=matrixFilled( Al,Bc,0.0);

  if(Ac!=Bl) {
    throw("Error: Can not multiply matrices, sizes does not match");
    return newMat;
  }

  for( unsigned int i=0; i<Al; i++ ) {
    for( unsigned int j=0; j<Bc; j++ ) {
      for( unsigned int k=0; k<Ac ;k++ ) {
	newMat[i][j] += matA[i][k]*matB[k][j];
      }
    }
  }
  return newMat;
}

//! Multiply each term of a vector by a scalar
template<class T, class U>
U multiply(U aVector, T aNb)
{
  U res;

  res.reserve( aVector.size() );
  
  unsigned int i;
  for(i=0; i<aVector.size(); i++){
    double x=aVector[i]*aNb;
    res.push_back(x);
  }
  return res;
}


//! Cholesky factorization
template<class T>
T cholesky( T A)
{

  // FIXME : vérifier que A est symétrique définie positive
  
  T B;
  unsigned int Al=A.size();
  unsigned int Ac=A[0].size();
  B = matrixFilled(Al, Ac, 0.0);

  unsigned int i,j,k;
  
  // first column
  i=0;

  // diagonal
  j=0;
  B[0][0]=sqrt(A[0][0]);
  
  // end of the column
  for(j=1;j<Ac;j++) {
    B[j][0] = A[0][j] / B[0][0];
  }

  // end of the matrix
  for(i=1;i<Al;i++){ // each column

    // diagonal
    double sum=0.0;
    for(k=0; k<i; k++) {
      sum += B[i][k]*B[i][k];
    }
    B[i][i] = sqrt( A[i][i] - sum );

    
    for(j=i+1;j<Al;j++){ // rows

      // one element
      sum = 0.0;
      for(k=0; k<i; k++) {
	sum += B[j][k]*B[i][k];
      }
      B[j][i] = (A[j][i] - sum) / B[i][i];
      
    }
  }
  return B;
}

//! Transposition of a matrix
template<class T>
T transpose( T &mat)
{
  unsigned int iSize=mat.size();
  unsigned int jSize=mat[0].size();

  typename T::value_type  aVector;
  T newMat;

  unsigned int i, j;

  for (j=0; j<jSize; j++) {
    for(i=0; i<iSize; i++) {
      aVector.push_back(mat[i][j]);
    }//j

    newMat.push_back(aVector);
    aVector.clear();
  }//i

  return newMat;
}

//! Calculate the mean vector of a matrix
template<class T>
vector<T> mean( vector<vector<T> > mat)
{
  vector<T> moyDim;
  moyDim.reserve(mat.size());

  unsigned int i,a; 
  a=mat.size();

  for(i=0;i<a;i++) {
    moyDim.push_back( mean(mat[i]) ); 
  }
  return moyDim;
}

//! Calculate the mean of a vector
template<class T>
T mean( vector<T> aVector, int begin=0, int during=0)
{
  if (during==0) {
    during = aVector.size() - begin; // if no end : take all
  }

  T aSum, aMean;

  aSum = sum(aVector, begin, during); // Sum
  aMean = aSum / (during - begin); // Mean

  return aMean;
}

//! Calculate a variance-covariance matrix from a list of vector
template<class U>
U varianceCovariance( U pop)
{
  // vector of means
  // => average of columns => transposition before calculations
  typename U::value_type  vecMeanCentered = mean( transpose(pop) );
  
  // centered population 
  // same size as the initial matrix
  U popMeanCentered = matrixFilled(pop.size(),pop[0].size(), 0.0);

  // centering
  // rows
  for(unsigned int i=0;i<pop.size();i++) {
    // columns
    for(unsigned int j=0;j<pop[i].size();j++) {
      popMeanCentered[i][j] = (pop[i][j] - vecMeanCentered[j]);
    }
  }
  
  // transposition of the centered matrix
  U popMeanCenteredT;
  popMeanCenteredT = transpose(popMeanCentered);


  // final varaince/covariance matrix
  U popVar;
  popVar = multiply( popMeanCenteredT, popMeanCentered );

  // multiplication by n-1 :
  for(unsigned int i=0;i<popVar.size();i++) {
    for(unsigned int j=0;j<popVar[i].size();j++) {
      popVar[i][j]=popVar[i][j]/(pop.size());
    }
  }

  return popVar;
}

//! Calculate the sum of a vector
template<class T>
T sum(vector<T> aVector, int begin=0, int during=0)
{
  if (during==0) {
    during = aVector.size() - begin;
  }

  T aSum=0;

  for (int j=begin; j<during; j++) {
    aSum = aSum + aVector[j]; // sum
  }//for (j)

  return aSum;

}

//! Calculate the standard deviation of a vector
template<class T>
T stdev(vector<T> aVector, int begin=0, int during=0)
{
  if (during==0) {
    during = aVector.size() - begin;
  }

  vector<T> deviation;
  T aMean, aDev, aStd;

  aMean = mean(aVector, begin, during); // mean

  for (unsigned int j=begin; j<during; j++) {
    aDev = aMean - aVector[j];
    deviation.push_back(aDev*aDev);
  }//for (j)

  aStd = sqrt( mean(deviation, begin, during) );

  return aStd;

}

//! Find the minimum value of a vector
template<class T>
typename T::value_type min(T aVector, unsigned int begin=0, unsigned int during=0)
{
  if (during==0) {
    during = aVector.size() - begin;
  }

  typename T::value_type aMin = aVector[begin];

  for (unsigned int i=begin+1; i<during; i++) {
    if ( aVector[i] < aMin ) {
      aMin = aVector[i];
    }
  }
  
  return aMin;
}

//! Find the maximum value of a vector
template<class T>
typename T::value_type max(T aVector, unsigned int begin=0, unsigned int during=0)
{
  if (during==0) {
    during = aVector.size() - begin;
  }

  typename T::value_type aMax = aVector[begin];

  for (unsigned int i=begin+1; i<during; i++) {
    if ( aVector[i] > aMax ) {
      aMax = aVector[i];
    }
  }
  
  return aMax;
}

//! Substraction of two vectors, terms by terms
template<class T>
T substraction(T from, T that)
{
  T res;
  
  res.reserve(from.size());

  for(unsigned int i=0; i<from.size(); i++){
    res.push_back( from[i]-that[i] );
  }
  return res;
}

//! Addition of two vectors, terms by terms
template<class T>
T addition(T from, T that)
{
  T res;

  res.reserve( from.size() );

  for(unsigned int i=0; i<from.size(); i++){
    res.push_back( from[i]+that[i] );
  }
  return res;
}

//! Calculate the absolute values of a vector
template<class T>
T absolute(T aVector)
{
  for(unsigned int i=0; i<aVector.size(); i++){
    aVector[i] = abs(aVector[i]);
  }
    
  return aVector;
}

#endif // MATRIX