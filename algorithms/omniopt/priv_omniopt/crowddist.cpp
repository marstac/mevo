#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "../OmniOptimizer.h"

#define large 10.0

void OmniOptimizer::assign_crowding_distance_list (OmniOptimizer::population *pop, list *lst, int front_size)
{
    int i;
    double avg_obj=0.0;
    double avg_realvar=0.0;
    double avg_binvar=0.0;
    double avg_var;
    list *temp;
    temp = lst;
    if (var_option==1)
    {
        if (nreal != 0)
        {
            assign_crowding_distance_list_realvar (pop, lst, front_size);
        }
        if (nbin != 0)
        {
            assign_crowding_distance_list_binvar (pop, lst, front_size);
        }
    }
    if (obj_option==1)
    {
        assign_crowding_distance_list_obj (pop, lst, front_size);
    }
    if (var_option==0)
    {
        for (i=0; i<front_size; i++)
        {
            pop->ind[temp->index]->setCrowdingDistance(pop->ind[temp->index]->getCrowdingDistance());
            temp = temp->child;
        }
    }
    if (obj_option==0)
    {
        if (nreal==0)
        {
            for (i=0; i<front_size; i++)
            {
                pop->ind[temp->index]->setCrowdingDistance(pop->ind[temp->index]->binvar_crowd_dist);
                temp = temp->child;
            }
        }
        if (nbin==0)
        {
            for (i=0; i<front_size; i++)
            {
                pop->ind[temp->index]->setCrowdingDistance(pop->ind[temp->index]->realvar_crowd_dist);
                temp = temp->child;
            }
        }
        if (nreal!=0 && nbin!=0)
        {
            for (i=0; i<front_size; i++)
            {
                pop->ind[temp->index]->setCrowdingDistance(pop->ind[temp->index]->realvar_crowd_dist + pop->ind[temp->index]->binvar_crowd_dist);
                temp = temp->child;
            }
        }
    }
    if (var_option==1 && obj_option==1)
    {
        for (i=0; i<front_size; i++)
        {
            if (pop->ind[temp->index]->getCrowdingDistance() != large)
            {
                avg_obj += pop->ind[temp->index]->getCrowdingDistance();
            }
            if (nreal!=0)
            {
                if (pop->ind[temp->index]->realvar_crowd_dist != large)
                {
                    avg_realvar += pop->ind[temp->index]->realvar_crowd_dist;
                }
            }
            if (nbin!=0)
            {
                if (pop->ind[temp->index]->binvar_crowd_dist != large)
                {
                    avg_binvar += pop->ind[temp->index]->binvar_crowd_dist;
                }
            }
            temp = temp->child;
        }
        temp = lst;
        avg_obj = avg_obj/front_size;
        avg_realvar = avg_realvar/front_size;
        avg_binvar = avg_binvar/front_size;
        avg_obj = avg_obj/nobj;
        if (nreal!=0)
        {
            avg_realvar = avg_realvar/nreal;
        }
        if (nbin!=0)
        {
            avg_binvar = avg_binvar/nbin;
        }
        avg_var = avg_realvar + avg_binvar;
        for (i=0; i<front_size; i++)
        {
            if (nreal!=0 && nbin!=0)
            {
                if (pop->ind[temp->index]->getCrowdingDistance() > avg_obj || (pop->ind[temp->index]->realvar_crowd_dist+pop->ind[temp->index]->binvar_crowd_dist) > avg_var)
                {
                    pop->ind[temp->index]->setCrowdingDistance(maximum (pop->ind[temp->index]->getCrowdingDistance(), (pop->ind[temp->index]->realvar_crowd_dist+pop->ind[temp->index]->binvar_crowd_dist)));
                }
                else
                {
                    pop->ind[temp->index]->setCrowdingDistance(minimum (pop->ind[temp->index]->getCrowdingDistance(), (pop->ind[temp->index]->realvar_crowd_dist+pop->ind[temp->index]->binvar_crowd_dist)));
                }
            }
            if (nreal==0)
            {
                if (pop->ind[temp->index]->getCrowdingDistance() > avg_obj || pop->ind[temp->index]->binvar_crowd_dist > avg_var)
                {
                    pop->ind[temp->index]->setCrowdingDistance(maximum (pop->ind[temp->index]->getCrowdingDistance(), pop->ind[temp->index]->binvar_crowd_dist));
                }
                else
                {
                    pop->ind[temp->index]->setCrowdingDistance(minimum (pop->ind[temp->index]->getCrowdingDistance(), pop->ind[temp->index]->binvar_crowd_dist));
                }
            }
            if (nbin==0)
            {
                if (pop->ind[temp->index]->getCrowdingDistance() > avg_obj || pop->ind[temp->index]->realvar_crowd_dist > avg_var)
                {
                    pop->ind[temp->index]->setCrowdingDistance(maximum (pop->ind[temp->index]->getCrowdingDistance(), pop->ind[temp->index]->realvar_crowd_dist));
                }
                else
                {
                    pop->ind[temp->index]->setCrowdingDistance(minimum (pop->ind[temp->index]->getCrowdingDistance(), pop->ind[temp->index]->realvar_crowd_dist));
                }
            }
            temp = temp->child;
        }
    }
    return;
}

