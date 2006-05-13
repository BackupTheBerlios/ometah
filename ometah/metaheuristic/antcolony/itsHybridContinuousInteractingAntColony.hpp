/***************************************************************************
 *  $Id: itsHybridContinuousInteractingAntColony.hpp,v 1.4 2006/05/13 10:05:55 nojhan Exp $
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
 
#ifndef ITSHCIAC
#define ITSHCIAC


#include <sstream>
#include <vector>
#include <cmath>

#include "../../common/random.hpp"
#include "../../common/matrix.hpp"
#include "../../common/geometry.hpp"
#include "../../common/Exception_oMetah.hpp"

#include "../itsMetaheuristic.hpp"
#include "../neldermead/itsNelderMead.hpp"

using namespace std;

namespace ometah {

/*! Métaheuristique d'optimisation :
 * Colonie de fourmis (Spots localisés) Hybridé avec Simplexe Nelder-Mead
 * Variables Continues
 */
class itsHybridContinuousInteractingAntColony : public itsMetaheuristic
{

public:
  void initialization();
  void intensification();
  void diversification();
  void learning();

  /*! Propriétés générales
   */

protected:

  /* Propriétés */
  
  //! Nombre de fourmis
  unsigned int antsNb;

  //! Seuil de prise en charge spots/messages
  double probaUseMemoryThreshold;
  
  //! Puissance de la prise en charge des spots/messages
  double probaUseMemoryPower;
  
  //! Probabilité de prise en charge des spots ou des messages : moyenne distribution
  // 0 = 100% spots ; 1 = 100% messages
  double probaUseMemoryMean;

  //! Probabilité de prise en charge des spots ou des messages : écart-type distribution
  double probaUseMemoryStd;

  //! Seuil de prise en charge du simplexe
  double probaUseNMSThreshold;
  
  //! Puissance de prise en charge du simplexe
  double probaUseNMSPower;
  
  //! Persistence des spots
  double spotsPersistence;

  //! valeur minimu des spots, en dessous ils sont supprimés
  double spotsMinValue;
  
  //! Nombre initial de message
  unsigned int messagesNbIni;
   
  //! Pourcentage de l'espace de recherche donnant l'écart-type de la distribution des amplitudes des mouvements sur toute les fourmis
  double moveRangeStdRatio;

  //! idem en moyenne
  double moveRangeMeanRatio;

  //! nb max d'évaluations pour le simplexe
  int nmsMaxEvals;

  
  /* Variables */

  //! Meilleure valeure
  double bestValue;
  
  //! Meilleure point
  vector< double > bestPoint;
  
  //! Vecteur des spots
  vector< double > spotsValues;
  //! Vecteur des valeurs des spots
  vector< vector< double > > spotsPoints;
  


  /*! Méthodes générales
   */
protected :
  //! Créé la population
  void createAnts();
  
  //! Ajoute une fourmi 
  //void addAnt( vector<double> aPoint, vector<double> aMoveRange, double aProbaUseMemory );
  void addAnt( vector<double> aPoint, double aMoveRange, double aProbaUseMemory );
  
  //! Evapore les sports
  void spotsEvaporate();
  
  //! Calcule la distance interindividuelle moyenne
  double medianLengthInPopulation();
  
public:
  //! constructeur
  itsHybridContinuousInteractingAntColony();




  /*! Propriétés des Fourmis
   * fourmis< variables >
   */
protected:

  //! état de la fourmi
  vector< double > antState;
  
  //! Probabilité de prise en charge des spots ou des messages
  vector< double > antProbaMemory;

  //! Probabilité de prise en charge des spots ou des messages : puissance
  //vector< double > antProbaMemoryPower;

  //! Amplitude des mouvements
  //vector< vector<double> > antMoveRange;
  vector<double> antMoveRange;

  //! coordonnées courantes
  // fourmis< point >
  vector< vector< double > > antCurrentPoint;

  //! valeurs courantes
  // fourmis< valeur >
  vector< double > antCurrentValue;

  //! Piles de messages : coordonnées
  // fourmis< pile< coordonnée<> > >
  vector< vector< vector<double> > > antIncomingPoints;

  //! Pile de message : valeurs
  // fourmis< pile< valeur > >
  vector< vector< double > > antIncomingValues;
  

  
  /*! Méthodes des Fourmis
   */
protected:
  //! routine de déplacement
  void antMove( int antId );
  //! fonction de choix par seuils
  int thresholdChoice( double state, double threshold, double power, char function='e' );
  //! déplacement sur spots
  int antMoveSpots( int antId );
  //! déplacement sur messages
  int antMoveMessages( int antId );
  //! intensification par simplexe
  int antMoveNMS( int antId );
  //! déplacement aléatoire
  int antMoveRandom( int antId );
  //! déplacement sur un point
  int antMoveTo( int antId, vector<double> aPoint );
  
  //! choisi un message dans la pile
  int antMessageChoose( int antId );
  //! choisi une fourmie parmi d'autre pour un envoi de massage
  int antMessageChooseMate( int antId );
  //! envoi un message à une autre fourmi
  void antMessageSend( int antIdSend, int antIdGet );
  
  //! gère la motivation
  void antIncreaseState( int antId );
  
  //! dépose un spot
  void antSpotLay( int antId, vector< double > point, double value ); 

  //! récupère les indices des spots les plus proches
  vector<int> antGetCloseSpots( int antId );

  //! récupère les indices des spots les plus proches
  int antGetCloserSpot( int antId );

  //! prend le plus lointain des spots visibles
  int antGetFarerInCloseSpot( int antId );

  /*! Méthodes d'accès aux propriétés
   */
public :
  //! Change le nombre de fourmis
  void SetAntsNb( unsigned int value );
  
  //! Change la persistence
  void SetSpotsPersistence( double Value );
  
  //! Change le nombre initial de messages
  void SetMessagesNbIni( unsigned int Value );

  //! Change le pourcentage de l'espace de recherche donnant l'écart-type de la distribution des amplitudes des mouvements sur toute les fourmis
  void SetMoveRangeStdRatio( double Value );

  //! Change le pourcentage de l'espace de recherche donnant la moyenne de la distribution des amplitudes des mouvements sur toute les fourmis
  void SetMoveRangeMeanRatio( double value );

  //! Probabilité de prise en charge des spots ou des messages : moyenne
  void SetProbaUseMemoryMean( double value );

  //! Probabilité de prise en charge des spots ou des messages : écart-type
  void SetProbaUseMemoryStd( double value );

  //! Seuil de prise en charge des spots ou des messages
  void SetProbaUseMemoryThreshold( double value );

  //! Puissance de la prise en charge des spots ou des messages
  void SetProbaUseMemoryPower( double value );

  //! Seuil de prise en charge des spots ou des messages
  void SetProbaUseNMSThreshold( double value );
  
  //! Puissance de prise en charge des spots ou des messages
  void SetProbaUseNMSPower( double value );
  
  //! Nb max d'evaluations pour le simplexe
  void SetNMSMaxEvals( int value );
};



class itsHybridContinuousInteractingAntColonyFactory : public itsMetaheuristicFactory
{
public:
    itsMetaheuristic* create();
};

}//ometah

#endif
