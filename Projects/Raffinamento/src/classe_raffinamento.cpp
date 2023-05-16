#include "classe_raffinamento.hpp"
#include "ordinamento.hpp"

using namespace LibreriaOrdinamento;

namespace LibreriaRaffinamento
{

///
///
///

bool MagliaTriangolare::ImportaMaglia(const string& percorso0,
                   const string& percorso1,
                   const string& percorso2)
{

  if(!ImportaCell0Ds(percorso0))
      return false;
  /*else //Stampa della mappa dei marcatori
  {
      cout << "Cell0D marcatori:" << endl;
      for(unsigned int i = 0; i < Punti.MarcatoriP.size(); i++)
        cout << "id:\t" << i << "\t marcatore:" << Punti.MarcatoriP[i]<< "\n\n";
  }*/

  if(!ImportaCell1Ds(percorso1))
      return false;
  /*else //Stampa della mappa dei marcatori
  {
      cout << "Cell1D marcatori:" << endl;
      for(unsigned int i = 0; i < Lati.MarcatoriL.size(); i++)
          cout << "id: " << i << "\t marcatore: " << Lati.MarcatoriL[i]<< "\n\n";
  }*/

  if(!ImportaCell2Ds(percorso2))
      return false;

  return true;

}

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

  Punti.NumeroP = listaLinee.size();

  if (Punti.NumeroP == 0)
  {
    cerr << "Non vi sono celle nella filza «Cell0D»" << endl;
    return false;
  }

  Punti.CoordinateP.reserve(Punti.NumeroP);

  for (const string& line : listaLinee)
  {
    istringstream converter(line);

    unsigned int id, marcatore;
    Vector2d coordinate;

    //La variabile «id» viene usata per scartare l'informazione dell'identificatore già implicita nel vettore
    converter >> id >> marcatore >> coordinate(0) >> coordinate(1);

    Punti.CoordinateP.push_back(coordinate);
    Punti.MarcatoriP.push_back(marcatore);

  }

  return true;
}

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

  Lati.NumeroL = listaLinee.size();

  if (Lati.NumeroL == 0)
  {
    cerr << "Non vi sono celle nella filza «Cell1D»" << endl;
    return false;
  }

  Lati.VerticiL.reserve(Lati.NumeroL);

  for (const string& linea : listaLinee)
  {
    istringstream convertitore(linea);

    unsigned int id, marcatore;
    Vector2i vertici;

    //La variabile «id» viene usata per scartare l'informazione dell'identificatore già implicita nel vettore
    convertitore >>  id >> marcatore >> vertici(0) >> vertici(1);

    Lati.VerticiL.push_back(vertici);
    Lati.MarcatoriL.push_back(marcatore);

  }

  return true;
}

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

  Triangoli.NumeroT = listaLinee.size();

  if (Triangoli.NumeroT == 0)
  {
    cerr << "Non vi sono celle nella filza «Cell2D»" << endl;
    return false;
  }

  Triangoli.VerticiT.reserve(Triangoli.NumeroT);
  Triangoli.LatiT.reserve(Triangoli.NumeroT);

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

    Triangoli.VerticiT.push_back(vertici);
    Triangoli.LatiT.push_back(lati);

  }

  return true;
}

///
///
///

