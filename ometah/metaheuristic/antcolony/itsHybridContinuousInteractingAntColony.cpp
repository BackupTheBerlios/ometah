
#include "itsHybridContinuousInteractingAntColony.hpp"


void itsHybridContinuousInteractingAntColony::initialization()
{
  itsMetaheuristic::initialization();

  antsNb = getSampleSize();
  createAnts();
}

void itsHybridContinuousInteractingAntColony::diversification()
{
  if ( antsNb != getSampleSizeCurrent() ) {
    ostringstream msg;
    msg << "ErrorSize: antsNb (" << antsNb << ")"
        << " and sample size (" << getSampleSizeCurrent() << ")"
        << " does not match";
    throw msg.str().c_str();
  }

  // move ants
  for ( unsigned int i=0; i < getSampleSizeCurrent(); i++ ) {
    antMove( i );
  }

  // ants -> sample
  for ( unsigned int i=0; i < antsNb; i++ ) {
    itsPoint p;
    p.setSolution( antCurrentPoint[i] );
    vector<double> val;
    val.push_back( antCurrentValue[i] );
    p.setValues( val );
    sample[i] = p;
  }

}

void itsHybridContinuousInteractingAntColony::intensification()
{
}

void itsHybridContinuousInteractingAntColony::learning()
{
}


/**********************
 * M�hodes g��ales *
 **********************/


/********** Constructeur **********/
itsHybridContinuousInteractingAntColony::itsHybridContinuousInteractingAntColony()
{

  setName("Hybrid Continuous Interacting Ant Colony");
  setKey("HCIAC");
  setAccronym("HCIAC");
  setDescription("An agent-based ant colony algorithm hybrided with a nelder-mead local search");
  setCitation("");
  setFamily("Ant Colony Algorithm");

  // propri��

  probaUseMemoryThreshold = 0.5;
  probaUseMemoryPower = 10;
  
  probaUseMemoryMean = 0.5;
  probaUseMemoryStd = 0.2;
  
  probaUseNMSThreshold = 0.5;
  probaUseNMSPower = 10;
  
  spotsPersistence = 0.5;
  spotsMinValue = 0.0001;

  moveRangeStdRatio = 0.8;
  moveRangeMeanRatio = 0.5;

  nmsMaxEvals = 100;
}


/********** Cr� la population **********/ 
void itsHybridContinuousInteractingAntColony::createAnts()
{

  // r�ervations vecteurs

  antProbaMemory.reserve( antsNb );
  //antMoveRange.reserve( antsNb*SpaceDim() );
  antMoveRange.reserve( antsNb );
  antCurrentPoint.reserve( antsNb * this->problem->getDimension() );
  antCurrentValue.reserve( antsNb );
  // trop gourmand : antIncomingPoints.reserve( antsNb*antsNb*SpaceDim() );
  antIncomingPoints.reserve( antsNb*antsNb );
  antIncomingValues.reserve( antsNb*antsNb );
  antState.reserve( antsNb );
  
  for( unsigned int i=0; i < antsNb; i++ ) {
    
    // tirage al�toire uniforme sur l'hypercube de l'espace de recherche
    
    vector<double> aPoint;
    /*
    aPoint.reserve( this->problem->getDimension() );
    aPoint = randomUniform( this->problem->boundsMinima(), this->problem->boundsMaxima() );
    */
    aPoint = this->sample[i].getSolution();
    
    // tire suivant N l'amplitude max de mouvement
    // distribution normale sur la moyenne de l'amplitude de l'espace de recherche
    double aMoveRange;
    aMoveRange = abs( randomNormal( 
				  mean( substraction(this->problem->boundsMaxima(),this->problem->boundsMinima()) ) * moveRangeMeanRatio, 
				  1/2 * mean( substraction(this->problem->boundsMaxima(),this->problem->boundsMinima()) ) * moveRangeStdRatio 
				  ) );
    
    // tire sur N la probabilit�de d�art d'utiliser un type de m�oire
    double aProbaUseMemory;
    aProbaUseMemory = randomNormal( probaUseMemoryMean, probaUseMemoryStd );
    // corrige les d�assements
    if ( aProbaUseMemory < 0 ) { aProbaUseMemory = 0; }
    if ( aProbaUseMemory > 1 ) { aProbaUseMemory = 1; }
    
    // ajoute la fourmi
    addAnt( aPoint, aMoveRange, aProbaUseMemory );
  }
}
  

