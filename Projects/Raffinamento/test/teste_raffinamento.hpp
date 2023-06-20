#ifndef __TESTE_RAFFINAMENTO_H
#define __TESTE_RAFFINAMENTO_H


//Variabili globali per il numero di triangoli da raffinare
unsigned int teta1 = 20; //Triangoli da raffinare nella prima maglia triangolare
unsigned int teta2 = 20; //Triangoli da raffinare nella prima maglia triangolare

#include <gtest/gtest.h>
#include <iostream>
#include "classe_raffinamento.hpp"
#include "ordinamento.hpp"


using namespace testing;
using namespace LibreriaRaffinamento;
using namespace LibreriaOrdinamento;


    //Primo teste per controllare che il vettore «vettoreOrdinato» viene ordinato correttamente in ordine crescente confrontandolo con il modello «vettoreRif»
TEST(VerificaOrdinamento, VerificaHeapSortCrescente)
{
    vector<int> vettore = {9, 13, 5, 10, 2, 7, 9, 4, 6, 12};
    vector<int> vettoreOrdinato = HeapSort<int>(vettore);
    vector<int> vettoreRif = {2, 4, 5, 6, 7, 9, 9, 10, 12 ,13};

    EXPECT_EQ(vettoreOrdinato, vettoreRif);  //Confronto fra il vettore ordinato dal «HeapSort» e quello di riferimento
}


    //Secondo teste per controllare che il vettore «vettoreOrdinato» venga ordinato correttamente in ordine decrescente confrontandolo con il modello «vettoreRif»
TEST(VerificaOrdinamento, VerificaHeapSortDecrescente)
{
  vector<Decrescente> vettore = {Decrescente(9), Decrescente(13),
                                 Decrescente(5), Decrescente(10),
                                 Decrescente(2), Decrescente(7),
                                 Decrescente(9), Decrescente(4),
                                 Decrescente(6), Decrescente(12)};

  vector<Decrescente> vettoreOrdinato = HeapSort<Decrescente>(vettore);

  vector<Decrescente> vettoreRif = {Decrescente(13), Decrescente(12),
                                    Decrescente(10), Decrescente(9),
                                    Decrescente(9), Decrescente(7),
                                    Decrescente(6), Decrescente(5),
                                    Decrescente(4), Decrescente(2)};

  EXPECT_EQ(vettoreOrdinato, vettoreRif); //Confronto fra il vettore ordinato dal «HeapSort» e quello di riferimento
}


    //Testi della prima maglia triangolare
TEST(VerificaRaffinamento, VerificaPrimaImportazione)
{
    MagliaTriangolare maglia;

    bool statoImportazione = maglia.ImportaMaglia("../Raffinamento/Dataset/Test1/Cell0Ds.csv",
                                                  "../Raffinamento/Dataset/Test1/Cell1Ds.csv",
                                                  "../Raffinamento/Dataset/Test1/Cell2Ds.csv");

    ASSERT_TRUE(statoImportazione); //Controllo che l'importazione è avvenuta con successo

    struct Punti puntiMaglia;
    struct Lati latiMaglia;
    struct Triangoli triangoliMaglia;

    maglia.CopiaDatiMaglia(puntiMaglia, latiMaglia, triangoliMaglia); //Copia dei dati cosí da essere disponibili al di fuori della classe

    for(unsigned int c = 0; c < triangoliMaglia.NumeroT; c++) //Per ogni triangolo
    {
      array<unsigned int, 3> lati = triangoliMaglia.LatiT[c]; //Si considerano gl'indici dei lati

      for(unsigned int e = 0; e < 3; e++) //Si controlla che i dati importati siano coerenti fra di loro
      {
          //Si memorizzano il primo e il seocndo((...)) indice dei punti del lato in corrispondenza dell'indice «lati[e]»
          const unsigned int inzioLato = latiMaglia.VerticiL[lati[e]][0];
          const unsigned int fineLato = latiMaglia.VerticiL[lati[e]][1];

          //Si verifica che gl'indici dei punti precendenti siano presenti in quelli memorizzati nei triangoli
          auto trovaInizioLato = find(triangoliMaglia.VerticiT[c].begin(), triangoliMaglia.VerticiT[c].end(), inzioLato);
          ASSERT_TRUE(trovaInizioLato != triangoliMaglia.VerticiT[c].end());

          auto trovaFineLato = find(triangoliMaglia.VerticiT[c].begin(), triangoliMaglia.VerticiT[c].end(), fineLato);
          ASSERT_TRUE(trovaFineLato != triangoliMaglia.VerticiT[c].end());
      }
    }
}