void MagliaTriangolare::AreaT(vector<double>& vettoreAree)
{

  //Faccio notare che la matrice «punti» ha dimensione 3x3 perché ogni colonna rappresenta un punto e la terza riga (2,0)-(2,1)-(2,2)
  //è nulla perché la maglia triangolare si trova nel piano x-y; inoltre l'ordinamento dei punti è veicolato dall'indice della colonna:
  //prima v'è il primo (1° colonna), poi il secondo e cosí via fino all'ultimo che si ricollega al primo punto completando il triangolo

  //Implicitamento si suppone che l'orientamento del triangolo sia antiorario e quindi dal primo punto fino al terzo ci si muove in quel senso
  //Le coordinate salvate in latoMax preserveranno anche tale orientamento presupponendo il vettore orientato dal primo al secondo punto

  Triangoli.LatiTMax.reserve(Triangoli.NumeroT);

  Vector3d lunghezze; //Vettore delle distanze tra i tre punti considerati
  MatrixXd puntiRelativi(2,3); //Vettore delle coordinate dei punti relativi

  array<unsigned int, 3> vertici; //Vettore dei tre indici che formano il triangolo
  double lato, base, altezza, area, angolo, prodScalare;

  for(unsigned int i=0; i<Triangoli.NumeroT; i++)
  {

      vertici = Triangoli.VerticiT[i]; //Vettore dei tre indici che formano il triangolo

      //Calcolo dei vettori relativi mantenendo il senso antiorario 0->1->2
      puntiRelativi.col(0) = Punti.CoordinateP[vertici[1]]-Punti.CoordinateP[vertici[0]]; //Coordinate del secondo punto relativo al primo
      puntiRelativi.col(1) = Punti.CoordinateP[vertici[2]]-Punti.CoordinateP[vertici[1]]; //Coordinate del terzo punto relativo al secondo
      puntiRelativi.col(2) = Punti.CoordinateP[vertici[0]]-Punti.CoordinateP[vertici[2]]; //Coordinate del primo punto relativo al secondo

      //Calcolo delle lunghezze dei lati
      lunghezze<< (puntiRelativi.col(0)).norm(), //Lunghezza del vettore tra il primo punto e il secondo
                  (puntiRelativi.col(1)).norm(), //Lunghezza del vettore tra il primo punto e il terzo
                  (puntiRelativi.col(2)).norm(); //Lunghezza del vettore tra il secondo punto e il terzo

      for(unsigned int i=0; i<3; i++) //Controllo che nessuna distanza sia nulla
          if(lunghezze[i]==0){cerr<<"Errore: due punti del triangolo coincidono."<<endl;}

      //Si estrae la distanza massima perché la classica formula «(base*altezza)/2» può essere calcolata solo con un altezza perperdicolare alla base e, considerando il lato maggiore,
      //tale caratteristica è sempre garantita (basta disegnare dei triangoli con un lato molto piccolo e gli altri lunghi per vedere il problema che perà faccia fatica ad esprimere)
      base = lunghezze.maxCoeff();

      //Memorizzazione della lunghezza («double lato») del vettore a fianco del vettore massimo e
      //del relativo prodotto scalare («double prodScalare») al fine di ricavare l'angolo che intercorre fra i due vettori

      //Tale struttura condizionale permette di identificare i due punti di massima distanza confrontando il vettore delle lunghezze e la base ricavata con il metodo «.maxCoeff()»
      //Dopodiché si memorizza in «lato» una delle due lunghezze al di fuori di base e in prodotto scalare il relativo prodotto scalare fra il vettore che definisce la base e quello scelto arbitrariamente
      if(base == lunghezze[0])
      {
          lato = lunghezze[2];
          prodScalare = (puntiRelativi.col(0)).dot(-puntiRelativi.col(2));
          Triangoli.LatiTMax.push_back({vertici[0], vertici[1]}); //Assegnazione degli indici dei punti del lato massimo
          Triangoli.Punte.push_back(vertici[2]); //Memorizzazione del punto opposto al lato massimo
      }
      else if(base == lunghezze[1])
      {
          lato = lunghezze[0];
          prodScalare = (puntiRelativi.col(1)).dot(-puntiRelativi.col(0));
          Triangoli.LatiTMax.push_back({vertici[1], vertici[2]}); //Assegnazione degli indici dei punti del lato massimo
          Triangoli.Punte.push_back(vertici[0]); //Memorizzazione del punto opposto al lato massimo
      }
      else
      {
          lato = lunghezze[1];
          prodScalare = (puntiRelativi.col(2)).dot(-puntiRelativi.col(1));
          Triangoli.LatiTMax.push_back({vertici[2], vertici[0]}); //Assegnazione degli indici dei punti del lato massimo
          Triangoli.Punte.push_back(vertici[1]); //Memorizzazione del punto opposto al lato massimo
      }

      //double prova = prodScalare/(lato*base);

      angolo = acos(prodScalare/(lato*base)); //Calcolo dell'angolo tramite l'arcocoseno del prodotto scalare normalizzato dalla formula v₁·v₂=‖v₁‖₂‖v₂‖₂cos(θ)
      altezza = lato*sin(angolo); //Calcolo dell'altezza tramite l'angolo interposto tra la base e il lato che in questo caso coincide con l'ipotenusa

      area = (base*altezza)/2; //Restituzione dell'area del triangolo

      vettoreAree.push_back(area);

  }

}

///
///
///

vector<unsigned int> MagliaTriangolare::EstraiTriDaRaffinare(const unsigned int& teta)
{
    vector<double> vettoreAree;
    AreaT(vettoreAree); //Costruzione del vettore delle aree

    vector<Decrescente> vettoreOrdAree = CreaVettoreDecrescente<double>(vettoreAree);
    vettoreOrdAree = HeapSort<Decrescente>(vettoreOrdAree);

    /*
      //Stampa delle aree nel vettore vettoreOrdAree
    cout<<"Aree\n"<<"Disordinata\t\tOrdinata\n\n";
    for(unsigned int i=0; i<vettoreOrdAree.size(); i++)
    {
        cout<<"Indice: "<<i<<"\tArea: "<<vettoreAree[i]<<"\t";
        cout<<"Indice: "<<i<<"\tArea: "<<vettoreOrdAree[i].valore<<"\n";
    }
    */

    vector<unsigned int> indiciDaRaff;
    indiciDaRaff.reserve(teta);

    for(unsigned int i=0; i<teta; i++)
    {
        unsigned int j = 0;
        while(vettoreOrdAree[i].valore != vettoreAree[j] || find(indiciDaRaff.begin(),indiciDaRaff.end(),j) != indiciDaRaff.end())
            j++;
        indiciDaRaff.push_back(j);
        //cout<<j<<"\t";
    }

    return indiciDaRaff;
}