/********** Ajoute une fourmi  **********/ 
//void itsHybridContinuousInteractingAntColony::addAnt( vector<double> aPoint, vector<double> aMoveRange, double aProbaUseMemory )
void itsHybridContinuousInteractingAntColony::addAnt( vector<double> aPoint, double aMoveRange, double aProbaUseMemory )
{
  // variables
  antProbaMemory.push_back( aProbaUseMemory );
  //antMoveRange.push_back( aMoveRange );
  antMoveRange.push_back( aMoveRange );
  antCurrentPoint.push_back( aPoint );
  
  // initialisations
  itsPoint p;
  p.setSolution( aPoint );
  antCurrentValue.push_back( evaluate( p ).getValues()[0] );
  antState.push_back( 0 );

  // ini piles messages
  vector<double> nullValuesStack;
  double nullValue = 0;
  nullValuesStack.push_back( nullValue );
  antIncomingValues.push_back( nullValuesStack );

  vector< vector<double> > nullPointsStack;
  vector<double> nullPoint( this->problem->getDimension(), 0 );
  nullPointsStack.push_back( nullPoint );
  antIncomingPoints.push_back( nullPointsStack );
}
  

  


/************************
 * M�hodes des Fourmis *
 ************************/


/********** routine de d�lacement **********/ 
void itsHybridContinuousInteractingAntColony::antMove( int antId )
{  
  printDebug( "antState", antState[0]);
  
  // si fourmi viens de faire un simplexe
  if ( antState[antId] == 0 ) {
    // re-placement al�toire :
    vector<double> aPoint;
    aPoint.reserve( this->problem->getDimension() );
    
    
    // tirage al�toire uniforme sur l'hypercube de l'espace de recherche
    // aPoint = RandomVector_Cube( getBoundsMinima(), getBoundsMaxima() );
    
    // tirage al�toire dans la zone de visibilit�    //aPoint = RandomVector_Noise( antCurrentPoint[antId], antMoveRange[antId] );
    vector<double> tempVec( this->problem->getDimension(),antMoveRange[antId] );
    aPoint = noiseUniform( antCurrentPoint[antId], tempVec );
    
    antMoveTo(antId, aPoint);
    
    // efface la pile de message
    vector<double> nullValuesStack;
    double nullValue = 0;
    nullValuesStack.push_back( nullValue );
    antIncomingValues[antId] =  nullValuesStack;
    
    vector< vector<double> > nullPointsStack;
    vector<double> nullPoint( this->problem->getDimension(), 0 );
    nullPointsStack.push_back( nullPoint );
    antIncomingPoints[antId] = nullPointsStack;
    
    
    // fin de cet �at
    antState[antId]=0.0001;
    
    spotsEvaporate();
    return;
    
  } else { // fourmi n'as pas encore fait de simplexe
    // d�lacement normal
    
    // choix du d�lacement
    int ok=0;
    int choice;
    // si choix des spots
    choice = thresholdChoice( antProbaMemory[antId], probaUseMemoryThreshold, probaUseMemoryPower, 'e' );
    
    if ( choice  ) {
      ok += antMoveSpots( antId );
    } else { // choix des messages
      ok += antMoveMessages( antId );
    }
        
    // si d�lacement spots/message fait
    if ( ok > 0 ) {
      spotsEvaporate();
      return;
    }
    
    // teste motivation simplexe
    choice = thresholdChoice( antState[antId], probaUseNMSThreshold, probaUseNMSPower, 'e' );

    if ( choice ) {
      // intensification par simplexe
      ok += antMoveNMS(antId);
      // d�ot spot
      antSpotLay( antId, antCurrentPoint[antId], antCurrentValue[antId] );
      // a fait son intensification
      antState[antId] = 0;

    } else { // pas motiv� pour le simplexe
      // test envoi de message
      choice = thresholdChoice( 1 - antState[antId], probaUseNMSThreshold, probaUseNMSPower, 'e' );
      
      if ( choice ) {
        // envoi de message
        antMessageSend( antId, antMessageChooseMate(antId) );	
        // augmentation motivation
        antIncreaseState(antId);
      }
    }
    
    // si d�lacement simplexe impossible
    if ( ok == 0 ) {
      antMoveRandom( antId );
      spotsEvaporate();
      return;
    }
    
  }
  
  spotsEvaporate();
}