void OmniOptimizer::assign_crowding_distance_list_obj (OmniOptimizer::population *pop, list *lst, int front_size)
{
    int **obj_array;
    int *dist;
    int i, j;
    list *temp;
    temp = lst;
    if (front_size==1)
    {
        pop->ind[lst->index]->setCrowdingDistance(large) ;
        return;
    }
    if (front_size==2)
    {
        pop->ind[lst->index]->setCrowdingDistance(large);
        pop->ind[lst->child->index]->setCrowdingDistance(large);
        return;
    }
    obj_array = (int **)malloc(nobj*sizeof(int *));
    dist = (int *)malloc(front_size*sizeof(int));
    for (i=0; i<nobj; i++)
    {
        obj_array[i] = (int *)malloc(front_size*sizeof(int));
    }
    for (j=0; j<front_size; j++)
    {
        dist[j] = temp->index;
        temp = temp->child;
    }
    for (i=0; i<nobj; i++)
    {
        for (j=0; j<front_size; j++)
        {
            obj_array[i][j] = dist[j];
        }
        quicksort_front_obj (pop, i, obj_array[i], front_size);
    }
    for (j=0; j<front_size; j++)
    {
        pop->ind[dist[j]]->setCrowdingDistance(0.0);
    }
    for (i=0; i<nobj; i++)
    {
        pop->ind[obj_array[i][0]]->setCrowdingDistance(large);
    }
    for (i=0; i<nobj; i++)
    {
        for (j=1; j<front_size-1; j++)
        {
            if (pop->ind[obj_array[i][j]]->getCrowdingDistance() != large)
            {
                if (pop->ind[obj_array[i][front_size-1]]->getObj()[i] == pop->ind[obj_array[i][0]]->getObj()[i])
                {

                    pop->ind[obj_array[i][j]]->setCrowdingDistance(pop->ind[obj_array[i][j]]->getCrowdingDistance() + 0.0);
                }
                else
                {
                    double val= (pop->ind[obj_array[i][j+1]]->getObj()[i] - pop->ind[obj_array[i][j-1]]->getObj()[i])/(pop->ind[obj_array[i][front_size-1]]->getObj()[i] - pop->ind[obj_array[i][0]]->getObj()[i]);
                    pop->ind[obj_array[i][j]]->setCrowdingDistance(pop->ind[obj_array[i][j]]->getCrowdingDistance() + val);
                }

            }
        }
    }
    for (j=0; j<front_size; j++)
    {
        if (pop->ind[dist[j]]->getCrowdingDistance() !=large)
        {
            pop->ind[dist[j]]->setCrowdingDistance((pop->ind[dist[j]]->getCrowdingDistance())/nobj);
        }
    }
    free (dist);
    for (i=0; i<nobj; i++)
    {
        free (obj_array[i]);
    }
    free (obj_array);

    return;
}

