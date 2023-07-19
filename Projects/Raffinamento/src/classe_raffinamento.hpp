#ifndef __RAFFINAMENTO_H
#define __RAFFINAMENTO_H

#include <iostream>
#include <fstream>
#include <cmath>

#include "Eigen/Eigen"
#include "ordinamento.hpp"


using namespace std;
using namespace Eigen;
using namespace LibreriaOrdinamento;


namespace LibreriaRaffinamento
{

//Le tre strutture successive memorizzano tutte le informazioni rilevanti di una maglia triangolare: punti, lati e triangoli
//Implicitamente e arbitrariamente si suppone che l'orientamento dei triangoli è antiorario per cui le sequenze
//degli indici dei punti e dei lati che descrivono i triangoli sono memorizzate preservando tale orientamento

struct Punti
{
unsigned int NumeroP = 0; //Numero di punti descritti dalla filza «Cell0Ds»
vector<Vector2d> CoordinateP = {}; //Coordinate (x,y) di ogni punti (si sceglie «Vector2d» anziché «array<double, 2> per applicare piú facilmente la norma)
vector<unsigned int> MarcatoriP = {}; //Marcatori di ogni punto
};


struct Lati
{
unsigned int NumeroL = 0; //Numero di lati in «Cell1Ds»
vector<array<unsigned int, 2>> VerticiL = {}; //Vettore degl'indici dei punti (rispetto alla filza «Cell0Ds» e ordinati in senso antiorario) del lato
vector<unsigned int> MarcatoriL = {}; //Marcatori di ogni lato
};


struct Triangoli
{
unsigned int NumeroT = 0; //Numero di triangoli descritti dalla filza «Cell2Ds»
vector<array<unsigned int, 3>> VerticiT = {}; //Vettore degl'indici dei punti (rispetto alla filza «Cell0Ds» e ordinati in senso antiorario) dei triangoli
vector<array<unsigned int, 3>> LatiT = {}; //Vettore degl'indici dei lati (rispetto alla filza «Cell1Ds» e ordinati in senso antiorario) dei triangoli

vector<Decrescente> LatiTriangoli; //Lista di strutture «Decrescente» per memorizzare i lati con marcatore nullo e l'indice del relativo triangolo cui appartengono
vector<array<unsigned int, 3>> LatiTMax = {}; //Vettore degl'indici dei punti (rispetto alla filza «Cell0Ds» e ordinati in senso antiorario) del lato massimo di ogni triangolo
vector<unsigned int> MarcatoriLatiTMax = {}; //Vettore dei marcatori dei lati massimi (utile per sapere il marcatore del punto medio venturo) di ogni triangolo
vector<unsigned int> Punte = {}; //Vettore degl'indici del punto opposto al lato massimo di ogni triangolo
};


//Nella classe «MagliaTriangolare» si memorizzano tutte le informazioni legate al triangolo come anche le funzioni per importare, raffinare, esportare ...
class MagliaTriangolare
{

    //Le informazioni sono private perché sono le funzioni legate all'oggetto
    //«MagliaTriangolare» devono essere in grado di modificarle e/o accedervi
    private:

        struct Punti Punti;
        struct Lati Lati;
        struct Triangoli Triangoli;

    public:

        ///\brief Importa la maglia triangolare e memorizza i dati nei membri privati «Punti», «Lati» e «Triangoli»
        ///\param percorso0: stringa del percorso della filza «Cell0Ds» descrivente i punti della maglia trinagolare
        ///\param percorso1: stringa del percorso della filza «Cell1Ds» descrivente i lati della maglia trinagolare
        ///\param percorso2: stringa del percorso della filza «Cell2Ds» descrivente i triangoli della maglia trinagolare
        ///\return il risultato della lettura: vera (1) se è un successo, falso (0) altrimenti
        bool ImportaMaglia(const string& percorso0,
                           const string& percorso1,
                           const string& percorso2);

        ///
        ///
        ///

        ///\brief Importa le proprietà dei punti dei triangoli dalla filza «Cell0Ds.csv»
        ///\param percorso: stringa del percorso della filza «Cell0Ds»
        ///\return il risultato della lettura: vera (1) se è un successo, falso (0) altrimenti
        bool ImportaPunti(const string& percorso);

        ///
        ///
        ///

