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
  if(!ImportaPunti(percorso0))
      return false;

  if(!ImportaLati(percorso1))
      return false;

  if(!ImportaTriangoli(percorso2))
      return false;

  return true;
}

///
///
///

bool MagliaTriangolare::ImportaPunti(const string& percorso)
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

bool MagliaTriangolare::ImportaLati(const string& percorso)
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
    array<unsigned int, 2> vertici;

    //La variabile «id» viene usata per scartare l'informazione dell'identificatore già implicita nel vettore
    convertitore >>  id >> marcatore >> vertici[0] >> vertici[1];

    Lati.VerticiL.push_back(vertici);
    Lati.MarcatoriL.push_back(marcatore);

  }

  return true;
}

///
///
///

bool MagliaTriangolare::ImportaTriangoli(const string& percorso)
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

vector<double> MagliaTriangolare::OrdinaTriangoliPerArea()
{

  //Faccio notare che la matrice «punti» ha dimensione 3x3 perché ogni colonna rappresenta un punto e la terza riga (2,0)-(2,1)-(2,2)
  //è nulla perché la maglia triangolare si trova nel piano x-y; inoltre l'ordinamento dei punti è veicolato dall'indice della colonna:
  //prima v'è il primo (1° colonna), poi il secondo e cosí via fino all'ultimo che si ricollega al primo punto completando il triangolo

  //Implicitamento si suppone che l'orientamento del triangolo sia antiorario e quindi dal primo punto fino al terzo ci si muove in quel senso
  //Le coordinate salvate in latoMax preserveranno anche tale orientamento presupponendo il vettore orientato dal primo al secondo punto

  Triangoli.LatiTMax.reserve(Triangoli.NumeroT);

  vector<double> vettoreAree;
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
          Triangoli.MarcatoriLatiTMax.push_back(Lati.MarcatoriL[Triangoli.LatiT[i][0]]); //Memorizzazione del marcatore associato al massimo lato
          Triangoli.Punte.push_back(vertici[2]); //Memorizzazione del punto opposto al lato massimo
      }
      else if(base == lunghezze[1])
      {
          lato = lunghezze[0];
          prodScalare = (puntiRelativi.col(1)).dot(-puntiRelativi.col(0));
          Triangoli.LatiTMax.push_back({vertici[1], vertici[2]}); //Assegnazione degli indici dei punti del lato massimo
          Triangoli.MarcatoriLatiTMax.push_back(Lati.MarcatoriL[Triangoli.LatiT[i][1]]); //Memorizzazione del marcatore associato al massimo lato
          Triangoli.Punte.push_back(vertici[0]); //Memorizzazione del punto opposto al lato massimo
      }
      else
      {
          lato = lunghezze[1];
          prodScalare = (puntiRelativi.col(2)).dot(-puntiRelativi.col(1));
          Triangoli.LatiTMax.push_back({vertici[2], vertici[0]}); //Assegnazione degli indici dei punti del lato massimo
          Triangoli.MarcatoriLatiTMax.push_back(Lati.MarcatoriL[Triangoli.LatiT[i][2]]); //Memorizzazione del marcatore associato al massimo lato
          Triangoli.Punte.push_back(vertici[1]); //Memorizzazione del punto opposto al lato massimo
      }

      //double prova = prodScalare/(lato*base);

      angolo = acos(prodScalare/(lato*base)); //Calcolo dell'angolo tramite l'arcocoseno del prodotto scalare normalizzato dalla formula v₁·v₂=‖v₁‖₂‖v₂‖₂cos(θ)
      altezza = lato*sin(angolo); //Calcolo dell'altezza tramite l'angolo interposto tra la base e il lato che in questo caso coincide con l'ipotenusa

      area = (base*altezza)/2; //Restituzione dell'area del triangolo

      vettoreAree.push_back(area);

  }

  return vettoreAree;

}

///
///
///

