#include <stdio.h>
#include <string.h>
#include <malloc.h>
#include <stdbool.h>

typedef struct medico Medico;
typedef struct singolo_turno SingoloTurno;

struct medico
{
    char id_medico[13];
    Medico *next;
};

Medico *sort_medici(Medico *medici)
{
    Medico *temp = NULL;

    Medico *m = medici;
    while(m != NULL)
    {
        Medico *item = malloc(sizeof(Medico));
        strcpy(item->id_medico, m->id_medico);
        item->next = NULL;

        Medico *curr = temp;
        Medico *prev = NULL;

        while ((curr != NULL) && (strcmp(curr->id_medico, item->id_medico) < 0))
        {
            prev = curr;
            curr = curr->next;
        }

        item->next = NULL;

        if(prev == NULL)
        {
            temp = item;
        }
        else
        {
            prev->next = item;
        }

        item->next = curr;

        m = m->next;
    }

    return temp;
}


void Merge(int A[], int p, int q, int r)
{
    int i, j, k, B[20];
    i = p;
    j = q+1;
    k = 0;

    while(i <= q && j <= r)
    {
        if(A[i] <  A[j])
        {
            B[k] = A[i];
            i++;
        }
        else
        {
            B[k] = A[j];
            j++;
        }
        k++;
    }

    while(i <= q)
    {
        B[k] = A[i];
        i++;
        k++;
    }

    while (j <= r)
    {
        B[k]  = A[j];
        j++;
        k++;
    }

    for (k = p; k <= r; k++) {
        A[k] = B[k-p];
    }
    return;
}

void MergeSort(int A[], int p, int r) {
    int q;
    if (p < r) {
        q = (p+r)/2;
        MergeSort(A, p, q);
        MergeSort(A, q+1, r);
        Merge(A, p, q, r);
    }
}

int main()
{
    Medico *medici;
    Medico *last_medici;
    for (int i = 0; i < 20; ++i) {
        Medico *t= malloc(sizeof(medici));
        sprintf(t->id_medico, "%d", i);
        t->next = NULL;
        if(medici == NULL)
        {
            medici = t;
            last_medici = t;
        }
        else
        {
            last_medici->next = t;
            last_medici = t;
        }
    }

    int numeri[20];
    for (int i = 0; i < 20; ++i) {
        numeri[i] =  (12 * i) % 20;
    }

    MergeSort(numeri, 0, 20-1);

    medici = sort_medici(medici);

    return 0;
}
