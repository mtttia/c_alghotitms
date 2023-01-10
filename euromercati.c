#include <stdio.h>
#include <malloc.h>

#define VECTOR_LENGTH 200
#define null NULL

typedef struct Listino Listino;
typedef struct Supermercati Supermercati;
typedef struct Prodotto Prodotto;

struct Listino{
    int CodiceProdotto; //order by
    int CodiceFornitore;
    float PrezzoUnitario;
    float QuantitaMinima;
};

// lista principale di strutture collegate supermercati
struct Supermercati{
    int CodiceSupermercato;
    Prodotto* Prodotti;
    Supermercati* next;
};

Listino* listinoPrezzi;
Supermercati* supermecati;

struct Prodotto{
    int CodiceProdotto; //order by
    int CodiceFornitore; //order by 2nd
    int QuantitaPresente;
    int QuantitaMinima;
    Prodotto *next;
};

void ChiudiSupermercato(int CodiceSupermercato, int CodiceSupermercatoEridita)
{
    Supermercati* prevSupermercati = NULL;
    Supermercati* curr = supermecati;
    Prodotto * prodotti;
    while(curr != NULL)
    {
        if(curr->CodiceSupermercato == CodiceSupermercato)
        {
           if(prevSupermercati == NULL)
           {
               //è il primo
               supermecati = curr->next;
           }
           else
           {
               //non è il primo
               prevSupermercati->next = curr->next;
           }
           prodotti = curr->Prodotti;
        }

        curr = curr->next;
    }

    //ora riassegno il listino
    curr = supermecati;

    while (curr != NULL)
    {
        if(curr->CodiceSupermercato == CodiceSupermercatoEridita)
        {
            //eredito i prodotti
            curr->Prodotti = prodotti;
        }

        curr = curr->next;
    }
}

void StampaProdotto(Prodotto p)
{
    printf("%d\n", p.CodiceProdotto);
}

void StampaProdottiInEsaurimento(int codiceSupermercato)
{
    Supermercati *currSup = supermecati;
    while (currSup != NULL)
    {
        if(currSup->CodiceSupermercato == codiceSupermercato)
        {
            //supermercato trovato
            Prodotto *currProd = currSup->Prodotti;
            while(currProd != null)
            {
                if(currProd->QuantitaPresente < currProd->QuantitaMinima)
                {
                    StampaProdotto(*currProd);
                }

                currProd = currProd->next;
            }
        }

        currSup = currSup->next;
    }
}

float getProdottoPrice(int codProdotto)
{
    for (int i = 0; i < VECTOR_LENGTH; ++i)
    {
        if(listinoPrezzi[i].CodiceProdotto == codProdotto)
        {
            return listinoPrezzi[i].PrezzoUnitario;
        }
    }

    return 0;
}

int getSupermercatoValue(Supermercati supermercato)
{
    Prodotto* currProd = supermercato.Prodotti;
    float value = 0;
    while(currProd != NULL)
    {
        value += getProdottoPrice(currProd->CodiceProdotto);
    }

    return value;
}

void MigliorSupermercato()
{
    Supermercati* curr = supermecati;
    float value = 0;
    Supermercati* market = null;

    while(curr != NULL)
    {
        float tempValue = getSupermercatoValue(*curr);

        if(tempValue > value)
        {
            value = tempValue;
            market = curr;
        }

        curr = curr->next;
    }

    printf("%d\n", market->CodiceSupermercato);
}

void TestData()
{

}

int main() {
    listinoPrezzi = (Listino*) malloc(sizeof(Listino) * VECTOR_LENGTH);
    supermecati = NULL;

    return 0;
}
