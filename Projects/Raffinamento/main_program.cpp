#include "classe_raffinamento.hpp"

using namespace LibreriaRaffinamento;

int main()
{

  MagliaTriangolare maglia;

  bool verifica = maglia.ImportaMaglia("../Raffinamento/Dataset/Test1/Cell0Ds.csv",
                                       "../Raffinamento/Dataset/Test1/Cell1Ds.csv",
                                       "../Raffinamento/Dataset/Test1/Cell2Ds.csv");

  if(!verifica)
      return 1;

  //Modifica «AreaT» cosicché sia una funzione della classe «MagliaTriangolare»
  //e restituisca il vettore delle aree in corrispondenza dell'indice del triangolo

  return 0;
}