/********** intensification par simplexe **********/
int itsHybridContinuousInteractingAntColony::antMoveNMS( int antId )
{
  
  //NMSearch NMS;
  itsNelderMead NMS;
  
  // link avec la fonction objectif de la m�hode appelante
  NMS.problem = this->problem;

  // no ending stopping criteria
  NMS.setValueMin(0.0);
  NMS.setIterationsMaxNumber( this->getIterationsMaxNumber() );
  
  // used stopping criterion
  NMS.setEvaluationsMaxNumber( this->nmsMaxEvals );
  
  
  // d�art sur la position actuelle
  itsPoint basePoint;

  // edges from sample hypercube
  vector<double> edges;
  for ( int i=0; i<this->problem->getDimension(); i++ ) {
    edges.push_back( antMoveRange[antId] );
  }
  
  basePoint.setSolution( antCurrentPoint[antId] );
  NMS.initSimplexFromBasePoint( basePoint, edges );

  // lancement
  NMS.startSilent();
  
  // r�up�e le point final
  antCurrentPoint[antId] = NMS.getOptimum().getSolution();
  // r�up�e la valeur finale
  antCurrentValue[antId] = NMS.getOptimum().getValues()[0];

  // ajoute le nb d'�aluations effectu�  SetCurrentEvalNumber( CurrentEvalNumber() + NMS.CurrentEvalNumber() );
  
  return 1;
}
 
/********** routine de d�lacement **********/ 
int itsHybridContinuousInteractingAntColony::antMoveSpots( int antId )
{
  vector<int> closeIds = antGetCloseSpots( antId );

  // si pas assez de spots �poximit�(0 ou 1 spots seulement), mouvement impossible
  if ( closeIds.size() < 2 ) {
    return 0;
  }
  
  vector< vector<double> > closePoints;
  closePoints.reserve( closeIds.size() );
  vector< double > closeValues;
  closeValues.reserve( closeIds.size() );
  
  // parcours les spots s�ectionn�
  unsigned int i;
  for( i=0; i<closeIds.size(); i++  ) {
    // les stockes
    closePoints.push_back( spotsPoints[ closeIds[i] ] );
    closeValues.push_back( spotsValues[ closeIds[i] ] );
  }

  // si mouvement possible vers centre gravit�spots visibles
  // FIXME : rajout de bruit plutot que d'aller exactement sur le CG ?
  return antMoveTo( 
		 antId, 
		 gravityCenter( closePoints, closeValues ) 
		 );
}

/********** routine de d�lacement **********/ 
int itsHybridContinuousInteractingAntColony::antMoveMessages( int antId )
{
  int i = antMessageChoose( antId );

  // si impossible de choisir un message
  if ( i==0 ) {

    return 0;
  }
  // antMessageChoose renvoi (index +1)
  // ok car index de 0 correspond �un vecteur nul de remplissage
  // FIXME : remplacer le vecteur de message par une matrice creuse?
  //i = i-1;

  int res = 0;
  // si valeur message meilleure
  // FIXME : comparer tout les messages? aller sur le meilleur message m�e si moins bon?
  if ( antIncomingValues[antId][i] < antCurrentValue[antId] ) {
    
    // bruite le point final
    /*vector<double> newPoint = RandomVector_Noise( 
						 antIncomingPoints[antId][i],
						 antMoveRange[antId]
						 );*/
    vector<double> tempVec( this->problem->getDimension(), antMoveRange[antId]);
    vector<double> newPoint = noiseUniform( 
						 antIncomingPoints[antId][i],
						 tempVec
						 );

    // essaye de bouger
    res = antMoveTo( antId, newPoint );
  }

  
  // efface le message
  antIncomingPoints[antId].erase( antIncomingPoints[antId].begin() + i );
  antIncomingValues[antId].erase( antIncomingValues[antId].begin() + i );
  
  return res;
}

/********** d�lacement al�toire **********/ 
int itsHybridContinuousInteractingAntColony::antMoveRandom( int antId ) 
{
  vector<double> aPoint;
  aPoint.reserve( this->problem->getDimension() );

  // tire une position au hasard dans l'espace
  //aPoint = RandomVector_Cube( getBoundsMinima(), getBoundsMaxima() );

  // modifie le vecteur de la fourmi

  // ajoute du bruit
  //aPoint = RandomVector_Noise( antCurrentPoint[antId], antMoveRange[antId]  );
  vector<double> tempVec( this->problem->getDimension(),antMoveRange[antId] );
  aPoint = noiseUniform( antCurrentPoint[antId], tempVec  );
  
  if ( antMoveTo( antId, aPoint ) ) {
    return 1;
  } else {
    return 0;
  }
}