void OmniOptimizer::assign_crowding_distance_list_realvar (OmniOptimizer::population *pop, list *lst, int front_size)
{
    int **obj_array;
    int *dist;
    int i, j;
    list *temp;
    temp = lst;
    if (front_size==1)
    {
        pop->ind[lst->index]->realvar_crowd_dist = large;
        return;
    }
    if (front_size==2)
    {
        pop->ind[lst->index]->realvar_crowd_dist = large;
        pop->ind[lst->child->index]->realvar_crowd_dist = large;
        return;
    }
    obj_array = (int **)malloc(nreal*sizeof(int *));
    dist = (int *)malloc(front_size*sizeof(int));
    for (i=0; i<nreal; i++)
    {
        obj_array[i] = (int *)malloc(front_size*sizeof(int));
    }
    for (j=0; j<front_size; j++)
    {
        dist[j] = temp->index;
        temp = temp->child;
    }
    for (i=0; i<nreal; i++)
    {
        for (j=0; j<front_size; j++)
        {
            obj_array[i][j] = dist[j];
        }
        quicksort_front_realvar (pop, i, obj_array[i], front_size);
    }
    for (j=0; j<front_size; j++)
    {
        pop->ind[dist[j]]->realvar_crowd_dist = 0.0;
    }
    for (i=0; i<nreal; i++)
    {
        if (pop->ind[obj_array[i][front_size-1]]->getRealVar()[i] == pop->ind[obj_array[i][0]]->getRealVar()[i])
        {
            pop->ind[obj_array[i][0]]->realvar_crowd_dist += 0.0;
            pop->ind[obj_array[i][front_size-1]]->realvar_crowd_dist += 0.0;
        }
        else
        {
            pop->ind[obj_array[i][0]]->realvar_crowd_dist += 2.0*(pop->ind[obj_array[i][1]]->getRealVar()[i] - pop->ind[obj_array[i][0]]->getRealVar()[i])/(pop->ind[obj_array[i][front_size-1]]->getRealVar()[i] - pop->ind[obj_array[i][0]]->getRealVar()[i]);
            pop->ind[obj_array[i][front_size-1]]->realvar_crowd_dist += 2.0*(pop->ind[obj_array[i][front_size-1]]->getRealVar()[i] - pop->ind[obj_array[i][front_size-2]]->getRealVar()[i])/(pop->ind[obj_array[i][front_size-1]]->getRealVar()[i] - pop->ind[obj_array[i][0]]->getRealVar()[i]);
        }
        for (j=1; j<front_size-1; j++)
        {
            if (pop->ind[obj_array[i][j]]->realvar_crowd_dist != large)
            {
                if (pop->ind[obj_array[i][front_size-1]]->getRealVar()[i] == pop->ind[obj_array[i][0]]->getRealVar()[i])
                {
                    pop->ind[obj_array[i][j]]->realvar_crowd_dist += 0.0;
                }
                else
                {
                    pop->ind[obj_array[i][j]]->realvar_crowd_dist += (pop->ind[obj_array[i][j+1]]->getRealVar()[i] - pop->ind[obj_array[i][j-1]]->getRealVar()[i])/(pop->ind[obj_array[i][front_size-1]]->getRealVar()[i] - pop->ind[obj_array[i][0]]->getRealVar()[i]);
                }

            }
        }
    }
    for (j=0; j<front_size; j++)
    {
        if (pop->ind[dist[j]]->realvar_crowd_dist !=large)
        {
            pop->ind[dist[j]]->realvar_crowd_dist = (pop->ind[dist[j]]->realvar_crowd_dist)/nreal;
        }
    }
    free (dist);
    for (i=0; i<nreal; i++)
    {
        free (obj_array[i]);
    }
    free (obj_array);
    return;
}