        ///\brief Importa le proprietà dei lati dei triangoli dalla filza «Cell1Ds.csv»
        ///\param maglia: stringa del percorso della filza «Cell1Ds»
        ///\return il risultato della lettura: vera (1) se è un successo, falso (0) altrimenti
        bool ImportaLati(const string& percorso);

        ///
        ///
        ///

        ///\brief Importa le proprietà dei triangoli dalla filza «Cell2Ds.csv»
        ///\param maglia: stringa del percorso della filza «Cell2Ds»
        ///\return il risultato della lettura: vera (1) se è un successo, falso (0) altrimenti
        bool ImportaTriangoli(const string& percorso);

        ///
        ///
        ///

        ///\brief Memorizza in un vettore le aree dei triangoli, individuandone nel mentre il lato massimo e la punta
        ///\return Il vettore delle aree dei triangoli
        vector<Decrescente> CalcolaAreeTriangoli();

        ///
        ///
        ///

        ///\brief Estrae gl'indici dei primi teta triangoli da raffinare con area maggiore
        ///\param teste: numero di triangoli da raffinare
        ///\return Il vettore degli indici dei triangoli da raffinare
        vector<unsigned int> EstraiTriangoliDaRaffinare(const unsigned int& teta);

        ///
        ///
        ///

        ///\brief Raffina la maglia triangolare tramite l'algoritmo «bisezione del lato più lungo» in forma complessa
        ///\param indici: vettore degli indici dei triangoli da raffinare
        ///\return Un oggetto «MagliaTriangolare» contenente i punti e i triangoli (senza lati) della maglia triangolare raffinata
        MagliaTriangolare Dissezionatore(const vector<unsigned int>& indici);

        ///
        ///
        ///

        ///\brief Aggiunge al vettore dei triangoli i tre generati dal punto medio corrente, precedente, dal lato massimo e dalla punta
        ///\param indicePM: indice del lato del Punto Medio
        ///\param indiciLM: indici del Lato Massimo
        ///\param indicePNT: indice della PuNTa
        ///\param indiciLMP: indici del Lato Massimo Precedente
        ///\param indicePMP: indice del Punto Medio Precedente
        void SmembraTriangolo(const unsigned int& indicePM,
                              const array<unsigned int,3>& indiciLM,
                              const unsigned int& indicePNT,
                              const array<unsigned int,3>& indiciLMP,
                              const unsigned int& indicePMP);

        ///
        ///
        ///

        ///\brief Individua (se esiste) il triangolo che condivide gl'indici del lato massimo ma che ha diverso indice da «indiceT»
        ///\param indiceT: indice del triangolo considerato e rispetto a cui si cerca il triangolo opposto
        ///\param indiciLM: indici del lato massimo condivisi dal triangolo opposto
        ///\return l'indice del triangolo opposto da «0» a «numero triangoli-1»: se uguale al «numero triangoli» allora esso non esiste
        unsigned int TrovaTriangoloOpposto(const unsigned int& indiceT,
                                           const array<unsigned int,3>& indiciLM);

        ///
        ///
        ///

        ///\brief Costruisce la struttura Lati e il membro Triangoli.LatiT a partire dalla struttura Punti e del membro Triangoli.VerticiT
        void CostruisciLati();

        ///
        ///
        ///

        ///\brief Esporta in tre filze, strutturalmente simili a quelli importati, dei dati dei punti, lati e triangoli della maglia triangolare
        ///\param nomeMaglia: stringa del nome con cui inizieranno i nomi di tutte e tre le filze create
        ///\param formato: formato [senza punto!] della filza creata; ad esempio testo (txt) o Excel (csv)
        ///\return il risultato della scrittura: vera (1) se è un successo, falso (0) altrimenti
        bool EsportaMaglia(const string& nomeMaglia,
                           const string& formato);

        ///
        ///
        ///

        ///\brief Copia i dati della maglia cosí da essere accessibili al di fuori di essa senza però modificarli
        ///\param punti: la struttura su cui si copiano i Punti della maglia triangolare
        ///\param lati: la struttura su cui si copiano i Lati della maglia triangolare
        ///\param triangoli: la struttura su cui si copiano i Triangoli della maglia triangolare
        void CopiaDatiMaglia(struct Punti& punti,
                             struct Lati& lati,
                             struct Triangoli& triangoli)
        {
            punti = Punti;
            lati = Lati;
            triangoli = Triangoli;
        }

};

}

#endif // __RAFFINAMENTO_H