/********** d�lacement sur un point **********/ 
int itsHybridContinuousInteractingAntColony::antMoveTo( int antId, vector<double> aPoint )
{

  // v�ifie la pr�ence dans les bornes
  if( ! isInBounds(aPoint,this->problem->boundsMinima(),this->problem->boundsMaxima()) ) {
    // renvoi false, ne peut sortir de l'espace de recherche
    return 0;
  }
  
  // change la coordonn� de la fourmi
  antCurrentPoint[ antId ] = aPoint;
  
  // met �jour la valeur de la fonction objectif
  itsPoint p;
  p.setSolution( aPoint );
  antCurrentValue[ antId ] = evaluate( p ).getValues()[0];

  // renvoi true, la fourmi �boug�
  return 1;
}



/********** choisi un message dans la pile **********/ 
int itsHybridContinuousInteractingAntColony::antMessageChoose( int antId )
{
  // si pas de message alors erreur
  if ( antIncomingValues[antId].size() <= 1 ) {
    return 0;
  }
  
  // FIXME : ne pas au hasard?
  // +1 pour �iter le 0 pris par le retour erreur
  // donc tire entre 1 et .size()
  int res =  ( rand() % (antIncomingValues[antId].size()-1) ) + 1;
  return res;
}


/********** choisi une fourmie parmi d'autre pour un envoi de message **********/ 
int itsHybridContinuousInteractingAntColony::antMessageChooseMate( int antId )
{
  // choisi une fourmi pour envoyer un message
  // FIXME : ne pas au hasard?
  int res = ( rand() % antsNb );
  return res;
}


/********** envoi un message �une autre fourmi **********/ 
void itsHybridContinuousInteractingAntColony::antMessageSend( int antIdSend, int antIdGet )
{
  antIncomingValues[antIdGet].push_back( antCurrentValue[antIdSend] );
  antIncomingPoints[antIdGet].push_back( antCurrentPoint[antIdSend] );
}


/********** g�e la motivation **********/
void itsHybridContinuousInteractingAntColony::antIncreaseState( int antId )
{
  //vector<int> spots = antGetCloseSpots( antId );

  //cout << "antIncreaseState: " << antState[antId];  

  //if (spots.size() == 0) {
    antState[antId] = antState[antId] + 0.1;

    //cout << " -> " << antState[antId] << endl;

    return;
    //}

    // FIXME augmentation de la motivation �revoir : augmente en fonction du nb de spot alors que n'est lanc� que si aucun spot !
    /*
  unsigned int i;

  // somme des spots visibles
  double sum = 0;
  for ( i=0; i < spots.size(); i++ ) {
    sum += spotsValues[i];
  }
  
  // somme de tout les spots
  double sumTot = 0;
  for ( i=0; i < spotsValues.size(); i++ ) {
    sumTot += spotsValues[i];
  }

  // ajoute la proportion de spots visibles parmi tout les spots
  antState[antId] += sum/sumTot;

  PrintDebug( antState[antId], 6);
  cout << "if it is shown, it's a bug" << endl;
    */
}

/********** d�ose un spot **********/ 
void itsHybridContinuousInteractingAntColony::antSpotLay( int antId, vector< double > point, double value )
{
  vector<int> spots = antGetCloseSpots( antId );
  
  // si pas de spots visibles
  if ( spots.size() == 0 ) {
    // d�ot d'un nouveau spot
    spotsValues.push_back( value );
    spotsPoints.push_back( point );
  } else {
    // renforcement d'un spot
    // FIXME : fonction de renforcement non lin�ire?
    int spot = antGetCloserSpot( antId );
    spotsValues[spot] += spotsValues[spot] / 2; // une moiti�en plus
    
    // baisse zone visible
    spot = antGetFarerInCloseSpot( antId );
    
    
    // FIXME il vaudrait mieux prendre la distance euclidienne au spot que de d�inir ainsi un hypercube sur chaque dimensions...
    /*for ( int i=0; i<this->problem->getDimension(); i++ ) {
      antMoveRange[antId][i] = abs( spotsPoints[spot][i] - antCurrentPoint[antId][i] );
      }*/

    antMoveRange[antId] = distanceEuclidian( antCurrentPoint[antId], spotsPoints[spot] );

    printDebug( "antMoveRange", antMoveRange[0]);
  }
}