void OmniOptimizer::assign_crowding_distance_list_binvar (OmniOptimizer::population *pop, list *lst, int front_size)
{
    int **obj_array;
    int *dist;
    int i, j;
    list *temp;
    temp = lst;
    if (front_size==1)
    {
        pop->ind[lst->index]->binvar_crowd_dist = large;
        return;
    }
    if (front_size==2)
    {
        pop->ind[lst->index]->binvar_crowd_dist = large;
        pop->ind[lst->child->index]->binvar_crowd_dist = large;
        return;
    }
    obj_array = (int **)malloc(nbin*sizeof(int *));
    dist = (int *)malloc(front_size*sizeof(int));
    for (i=0; i<nbin; i++)
    {
        obj_array[i] = (int *)malloc(front_size*sizeof(int));
    }
    for (j=0; j<front_size; j++)
    {
        dist[j] = temp->index;
        temp = temp->child;
    }
    for (i=0; i<nbin; i++)
    {
        for (j=0; j<front_size; j++)
        {
            obj_array[i][j] = dist[j];
        }
        quicksort_front_binvar (pop, i, obj_array[i], front_size);
    }
    for (j=0; j<front_size; j++)
    {
        pop->ind[dist[j]]->binvar_crowd_dist = 0.0;
    }
    for (i=0; i<nbin; i++)
    {
        if (pop->ind[obj_array[i][front_size-1]]->getIntVar()[i] == pop->ind[obj_array[i][0]]->getIntVar()[i])
        {
            pop->ind[obj_array[i][0]]->binvar_crowd_dist += 0.0;
            pop->ind[obj_array[i][front_size-1]]->binvar_crowd_dist += 0.0;
        }
        else
        {
            pop->ind[obj_array[i][0]]->binvar_crowd_dist += 2.0*(pop->ind[obj_array[i][1]]->getIntVar()[i] - pop->ind[obj_array[i][0]]->getIntVar()[i])/(pop->ind[obj_array[i][front_size-1]]->getIntVar()[i] - pop->ind[obj_array[i][0]]->getIntVar()[i]);
            pop->ind[obj_array[i][front_size-1]]->binvar_crowd_dist += 2.0*(pop->ind[obj_array[i][front_size-1]]->getIntVar()[i] - pop->ind[obj_array[i][front_size-2]]->getIntVar()[i])/(pop->ind[obj_array[i][front_size-1]]->getIntVar()[i] - pop->ind[obj_array[i][0]]->getIntVar()[i]);
        }
        for (j=1; j<front_size-1; j++)
        {
            if (pop->ind[obj_array[i][j]]->binvar_crowd_dist != large)
            {
                if (pop->ind[obj_array[i][front_size-1]]->getIntVar()[i] == pop->ind[obj_array[i][0]]->getIntVar()[i])
                {
                    pop->ind[obj_array[i][j]]->binvar_crowd_dist += 0.0;
                }
                else
                {
                    pop->ind[obj_array[i][j]]->binvar_crowd_dist += (pop->ind[obj_array[i][j+1]]->getIntVar()[i] - pop->ind[obj_array[i][j-1]]->getIntVar()[i])/(pop->ind[obj_array[i][front_size-1]]->getIntVar()[i] - pop->ind[obj_array[i][0]]->getIntVar()[i]);
                }

            }
        }
    }
    for (j=0; j<front_size; j++)
    {
        if (pop->ind[dist[j]]->binvar_crowd_dist !=large)
        {
            pop->ind[dist[j]]->binvar_crowd_dist = (pop->ind[dist[j]]->binvar_crowd_dist)/nbin;
        }
    }
    free (dist);
    for (i=0; i<nbin; i++)
    {
        free (obj_array[i]);
    }
    free (obj_array);
    return;
}

