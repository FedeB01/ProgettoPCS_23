#ifndef __RAFFINAMENTO_H
#define __RAFFINAMENTO_H

#include <iostream>
#include <fstream>
#include <cmath>

#include "Eigen/Eigen"

using namespace std;
using namespace Eigen;

namespace LibreriaRaffinamento
{

class MagliaTriangolare
{
    private:

        struct Punti
        {
        unsigned int NumeroP = 0; //Numero di elementi in «Cell0D»
        vector<Vector2d> CoordinateP = {}; //Coordinate (x,y) di «Cell0D» con dimensione «2xNumeroP» (Vector2d è necessario per la relativa norma)
        vector<unsigned int> MarcatoriP = {}; //Marcatori di «Cell0D» con dimensione «1xNumeroP»
        } Punti;

        struct Lati
        {
        unsigned int NumeroL = 0; //Numero di elementi in «Cell1D»
        vector<array<unsigned int, 2>> VerticiL = {}; //Indici dei vertici (daId,aId) di «Cell1D» con dimensione «2xNumeroL»
        vector<unsigned int> MarcatoriL = {}; //Marcatori di «Cell1D» con dimensione «1xNumeroL»
        } Lati;

        struct Triangoli
        {
        unsigned int NumeroT = 0; //Numero di elementi in «Cell2D»
        vector<array<unsigned int, 3>> VerticiT = {}; //Indici dei vertici (daId,aId) di «Cell1D» con dimensione «1xNumeroT»
        vector<array<unsigned int, 3>> LatiT = {}; //Indici di «Cell2D» e «Cell1D», dimensione «1xNumeroT»

        vector<unsigned int> Punte= {}; //Indici dei punti, ordinati in senso antiorario, del lato massimo di ogni triangolo con dimensione «1xNumeroT»
        vector<array<unsigned int, 2>> LatiTMax= {}; //Indici dei punti, ordinati in senso antiorario, del lato massimo di ogni triangolo con dimensione «1xNumeroT»
        vector<unsigned int> MarcatoriLatiTMax= {}; //Marcatori dei lati massimi, utile per sapere il marcatore del punto medio venturo, con dimensione «1xNumeroT»
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

        ///\brief Importa le proprietà di «Cell0D» dalla filza «Cell0Ds.csv»
        ///\param maglia: una struttura «MagliaTriangolare»
        ///\return il risultato della lettura: vera se è un successo, falso altrimenti
        bool ImportaCell0Ds(const string& percorso);

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

        ///\brief Importa le proprietà di «Cell2D» dalla filza «Cell2Ds.csv»
        ///\param maglia: una struttura «MagliaTriangolare»
        ///\return il risultato della lettura: vera se è un successo, falso altrimenti
        bool ImportaCell2Ds(const string& percorso);

        ///
        ///
        ///

        ///\brief Calcola l'area di un insieme di tre punti «distinti» descriventi un triangolo e individua il lato massimo
        ///\param vertici: la matrice di dimensioni 3x3 dei punti del triangolo
        ///\return vuoto perché si modificano i parametri in ingresso
        void AreaT(vector<double>& aree);

        //«AreaT» è una funzione della classe «MagliaTriangolare» che restituisce il vettore delle aree in corrispondenza dell'indice del triangolo

        ///
        ///
        ///

        ///\brief
        ///\param
        ///\return
        vector<unsigned int> EstraiTriDaRaffinare(const unsigned int& teta);

        ///
        ///
        ///

        ///\brief
        ///\param
        ///\return
        MagliaTriangolare Dissezionatore(const vector<unsigned int>& indici);

        ///
        ///
        ///

        ///\brief
        ///\param
        ///\return
        void SmembraTriangolo(const unsigned int& indicePM,
                              const array<unsigned int,2>& indiciLM,
                              const unsigned int& indicePNT,
                              const array<unsigned int,2>& indiciLMP,
                              const unsigned int& indicePMP);

        ///
        ///
        ///

        ///\brief
        ///\param
        ///\return
        unsigned int TrovaTriangoloOpposto(unsigned int& indiceT,
                                           const array<unsigned int,2>& indiciLM);

        ///
        ///
        ///

        ///\brief
        ///\param
        ///\return
        void CostruisciLati();

        ///
        ///
        ///

        ///\brief
        ///\param
        ///\return
        void EsportaMaglia(const string& formato);

};


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