/********** r�up�e les indices des spots les plus proches **********/
vector<int> itsHybridContinuousInteractingAntColony::antGetCloseSpots( int antId )
{
  unsigned int n = spotsValues.size();
  
  if ( n==0 ) {
    vector<int> nullVec;
    return nullVec;
  }
  
  // r�erve le plus possible d'espace
  vector<int> spotsIds;
  spotsIds.reserve( n );
  
  unsigned int i;
  for ( i=0; i<n; i++ ) {
    // si le spot est dans la zone visible
    /*int k = 0;
    for( int j=0; j<this->problem->getDimension(); j++ ) {
      if( abs( antCurrentPoint[antId][j] - spotsPoints[i][j] ) < antMoveRange[antId][j] ) {
	k++;
      }
      }
      
    if ( k == this->problem->getDimension() ) {
      spotsIds.push_back( i );
      }*/
    
    double dist = distanceEuclidian( antCurrentPoint[antId], spotsPoints[i] );
    if ( dist < antMoveRange[antId] ) {
      spotsIds.push_back( i );
    }
  }

  return spotsIds;
}


/********** r�up�e les indices des spots les plus proches **********/
int itsHybridContinuousInteractingAntColony::antGetCloserSpot( int antId )
{
  unsigned int n = spotsValues.size();

  // si existe des spots
  if (n==0) {
    return -1;
  }
  
  // r�erve le plus possible d'espace
  vector<int> spotsIds;
  spotsIds.reserve( n );
  
  double shortestDistance = distanceEuclidian( antCurrentPoint[antId], spotsPoints[0] );
  int closestSpotId = 0;

  // si plus de 1 spots
  if ( n>1 ) {
    unsigned int i;
    for ( i=1; i<n; i++ ) {
      // si le spot est dans la zone visible
      double dist = distanceEuclidian( antCurrentPoint[antId], spotsPoints[i] );
      if ( dist < shortestDistance ) {
        shortestDistance = dist;
        closestSpotId = i;
      }
    }
  }

  return closestSpotId;
}


//! prend le plus lointain des spots visibles
int itsHybridContinuousInteractingAntColony::antGetFarerInCloseSpot( int antId )
{
  vector<int> spotsIds = antGetCloseSpots( antId );
  
  unsigned int n = spotsIds.size();

  // si n'existe pas de spots
  if ( n == 0 ) {
    return -1;
  }
  
  double bigestDistance = distanceEuclidian( antCurrentPoint[antId], spotsPoints[spotsIds[0]] );
  int fareSpotId = 0;

  // si plus de 1 spots
  if ( n>1 ) {
    unsigned int i;
    for ( i=1; i<n; i++ ) {
      // distance entre la fourmi et ce spot
      double dist = distanceEuclidian( antCurrentPoint[antId], spotsPoints[i] );
      // si distance plus grande que la plus grande
      if ( dist > bigestDistance ) {
        bigestDistance = dist;
        fareSpotId = i;
      }
    }
  }

  return fareSpotId;
}

//! fonction de choix par seuils
int itsHybridContinuousInteractingAntColony::thresholdChoice( double state, double threshold, double power, char function )
{
  
  // tire une valeur au hasard
  double val = (double)rand()/RAND_MAX;
  double lim;
  
  switch ( function ) {
  case 'e':
    // fonction type exponentielle
    lim = 1/( 1+exp( -1*power*state+threshold*power ) );
    break;
    // fonction type exponentielle par d�aut
    lim = 1/( 1+exp( -1*power*state+threshold*power ) );
  default:

    break;
  }

  // si tirage inf�ieur �valeur du seuil pour cet �at
  if ( val <= lim ) {
    return 1;
  } else {
    return 0;
  }
  
}

/********** Evapore les sports **********/
void itsHybridContinuousInteractingAntColony::spotsEvaporate()
{
  // r�uit l'attrait des spots
  // S - (1-p) * S
  spotsValues = substraction( 
				   spotsValues,
				   multiply( spotsValues, (1-spotsPersistence ) )
				   );

  // supprime les spots trop faibles
  unsigned int i;
  for( i=0; i < spotsValues.size(); i++ ) {
    if( spotsValues[i] <= spotsMinValue ) {
      spotsValues.erase( spotsValues.begin()+i );
      spotsPoints.erase( spotsPoints.begin()+i );
    }
  }
}