void OmniOptimizer::assign_crowding_distance_indices (OmniOptimizer::population *pop, int c1, int c2)
{
    int i;
    int front_size;
    double avg_obj = 0.0;
    double avg_realvar = 0.0;
    double avg_binvar = 0.0;
    double avg_var;
    front_size = c2-c1+1;
    if (var_option==1)
    {
        if (nreal != 0)
        {
            assign_crowding_distance_indices_realvar (pop, c1, c2);
        }
        if (nbin != 0)
        {
            assign_crowding_distance_indices_binvar (pop, c1, c2);
        }
    }
    if (obj_option==1)
    {
        assign_crowding_distance_indices_obj (pop, c1, c2);
    }
    if (var_option==0)
    {
        for (i=0; i<front_size; i++)
        {
            pop->ind[i+c1]->setCrowdingDistance(pop->ind[i+c1]->getCrowdingDistance());
        }
    }
    if (obj_option==0)
    {
        if (nreal==0)
        {
            for (i=0; i<front_size; i++)
            {
                pop->ind[i+c1]->setCrowdingDistance(pop->ind[i+c1]->binvar_crowd_dist);
            }
        }
        if (nbin==0)
        {
            for (i=0; i<front_size; i++)
            {
                pop->ind[i+c1]->setCrowdingDistance(pop->ind[i+c1]->realvar_crowd_dist);
            }
        }
        if (nreal!=0 && nbin!=0)
        {
            for (i=0; i<front_size; i++)
            {
                pop->ind[i+c1]->setCrowdingDistance(pop->ind[i+c1]->realvar_crowd_dist + pop->ind[i+c1]->binvar_crowd_dist);
            }
        }
    }
    if (var_option==1 && obj_option==1)
    {
        for (i=0; i<front_size; i++)
        {
            if (pop->ind[i+c1]->getCrowdingDistance() != large)
            {
                avg_obj += pop->ind[i+c1]->getCrowdingDistance();
            }
            if (nreal!=0)
            {
                if (pop->ind[i+c1]->realvar_crowd_dist != large)
                {
                    avg_realvar += pop->ind[i+c1]->realvar_crowd_dist;
                }
            }
            if (nbin!=0)
            {
                if (pop->ind[i+c1]->binvar_crowd_dist != large)
                {
                    avg_binvar += pop->ind[i+c1]->binvar_crowd_dist;
                }
            }
        }
        avg_obj = avg_obj/front_size;
        avg_realvar = avg_realvar/front_size;
        avg_binvar = avg_binvar/front_size;
        avg_obj = avg_obj/nobj;
        if (nreal!=0)
        {
            avg_realvar = avg_realvar/nreal;
        }
        if (nbin!=0)
        {
            avg_binvar = avg_binvar/nbin;
        }
        avg_var = avg_realvar + avg_binvar;
        for (i=0; i<front_size; i++)
        {
            if (nreal!=0 && nbin!=0)
            {
                if (pop->ind[i+c1]->getCrowdingDistance() > avg_obj || (pop->ind[i+c1]->realvar_crowd_dist+pop->ind[i+c1]->binvar_crowd_dist) > avg_var)
                {
                    pop->ind[i+c1]->setCrowdingDistance(maximum (pop->ind[i+c1]->getCrowdingDistance(), (pop->ind[i+c1]->realvar_crowd_dist+pop->ind[i+c1]->binvar_crowd_dist)));
                }
                else
                {
                    pop->ind[i+c1]->setCrowdingDistance(minimum (pop->ind[i+c1]->getCrowdingDistance(), (pop->ind[i+c1]->realvar_crowd_dist+pop->ind[i+c1]->binvar_crowd_dist)));
                }
            }
            if (nreal==0)
            {
                if (pop->ind[i+c1]->getCrowdingDistance() > avg_obj || pop->ind[i+c1]->binvar_crowd_dist > avg_var)
                {
                    pop->ind[i+c1]->setCrowdingDistance(maximum (pop->ind[i+c1]->getCrowdingDistance(), pop->ind[i+c1]->binvar_crowd_dist));
                }
                else
                {
                    pop->ind[i+c1]->setCrowdingDistance(minimum (pop->ind[i+c1]->getCrowdingDistance(), pop->ind[i+c1]->binvar_crowd_dist));
                }
            }
            if (nbin==0)
            {
                if (pop->ind[i+c1]->getCrowdingDistance() > avg_obj || pop->ind[i+c1]->realvar_crowd_dist > avg_var)
                {
                    pop->ind[i+c1]->setCrowdingDistance(maximum (pop->ind[i+c1]->getCrowdingDistance(), pop->ind[i+c1]->realvar_crowd_dist));
                }
                else
                {
                    pop->ind[i+c1]->setCrowdingDistance(minimum (pop->ind[i+c1]->getCrowdingDistance(), pop->ind[i+c1]->realvar_crowd_dist));
                }
            }
        }
    }
    return;
}

void OmniOptimizer::assign_crowding_distance_indices_obj (OmniOptimizer::population *pop, int c1, int c2)
{
    int **obj_array;
    int *dist;
    int i, j;
    int front_size;
    front_size = c2-c1+1;
    if (front_size==1)
    {
        pop->ind[c1]->setCrowdingDistance(large);
        return;
    }
    if (front_size==2)
    {
        pop->ind[c1]->setCrowdingDistance(large);
        pop->ind[c2]->setCrowdingDistance(large);
        return;
    }
    obj_array = (int **)malloc(nobj*sizeof(int *));
    dist = (int *)malloc(front_size*sizeof(int));
    for (i=0; i<nobj; i++)
    {
        obj_array[i] = (int *)malloc(front_size*sizeof(int));
    }
    for (j=0; j<front_size; j++)
    {
        dist[j] = c1++;
    }
    for (i=0; i<nobj; i++)
    {
        for (j=0; j<front_size; j++)
        {
            obj_array[i][j] = dist[j];
        }
        quicksort_front_obj (pop, i, obj_array[i], front_size);
    }
    for (j=0; j<front_size; j++)
    {
        pop->ind[dist[j]]->setCrowdingDistance(0.0);
    }
    for (i=0; i<nobj; i++)
    {
        pop->ind[obj_array[i][0]]->setCrowdingDistance(large);
    }
    for (i=0; i<nobj; i++)
    {
        for (j=1; j<front_size-1; j++)
        {
            if (pop->ind[obj_array[i][j]]->getCrowdingDistance() != large)
            {
                if (pop->ind[obj_array[i][front_size-1]]->getObj()[i] == pop->ind[obj_array[i][0]]->getObj()[i])
                {
                    pop->ind[obj_array[i][j]]->setCrowdingDistance(pop->ind[obj_array[i][j]]->getCrowdingDistance() + 0.0);
                }
                else
                {
                    double val = (pop->ind[obj_array[i][j+1]]->getObj()[i] - pop->ind[obj_array[i][j-1]]->getObj()[i])/(pop->ind[obj_array[i][front_size-1]]->getObj()[i] - pop->ind[obj_array[i][0]]->getObj()[i]);
                    pop->ind[obj_array[i][j]]->setCrowdingDistance(pop->ind[obj_array[i][j]]->getCrowdingDistance() +val);
                }
            }
        }
    }
    for (j=0; j<front_size; j++)
    {
        if (pop->ind[dist[j]]->getCrowdingDistance() != large)
        {
            pop->ind[dist[j]]->setCrowdingDistance((pop->ind[dist[j]]->getCrowdingDistance())/nobj);
        }
    }
    free (dist);
    for (i=0; i<nobj; i++)
    {
        free (obj_array[i]);
    }
    free (obj_array);
    return;
}

