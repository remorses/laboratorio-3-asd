#include "queueASD-array.h"

using namespace queue;

/****************************************************************/
/* 			FUNZIONI AUSILIARIE 			*/
/****************************************************************/


/****************************************************************/
void add(int pos, Elem e, Queue& l) // aggiunge e nella posizione pos
{
   if (pos < 0 || pos > l.size) {cout << "\nPosizione fuori range, operazione ignorata\n"; return;}
   
   if (l.size == l.maxsize) // l'array e' pieno, per poter inserire un nuovo elemento devo ridimensionare l'array espandendolo; l'unico modo e' creare un array piu' grande e copiare in esso gli elementi di l
      {
      cout << "\nMessaggio di sistema: Espansione dell'array\n";
      l.size++;
      l.maxsize += BLOCKDIM;
      Elem* auxArray = new Elem[l.maxsize];
      for (int i=0; i<pos; ++i)
          auxArray[i] = l.queue[i];
      auxArray[pos] = e;
      for (int i=pos+1; i<l.size; ++i)
          auxArray[i] = l.queue[i-1];
      delete[] l.queue;
      l.queue = auxArray; 
      return;
      }

   // se la funzione non ha incontrato un return, vuole dire che pos e' nel range corretto e l'array non e' pieno; devo solo posizionare l'elemento in pos e shiftare a destra gli altri, aggiornando la size della lista
     l.size++;
     for (int i=l.size-1; i>pos; --i)
           l.queue[i] = l.queue[i-1];
     l.queue[pos] = e;
     return;
}


/****************************************************************/
Elem remove(int pos, Queue& l)
{
   if (pos < 0 || pos >= l.size) 
     {
      cout << "\nPosizione fuori range, operazione ignorata, si restituisce " << EMPTYELEM << "\n"; 
      return EMPTYELEM;
     }
  
// se rimuovendo l'elemento si liberano BLOCKDIM celle, allora ridimensioniamo l'array; l'unico modo e' creare un array piu' piccolo e copiare in esso gli elementi di l
   if (l.maxsize-l.size+1==BLOCKDIM && l.maxsize>BLOCKDIM)
   {
      cout << "\nMessaggio di sistema: Contrazione dell'array\n";
      l.size--;
      l.maxsize -= BLOCKDIM;
      Elem* auxArray = new Elem[l.maxsize];
      for (int i=0; i<pos; ++i)
          auxArray[i] = l.queue[i];
      for (int i=pos; i<l.size; ++i)
          auxArray[i] = l.queue[i+1];
      Elem tmp=l.queue[pos];
      delete[] l.queue;
      l.queue = auxArray; 
      return tmp;
   }

// se la funzione non ha incontrato un return, vuole dire che pos e' nel range corretto e la lista non puo' essere ridotta di dimensione; devo solo restituire l'elemento in pos e shiftare a sinistra gli altri, aggiornando la size della lista.
     Elem tmp=l.queue[pos];
     for (int i=pos+1; i<l.size; ++i)
           l.queue[i-1]=l.queue[i];
     l.size--;
     return tmp;
}


/****************************************************************/
/* 			FUNZIONI SULLE CODE 			*/
/****************************************************************/

Queue queue::createEmpty(){
    Queue q;
    q.size = 0;
    q.maxsize = BLOCKDIM;
    Elem* aux = new Elem[BLOCKDIM];
    q.queue = aux;
    return q;
}


/****************************************************************/
void queue::enqueue(Elem e, Queue& l) // aggiunge e in coda
{
   add(l.size, e, l);
}


/****************************************************************/
Elem queue::dequeue(Queue& l) // rimuove il primo elemento e lo restituisce
{
   return remove(0, l);
}


/****************************************************************/
Elem queue::first(Queue& l) // restituisce il primo elemento
{
   if (isEmpty(l)) return EMPTYELEM;   
   else return l.queue[0];
}




/****************************************************************/
bool queue::isEmpty(const Queue& l)
{
   return (l.size == 0);
}



/****************************************************************/
Queue readFromFile(string nome_file)
{
    ifstream ifs(nome_file.c_str()); // apertura di uno stream associato ad un file, in lettura
    return readFromStream(ifs);
}


/****************************************************************/
Queue readFromStdin()
{
    cout << "\nInserire una sequenza di numeri separati da spazi seguiti da " << FINEINPUT << " per terminare\n";
    return readFromStream((std::cin));
}


/****************************************************************/
Queue readFromStream(istream& str)
{
    Queue l = createEmpty();        
    int dato;
    str>>dato;
    if (!str) throw runtime_error("Errore inserimento dati\n");
    while (dato!= FINEINPUT)  // assumiamo che il segnale di fine input nel file sia  FINEINPUT
        {     
        enqueue(dato, l);   // aggiunge l'elemento alla fine della lista
        str>>dato;
        if (!str) throw runtime_error("Errore inserimento dati\n");
        }
return l;
}

/****************************************************************/
void print(const Queue& l)
{
cout << endl;
for (int i=0; i<l.size; ++i)
   cout << l.queue[i] << "; "; 
cout << endl;
}