///
///
///

//L'algoritmo deve modificare il triangolo corrente in due modi: calcolare il punto medio e memorizzarlo alla fine di tutti i punti; ricavare i due/tre triangoli nati
//dall'interazione con il punto medio e i punti già esistenti; successivamente l'algoritmo deve trovare il prossimo triangolo e verificare se continuare o fermarsi a partire da esso

MagliaTriangolare MagliaTriangolare::Dissezionatore(const vector<unsigned int>& indiciR)
{
    MagliaTriangolare magliaR;
    magliaR.Punti = Punti;
    magliaR.Triangoli.VerticiT.reserve(3*Triangoli.NumeroT); //Stima dei triangoli aggiunti dal raffinamento: data una coda, esclusi l'inizio, si aggiungono tre triangoli per elemento per cui si triplica in numero corrente

    //Vettore di booleani per verificare lo stato di un triangolo: è 1 se un triangolo è stato dissezzionato, 0 altrimenti
    VectorXi statoTriangolo = VectorXi::Zero(Triangoli.NumeroT);
    Vector2d puntoMedio;
    array<unsigned int, 2> indiciPunti;
    unsigned int iTCorrente, indicePunta;

    bool v;

    for(unsigned int i=0; i<indiciR.size(); i++)
    {
        if(statoTriangolo[indiciR[i]] == 0)
        {
            iTCorrente = indiciR[i];
            indiciPunti = Triangoli.LatiTMax[iTCorrente];
            indicePunta = Triangoli.Punte[iTCorrente];

            puntoMedio<<(Punti.CoordinateP[indiciPunti[0]][0]+Punti.CoordinateP[indiciPunti[1]][0])/2,
                        (Punti.CoordinateP[indiciPunti[0]][1]+Punti.CoordinateP[indiciPunti[1]][1])/2;
            magliaR.Punti.CoordinateP.push_back(puntoMedio);
            magliaR.Punti.NumeroP++;

            //Inserimento dei sottotriangoli del triangolo si partenza il quale non presenta un punto medio precedente
            magliaR.Triangoli.VerticiT.push_back({magliaR.Punti.NumeroP-1, indiciPunti[1], indicePunta});
            magliaR.Triangoli.VerticiT.push_back({magliaR.Punti.NumeroP-1, indicePunta, indiciPunti[0]});

            statoTriangolo[iTCorrente] = 1;

            do
            {
                TrovaTriangoloOpposto(iTCorrente, indiciPunti);

                if(iTCorrente == Triangoli.NumeroT) //Si esce dal ciclo while perché in questo caso il triangolo successivo non esiste dato che il lato massimo di quello precedente si affaccia al bordo della maglia triangolare
                {
                    v = 0;
                }
                else if(statoTriangolo[iTCorrente] == 1) //In questo caso il triangolo successivo al precedente è già stato dissezionato per cui esiste già un punto medio a cui ricollegarsi
                {
                    unsigned int k,h=0;

                    for(k=0; k<magliaR.Triangoli.NumeroT; k++) //Questo ciclo trova il triangolo in magliaR che contiene ambo i punti del lato massimo precedente
                        if(find(magliaR.Triangoli.VerticiT[k].begin(),magliaR.Triangoli.VerticiT[k].end(),indiciPunti[0]) != magliaR.Triangoli.VerticiT[k].end() &&
                           find(magliaR.Triangoli.VerticiT[k].begin(),magliaR.Triangoli.VerticiT[k].end(),indiciPunti[1]) != magliaR.Triangoli.VerticiT[k].end())
                            break;

                    //Il seguente ciclo while riesce a trovare l'indice del punto diverso da indiciPunti[0] e indiciPunti[1]
                    while(magliaR.Triangoli.VerticiT[k][h] == indiciPunti[0] || magliaR.Triangoli.VerticiT[k][h] == indiciPunti[1])
                        h++;

                    //In questo caso indiciPunti[0] perché nel successivo triangolo per mantenere il senso antiorario bisogna invertire il senso di indiciPunti
                    magliaR.Triangoli.VerticiT.push_back({magliaR.Punti.NumeroP-1, indiciPunti[0], magliaR.Triangoli.VerticiT[k][h]});
                    magliaR.Triangoli.VerticiT.push_back({magliaR.Punti.NumeroP-1, magliaR.Triangoli.VerticiT[k][h], indiciPunti[1]});

                    magliaR.Triangoli.VerticiT.erase(magliaR.Triangoli.VerticiT.begin()+k);

                    v = 0;
                }
                else if(Triangoli.LatiTMax[iTCorrente][0] == indiciPunti[1] && Triangoli.LatiTMax[iTCorrente][1] == indiciPunti[0]) //Si controlla che il lato massimo precedente coincida con il successivo (il senso antiorario scambia gli indici per questo si controlla il primo del nuovo con il secondo del vecchio)
                {
                    indiciPunti = Triangoli.LatiTMax[iTCorrente];
                    indicePunta = Triangoli.Punte[iTCorrente];

                    magliaR.Triangoli.VerticiT.push_back({magliaR.Punti.NumeroP-1, indiciPunti[1], indicePunta});
                    magliaR.Triangoli.VerticiT.push_back({magliaR.Punti.NumeroP-1, indicePunta, indiciPunti[0]});

                    statoTriangolo[iTCorrente] = 1;

                    v = 0;
                }
                else //In questo caso il triangolo successivo esiste (ovvero il lato massimo del triangolo precednete non si affaccia al bordo), non è stato dissezionato e non ha il lato massimo coincidente con quello del triangolo precedente; di conseguenza bisogna smembrare il triangolo in questione
                {

                    magliaR.SmembraTriangolo(magliaR.Punti.NumeroP,Triangoli.LatiTMax[iTCorrente],Triangoli.Punte[iTCorrente],indiciPunti,indicePunta);

                    indiciPunti = Triangoli.LatiTMax[iTCorrente];
                    indicePunta = Triangoli.Punte[iTCorrente];

                    puntoMedio<<(Punti.CoordinateP[indiciPunti[0]][0]+Punti.CoordinateP[indiciPunti[1]][0])/2,
                                (Punti.CoordinateP[indiciPunti[0]][1]+Punti.CoordinateP[indiciPunti[1]][1])/2;
                    magliaR.Punti.CoordinateP.push_back(puntoMedio);
                    magliaR.Punti.NumeroP++;

                    statoTriangolo[iTCorrente] = 1;

                    v = 1;
                }

            }
            while(v);

        }
    }

    return magliaR;
}