TEST(VerificaRaffinamento, VerificaPrimaMagliaRaffinata)
{
    MagliaTriangolare maglia, magliaRaffinata;

    bool statoImportazione = maglia.ImportaMaglia("../Raffinamento/Dataset/Test1/Cell0Ds.csv",
                                                  "../Raffinamento/Dataset/Test1/Cell1Ds.csv",
                                                  "../Raffinamento/Dataset/Test1/Cell2Ds.csv");

    ASSERT_TRUE(statoImportazione); //Controllo che l'importazione è avvenuta con successo

    vector<unsigned int> triangoliDaRaffinare = maglia.EstraiTriangoliDaRaffinare(teta1);

    magliaRaffinata = maglia.Dissezionatore(triangoliDaRaffinare);
    magliaRaffinata.CostruisciLati();

    struct Punti puntiMagliaR;
    struct Lati latiMagliaR;
    struct Triangoli triangoliMagliaR;

    magliaRaffinata.CopiaDatiMaglia(puntiMagliaR, latiMagliaR, triangoliMagliaR);


    //Il ciclo «for» è il medesimo del teste «VerificaPrimaImportazione» ma applicata alla prima maglia raffinata cosí da controllarne la coerenza
    for(unsigned int c = 0; c < triangoliMagliaR.NumeroT; c++)
    {
      array<unsigned int, 3> lati = triangoliMagliaR.LatiT[c];

      for(unsigned int e = 0; e < 3; e++)
      {
          const unsigned int inzioLato = latiMagliaR.VerticiL[lati[e]][0];
          const unsigned int fineLato = latiMagliaR.VerticiL[lati[e]][1];

          auto trovaInizioLato = find(triangoliMagliaR.VerticiT[c].begin(), triangoliMagliaR.VerticiT[c].end(), inzioLato);
          ASSERT_TRUE(trovaInizioLato != triangoliMagliaR.VerticiT[c].end());

          auto trovaFineLato = find(triangoliMagliaR.VerticiT[c].begin(), triangoliMagliaR.VerticiT[c].end(), fineLato);
          ASSERT_TRUE(trovaFineLato != triangoliMagliaR.VerticiT[c].end());
      }
    }
}


TEST(VerificaRaffinamento, VerificaPrimaEsportazione)
{
    MagliaTriangolare maglia, magliaRaffinata, magliaEsportata;

    bool statoImportazione = maglia.ImportaMaglia("../Raffinamento/Dataset/Test1/Cell0Ds.csv",
                                                  "../Raffinamento/Dataset/Test1/Cell1Ds.csv",
                                                  "../Raffinamento/Dataset/Test1/Cell2Ds.csv");

    ASSERT_TRUE(statoImportazione); //Controllo che l'importazione è avvenuta con successo

    unsigned int teta = 20;
    vector<unsigned int> triangoliDaRaffinare = maglia.EstraiTriangoliDaRaffinare(teta);

    magliaRaffinata = maglia.Dissezionatore(triangoliDaRaffinare);
    magliaRaffinata.CostruisciLati();
    bool statoEsportazione = magliaRaffinata.EsportaMaglia("Test1/Teste Prima Maglia","csv");

    ASSERT_TRUE(statoEsportazione); //Controllo che l'esportazione è avvenuta con successo
}


    //Testi della seconda maglia triangolare