vector<unsigned int> MagliaTriangolare::EstraiTriangoliDaRaffinare()
{

    vector<double> vettoreAree = OrdinaTriangoliPerArea(); //Costruzione del vettore delle aree

    vector<Decrescente> vettoreOrdAree = CreaVettoreDecrescente<double>(vettoreAree);
    vettoreOrdAree = HeapSort<Decrescente>(vettoreOrdAree);

    /*  //Stampa delle aree nel vettore vettoreOrdAree
    cout<<"Aree\n"<<"Disordinata\t\tOrdinata\n\n";
    for(unsigned int i=0; i<vettoreOrdAree.size(); i++)
    {
        cout<<"Indice: "<<i<<"\tArea: "<<vettoreAree[i]<<"\t";
        cout<<"Indice: "<<i<<"\tArea: "<<vettoreOrdAree[i].valore<<"\n";
    }*/

    unsigned int teta;

      //Codice per ricevere in ingresso il valore di teta dall'utente una volta misurato il numero di triangoli
//      cout<<"Totale: "<< vettoreOrdAree.size()<<" triangoli\n"
//          <<"Inserire come numero intero il numero di triangoli da raffinare a partire da quelli con area maggiore: ";
//      cin >> teta;

    //Per qualche motivazione QtCreator non mi permette d'inserire un numero se non avvio il programma in modalità terminale
    //(Barra a sinistra>«Projects»>«Run»> sotto «Build & Run»>seleziona «Run in terminal»); per ora lo faccio manulamente.

    teta = 20;

    vector<unsigned int> indiciDaRaffinare;
    indiciDaRaffinare.reserve(teta);

    for(unsigned int i=0; i<teta; i++)
    {
        unsigned int j = 0;
        while(vettoreOrdAree[i].valore != vettoreAree[j] || find(indiciDaRaffinare.begin(),indiciDaRaffinare.end(),j) != indiciDaRaffinare.end())
            j++;
        indiciDaRaffinare.push_back(j);
    }

    return indiciDaRaffinare;
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
            magliaR.Punti.MarcatoriP.push_back(Triangoli.MarcatoriLatiTMax[iTCorrente]);
            magliaR.Punti.NumeroP++;

            //Inserimento dei sottotriangoli del triangolo si partenza il quale non presenta un punto medio precedente
            magliaR.Triangoli.VerticiT.push_back({magliaR.Punti.NumeroP-1, indiciPunti[1], indicePunta});
            magliaR.Triangoli.VerticiT.push_back({magliaR.Punti.NumeroP-1, indicePunta, indiciPunti[0]});

            magliaR.Triangoli.NumeroT+=2;
            statoTriangolo[iTCorrente] = 1;

            do
            {
                iTCorrente = TrovaTriangoloOpposto(iTCorrente, indiciPunti);

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

                    magliaR.Triangoli.NumeroT++;
                    magliaR.Triangoli.VerticiT.erase(magliaR.Triangoli.VerticiT.begin()+k);

                    v = 0;
                }
                else if(Triangoli.LatiTMax[iTCorrente][0] == indiciPunti[1] && Triangoli.LatiTMax[iTCorrente][1] == indiciPunti[0]) //Si controlla che il lato massimo precedente coincida con il successivo (il senso antiorario scambia gli indici per questo si controlla il primo del nuovo con il secondo del vecchio)
                {
                    indiciPunti = Triangoli.LatiTMax[iTCorrente];
                    indicePunta = Triangoli.Punte[iTCorrente];

                    magliaR.Triangoli.VerticiT.push_back({magliaR.Punti.NumeroP-1, indiciPunti[1], indicePunta});
                    magliaR.Triangoli.VerticiT.push_back({magliaR.Punti.NumeroP-1, indicePunta, indiciPunti[0]});

                    magliaR.Triangoli.NumeroT+=2;
                    statoTriangolo[iTCorrente] = 1;

                    v = 0;
                }
                else //In questo caso il triangolo successivo esiste (ovvero il lato massimo del triangolo precednete non si affaccia al bordo), non è stato dissezionato e non ha il lato massimo coincidente con quello del triangolo precedente; di conseguenza bisogna smembrare il triangolo in questione
                {

                    indicePunta = Triangoli.Punte[iTCorrente];
                    magliaR.SmembraTriangolo(magliaR.Punti.NumeroP,Triangoli.LatiTMax[iTCorrente],indicePunta,indiciPunti,magliaR.Punti.NumeroP-1);
                    indiciPunti = Triangoli.LatiTMax[iTCorrente];

                    puntoMedio<<(Punti.CoordinateP[indiciPunti[0]][0]+Punti.CoordinateP[indiciPunti[1]][0])/2,
                                (Punti.CoordinateP[indiciPunti[0]][1]+Punti.CoordinateP[indiciPunti[1]][1])/2;
                    magliaR.Punti.CoordinateP.push_back(puntoMedio);
                    magliaR.Punti.MarcatoriP.push_back(Triangoli.MarcatoriLatiTMax[iTCorrente]);
                    magliaR.Punti.NumeroP++;

                    statoTriangolo[iTCorrente] = 1;

                    v = 1;
                }

            }
            while(v);

        }
    }


    //Aggiunta dei triangoli non dissezionati
    for(unsigned int i=0; i<Triangoli.NumeroT; i++)
        if(statoTriangolo[i] == 0)
        {
            magliaR.Triangoli.VerticiT.push_back(Triangoli.VerticiT[i]);
            magliaR.Triangoli.NumeroT++;
        }


    /*//Stampa delle coordinate dei punti originali e raffinati
    cout<<"\tPunti originali\n";
    for(unsigned int i=0; i<Punti.NumeroP; i++)
        cout<<"("<<Punti.CoordinateP[i][0]<<","<<Punti.CoordinateP[i][1]<<"(\n";
    cout<<"\n\tPunti raffinati\n";
    for(unsigned int i=0; i<magliaR.Punti.NumeroP; i++)
        cout<<"("<<magliaR.Punti.CoordinateP[i][0]<<","<<magliaR.Punti.CoordinateP[i][1]<<")\n";*/


    /*//Stampa delle coordinte di ciascun triangolo della maglia triangolare in un formato rappresentabile per geogebra
    cout<<"\tMaglia originali\n\n";
    for(unsigned int i=0; i<Triangoli.NumeroT; i++)
    {
        cout<<"Poligono(";
        for(unsigned int j=0; j<Triangoli.VerticiT[i].size(); j++)
            cout<<"("<<Punti.CoordinateP[Triangoli.VerticiT[i][j]][0]<<","<<Punti.CoordinateP[Triangoli.VerticiT[i][j]][1]<<"),";
        cout<<"("<<Punti.CoordinateP[Triangoli.VerticiT[i][0]][0]<<","<<Punti.CoordinateP[Triangoli.VerticiT[i][0]][1]<<"))\n";
    }
    cout<<"\n\n\n\tMaglia raffinata\n\n";
    for(unsigned int i=0; i<magliaR.Triangoli.NumeroT; i++)
    {
        cout<<"Poligono(";
        for(unsigned int j=0; j<magliaR.Triangoli.VerticiT[i].size(); j++)
            cout<<"("<<magliaR.Punti.CoordinateP[magliaR.Triangoli.VerticiT[i][j]][0]<<","<<magliaR.Punti.CoordinateP[magliaR.Triangoli.VerticiT[i][j]][1]<<"),";
        cout<<"("<<magliaR.Punti.CoordinateP[magliaR.Triangoli.VerticiT[i][0]][0]<<","<<magliaR.Punti.CoordinateP[magliaR.Triangoli.VerticiT[i][0]][1]<<"))\n";
    }*/


    /*//Stampa per la visualizzazione su Geogebra
    for(unsigned int i=Punti.NumeroP; i<magliaR.Punti.NumeroP; i++)
        cout<<"A"<<i<<"=("<<magliaR.Punti.CoordinateP[i][0]<<","<<magliaR.Punti.CoordinateP[i][1]<<")\n"
            <<"ImpColore(A"<<i<<",rosso)\n";
    for(unsigned int i=0; i<magliaR.Triangoli.NumeroT; i++)
    {
        cout<<"Poligono(";
        for(unsigned int j=0; j<magliaR.Triangoli.VerticiT[i].size(); j++)
            cout<<"("<<magliaR.Punti.CoordinateP[magliaR.Triangoli.VerticiT[i][j]][0]<<","<<magliaR.Punti.CoordinateP[magliaR.Triangoli.VerticiT[i][j]][1]<<"),";
        cout<<"("<<magliaR.Punti.CoordinateP[magliaR.Triangoli.VerticiT[i][0]][0]<<","<<magliaR.Punti.CoordinateP[magliaR.Triangoli.VerticiT[i][0]][1]<<"))\n";
    }*/

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

        Triangoli.NumeroT+=3;
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
        Triangoli.NumeroT+=3;
    }
}

