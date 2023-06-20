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
  template<typename T> vector<T> HeapSort(const vector<T>& v){

    unsigned int i = 0;
    vector<T> mucchioBin = v;
    T bicerino; //Variabile per memorizzare temporaneamente l'elemento da scambiare (il nome è l'adattamento del piemontese «bicerin» che letteralmente significa «bicchierino»)

    //DA MODIFICARE!:'(
    //Questo ciclo «while» costruisce da una lista il mucchio binario (traduzione dell'inglese «heap binary»),
    //ovvero di un albero binario in cui un qualsiasi nodo ha l'etichetta associata maggiore di quelle dei suoi [eventuali] nodi figli

    //L'idea di fondo il ciclo si basa su un controllo continuo del vettore, interpretato come un albero binario, e in particolare dei nodi parenti con i nodi figli.
    //Infatti il «while» continua finché l'indice è minore della dimensione, indice che viene aumentato solo se non avvengono scambi; di conseguenza
    //il ciclo termina quando tutti i parenti hanno un etichetta maggiore di tutti i figli, proprio come definito da un mucchio binario

    while(i<v.size()){
        //A ogni iterazione l'indice i viene rinizializzato a zero per contare da capo i nodi non scambiati
        i=0;

        //Il controllo dei nodi viene ciclato dal «for»
        for(unsigned int j=0; j<v.size(); j++){

            //il primo «if» controlla che il parente j-esimo sia maggiore del suo primo figlio (2*j+1)-esimo
            //verificando inoltre che esso esista, ovvero che l'indice relativo sia minore della dimensione del vettore «n=v.size()»
            if(mucchioBin[j]<mucchioBin[2*j+1] && (2*j+1)<v.size())
                {
                    //In caso positivo si scambia il nodo parente col primo figlio senza aumentare l'indice i
                     bicerino = mucchioBin[j];
                     mucchioBin[j] = mucchioBin[2*j+1];
                     mucchioBin[2*j+1] = bicerino;
                }

            //In caso negativo si ripete il medesimo controllo di prima ma questa volta al secondo nodo (2*j+2)-esimo
            else if(mucchioBin[j]<mucchioBin[2*j+2] && (2*j+2)<v.size())
                {
                    //In caso positivo si scambia il nodo parente col secondo figlio senza aumentare l'indice i
                     bicerino = mucchioBin[j];
                     mucchioBin[j] = mucchioBin[2*j+2];
                     mucchioBin[2*j+2] = bicerino;
                }

            //In caso in cui il nodo parente sia maggiore di ambo i figli non si fa alcuna scambia aumentado semplicemente l'indice i
            else{i++;}
        }
    }


    //Questo ciclo «for» invece riordina la lista scambiando inizialmente il primo elemento con l'ultimo, successivamente riposizione il nuovo primo elemento (coincidente con l'ultimo di prima)
    //in modo tale che l'albero binario rimanente, escluso l'ultimo nodo, soddisfi le caratteristiche di un mucchio binario descritte nel precedente ciclo; l'algoritmo viene ripetuto questa
    //volta considerando il primo e il penultimo elemento e cosí via finché si finiscono nodi del mucchio binario originale.

    for(unsigned int k=0; k<mucchioBin.size(); k++){   //k rappresenta il numero di nodi ordinati e quindi da escludere; in totale il ciclo verrà ripetuto n volte coincidente col numero di nodi da ordinare

      //Questa parte scambia il primo con il (n-k-1)-esimo elemento con «n» coincidente con la dimensione del vettore in ingresso «v».
      bicerino=mucchioBin[mucchioBin.size()-k-1];
      mucchioBin[mucchioBin.size()-k-1]=mucchioBin[0];
      mucchioBin[0]=bicerino;

      i=0;

      //La condizione «i!=v.size()» non significa che si scandiscono tutti i nodi dell'albero, ma è semplicemente un numero arbitrario per uscire dal ciclo
      //(infatti lo si impone uguale a i quando non vi sono più scambi favorevoli nell'«else»); la scelta di «i!=v.size()» è arbitraria e deriva dal fatto che i
      //non sarà mai uguale a tal numero all'interno del while se non per l'«else»
      while(i!=v.size()){

        //Controlla che l'eticchetta dell'i-esimo nodo sia minore del suo primo figlio in corrispondenza dell'(2*i+1)-esimo nodo
        //Inoltre si controlla pure che il primo figlio esista effettivamente verificando la condizione «(2*i+1)<(n-k-1)», in cui n-k-1 esclude il nodo scambiato all'inizio.
        if(mucchioBin[i]<mucchioBin[2*i+1] && (2*i+1)<(v.size()-k-1))
          {
            //Se la prima condizione è verificata (ovvero l'etichetta dell'i-esimo nodo parente è minore di quella del suo primo figlio)
            //allora si controlla anche quella del secondo figlio, verificandone pure l'esistenza sempre escludendo il nodo scambiato all'inizio.
            if(mucchioBin[i]<mucchioBin[2*i+2] && (2*i+2)<(v.size()-k-1))
            {
                //Se l'i-esimo nodo parente è pure minore del secondo figlio allora si sceglie il figlio con l'etichetta maggiore e lo si scambia con il parente.
                if(mucchioBin[2*i+1]>mucchioBin[2*i+2])
                {
                     bicerino = mucchioBin[2*i+1];
                     mucchioBin[2*i+1] = mucchioBin[i];
                     mucchioBin[i] = bicerino;
                     i=2*i+1;
                }
                else
                {
                     bicerino = mucchioBin[2*i+2];
                     mucchioBin[2*i+2] = mucchioBin[i];
                     mucchioBin[i] = bicerino;
                     i=2*i+2;
                }
            }
            //Altrimenti se solo il primo figlio è maggiore con il parente si scambiano questi due senza indugio.
            else
            {
                 bicerino = mucchioBin[2*i+1];
                 mucchioBin[2*i+1] = mucchioBin[i];
                 mucchioBin[i] = bicerino;
                 i=2*i+1;
            }
          }
        //In caso negativo del primo nodo (sia che non esista sia che sia maggiore) si ripetono le stesse condizioni al secondo figlio in corrispondenza dell'(2*i+2)-esimo nodo.
        else if(mucchioBin[i]<mucchioBin[2*i+2] && (2*i+2)<(v.size()-k-1))
        {
             bicerino = mucchioBin[2*i+2];
             mucchioBin[2*i+2] = mucchioBin[i];
             mucchioBin[i] = bicerino;
             i=2*i+2;
        }
        //Se ambo le condizioni non sono verificate il nodo ha un etichetta maggiore di ambo i suoi [eventuali] nodi figli, condizione che si verifica solo se l'algoritmo è terminato.
        else{i=v.size();}
        }
      }

    return mucchioBin;

  }

}

#endif // __ORDINAMENTO_H