TEST(VerificaRaffinamento, VerificaSecondaImportazione)
{
    MagliaTriangolare maglia;

    bool statoImportazione = maglia.ImportaMaglia("../Raffinamento/Dataset/Test2/Cell0Ds.csv",
                                                  "../Raffinamento/Dataset/Test2/Cell1Ds.csv",
                                                  "../Raffinamento/Dataset/Test2/Cell2Ds.csv");

    ASSERT_TRUE(statoImportazione); //Controllo che l'importazione è avvenuta con successo

    struct Punti puntiMaglia;
    struct Lati latiMaglia;
    struct Triangoli triangoliMaglia;

    maglia.CopiaDatiMaglia(puntiMaglia, latiMaglia, triangoliMaglia); //Copia dei dati cosí da essere disponibili al di fuori della classe

    //Il ciclo «for» è il medesimo del teste «VerificaPrimaImportazione» ma applicata alla seconda maglia cosí da controllarne la coerenza
    for(unsigned int c = 0; c < triangoliMaglia.NumeroT; c++)
    {
      array<unsigned int, 3> lati = triangoliMaglia.LatiT[c];

      for(unsigned int e = 0; e < 3; e++)
      {
          const unsigned int inzioLato = latiMaglia.VerticiL[lati[e]][0];
          const unsigned int fineLato = latiMaglia.VerticiL[lati[e]][1];

          auto trovaInizioLato = find(triangoliMaglia.VerticiT[c].begin(), triangoliMaglia.VerticiT[c].end(), inzioLato);
          ASSERT_TRUE(trovaInizioLato != triangoliMaglia.VerticiT[c].end());

          auto trovaFineLato = find(triangoliMaglia.VerticiT[c].begin(), triangoliMaglia.VerticiT[c].end(), fineLato);
          ASSERT_TRUE(trovaFineLato != triangoliMaglia.VerticiT[c].end());
      }
    }
}


TEST(VerificaRaffinamento, VerificaSecondaMagliaRaffinata)
{
    MagliaTriangolare maglia, magliaRaffinata;

    bool statoImportazione = maglia.ImportaMaglia("../Raffinamento/Dataset/Test2/Cell0Ds.csv",
                                                  "../Raffinamento/Dataset/Test2/Cell1Ds.csv",
                                                  "../Raffinamento/Dataset/Test2/Cell2Ds.csv");

    ASSERT_TRUE(statoImportazione); //Controllo che l'importazione è avvenuta con successo

    vector<unsigned int> triangoliDaRaffinare = maglia.EstraiTriangoliDaRaffinare(teta2);

    magliaRaffinata = maglia.Dissezionatore(triangoliDaRaffinare);
    magliaRaffinata.CostruisciLati();

    struct Punti puntiMagliaR;
    struct Lati latiMagliaR;
    struct Triangoli triangoliMagliaR;

    magliaRaffinata.CopiaDatiMaglia(puntiMagliaR, latiMagliaR, triangoliMagliaR);

    //Il ciclo «for» è il medesimo del teste «VerificaPrimaImportazione» ma applicata alla seconda maglia raffinata cosí da controllarne la coerenza
    for(unsigned int c = 0; c < triangoliMagliaR.NumeroT; c++)
    {
      array<unsigned int, 3> lati = triangoliMagliaR.LatiT[c];

      for(unsigned int e = 0; e < 3; e++)  //Controllo che i dati importati siano coerenti fra di loro
      {
          const unsigned int inzioLato = latiMagliaR.VerticiL[lati[e]][0];
          const unsigned int fineLato = latiMagliaR.VerticiL[lati[e]][1];

          auto trovaInizioLato = find(triangoliMagliaR.VerticiT[c].begin(), triangoliMagliaR.VerticiT[c].end(), inzioLato);
          ASSERT_TRUE(trovaInizioLato != triangoliMagliaR.VerticiT[c].end());

          auto trovaFineLato = find(triangoliMagliaR.VerticiT[c].begin(), triangoliMagliaR.VerticiT[c].end(), fineLato);
          ASSERT_TRUE(trovaFineLato != triangoliMagliaR.VerticiT[c].end());
      }
    }
}


TEST(VerificaRaffinamento, VerificaSecondaEsportazione)
{
    MagliaTriangolare maglia, magliaRaffinata, magliaEsportata;

    bool statoImportazione = maglia.ImportaMaglia("../Raffinamento/Dataset/Test2/Cell0Ds.csv",
                                                  "../Raffinamento/Dataset/Test2/Cell1Ds.csv",
                                                  "../Raffinamento/Dataset/Test2/Cell2Ds.csv");

    ASSERT_TRUE(statoImportazione); //Controllo che l'importazione è avvenuta con successo

    unsigned int teta = 20;
    vector<unsigned int> triangoliDaRaffinare = maglia.EstraiTriangoliDaRaffinare(teta);

    magliaRaffinata = maglia.Dissezionatore(triangoliDaRaffinare);
    magliaRaffinata.CostruisciLati();
    bool statoEsportazione = magliaRaffinata.EsportaMaglia("Test2/Teste Seconda Maglia","csv");

    ASSERT_TRUE(statoEsportazione); //Controllo che l'esportazione è avvenuta con successo
}


#endif // __TESTE_RAFFINAMENTO_H
