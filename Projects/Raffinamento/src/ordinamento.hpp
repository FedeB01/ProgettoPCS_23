#ifndef __ORDINAMENTO_H
#define __ORDINAMENTO_H

#include <list>
#include "Eigen/Eigen"

using namespace std;
using namespace Eigen;

namespace LibreriaOrdinamento {

  //La struttura «Decrescente» permette di applicare l'algoritmo «HeapSort» ordinando però i valori in modo decrescente
  struct Decrescente
  {
    double valore;
    Decrescente() = default;
    Decrescente(const double& valore): valore(valore){}
  };


  ///\brief Crea un vettore formato di strutture «Decrescente» coi medesimi valori memorizzati nel vettore in ingresso
  ///\param vettore: il vettore di riferimento che si vuole specchiare
  ///\return il vettore di elementi «Decrescente»
  template<typename T> vector<Decrescente> CreaVettoreDecrescente(const vector<T>& vettore)
  {
      vector<Decrescente> vettoreD;
      vettoreD.reserve(vettore.size());

      for(unsigned int i=0; i<vettore.size(); i++)
      {
          Decrescente elemento(vettore[i]);
          vettoreD.push_back(elemento);
      }

      return vettoreD;
  }


  //Riformulazione dell'operazione di flusso «<<» per l'oggetto «Decrescente» ma che in pratica nel programma non viene
  //mai usata (assieme anche all'operazione di disuguaglianza «!=», definita súbito dopo); si aggiunge comunque per completezza
  inline ostream &operator<<(ostream &os, const Decrescente &oggetto)
  {
    os << oggetto.valore;
    return os;
  }

  //Riformulaizone delle operazioni buleane uguale (==), diverso (!=), minore (<) e maggiore (>) per la struttura «Decrescente»
  //Nel complesso si inverte il maggiore (>) con il miniore (<) cosicché l'algoritmo successivo possa anche ordinare decrescentemente
  inline bool operator==(const Decrescente &oggetto1, const Decrescente &oggetto2)
  {
    return oggetto1.valore == oggetto2.valore;
  }

  inline bool operator!=(const Decrescente &oggetto1, const Decrescente &oggetto2)
  {
    return !(oggetto1 == oggetto2);
  }

  inline bool operator<(const Decrescente &oggetto1, const Decrescente &oggetto2)
  {
    return (oggetto1.valore > oggetto2.valore);
  }

  inline bool operator>(const Decrescente &oggetto1, const Decrescente &oggetto2)
  {
    return (oggetto1.valore < oggetto2.valore);
  }



