#ifndef __RAFFINAMENTO_H
#define __RAFFINAMENTO_H

#include <iostream>
#include <fstream>
#include <map>
#include "Eigen/Eigen"

using namespace std;
using namespace Eigen;

namespace LibreriaRaffinamento
{

class MagliaTriangolare
{
    public:

        struct Punti
        {
        unsigned int NumeroCell0D = 0; //Numero di elementi in «Cell0D»
        vector<Vector2d> Cell0DCoordinate = {}; //Coordinate (x,y) di «Cell0D» con dimensione «2xNumeroCell0D»
        vector<unsigned int> Cell0DMarcatori = {}; //Marcatori di «Cell0D» con dimensione «1xNumeroCell0D»  //Modifica creando una mappa che associ all'indice il relativo marcatore (mi serve successivamente quando bisogna assegnare al nuovo punto il marcatore)
        } Punti;

        struct Lati
        {
        unsigned int NumeroCell1D = 0; //Numero di elementi in «Cell1D»
        vector<Vector2i> Cell1DVertici = {}; //Indici dei vertici (daId,aId) di «Cell1D» con dimensione «2xNumeroCell1D»
        vector<unsigned int> Cell1DMarcatori = {}; //Marcatori di «Cell1D» con dimensione «1xNumeroCell1D»
        } Lati;

        struct Triangoli
        {
        unsigned int NumeroCell2D = 0; //Numero di elementi in «Cell2D»
        vector<array<unsigned int, 3>> Cell2DVertici = {}; //Indici dei vertici (daId,aId) di «Cell1D» con dimensione «1xNumeroCell2DVertici[NumeroCell2D]»
        vector<array<unsigned int, 3>> Cell2DLati = {}; //Indici di «Cell2D» e «Cell1D», dimensione «1xNumeroCell2DLati[NumeroCell2D]»
        } Triangoli;


    public:

        ///\brief Importa la maglia triangolare and testa se la maglia è corretta
        ///\param maglia: una struttura «MagliaTriangolare»
        ///\return il risultato della lettura: vera se è un successo, falso altrimenti
        bool ImportaMaglia(const string& percorso0,
                           const string& percorso1,
                           const string& percorso2);

        ///
        ///
        ///
        ///
        ///

        ///\brief Importa le proprietà di «Cell0D» dalla filza «Cell0Ds.csv»
        ///\param maglia: una struttura «MagliaTriangolare»
        ///\return il risultato della lettura: vera se è un successo, falso altrimenti
        bool ImportaCell0Ds(const string& percorso);

        ///
        ///
        ///
        ///
        ///

        ///\brief Importa le proprietà di «Cell1D» dalla filza «Cell1Ds.csv»
        ///\param maglia: una struttura «MagliaTriangolare»
        ///\return il risultato della lettura: vera se è un successo, falso altrimenti
        bool ImportaCell1Ds(const string& percorso);

        ///
        ///
        ///
        ///
        ///

        ///\brief Importa le proprietà di «Cell2D» dalla filza «Cell2Ds.csv»
        ///\param maglia: una struttura «MagliaTriangolare»
        ///\return il risultato della lettura: vera se è un successo, falso altrimenti
        bool ImportaCell2Ds(const string& percorso);

};



///\brief Calcola l'area di un insieme di tre punti «distinti» descriventi un triangolo e individua il lato massimo
///\param vertici: la matrice di dimensioni 3x3 dei punti del triangolo
///\return vuoto perché si modificano i parametri in ingresso
void AreaT(const MatrixXd& vertici, double& area, VectorXd& latoMax);

/*

  //Vedi se aggiungere una classe oppure no

  class Empty
  {
    public:
      void Show() const { std::cout<< "Hello world;"<< std::endl; }
  };

*/

}

#endif // __RAFFINAMENTO_H