///
///
///

unsigned int MagliaTriangolare::TrovaTriangoloOpposto(const unsigned int& indiceT, const array<unsigned int,2>& indiciLM)
{
    //L'indice del triangolo successivo è individuato (se esistente) dal seguente ciclo «for»
    unsigned int j;
    for(j=0; j<Triangoli.NumeroT; j++)
    {
        if(find(Triangoli.VerticiT[j].begin(),Triangoli.VerticiT[j].end(),indiciLM[0]) != Triangoli.VerticiT[j].end() &&
           find(Triangoli.VerticiT[j].begin(),Triangoli.VerticiT[j].end(),indiciLM[1]) != Triangoli.VerticiT[j].end() &&
           j != indiceT) //Le condizioni trovano (se esiste) un triangolo che ha contemporaneamente i medesimi punti del lato massimo ma diverso indice dal triangolo corrente
            break;
    }
    return j;
}

///
///
///

void MagliaTriangolare::CostruisciLati()
{

    Lati.VerticiL.reserve(3*Triangoli.NumeroT); //Stima massima dei lati; non vi saranno mai
    Triangoli.LatiT.resize(Triangoli.NumeroT);

    array<unsigned int,2> lato;
    array<unsigned int,3> triangolo;

    //Un lato sarà condiviso da un solo altro triangolo da quello considerato e se per uno è in senso antiorario per l'altro è in senso orario; in altre parole il lato (n_1,n_2)
    //orientato in senso antiorario in un triangolo è nell'altro orientato in senso antiorario e quindi in quest'ultimo è salvato come (n_2,n_1) per mantenere il senso antiorario.
    //Pertanto è sufficiente controllare che l'inverso di un lato sia presente nel vettore dei lati

    for(unsigned int i=0; i<Triangoli.NumeroT; i++) //Per ogni triangolo
    {
        triangolo = Triangoli.VerticiT[i];

        for(unsigned int j=0; j<3; j++)
        {
            lato = {triangolo[(j==2? 0 :j+1)], triangolo[j]};
            unsigned int k = 0;

            for(k=0; k<Lati.VerticiL.size(); k++)
                if(lato==Lati.VerticiL[k])
                    break;

            if(k == Lati.VerticiL.size())
            {
                Lati.VerticiL.push_back({lato[1],lato[0]}); //Si inserisce il lato invertito
                Lati.NumeroL++;
                Triangoli.LatiT[i][j] = Lati.NumeroL-1;

                //I seguenti «if» successivi vengono impiegati per individuare il marcatore del lato
                if(Punti.MarcatoriP[lato[0]] == 0 || Punti.MarcatoriP[lato[1]] == 0)
                    Lati.MarcatoriL.push_back(0);
                else if(Punti.MarcatoriP[lato[0]] != Punti.MarcatoriP[lato[1]])
                    Lati.MarcatoriL.push_back(0);
                else if(Punti.MarcatoriP[lato[0]] != 1 &&
                        Punti.MarcatoriP[lato[0]] != 2 &&
                        Punti.MarcatoriP[lato[0]] != 3 &&
                        Punti.MarcatoriP[lato[0]] != 4)
                    Lati.MarcatoriL.push_back(Punti.MarcatoriP[lato[0]]);
                else
                    Lati.MarcatoriL.push_back(Punti.MarcatoriP[lato[1]]);
            }
            else
            {
                Triangoli.LatiT[i][j] = k;
            }
        }
    }
}

