#include "classe_raffinamento.hpp"

namespace LibreriaRaffinamento
{

///
///
///
///
///

bool MagliaTriangolare::ImportaMaglia(const string& percorso0,
                   const string& percorso1,
                   const string& percorso2)
{

  if(!ImportaCell0Ds(percorso0))
      return false;
  else //Stampa della mappa dei marcatori
  {
      cout << "Cell0D marcatori:" << endl;
      for(unsigned int i = 0; i < Punti.Cell0DMarcatori.size(); i++)
        cout << "id:\t" << i << "\t marcatore:" << Punti.Cell0DMarcatori[i]<< "\n\n";
  }

  if(!ImportaCell1Ds(percorso1))
      return false;
  else //Stampa della mappa dei marcatori
  {
      cout << "Cell1D marcatori:" << endl;
      for(unsigned int i = 0; i < Lati.Cell1DMarcatori.size(); i++)
          cout << "id: " << i << "\t marcatore: " << Lati.Cell1DMarcatori[i]<< "\n\n";
  }

  if(!ImportaCell2Ds(percorso2))
      return false;

  return true;

}

///
///
///
///
///

bool MagliaTriangolare::ImportaCell0Ds(const string& percorso)
{


  ifstream filza;
  filza.open(percorso);

  if(filza.fail())
    return false;

  list<string> listaLinee;
  string linea;
  while (getline(filza, linea))
    listaLinee.push_back(linea);

  filza.close();

  listaLinee.pop_front();

  Punti.NumeroCell0D = listaLinee.size();

  if (Punti.NumeroCell0D == 0)
  {
    cerr << "Non vi sono celle nella filza «Cell0D»" << endl;
    return false;
  }

  Punti.Cell0DCoordinate.reserve(Punti.NumeroCell0D);

  for (const string& line : listaLinee)
  {
    istringstream converter(line);

    unsigned int id, marcatore;
    Vector2d coordinate;

    //La variabile «id» viene usata per scartare l'informazione dell'identificatore già implicita nel vettore
    converter >> id >> marcatore >> coordinate(0) >> coordinate(1);

    Punti.Cell0DCoordinate.push_back(coordinate);
    Punti.Cell0DMarcatori.push_back(marcatore);

  }

  return true;
}

///
///
///
///
///

bool MagliaTriangolare::ImportaCell1Ds(const string& percorso)
{

  ifstream filza;
  filza.open(percorso);

  if(filza.fail())
    return false;

  list<string> listaLinee;
  string linea;
  while (getline(filza, linea))
    listaLinee.push_back(linea);

  filza.close();

  listaLinee.pop_front();

  Lati.NumeroCell1D = listaLinee.size();

  if (Lati.NumeroCell1D == 0)
  {
    cerr << "Non vi sono celle nella filza «Cell1D»" << endl;
    return false;
  }

  Lati.Cell1DVertici.reserve(Lati.NumeroCell1D);

  for (const string& linea : listaLinee)
  {
    istringstream convertitore(linea);

    unsigned int id, marcatore;
    Vector2i vertici;

    //La variabile «id» viene usata per scartare l'informazione dell'identificatore già implicita nel vettore
    convertitore >>  id >> marcatore >> vertici(0) >> vertici(1);

    Lati.Cell1DVertici.push_back(vertici);
    Lati.Cell1DMarcatori.push_back(marcatore);

  }

  return true;
}

///
///
///
///
///

bool MagliaTriangolare::ImportaCell2Ds(const string& percorso)
{

  ifstream filza;
  filza.open(percorso);

  if(filza.fail())
    return false;

  list<string> listaLinee;
  string linea;
  while (getline(filza, linea))
    listaLinee.push_back(linea);

  filza.close();

  listaLinee.pop_front();

  Triangoli.NumeroCell2D = listaLinee.size();

  if (Triangoli.NumeroCell2D == 0)
  {
    cerr << "Non vi sono celle nella filza «Cell2D»" << endl;
    return false;
  }

  Triangoli.Cell2DVertici.reserve(Triangoli.NumeroCell2D);
  Triangoli.Cell2DLati.reserve(Triangoli.NumeroCell2D);

  for (const string& linea : listaLinee)
  {
    istringstream converter(linea);

    unsigned int id;
    array<unsigned int, 3> vertici;
    array<unsigned int, 3> lati;

    converter >>  id;
    for(unsigned int i = 0; i < 3; i++)
      converter >> vertici[i];
    for(unsigned int i = 0; i < 3; i++)
      converter >> lati[i];

    Triangoli.Cell2DVertici.push_back(vertici);
    Triangoli.Cell2DLati.push_back(lati);

  }

  return true;
}

///
///
///
///
///

void AreaT(const Matrix3d& vertici, double& area, Vector3d& latoMax)
{

  //Faccio notare che la matrice «punti» ha dimensione 3x3 perché ogni colonna rappresenta un punto e la terza riga (2,0)-(2,1)-(2,2)
  //è nulla perché la maglia triangolare si trova nel piano x-y; inoltre l'ordinamento dei punti è veicolato dall'indice della colonna:
  //prima v'è il primo (1° colonna), poi il secondo e cosí via fino all'ultimo che si ricollega al primo punto completando il triangolo

  //Implicitamento si suppone che l'orientamento del triangolo sia antiorario e quindi dal primo punto fino al terzo ci si muove in quel senso
  //Le coordinate salvate in latoMax preserveranno anche tale orientamento presupponendo il vettore orientato dal primo al secondo punto

  Vector3d distanze = Vector3d::Zero(); //Vettore memorizzante le distanze tra i tre punti considerati

  //Arbitrariamento di prende come riferimento il primo punto (punti.col(0)) calcolando le rispetto a esso

  distanze<< (vertici.col(1)-vertici.col(0)).norm(), //Distanza primo punto (punti.col(0)) con i+1-esimo (punti.col(i+1))
             (vertici.col(2)-vertici.col(0)).norm(), //Distanza primo punto (punti.col(0)) con i+2-esimo (punti.col(i+2))
             (vertici.col(2)-vertici.col(1)).norm(); //Distanza i+1-esimo (punti.col(i+1)) con i+2-esimo (punti.col(i+2))

  for(unsigned int i=0; i<3; i++) //Controllo che nessuna distanza sia nulla
      if(distanze[i]==0){cerr<<"Errore: due punti del triangolo coincidono"<<endl;}

  //Si estrae la distanza massima perché la classica formula «(base*altezza)/2» può essere calcolata solo con un altezza perperdicolare alla base e, considerando il lato maggiore,
  //tale caratteristica è sempre garantita (basta disegnare dei triangoli con un lato molto piccolo e gli altri lunghi per vedere il problema che perà faccia fatica ad esprimere)
  double base = distanze.maxCoeff();

  //Memorizzazione della lunghezza («double lato») del vettore a fianco del vettore massimo e
  //del relativo prodotto scalare («double prodScalare») al fine di ricavare l'angolo che intercorre fra i due vettori
  double prodScalare = 0, lato = 0;

  //Tale struttura condizionale permette di identificare i due punti di massima distanza confrontando il vettore delle distanze e la base ricavata con il metodo «.maxCoeff()»
  //Dopodiché si memorizza in «lato» una delle due distanze al di fuori di base e in prodotto scalare il relativo prodotto scalare fra il vettore che definisce la base e quello scelto arbitrariamente
  if(base == distanze[0])
  {
      lato = distanze[1];
      prodScalare = (vertici.col(1)-vertici.col(0)).dot(vertici.col(2)-vertici.col(0));

      //Assegnazione delle coordinate dei punti descriventi in lato massimo
      latoMax.col(0) = vertici.col(0);
      latoMax.col(1) = vertici.col(1);
  }
  else if(base == distanze[1])
  {
      lato = distanze[2];
      prodScalare = (vertici.col(0)-vertici.col(2)).dot(vertici.col(1)-vertici.col(2));

      //Assegnazione delle coordinate dei punti descriventi in lato massimo
      latoMax.col(0) = vertici.col(2);
      latoMax.col(1) = vertici.col(0);
  }
  else
  {
      lato = distanze[0];
      prodScalare = (vertici.col(2)-vertici.col(0)).dot(vertici.col(1)-vertici.col(0));

      //Assegnazione delle coordinate dei punti descriventi in lato massimo
      latoMax.col(0) = vertici.col(1);
      latoMax.col(1) = vertici.col(2);
  }

  double angolo = acos(prodScalare/(lato*base)); //Calcolo dell'angolo tramite l'arcocoseno del prodotto scalare normalizzato dalla formula v₁·v₂=‖v₁‖₂‖v₂‖₂cos(θ)
  double altezza = lato*sin(angolo); //Calcolo dell'altezza tramite l'angolo interposto tra la base e il lato che in questo caso coincide con l'ipotenusa

  area = (base*altezza)/2; //Restituzione dell'area del triangolo
}

}