  ///\brief Ordina un vettore di elementi di tipo «T» per valori crescenti tramite l'algoritmo «HeapSort»
  ///\param v: il vettore da ordinare
  ///\return il vettore ordinato crescentemente
  template<typename T> vector<T> HeapSort(const vector<T>& v)
  {

    unsigned int g = 0, f = 0; //Indici del nodo del figlio e del genitore
    vector<T> mucchioBin = v;
    T bicerino; //Variabile per memorizzare temporaneamente l'elemento da scambiare; il nome è l'adattamento del piemontese «bicerin» che letteralmente significa «bicchierino»


    //Questo ciclo «for» costruisce da una lista generica, interpretata come un albero binario, un mucchio binario (traduzione dell'inglese «heap binary»);
    //il mucchio binario è a sua volta un albero binario in cui un qualsiasi nodo ha l'etichetta associata maggiore di quelle dei suoi [eventuali] nodi figli

    //L'idea di fondo del ciclo è quella di scandire il vettore scambiando ogni nodo col suo relativo genitore se l'etichetta del primo è maggiore del secondo;
    //tale confronto si ripete col prossimo genitore nella gerarchia finché il nuovo genitore ha un'etichetta maggiore o si è raggiunta la cima.

    for(unsigned int i=1; i<v.size(); i++) //Si parte scandendo il secondo elemento della lista perché il primo nodo del mucchio binario non deve essere confrontato con nessun nodo
    {

        f = i; //Variabile ausiliaria per poter muoversi dinamicamente con il nodo ogni volta che viene scambiato lasciando intoccato l'indice «i»

        do
        {
            //Controllo che l'indice del nodo da controllare sia pari (resto nullo rispetto a 2) o dispari (resto unitario rispetto a 2)
            //poiché ciò condiziona la scelta dell'indice del nodo genitore da considerare nel futuro controllo
            g = f % 2 == 0 ? (f-2)/2 : (f-1)/2 ;

            //Controllo che il genitore abbia un'etichetta minore di suo figlio
            if(mucchioBin[g]<mucchioBin[f])
            {
                //In caso positivo si scambia il nodo genitore col figlio
                bicerino = mucchioBin[g];
                mucchioBin[g] = mucchioBin[f];
                mucchioBin[f] = bicerino;
                f = g;
            }
            else break; //Altrimenti si esce dal ciclo «while»

        }
        //Il ciclo «while» cessa quando il nodo genitore (già controllato) ha indice 0, ovvero è in cima al mucchio binario,
        //perché in tal caso non vi sono piú genitori da controllare
        while(g!=0);


    }



    //Questo ciclo «for» invece riordina la lista scambiando inizialmente il primo elemento con l'ultimo, successivamente riposizione il nuovo primo elemento
    //(coincidente con l'ultimo di prima) in modo tale che l'albero binario rimanente, escluso l'ultimo nodo, soddisfi le caratteristiche di un mucchio binario
    //descritte nel precedente ciclo; l'algoritmo viene ripetuto escludendo di volta in volta il primo elemento scambiato finché non finiscono i nodi del mucchio binario originale.

    for(unsigned int k=0; k<mucchioBin.size(); k++)
    {

      //Questa parte scambia il primo con il (n-k-1)-esimo elemento con «n» coincidente con la dimensione del vettore in ingresso «v».
      bicerino=mucchioBin[mucchioBin.size()-k-1];
      mucchioBin[mucchioBin.size()-k-1]=mucchioBin[0];
      mucchioBin[0]=bicerino;

      g=0; //Indice del nodo genitore in cima all'albero binario

      while(g!=v.size())
      {

        f=2*g+1; //Indice del primo figlio del geniotre g

        //Controlla che l'etichetta del nodo genitore sia minore del suo primo figlio; inoltre si verifica che il primo figlio
        //esista effettivamente tramite la condizione «f<(n-k-1)», in cui n-k-1 esclude il nodo scambiato all'inizio.
        if(mucchioBin[g]<mucchioBin[f] && f<(v.size()-k-1))
        {
            //Si controlla l'etichetta del secondo figlio, verificandone pure l'esistenza sempre escludendo il nodo scambiato all'inizio.
            if(mucchioBin[g]<mucchioBin[f+1] && (f+1)<(v.size()-k-1))
            {
                //Se pure il secondo figlio è minore del genitore allora si sceglie il figlio con l'etichetta maggiore e lo si scambia con il genitore.
                if(mucchioBin[f]>mucchioBin[f+1])
                {
                     bicerino = mucchioBin[f];
                     mucchioBin[f] = mucchioBin[g];
                     mucchioBin[g] = bicerino;
                     g=f;
                }
                else
                {
                     bicerino = mucchioBin[f+1];
                     mucchioBin[f+1] = mucchioBin[g];
                     mucchioBin[g] = bicerino;
                     g=f+1;
                }
            }
            //Altrimenti se solo il primo figlio è maggiore con il genitore si scambiano questi due senza indugio.
            else
            {
                 bicerino = mucchioBin[f];
                 mucchioBin[f] = mucchioBin[g];
                 mucchioBin[g] = bicerino;
                 g=f;
            }
        }
        //In caso il primo figlio non abbia etichetta maggiore del genitore si ripetono le stesse condizioni al secondo figlio
        else if(mucchioBin[g]<mucchioBin[f+1] && (f+1)<(v.size()-k-1))
        {
             bicerino = mucchioBin[f+1];
             mucchioBin[f+1] = mucchioBin[g];
             mucchioBin[g] = bicerino;
             g=f+1;
        }
        //Se ambo le condizioni non sono verificate il nodo ha un etichetta maggiore di ambo i suoi [eventuali] nodi figli, condizione che si verifica solo se l'algoritmo è terminato.
        else g=v.size();

      }
    }

    return mucchioBin;

  }

}

#endif // __ORDINAMENTO_H
