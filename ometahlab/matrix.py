#!/usr/bin/python
# -*- coding: iso-8859-1 -*-
#
# Ometahlab is a set of Python scripts to make experiments on Ometah.
#####################################################################
# Author: Jean-Philippe Aumasson <jeanphilippe.aumasson@gmail.com>
# File : matrix.py
# Functions for matrix operations.
#
#  Open Metaheuristic is a Library aimed at the conception of metaheuristics 
#  for difficult optimization.
#   
#  This program is free software; you can redistribute it and/or modify
#  it under the terms of the GNU Lesser General Public License as published by
#  the Free Software Foundation; either version 2 of the License, or
#  (at your option) any later version.
#
#  This program is distributed in the hope that it will be useful,
#  but WITHOUT ANY WARRANTY; without even the implied warranty of
#  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
#  GNU Lesser General Public License for more details.
#
#  You should have received a copy of the GNU Lesser General Public License
#  along with this program; if not, write to the Free Software
#  Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.
#

from Numeric import *
import Numeric
from LinearAlgebra import *

"""
Method used for ACP :

   1. Organize your data into column vectors, so you end up with a d \times n matrix, D.
   2. Find the empirical mean along each dimension, so you end up with a d \times 1 empirical mean vector, M.
   3. Subtract the empirical mean vector M from each column of the data matrix D. Store mean-subtracted data d \times n matrix in S.
   4. Find the empirical covariance d\times d matrix C of S. C = S \cdot S^T.
   5. Compute and sort by decreasing eigenvalue, the eigenvectors V of C.
   6. Save the mean vector M. Save the first k columns of V as P. P will have dimension d \times k. 1 \leq k \leq d

http://en.wikipedia.org/wiki/Principal_components_analysis

By ordering the eigenvectors in the order of descending eigenvalues (largest first),
one can create an ordered orthogonal basis with the first eigenvector having the direction
of largest variance of the data.
In this way, we can find directions in which the data set has the most significant
amountsof energy.

http://www.cis.hut.fi/~jhollmen/dippa/node30.html
"""


# TODO
# interface to get coordinates list from vector of the matrix
 # make a class ! attribute : eigenvectors, means !

class ACP:

    def __init__(self):
        """ Constructor """
        # the matrix we work on, n * dim
        self.__matrix = None
        # transposed matrix of ordered eigenvectors
        self.__eigenv = None
        # vectors of means over M dimensions 
        self.__means = None

    def setMatrix(self, M):
        """ Initialize matrices, with list of sublists S,
        with M = list of sublist, one sublist for each observation, containing dimensions coordinates. """
        S = Numeric.array(M)
        self.__matrix = S
        C = self.__covar()
        self.__eigenv = transpose(self.__orderedEigenvectors(C))

    def getMatrix(self):
        return self.__matrix

    def __covar(self):
        """ returns the covariance matrix of  M, n*d, corresponding to N points of dimension d
        Each line of M (sublist) is a set of values, one for each dimension."""        
        import copy
        M = self.__matrix
        # initialize dimension and number of vectors
        d = len(M[0]) # nb of dimensions = a row's
        n = len(M)    # nb of observations                
        means = array(zeros(d))
        # as a vector of floats
        means = means + 0.1 - 0.1
        # find empirical mean along each dimension
        for dim in range(d):
            for i in range(n):
                means[dim] += float(M[i][dim])
            means[dim] = float(means[dim]) / float(n)
        self.__means = means
        N = copy.copy(M)
        # substract means from each row of M
        for i in range(n):
            for dim in range(d):
                N[i][dim] = (M[i][dim] - means[dim])
        # return d*d covariance matrix
        return matrixmultiply( transpose(N), N )

    def __orderedEigenvectors(self, A):
        """ Returns the matrix of th eigenvectors ordered by decreasing eigenvalue of A (square)"""        
        values = eigenvalues(A)
        vectors = eigenvectors(A)
        svectors = eigenvectors(A)

        svalues = sort(values)
        index = array(zeros(len(values)))

        for i in range( len(values) ):
            for j in range( len(svalues) ):
                if values[i] == svalues[j]:
                    index[i] = j
    
        # vectors[0] is eigenvalues
        for i in range( len(vectors[1]) ):
            svectors[1][index[i]] = vectors[1][i]

        return svectors[1]


    def reduceDim(self, lineIndex, dim):
        """ Returns a list of dim coordinates."""
        Ve = self.__matrix[lineIndex] - self.__means
        return matrixmultiply(self.__eigenv[:dim], Ve)


# TEMP MAIN INSTRUCTION, TO DELETE !!!!!

if __name__ == '__main__':

    a = ACP()
    a.setMatrix([[2,3,4], [4,5,9],[0,1,0], [-1,-8,-9]])
    print 'MATRIX'
    print a.getMatrix()
    print 'ACP'
    for i in range(4):
        x = a.reduceDim(i,1)
        print x
    
    """
    print '5 observations, 4 dimensions'
    #S = N.reshape([4.0, 2.0, .60, 4.2, 2.1, .59, 3.9, 2.0 , .58, 4.3, 2.1, .62, 4.1, 2.2, .63], (5,3))
    S = Numeric.reshape([10, 6, 9, 8, 3, 1, 2, 4, 5, 5,2, 1, 1, 1, 0, 0, 0, 0, 1, 0 ], (5,4))
    print 'Initial matrix : (S)'
    print S
    
    M = mean(S)
    
    C = covar(S)
    E = orderedEigenvalues(C)
    P = orderedEigenvectors(C)
    Pt = transpose(P)
    
    print 'covar : (C)'
    print C
    print 'eigenvalues :'
    print E
    print 'eigenvectors ordered by eigenvalue: (P)'
    print P
    print 'P^t for dim reduced to 2'
    Pt = Pt[:2]
    print Pt
    print  'means along each dim'
    print M
    
    print 'P^t * Ve'
    
    for i in range(len(S)):
        Ve = S[i] - M
        print matrixmultiply(Pt, Ve)
    """
        