void OmniOptimizer::assign_crowding_distance_indices_realvar (OmniOptimizer::population *pop, int c1, int c2)
{
    int **obj_array;
    int *dist;
    int i, j;
    int front_size;
    front_size = c2-c1+1;
    if (front_size==1)
    {
        pop->ind[c1]->realvar_crowd_dist = large;
        return;
    }
    if (front_size==2)
    {
        pop->ind[c1]->realvar_crowd_dist = large;
        pop->ind[c2]->realvar_crowd_dist = large;
        return;
    }
    obj_array = (int **)malloc(nreal*sizeof(int *));
    dist = (int *)malloc(front_size*sizeof(int));
    for (i=0; i<nreal; i++)
    {
        obj_array[i] = (int *)malloc(front_size*sizeof(int));
    }
    for (j=0; j<front_size; j++)
    {
        dist[j] = c1++;
    }
    for (i=0; i<nreal; i++)
    {
        for (j=0; j<front_size; j++)
        {
            obj_array[i][j] = dist[j];
        }
        quicksort_front_realvar (pop, i, obj_array[i], front_size);
    }
    for (j=0; j<front_size; j++)
    {
        pop->ind[dist[j]]->realvar_crowd_dist = 0.0;
    }
    for (i=0; i<nreal; i++)
    {
        if (pop->ind[obj_array[i][front_size-1]]->getRealVar()[i] == pop->ind[obj_array[i][0]]->getRealVar()[i])
        {
            pop->ind[obj_array[i][0]]->realvar_crowd_dist += 0.0;
            pop->ind[obj_array[i][front_size-1]]->realvar_crowd_dist += 0.0;
        }
        else
        {
            pop->ind[obj_array[i][0]]->realvar_crowd_dist += 2.0*(pop->ind[obj_array[i][1]]->getRealVar()[i] - pop->ind[obj_array[i][0]]->getRealVar()[i])/(pop->ind[obj_array[i][front_size-1]]->getRealVar()[i] - pop->ind[obj_array[i][0]]->getRealVar()[i]);
            pop->ind[obj_array[i][front_size-1]]->realvar_crowd_dist += 2.0*(pop->ind[obj_array[i][front_size-1]]->getRealVar()[i] - pop->ind[obj_array[i][front_size-2]]->getRealVar()[i])/(pop->ind[obj_array[i][front_size-1]]->getRealVar()[i] - pop->ind[obj_array[i][0]]->getRealVar()[i]);
        }
        for (j=1; j<front_size-1; j++)
        {
            if (pop->ind[obj_array[i][j]]->realvar_crowd_dist != large)
            {
                if (pop->ind[obj_array[i][front_size-1]]->getRealVar()[i] == pop->ind[obj_array[i][0]]->getRealVar()[i])
                {
                    pop->ind[obj_array[i][j]]->realvar_crowd_dist += 0.0;
                }
                else
                {
                    pop->ind[obj_array[i][j]]->realvar_crowd_dist += (pop->ind[obj_array[i][j+1]]->getRealVar()[i] - pop->ind[obj_array[i][j-1]]->getRealVar()[i])/(pop->ind[obj_array[i][front_size-1]]->getRealVar()[i] - pop->ind[obj_array[i][0]]->getRealVar()[i]);
                }
            }
        }
    }
    for (j=0; j<front_size; j++)
    {
        if (pop->ind[dist[j]]->realvar_crowd_dist != large)
        {
            pop->ind[dist[j]]->realvar_crowd_dist = (pop->ind[dist[j]]->realvar_crowd_dist)/nreal;
        }
    }
    free (dist);
    for (i=0; i<nreal; i++)
    {
        free (obj_array[i]);
    }
    free (obj_array);
    return;
}

