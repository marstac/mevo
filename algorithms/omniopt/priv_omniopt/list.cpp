/* A custom doubly linked OmniOptimizer::list implemenation */

#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "../OmniOptimizer.h"

/* Insert an element X into the OmniOptimizer::list at location specified by NODE */
void OmniOptimizer::insert (OmniOptimizer::list *node, int x)
{
    OmniOptimizer::list *temp;
    if (node==NULL)
    {
        printf("\n Error!! asked to enter after a NULL pointer, hence exiting \n");
        exit(1);
    }
    temp = (OmniOptimizer::list *)malloc(sizeof(OmniOptimizer::list));
    temp->index = x;
    temp->child = node->child;
    temp->parent = node;
    if (node->child != NULL)
    {
        node->child->parent = temp;
    }
    node->child = temp;
    return;
}

/* Delte the node NODE from the OmniOptimizer::list */
OmniOptimizer::list* OmniOptimizer::del (OmniOptimizer::list *node)
{
    OmniOptimizer::list *temp;
    if (node==NULL)
    {
        printf("\n Error!! asked to delete a NULL pointer, hence exiting \n");
        exit(1);
    }
    temp = node->parent;
    temp->child = node->child;
    if (temp->child!=NULL)
    {
        temp->child->parent = temp;
    }
    free (node);
    return (temp);
}