///
///
///

bool MagliaTriangolare::EsportaMaglia(const string& nomeMaglia, const string& formato)
{

    string nomeFilza;
    ofstream filza;


    //Esportazione punti
    nomeFilza = "./"+nomeMaglia+" 01 Punti."+formato;
    filza.open(nomeFilza);

    if (filza.fail())
        return false;

    filza<<"Id Marcatore X Y"<<endl;

    for(unsigned int i=0; i<Punti.NumeroP; i++)
        filza<<i<<" "
             <<Punti.MarcatoriP[i]<<" "
             <<Punti.CoordinateP[i][0]<<" "
             <<Punti.CoordinateP[i][1]<<endl;

    filza.close();


    //Esportazione lati
    nomeFilza = "./"+nomeMaglia+" 02 Lati."+formato;
    filza.open(nomeFilza);

    if (filza.fail())
        return false;

    filza<<"Id Marcatore Origine Fine"<<endl;

    for(unsigned int i=0; i<Lati.NumeroL; i++)
        filza<<i<<" "
             <<Lati.MarcatoriL[i]<<" "
             <<Lati.VerticiL[i][0]<<" "
             <<Lati.VerticiL[i][1]<<endl;

    filza.close();


    //Esportazione triangoli
    nomeFilza = "./"+nomeMaglia+" 03 Triangoli."+formato;
    filza.open(nomeFilza);

    if (filza.fail())
        return false;

    filza<<"Id Vertici Lati"<<endl;

    for(unsigned int i=0; i<Triangoli.NumeroT; i++)
        filza<<i<<" "
             <<Triangoli.VerticiT[i][0]<<" "
             <<Triangoli.VerticiT[i][1]<<" "
             <<Triangoli.VerticiT[i][2]<<" "
             <<Triangoli.LatiT[i][0]<<" "
             <<Triangoli.LatiT[i][1]<<" "
             <<Triangoli.LatiT[i][2]<<endl;

    filza.close();

    return true;

}

}