/********** Calcule la distance interindividuelle moyenne **********/ 
double itsHybridContinuousInteractingAntColony::medianLengthInPopulation()
{
  return 0;
}


/***********************************
 * M�hodes d'acc� aux propri�� *
 ***********************************/



/********** Change le nombre de fourmis **********/ 
void itsHybridContinuousInteractingAntColony::SetAntsNb( unsigned int value )
{
  antsNb = value;
}
  

/********** Change la persistence **********/ 
void itsHybridContinuousInteractingAntColony::SetSpotsPersistence( double value )
{
  spotsPersistence = value;
}


/********** Change le nombre initial de messages **********/ 
void itsHybridContinuousInteractingAntColony::SetMessagesNbIni( unsigned int value )
{
  messagesNbIni = value;
}


/********** Change le pourcentage de l'espace de recherche donnant l'�art-type de la distribution des amplitudes des mouvements sur toute les fourmis **********/ 
void itsHybridContinuousInteractingAntColony::SetMoveRangeStdRatio( double value )
{
  if ( value > 1.0 ) {
    /*
    string aDescription;
    aDescription = "Move ranges standard-deviation ratio must be lesser than 1.0";
    itsError aError("itsHybridContinuousInteractingAntColony", "Bad move range std-dev ratio", aDescription);
    throw aError;
    */
    cerr << "<warning>Move ranges standard-deviation ratio too high : " << value << "</warning>" << endl;
    moveRangeStdRatio = 1.0;
  }
  moveRangeStdRatio = value;
}

/********** Change le pourcentage de l'espace de recherche donnant la moyenne de la distribution des amplitudes des mouvements sur toute les fourmis **********/ 
void itsHybridContinuousInteractingAntColony::SetMoveRangeMeanRatio( double value )
{
  if ( value > 1.0 ) {
    /*
    string aDescription;
    aDescription = "Move ranges mean ratio must be lesser than 1.0";
    itsError aError("itsHybridContinuousInteractingAntColony", "Bad move range mean ratio", aDescription);
    throw aError;
    */
    cerr << "<warning>Move ranges mean ratio too high : " << value << "</warning>" << endl;
    moveRangeMeanRatio = 1.0;
  }
  moveRangeMeanRatio = value;
}


/********** Probabilit�de prise en charge des spots ou des messages : moyenne distribution **********/ 
void itsHybridContinuousInteractingAntColony::SetProbaUseMemoryMean( double value )
{
  if ( value > 1.0 ) {
    cerr << "<warning>probaUseMemoryMean too high : " << value << "</warning>" << endl;
    probaUseMemoryMean = 1.0;
  }
  probaUseMemoryMean = value;
}


/********** Probabilit�de prise en charge des spots ou des messages : �art-type distribution **********/ 
void itsHybridContinuousInteractingAntColony::SetProbaUseMemoryStd( double value )
{
  if ( value > 1.0 ) {
    cerr << "<warning>probaUseMemoryStd too high : " << value << "</warning>" << endl;
    probaUseMemoryStd = 1.0;
  }
  probaUseMemoryStd = value;
}


/********** Seuil de prise en charge des spots ou des messages **********/ 
void itsHybridContinuousInteractingAntColony::SetProbaUseMemoryThreshold( double value )
{
  probaUseMemoryThreshold = value;
}

/********** Puissance de la prise en charge des spots ou des messages **********/ 
void itsHybridContinuousInteractingAntColony::SetProbaUseMemoryPower( double value )
{
  probaUseMemoryPower = value;
}


/********** Seuil de prise en charge des spots ou des messages **********/ 
void itsHybridContinuousInteractingAntColony::SetProbaUseNMSThreshold( double value )
{
  probaUseNMSThreshold = value;
}


/********** Puissance de prise en charge des spots ou des messages **********/ 
void itsHybridContinuousInteractingAntColony::SetProbaUseNMSPower( double value )
{
  probaUseNMSPower = value;
}

//! Nb max d'evaluations pour le simplexe
void itsHybridContinuousInteractingAntColony::SetNMSMaxEvals( int value )
{
  nmsMaxEvals = value;
}




itsMetaheuristic* itsHybridContinuousInteractingAntColonyFactory::create()
{
    return new itsHybridContinuousInteractingAntColony;
}
