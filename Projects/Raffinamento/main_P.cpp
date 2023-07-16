
//Variabili globali per il numero di triangoli da raffinare
unsigned int teta1 = 144; //Triangoli da raffinare nella prima maglia triangolare (con un massimo di 144)
unsigned int teta2 = 20; //Triangoli da raffinare nella prima maglia triangolare (con un massimo di 186)

#include "classe_raffinamento.hpp"

using namespace LibreriaRaffinamento;

int main()
{

  bool statoImportazione, statoEsportazione;
  vector<unsigned int> triangoliDaRaffinare;


    //Programma applicato alla prima maglia triangolare, i cui dati sono contenunti nel percorso «../Raffinamento/Dataset/Test1»
  MagliaTriangolare primaMaglia, primaMagliaRaffinata;
  statoImportazione = primaMaglia.ImportaMaglia("../Raffinamento/Dataset/Test1/Cell0Ds.csv", //Si importa la maglia
                                                "../Raffinamento/Dataset/Test1/Cell1Ds.csv",
                                                "../Raffinamento/Dataset/Test1/Cell2Ds.csv");

  if(!statoImportazione) //Si verifica che l'importazione è avvenuta con successo
      return 1;

  triangoliDaRaffinare = primaMaglia.EstraiTriangoliDaRaffinare(teta1); //Si estraggono i triangoli da raffinare

  primaMagliaRaffinata = primaMaglia.Dissezionatore(triangoliDaRaffinare); //Si raffina la maglia
  primaMagliaRaffinata.CostruisciLati(); //Si costruiscono i lati dai dati raffinati completando la maglia
  statoEsportazione = primaMagliaRaffinata.EsportaMaglia("Test1/Prima Maglia","csv"); //Si esporta la maglia in tra nuove filza

  if(!statoEsportazione) //Si verifica che l'esportazione è avvenuta con successo
      return 1;



    //Programma applicato alla seconda maglia triangolare, i cui dati sono contenunti nel percorso «../Raffinamento/Dataset/Test2»
  MagliaTriangolare secondaMaglia, secondaMagliaRaffinata; //Si ripetono gli stessi passaggi della prima maglia ma per la seconda
  statoImportazione = secondaMaglia.ImportaMaglia("../Raffinamento/Dataset/Test2/Cell0Ds.csv",
                                                  "../Raffinamento/Dataset/Test2/Cell1Ds.csv",
                                                  "../Raffinamento/Dataset/Test2/Cell2Ds.csv");

  if(!statoImportazione)
      return 1;

  triangoliDaRaffinare = secondaMaglia.EstraiTriangoliDaRaffinare(teta2);

  secondaMagliaRaffinata = secondaMaglia.Dissezionatore(triangoliDaRaffinare);
  secondaMagliaRaffinata.CostruisciLati();
  statoEsportazione = secondaMagliaRaffinata.EsportaMaglia("Test2/Seconda Maglia","csv");

  if(!statoEsportazione)
      return 1;

  cout<<"\nFinito ;)\n\n";

  return 0;
}