void OmniOptimizer::assign_crowding_distance_indices_binvar (OmniOptimizer::population *pop, int c1, int c2)
{
    int **obj_array;
    int *dist;
    int i, j;
    int front_size;
    front_size = c2-c1+1;
    if (front_size==1)
    {
        pop->ind[c1]->binvar_crowd_dist = large;
        return;
    }
    if (front_size==2)
    {
        pop->ind[c1]->binvar_crowd_dist = large;
        pop->ind[c2]->binvar_crowd_dist = large;
        return;
    }
    obj_array = (int **)malloc(nbin*sizeof(int *));
    dist = (int *)malloc(front_size*sizeof(int));
    for (i=0; i<nbin; i++)
    {
        obj_array[i] = (int *)malloc(front_size*sizeof(int));
    }
    for (j=0; j<front_size; j++)
    {
        dist[j] = c1++;
    }
    for (i=0; i<nbin; i++)
    {
        for (j=0; j<front_size; j++)
        {
            obj_array[i][j] = dist[j];
        }
        quicksort_front_binvar (pop, i, obj_array[i], front_size);
    }
    for (j=0; j<front_size; j++)
    {
        pop->ind[dist[j]]->binvar_crowd_dist = 0.0;
    }
    for (i=0; i<nbin; i++)
    {
        if (pop->ind[obj_array[i][front_size-1]]->getIntVar()[i] == pop->ind[obj_array[i][0]]->getIntVar()[i])
        {
            pop->ind[obj_array[i][0]]->binvar_crowd_dist += 0.0;
            pop->ind[obj_array[i][front_size-1]]->binvar_crowd_dist += 0.0;
        }
        else
        {
            pop->ind[obj_array[i][0]]->binvar_crowd_dist += 2.0*(pop->ind[obj_array[i][1]]->getIntVar()[i] - pop->ind[obj_array[i][0]]->getIntVar()[i])/(pop->ind[obj_array[i][front_size-1]]->getIntVar()[i] - pop->ind[obj_array[i][0]]->getIntVar()[i]);
            pop->ind[obj_array[i][front_size-1]]->binvar_crowd_dist += 2.0*(pop->ind[obj_array[i][front_size-1]]->getIntVar()[i] - pop->ind[obj_array[i][front_size-2]]->getIntVar()[i])/(pop->ind[obj_array[i][front_size-1]]->getIntVar()[i] - pop->ind[obj_array[i][0]]->getIntVar()[i]);
        }
        for (j=1; j<front_size-1; j++)
        {
            if (pop->ind[obj_array[i][j]]->binvar_crowd_dist != large)
            {
                if (pop->ind[obj_array[i][front_size-1]]->getIntVar()[i] == pop->ind[obj_array[i][0]]->getIntVar()[i])
                {
                    pop->ind[obj_array[i][j]]->binvar_crowd_dist += 0.0;
                }
                else
                {
                    pop->ind[obj_array[i][j]]->binvar_crowd_dist += (pop->ind[obj_array[i][j+1]]->getIntVar()[i] - pop->ind[obj_array[i][j-1]]->getIntVar()[i])/(pop->ind[obj_array[i][front_size-1]]->getIntVar()[i] - pop->ind[obj_array[i][0]]->getIntVar()[i]);
                }
            }
        }
    }
    for (j=0; j<front_size; j++)
    {
        if (pop->ind[dist[j]]->binvar_crowd_dist != large)
        {
            pop->ind[dist[j]]->binvar_crowd_dist = (pop->ind[dist[j]]->binvar_crowd_dist)/nbin;
        }
    }
    free (dist);
    for (i=0; i<nbin; i++)
    {
        free (obj_array[i]);
    }
    free (obj_array);
    return;
}
