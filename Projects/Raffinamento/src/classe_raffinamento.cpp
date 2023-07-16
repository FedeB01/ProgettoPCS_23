
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
  if(!ImportaPunti(percorso0)) //Verifica che l'importazione delle informazioni dei punti sia avvenuta con successo
      return false;

  if(!ImportaLati(percorso1)) //Verifica che l'importazione delle informazioni dei lati sia avvenuta con successo
      return false;

  if(!ImportaTriangoli(percorso2)) //Verifica che l'importazione delle informazioni dei triangoli sia avvenuta con successo
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

  if(filza.fail()) //Verifica che l'apertura della filza sia avvenuta con successo
  {
    cerr << "Errore nell'apertura della filza «Cell0D»" << endl;
    return false;
  }

  list<string> listaLinee;
  string linea;
  while (getline(filza, linea)) //Trasferimento dei dati nella filza in un vettore di stringhe ognuna rappresentante un'intera riga di testo
    listaLinee.push_back(linea);

  filza.close();

  listaLinee.pop_front(); //Eliminazione il primo elemento perché contenente metadati e non dati

  Punti.NumeroP = listaLinee.size();

  if (Punti.NumeroP == 0) //Misura del numero di elementi rilevati
  {
    cerr << "Non vi sono celle nella filza «Cell0D»" << endl;
    return false;
  }

  Punti.CoordinateP.reserve(Punti.NumeroP);

  for (const string& line : listaLinee) //Separazione delle informazioni contenunte in ogni linea estratta nei vari membri della struttura «Punto»
  {
    istringstream converter(line);

    unsigned int id, marcatore;
    Vector2d coordinate;

    //La variabile «id» viene usata per scartare l'informazione dell'identificatore già implicita negl'indici del vettore
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

  if(filza.fail()) //Verifica che l'apertura della filza sia avvenuta con successo
  {
    cerr << "Errore nell'apertura della filza «Cell1D»" << endl;
    return false;
  }

  list<string> listaLinee;
  string linea;
  while (getline(filza, linea)) //Trasferimento dei dati nella filza in un vettore di stringhe ognuna rappresentante un'intera riga di testo
    listaLinee.push_back(linea);

  filza.close();

  listaLinee.pop_front(); //Eliminazione il primo elemento perché contenente metadati e non dati

  Lati.NumeroL = listaLinee.size();

  if (Lati.NumeroL == 0) //Misura del numero di elementi rilevati
  {
    cerr << "Non vi sono celle nella filza «Cell1D»" << endl;
    return false;
  }

  Lati.VerticiL.reserve(Lati.NumeroL);

  for (const string& linea : listaLinee) //Separazione delle informazioni contenunte in ogni linea estratta nei vari membri della struttura «Lati»
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

  if(filza.fail()) //Verifica che l'apertura della filza sia avvenuta con successo
  {
    cerr << "Errore nell'apertura della filza «Cell2D»" << endl;
    return false;
  }

  list<string> listaLinee;
  string linea;
  while (getline(filza, linea)) //Trasferimento dei dati nella filza in un vettore di stringhe ognuna rappresentante un'intera riga di testo
    listaLinee.push_back(linea);

  filza.close();

  listaLinee.pop_front(); //Eliminazione il primo elemento perché contenente metadati e non dati

  Triangoli.NumeroT = listaLinee.size();

  if (Triangoli.NumeroT == 0) //Misura del numero di elementi rilevati
  {
    cerr << "Non vi sono celle nella filza «Cell2D»" << endl;
    return false;
  }

  Triangoli.VerticiT.reserve(Triangoli.NumeroT);
  Triangoli.LatiT.reserve(Triangoli.NumeroT);

  for (const string& linea : listaLinee) //Separazione delle informazioni contenunte in ogni linea estratta nei vari membri della struttura «Triangoli»
  {
    istringstream converter(linea);

    unsigned int id;
    array<unsigned int, 3> vertici;
    array<unsigned int, 3> lati;

    //La variabile «id» viene usata per scartare l'informazione dell'identificatore già implicita negl'indici del vettore
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

vector<double> MagliaTriangolare::CalcolaAreeTriangoli()
{

  //Visto che implicitamento si suppone che l'orientamento del triangolo sia antiorario (ovvero dal primo punto fino al terzo ci si muove in tal senso)
  //Le coordinate salvate in latoMax preserveranno anche tale orientamento presupponendo il vettore orientato dal primo al secondo punto

  Triangoli.LatiTMax.reserve(Triangoli.NumeroT); //Il numero di lati massimi coincide con il numero di triangoli perché ciascuno di essi ha un lato massimo [eventualmente ripetuto]

  vector<double> vettoreAree;
  vettoreAree.reserve(Triangoli.NumeroT);

  Vector3d lunghezze; //Vettore delle distanze tra i tre punti considerati
  MatrixXd puntiRelativi(2,3); //Vettore delle coordinate dei punti relativi

  array<unsigned int, 3> vertici; //Vettore dei tre indici che formano il triangolo
  double base, area;

  for(unsigned int i=0; i<Triangoli.NumeroT; i++)
  {

      vertici = Triangoli.VerticiT[i]; //Vettore dei tre indici che formano il triangolo

      //Calcolo dei vettori relativi mantenendo il senso antiorario 0->1->2
      for(unsigned int j=0; j<3; j++)
      {
          //Memorizzazione delle coordinate dei vettori relativi: dal primo punto al secondo, dal secondo al terzo e infine dal terzo al primo
          puntiRelativi.col(j) = Punti.CoordinateP[vertici[(j==2)?0:j+1]]-Punti.CoordinateP[vertici[j]];
          lunghezze(j) = puntiRelativi.col(j).norm(); //Calcolo della lunghezze dei lati lati relativi
          if(lunghezze[j]==0) //Controllo che nessuna distanza sia nulla, ovvero che due punti non coincidano
              cerr<<"Errore: due punti del triangolo coincidono"<<endl;
      }

      /*
        //Precedente versione del piú agile e immediato ciclo «for» per la costruzione degli oggetti «puntiRelativi» e «lunghezze»

      puntiRelativi.col(0) = Punti.CoordinateP[vertici[1]]-Punti.CoordinateP[vertici[0]]; //Coordinate del secondo punto relativo al primo
      puntiRelativi.col(1) = Punti.CoordinateP[vertici[2]]-Punti.CoordinateP[vertici[1]]; //Coordinate del terzo punto relativo al secondo
      puntiRelativi.col(2) = Punti.CoordinateP[vertici[0]]-Punti.CoordinateP[vertici[2]]; //Coordinate del primo punto relativo al secondo

      //Calcolo delle lunghezze dei lati
      lunghezze<< (puntiRelativi.col(0)).norm(), //Lunghezza del vettore tra il primo punto e il secondo
                  (puntiRelativi.col(1)).norm(), //Lunghezza del vettore tra il primo punto e il terzo
                  (puntiRelativi.col(2)).norm(); //Lunghezza del vettore tra il secondo punto e il terzo


      for(unsigned int i=0; i<3; i++) //Controllo che nessuna distanza sia nulla
          if(lunghezze[i]==0){cerr<<"Errore: due punti del triangolo coincidono."<<endl;}
      */

      //Si estrae la distanza massima perché la classica formula «(base*altezza)/2» può essere calcolata solo con un altezza perperdicolare alla base e, considerando il lato maggiore,
      //tale caratteristica è sempre garantita (basta disegnare dei triangoli con un lato molto piccolo e gli altri lunghi per vedere il problema che però faccio fatica ad esprimere)
      base = lunghezze.maxCoeff();

      //Tale struttura condizionale permette di identificare i due punti di massima distanza confrontando il vettore delle lunghezze e la base ricavata con il metodo «.maxCoeff()».
      if(base == lunghezze[0])
      {
          //Si calcola l'area tramite il modulo del prodotto vettoriale tra il vettore piú lungo e quello successivo (cosí da garantirne la positività);
          //inoltre in questo caso il prodotto vettoriale si riduce alla sola terza componente poiché i vettori giacciono su un piano orizzontale
          area = 0.5*(puntiRelativi.col(0)[0]*puntiRelativi.col(1)[1]-puntiRelativi.col(1)[0]*puntiRelativi.col(0)[1]);
          vettoreAree.push_back(area);

          Triangoli.LatiTMax.push_back({vertici[0], vertici[1]}); //Indici dei punti del lato massimo
          Triangoli.MarcatoriLatiTMax.push_back(Lati.MarcatoriL[Triangoli.LatiT[i][0]]); //Marcatore associato al massimo lato
          Triangoli.Punte.push_back(vertici[2]); //Punto opposto al lato massimo
      }
      else if(base == lunghezze[1])
      {
          //Come nel primo caso ma riadattato nel caso il secondo lato sia quello piú lungo

          area = 0.5*(puntiRelativi.col(1)[0]*puntiRelativi.col(2)[1]-puntiRelativi.col(2)[0]*puntiRelativi.col(1)[1]);
          vettoreAree.push_back(area);

          Triangoli.LatiTMax.push_back({vertici[1], vertici[2]});
          Triangoli.MarcatoriLatiTMax.push_back(Lati.MarcatoriL[Triangoli.LatiT[i][1]]);
          Triangoli.Punte.push_back(vertici[0]);
      }
      else
      {
          //Come nel primo caso ma riadattato nel caso il terzo lato sia quello piú lungo

          area = 0.5*(puntiRelativi.col(2)[0]*puntiRelativi.col(0)[1]-puntiRelativi.col(0)[0]*puntiRelativi.col(2)[1]);
          vettoreAree.push_back(area);

          Triangoli.LatiTMax.push_back({vertici[2], vertici[0]});
          Triangoli.MarcatoriLatiTMax.push_back(Lati.MarcatoriL[Triangoli.LatiT[i][2]]);
          Triangoli.Punte.push_back(vertici[1]);
      }

  }

  return vettoreAree;

}

///
///
///

vector<unsigned int> MagliaTriangolare::EstraiTriangoliDaRaffinare(const unsigned int& teta)
{

    vector<double> vettoreAree = CalcolaAreeTriangoli(); //Costruzione del vettore delle aree
    vector<Decrescente> vettoreOrdAree = CreaVettoreDecrescente<double>(vettoreAree);

    //Si sceglie l'algoritmo di HeapSort perché è uno dei migliori avendo nel caso peggiore una complessità temporale di O(n*log(n)) (in base «e»)
    vettoreOrdAree = HeapSort<Decrescente>(vettoreOrdAree);

    vector<unsigned int> indiciDaRaffinare;
    indiciDaRaffinare.reserve(teta);

    for(unsigned int i=0; i<teta; i++) //Estrazione dei primi «teta» indici dei triangoli con area maggiore secondo il vettore «vettoreOrdAree»
    {
        unsigned int j = 0;

        //La condizione nel ciclo «while» confronta il vettore delle aree ordinato («vettoreOrdAree») e no («vettoreAree»): l'indice j aumenta finché
        //l'area «vettoreAree[j]» conincide con quella considerata «vettoreOrdAree[i].valore» e l'indice j non sia già presente in «indiciDaRaffinare».
        //Infatti il vettore «vettoreAree» preserva l'ordine del vettore dei triangoli originali quindi dal suo confronto si può ricavare l'indice.
        while(vettoreOrdAree[i].valore != vettoreAree[j] || find(indiciDaRaffinare.begin(),indiciDaRaffinare.end(),j) != indiciDaRaffinare.end())
            j++;
        indiciDaRaffinare.push_back(j);
    }

    return indiciDaRaffinare;
}

///
///
///

MagliaTriangolare MagliaTriangolare::Dissezionatore(const vector<unsigned int>& indiciR)
{
    MagliaTriangolare magliaR; //Definizione della nuova maglia raffinata
    magliaR.Punti = Punti; //L'unica informazione copiata dalla precedente maglia sono i «Punti» che non vengono modificati ma solo espansi

    //Stima dei triangoli aggiunti dal raffinamento: escluso l'inizio, si aggiungono in genere tre sottotriangoli per ogni
    //triangolo originale; pertanto si triplica il numero totale di triangoli seppure tale numero non verrà mai raggiunto
    magliaR.Triangoli.VerticiT.reserve(3*Triangoli.NumeroT);

    //Vettore di buleani per verificare lo stato di un triangolo: è 1 se un triangolo è stato dissezionato, 0 altrimenti
    //(si usa «VectorXi» anziché «vector<bool>» perché in tal modo si può inizializzare molto piú facilmente il vettore a 0)
    VectorXi statoTriangolo = VectorXi::Zero(Triangoli.NumeroT);
    Vector2d puntoMedio;
    array<unsigned int, 2> indiciPunti;
    unsigned int iTCorrente, indicePunta;

    bool v;

    for(unsigned int i=0; i<indiciR.size(); i++) //Per ogni triangolo da raffinare
    {
        if(statoTriangolo[indiciR[i]] == 0) //Se il triangolo in considerazione non è già stato raffinato
        {
            //Si memorizzano le informazioni rilevanti: indice, indici del lato massimo e indice della punta
            iTCorrente = indiciR[i];
            indiciPunti = Triangoli.LatiTMax[iTCorrente];
            indicePunta = Triangoli.Punte[iTCorrente];

            //Si calcola il punto medio tramite le coordinate x-y
            puntoMedio<<(Punti.CoordinateP[indiciPunti[0]][0]+Punti.CoordinateP[indiciPunti[1]][0])/2,
                        (Punti.CoordinateP[indiciPunti[0]][1]+Punti.CoordinateP[indiciPunti[1]][1])/2;
            magliaR.Punti.CoordinateP.push_back(puntoMedio); //Si aggiunge al vettore dei punti della «magliR»
            magliaR.Punti.MarcatoriP.push_back(Triangoli.MarcatoriLatiTMax[iTCorrente]); //Si assegna lo stesso marcatore del relativo lato e infine
            magliaR.Punti.NumeroP++; //Si aumenta il numero di punti nella «magliaR»

            //Inserimento dei sottotriangoli nella struttura «Triangoli» della «magliaR» (si creano solo due sottotriangoli
            //e non tre [come nella funzione «SmembraTriangolo»] perché il triangolo iniziale non presenta un punto medio precedente)
            magliaR.Triangoli.VerticiT.push_back({magliaR.Punti.NumeroP-1, indiciPunti[1], indicePunta});
            magliaR.Triangoli.VerticiT.push_back({magliaR.Punti.NumeroP-1, indicePunta, indiciPunti[0]});

            magliaR.Triangoli.NumeroT+=2; //Si aumenta il numero di triangoli della «magliR»
            statoTriangolo[iTCorrente] = 1; //Si segna il triangolo corrente come dissezionato

            do
            {
                iTCorrente = TrovaTriangoloOpposto(iTCorrente, indiciPunti); //Si trova l'indice del prossimo triangolo

                //Struttura if per verificare se continuare o fermarsi a dissezionare a seconda delle caratteristiche del triangolo successivo
                if(iTCorrente == Triangoli.NumeroT)
                {
                    //Si esce dal ciclo while perché in questo caso il triangolo successivo non esiste
                    //dato che il lato massimo di quello precedente si affaccia al bordo della maglia triangolare

                    v = 0;
                }
                else if(statoTriangolo[iTCorrente] == 1)
                {
                    //In questo caso il triangolo successivo al precedente è già stato dissezionato per cui esiste già un punto medio a cui ricollegarsi

                    unsigned int k,h=0;

                    //Questo ciclo «for» trova il triangolo in «magliaR» che contiene ambo gl'indici dei punti del lato massimo precedente. Non è necessario aggiungere
                    //altre condizioni visto che, siccome due code si stanno fondendo, si sa che il triangolo è al bordo della maglia presentamente descritta da «magliaR»
                    for(k=0; k<magliaR.Triangoli.NumeroT; k++)
                        if(find(magliaR.Triangoli.VerticiT[k].begin(),magliaR.Triangoli.VerticiT[k].end(),indiciPunti[0]) != magliaR.Triangoli.VerticiT[k].end() &&
                           find(magliaR.Triangoli.VerticiT[k].begin(),magliaR.Triangoli.VerticiT[k].end(),indiciPunti[1]) != magliaR.Triangoli.VerticiT[k].end())
                            break;

                    //Il seguente ciclo «while» trova l'indice della punta del triangolo individuato precedentemente
                    //scandendo tutti i punti e trovando quello diverso dagl'indici del lato massimo («indiciPunti[0]» e «indiciPunti[1]»)
                    while(magliaR.Triangoli.VerticiT[k][h] == indiciPunti[0] || magliaR.Triangoli.VerticiT[k][h] == indiciPunti[1])
                        h++;

                    //I comandi successivi sono simili a quelli inziali (righe 358, 359) ma si scambiano indiciPunti[0] e indiciPunti[0]; ciò accade
                    //perché nel triangolo successivo, per mantenere il senso antiorario, bisogna invertire il senso del lato massimo precedente.
                    magliaR.Triangoli.VerticiT.push_back({magliaR.Punti.NumeroP-1, indiciPunti[0], magliaR.Triangoli.VerticiT[k][h]});
                    magliaR.Triangoli.VerticiT.push_back({magliaR.Punti.NumeroP-1, magliaR.Triangoli.VerticiT[k][h], indiciPunti[1]});

                    //Si elimina il triangolo individuato dal primo ciclo «while» perché nel processo precedente è stato sostanzialmente separato in altri due sottotriangoli
                    magliaR.Triangoli.VerticiT.erase(magliaR.Triangoli.VerticiT.begin()+k);
                    magliaR.Triangoli.NumeroT++; //Il bilancio totale è di 1 perché si aggiungono due triangoli e se ne toglie uno: 2-1=1

                    v = 0; //Si esce dal ciclo perché la coda è terminata fondendosi con un'altra
                }
                else if(Triangoli.LatiTMax[iTCorrente][0] == indiciPunti[1] && Triangoli.LatiTMax[iTCorrente][1] == indiciPunti[0])
                {
                    //Si controlla che il lato massimo precedente coincida con il successivo (si faccia attenzione che mantendendo,
                    //il senso antiorario, si ha un'inversione degl'indici, ragion per cui si controlla il primo del nuovo con
                    //il secondo del vecchio e viceversa; è molto simile a ciò che accade nel commento di riga 396, 397

                    //Aggiornamento degli indici del lato massimo e della punta
                    indiciPunti = Triangoli.LatiTMax[iTCorrente];
                    indicePunta = Triangoli.Punte[iTCorrente];

                    //I comandi successivi sono simili a quelli inziali (righe 358, 359)
                    magliaR.Triangoli.VerticiT.push_back({magliaR.Punti.NumeroP-1, indiciPunti[1], indicePunta});
                    magliaR.Triangoli.VerticiT.push_back({magliaR.Punti.NumeroP-1, indicePunta, indiciPunti[0]});

                    magliaR.Triangoli.NumeroT+=2;
                    statoTriangolo[iTCorrente] = 1;

                    v = 0; //Si esce dal ciclo perché la coda non può andare avanti dato che i lati massimi [precedente e successivo] coincidono
                }
                else
                {
                    //In questo caso il triangolo successivo presenta le seguenti caratteristiche:
                    //      1) esiste (ovvero il lato massimo del triangolo precedente non si affaccia al bordo),
                    //      2) non è stato dissezionato,
                    //      4) non ha il lato massimo coincidente con quello del triangolo precedente;
                    //di conseguenza bisogna smembrare il triangolo in questione

                    //I comandi successivi aggionarno in momenti precisi l'indice della punta, del lato massimo
                    //La vera funzione che aggiunge invece i nuovi sottotriangoli alla «magliaR» è la funzione «SmembraTriangolo»
                    indicePunta = Triangoli.Punte[iTCorrente];
                    magliaR.SmembraTriangolo(magliaR.Punti.NumeroP,Triangoli.LatiTMax[iTCorrente],indicePunta,indiciPunti,magliaR.Punti.NumeroP-1);
                    indiciPunti = Triangoli.LatiTMax[iTCorrente];

                    //Si calcola il nuovo punto medio da aggiungere alla fine e il relativo marcatore aggiornando anche il numero di punti e lo stato del triangolo
                    puntoMedio<<(Punti.CoordinateP[indiciPunti[0]][0]+Punti.CoordinateP[indiciPunti[1]][0])/2,
                                (Punti.CoordinateP[indiciPunti[0]][1]+Punti.CoordinateP[indiciPunti[1]][1])/2;
                    magliaR.Punti.CoordinateP.push_back(puntoMedio);
                    magliaR.Punti.MarcatoriP.push_back(Triangoli.MarcatoriLatiTMax[iTCorrente]);
                    magliaR.Punti.NumeroP++;

                    statoTriangolo[iTCorrente] = 1;

                    v = 1; //Si continua a rimanere nel ciclo perché in questo caso la coda non è ancora terminata
                }

            }
            while(v == 1);

        }
    }

    for(unsigned int i=0; i<Triangoli.NumeroT; i++) //Aggiunta dei triangoli non dissezionati
        if(statoTriangolo[i] == 0)
        {
            magliaR.Triangoli.VerticiT.push_back(Triangoli.VerticiT[i]);
            magliaR.Triangoli.NumeroT++;
        }


    /*//Stampa per la visualizzazione dei punti raffinati su Geogebra
    for(unsigned int i=Punti.NumeroP; i<magliaR.Punti.NumeroP; i++)
        cout<<"A"<<i<<"=("<<magliaR.Punti.CoordinateP[i][0]<<","<<magliaR.Punti.CoordinateP[i][1]<<")\n"
            <<"ImpColore(A"<<i<<",Red)\n";*/


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

    //Si scompone il triangolo corrente in tre sottotriangoli generati a seconda della posizione del punto medio precedente:
    //la costruzione mantiene fisso il punto medio corrente, che quindi fa da origine ai tre sottotriangoli, cambiando gli altri due punti tre volte.

    array<unsigned int,3> triangolo;

    triangolo[0] = indicePM; //Il primo punto è l'indice del punto medio
    triangolo[1] = indiciLM[1]; //Il secondo punto è il secondo indice del lato massimo (cosí da seguire il senso antiorario)

    //Questa struttura «if» identifica la posizione del punto medio precedente: nel primo caso è súbito dopo
    //il secondo punto del lato massimo mentre nel secondo caso è dopo la punta del triangolo corrente.
    //La conoscenza di ciò cambia il modo in cui si costruiscono i tre triangoli ed è perciò necessaria
    if(indiciLM[1] == indiciLMP[1]) //Il punto medio precedente è súbito dopo il secondo punto del lato massimo
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
    else //Il punto medio precedente è dopo la punta del triangolo
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
    //Se il valore di «j» è uguale a Triangoli.NumeroT allora il triangolo opposto non esiste e quello corrente deve trovarsi al bordo
    return j;
}

///
///
///

void MagliaTriangolare::CostruisciLati()
{

    Lati.VerticiL.reserve(3*Triangoli.NumeroT); //Stima massima dei lati perché molti di questi non vi saranno mai
    Triangoli.LatiT.resize(Triangoli.NumeroT);

    array<unsigned int,2> lato;
    array<unsigned int,3> triangolo;

    //Un lato sarà condiviso da un solo altro triangolo rispetto a quello considerato e se per uno è in senso antiorario per l'altro è in senso orario;
    //in altre parole il lato (n_1,n_2) orientato in senso antiorario in un triangolo è nell'altro orientato in senso orario e quindi in quest'ultimo
    //è salvato come (n_2,n_1) per mantenere il senso antiorario. Pertanto è sufficiente controllare che l'inverso di un lato sia presente nel vettore dei lati.

    for(unsigned int i=0; i<Triangoli.NumeroT; i++) //Per ogni triangolo
    {
        triangolo = Triangoli.VerticiT[i]; //Si memorizzano gl'indici dei vertici dell'i-esimo triangolo

        for(unsigned int j=0; j<3; j++) //Per ogni vertice
        {
            //Si memorizzano, invertendoli, gl'indici del lato per controllare (vedi ragionamento sopra) che sia già presente
            lato = {triangolo[(j==2? 0 :j+1)], triangolo[j]};
            unsigned int k;

            //Questo ciclo «for» individua (se esiste) il lato precedentemente costruito all'interno dei lati già memorizzati in «Lati.VerticiL»;
            //se «k=Lati.VerticiL.size()» allora il lato non è stato memorizzato all'interno della «magliaR»
            for(k=0; k<Lati.VerticiL.size(); k++)
                if(lato == Lati.VerticiL[k])
                    break;

            //Si potrebbe usare if(find(Lati.VerticiL.begin(),Lati.VerticiL.end(),lato) == Lati.VerticiL.end())
            //ma ho notato che si impiega meno tempo con il ciclo «for» precedente
            if(k == Lati.VerticiL.size())
            {
                //In questo caso il lato non è presente in «Lati.VerticiL» per cui deve essere aggiunto

                Lati.VerticiL.push_back({lato[1],lato[0]}); //Si inserisce il lato invertito cosí da preservare il senso antiorario
                Lati.NumeroL++; //Si aumenta il numero di lati
                Triangoli.LatiT[i][j] = Lati.NumeroL-1; //Si aggiunge l'indice considerato al membro «LatiT» della struttura «Triangoli»

                //I seguenti «if» successivi vengono impiegati per individuare il marcatore del lato
                if(Punti.MarcatoriP[lato[0]] == 0 || Punti.MarcatoriP[lato[1]] == 0)
                    Lati.MarcatoriL.push_back(0);
                else if(Punti.MarcatoriP[lato[0]] == Punti.MarcatoriP[lato[1]])
                    Lati.MarcatoriL.push_back(Punti.MarcatoriP[lato[0]]);
                else if(Punti.MarcatoriP[lato[0]] == 1 ||
                        Punti.MarcatoriP[lato[0]] == 2 ||
                        Punti.MarcatoriP[lato[0]] == 3 ||
                        Punti.MarcatoriP[lato[0]] == 4)
                    Lati.MarcatoriL.push_back(Punti.MarcatoriP[lato[1]]);
                else if(Punti.MarcatoriP[lato[1]] == 1 ||
                        Punti.MarcatoriP[lato[1]] == 2 ||
                        Punti.MarcatoriP[lato[1]] == 3 ||
                        Punti.MarcatoriP[lato[1]] == 4)
                    Lati.MarcatoriL.push_back(Punti.MarcatoriP[lato[0]]);
                else
                    Lati.MarcatoriL.push_back(0);
            }
            else
            {
                //In questo caso il triangolo è già presente in «Lati.VerticiL» e «k» è il suo indice
                //per cui basta aggiungere quello al membro «LatiT» della struttura «Triangoli»
                Triangoli.LatiT[i][j] = k;
            }
        }
    }


    /*//Stampa per la visualizzazione dei lati dei triangoli su Geogebra
    for(unsigned int i=0; i<Lati.NumeroL; i++)
        cout<<"B"<<i<<"=Segmento(("<<Punti.CoordinateP[Lati.VerticiL[i][0]][0]<<","<<Punti.CoordinateP[Lati.VerticiL[i][0]][1]<<"),"
            <<"("<<Punti.CoordinateP[Lati.VerticiL[i][1]][0]<<","<<Punti.CoordinateP[Lati.VerticiL[i][1]][1]<<"))\n"
            <<"ImpColore(B"<<i<<",Blue)\n";
    cout<<"\n\n";*/


    /*//Stampa per la visualizzazione dei marcatori dei lati su Geogebra
    for(unsigned int i=0; i<Lati.NumeroL; i++)
    {
        cout<<"B"<<i<<"=Segmento(("<<Punti.CoordinateP[Lati.VerticiL[i][0]][0]<<","<<Punti.CoordinateP[Lati.VerticiL[i][0]][1]<<"),"
            <<"("<<Punti.CoordinateP[Lati.VerticiL[i][1]][0]<<","<<Punti.CoordinateP[Lati.VerticiL[i][1]][1]<<"))\n";
        if(Lati.MarcatoriL[i] == 0)
            cout<<"ImpColore(B"<<i<<",Blu)\n";
        else if(Lati.MarcatoriL[i] == 8)
            cout<<"ImpColore(B"<<i<<",Giallo)\n";
        else if(Lati.MarcatoriL[i] == 7)
            cout<<"ImpColore(B"<<i<<",Arancione)\n";
        else if(Lati.MarcatoriL[i] == 6)
            cout<<"ImpColore(B"<<i<<",Verde)\n";
        else
            cout<<"ImpColore(B"<<i<<",Rosso)\n";
    }
    cout<<"\n\n";*/

}

///
///
///

bool MagliaTriangolare::EsportaMaglia(const string& nomeMaglia, const string& formato)
{

    string nomeFilza;
    ofstream filza;


    //Esportazione punti
    nomeFilza = "./Maglia Raffinata/"+nomeMaglia+" 01 Punti."+formato; //Costruzione del nome della filza
    filza.open(nomeFilza);

    if (filza.fail()) //Verifica che l'apertura della filza sia avvenuta con successo
        return false;

    filza<<"Id Marcatore X Y"<<endl; //Inserimento dei metadati

    for(unsigned int i=0; i<Punti.NumeroP; i++) //Ciclo «for» per trasferire tutti i dati necessari a creare le relative filze «Excel»
        filza<<i<<" "
             <<Punti.MarcatoriP[i]<<" "
             <<Punti.CoordinateP[i][0]<<" "
             <<Punti.CoordinateP[i][1]<<endl;

    filza.close();


    //Esportazione lati
    nomeFilza = "./Maglia Raffinata/"+nomeMaglia+" 02 Lati."+formato; //Costruzione del nome della filza
    filza.open(nomeFilza);

    if (filza.fail()) //Verifica che l'apertura della filza sia avvenuta con successo
        return false;

    filza<<"Id Marcatore Origine Fine"<<endl; //Inserimento dei metadati

    for(unsigned int i=0; i<Lati.NumeroL; i++) //Ciclo «for» per trasferire tutti i dati necessari a creare le relative filze «Excel»
        filza<<i<<" "
             <<Lati.MarcatoriL[i]<<" "
             <<Lati.VerticiL[i][0]<<" "
             <<Lati.VerticiL[i][1]<<endl;

    filza.close();


    //Esportazione triangoli
    nomeFilza = "./Maglia Raffinata/"+nomeMaglia+" 03 Triangoli."+formato; //Costruzione del nome della filza
    filza.open(nomeFilza);

    if (filza.fail()) //Verifica che l'apertura della filza sia avvenuta con successo
        return false;

    filza<<"Id Vertici Lati"<<endl; //Inserimento dei metadati

    for(unsigned int i=0; i<Triangoli.NumeroT; i++) //Ciclo «for» per trasferire tutti i dati necessari a creare le relative filze «Excel»
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