///
///
///

void MagliaTriangolare::SmembraTriangolo(const unsigned int& indicePM, //indice del Punto Medio
                                         const array<unsigned int,2>& indiciLM, //indici del Lato Massimo
                                         const unsigned int& indicePNT, //indice della PuNTa
                                         const array<unsigned int,2>& indiciLMP, //indici del Lato Massimo Precedente
                                         const unsigned int& indicePMP) //indice del Punto Medio Precedente
{

    //Scomposizione del triangoli in tre sottotriangoli a seconda della posizione del punto medio precedente (indicePMP)
    //confrontato con il secondo punto del lato massimo precedente (indiciLMP[1])

    array<unsigned int,3> triangolo;

    triangolo[0] = indicePM;
    triangolo[1] = indiciLM[1];

    if(indiciLM[1] == indiciLMP[1])
    {
        triangolo[2] = indicePMP;
        Triangoli.VerticiT.push_back(triangolo);

        triangolo[1] = indicePMP;
        triangolo[2] = indicePNT;
        Triangoli.VerticiT.push_back(triangolo);

        triangolo[1] = indicePNT;
        triangolo[2] = indiciLM[0];
        Triangoli.VerticiT.push_back(triangolo);
    }
    else
    {
        triangolo[2] = indicePNT;
        Triangoli.VerticiT.push_back(triangolo);

        triangolo[1] = indicePNT;
        triangolo[2] = indicePMP;
        Triangoli.VerticiT.push_back(triangolo);

        triangolo[1] = indicePMP;
        triangolo[2] = indiciLM[0];
        Triangoli.VerticiT.push_back(triangolo);
    }
}

///
///
///

void MagliaTriangolare::TrovaTriangoloOpposto(unsigned int& indiceT, const array<unsigned int,2>& indiciLM)
{
    //L'indice del triangolo successivo è individuato (se esistente) dal seguente ciclo «for»
    for(unsigned int j=0; j<Triangoli.NumeroT; j++)
    {
        if(find(Triangoli.VerticiT[j].begin(),Triangoli.VerticiT[j].end(),indiciLM[0]) != Triangoli.VerticiT[j].end() &&
           find(Triangoli.VerticiT[j].begin(),Triangoli.VerticiT[j].end(),indiciLM[1]) != Triangoli.VerticiT[j].end() &&
           j != indiceT) //Le condizioni vno trovano (se esiste) un triangolo che ha contemporaneamente i medesimi punti del lato massimo ma diverso indice dal triangolo corrente
        {
            indiceT = j;
            break; //Si esce dal ciclo for perché il triangolo successivo esiste
        }
        else
            indiceT = j;
    }
}

}
