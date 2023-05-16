#include "classe_raffinamento.hpp"

using namespace LibreriaRaffinamento;

int main()
{

  MagliaTriangolare maglia, magliaRaffinata;
  bool verifica = maglia.ImportaMaglia("../Raffinamento/Dataset/Test1/Cell0Ds.csv",
                                       "../Raffinamento/Dataset/Test1/Cell1Ds.csv",
                                       "../Raffinamento/Dataset/Test1/Cell2Ds.csv");

  if(!verifica)
      return 1;

  //Per qualche motivazione QtCreator non mi permette di inserire un numero se non avvio il programma in modalità terminale
  //(Barra a sinistra>«Projects»>«Run»> sotto «Build & Run»>seleziona «Run in terminal»); per il momento lo inserisco manulamente
  unsigned int teta = 10;
  /*
    //Codice per ricevere in ingresso il valore di teta dall'utente una volta misurato il numero di triangoli
  cout<<"Totale: "<< vettoreOrdAree.size()<<" triangoli\n"
      <<"Inserire come numero intero il numero di triangoli da raffinare a partire da quelli con area maggiore: ";
  cin >> teta;
  cout<<"Numero inserito: "<<teta<<"\n";
  */

  vector<unsigned int> triangoliDaRaffinare = maglia.EstraiTriDaRaffinare(teta);

  /*
    //Stampa degli indici dei triangoli da raffinare
  for(unsigned int i=0; i<triangoliDaRaffinare.size(); i++)
      cout<<triangoliDaRaffinare[i]<<"\t";
  cout<<"\n";
  */

  magliaRaffinata = maglia.Dissezionatore(triangoliDaRaffinare);

  //Manca solo la funzione di esportazione per ricreare le tre filze Excel che descrivono la maglia triangolare

  return 0;
}
