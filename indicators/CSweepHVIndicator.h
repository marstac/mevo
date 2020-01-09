#ifndef CSWEEPHVINDICATOR_H
#define CSWEEPHVINDICATOR_H

#define AVL_DEPTH
#define VARIANT 4

/*************************************************************************

 hypervolume computation

 ---------------------------------------------------------------------

                       Copyright (c) 2010
                  Carlos M. Fonseca <cmfonsec@ualg.pt>
             Manuel Lopez-Ibanez <manuel.lopez-ibanez@ulb.ac.be>
                    Luis Paquete <paquete@dei.uc.pt>

 This program is free software (software libre); you can redistribute
 it and/or modify it under the terms of the GNU General Public License
 as published by the Free Software Foundation; either version 2 of the
 License, or (at your option) any later version.

 This program is distributed in the hope that it will be useful, but
 WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 General Public License for more details.

 You should have received a copy of the GNU General Public License
 along with this program; if not, you can obtain a copy of the GNU
 General Public License at:
                 http://www.gnu.org/copyleft/gpl.html
 or by writing to:
           Free Software Foundation, Inc., 59 Temple Place,
                 Suite 330, Boston, MA 02111-1307 USA

 ----------------------------------------------------------------------

 Relevant literature:

 [1]  C. M. Fonseca, L. Paquete, and M. Lopez-Ibanez. An
      improved dimension-sweep algorithm for the hypervolume
      indicator. In IEEE Congress on Evolutionary Computation,
      pages 1157-1163, Vancouver, Canada, July 2006.

 [2]  Nicola Beume, Carlos M. Fonseca, Manuel López-Ibáñez, Luís
      Paquete, and J. Vahrenhold. On the complexity of computing the
      hypervolume indicator. IEEE Transactions on Evolutionary
      Computation, 13(5):1075-1082, 2009.

*************************************************************************/

struct avl_node_t {
    struct avl_node_t *next;
    struct avl_node_t *prev;
    struct avl_node_t *parent;
    struct avl_node_t *left;
    struct avl_node_t *right;
    void *item;
    double domr;
#ifdef AVL_DEPTH
    unsigned char depth;
#endif
};

struct avl_tree_t {
    avl_node_t *head;
    avl_node_t *tail;
    avl_node_t *top;
    //avl_compare_t cmp;
    //avl_freeitem_t freeitem;
};

struct dlnode {
    double *x;                    /* The data vector              */
    struct dlnode **next;         /* Next-node vector             */
    struct dlnode **prev;         /* Previous-node vector         */
    struct avl_node_t * tnode;
    int ignore;
    int ignore_best; //used in define_order
#if VARIANT >= 2
    double *area;                 /* Area */
#endif
#if VARIANT >= 3
    double *vol;                  /* Volume */
#endif
};


/*  Author:
  Marcin A. Stachura <m.stachura@mchtr.pw.edu.pl>

  Copyright (c) 2015 Marcin A. Stachura

  This program is free software: you can redistribute it and/or modify
  it under the terms of the GNU Lesser General Public License as published by
  the Free Software Foundation, either version 3 of the License, or
  (at your option) any later version.

  This program is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU Lesser General Public License for more details.

  You should have received a copy of the GNU Lesser General Public License
  along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#include "CHyperVolumeIndicator.h"
namespace MEVO {
class CSweepHVIndicator : public CHyperVolumeIndicator
{
public:
    CSweepHVIndicator();
    ~CSweepHVIndicator();
    virtual const char* getName();
protected:

    double calculateHypervolume(std::deque< std::deque<double> > &_front);
    /** Function returns name of an indicator
      @return algorithm name
     */

    int stop_dimension;
    avl_tree_t *tree;

    double fpli_hv(double *data, int d, int n, const double *ref);


    /* User supplied function to compare two items like strcmp() does.
     * For example: cmp(a,b) will return:
     *   -1  if a < b
     *    0  if a = b
     *    1  if a > b
     */
    int (*avl_compare_t)(const void *, const void *);

    /* User supplied function to delete an item when a node is free()d.
     * If NULL, the item is not free()d.
     */
    void (*avl_freeitem_t)(void *);

    void avl_rebalance(avl_tree_t *, avl_node_t *);
    int avl_check_balance(avl_node_t *avlnode);
    int avl_search_closest(const avl_tree_t *avltree, const void *item, avl_node_t **avlnode);
    avl_tree_t * avl_init_tree(avl_tree_t *rc);
    avl_tree_t * avl_alloc_tree();
    void avl_clear_tree(avl_tree_t *avltree);
    void avl_clear_node(avl_node_t *newnode);
    avl_node_t * avl_insert_top(avl_tree_t *avltree, avl_node_t *newnode);
    avl_node_t * avl_insert_before(avl_tree_t *avltree, avl_node_t *node, avl_node_t *newnode);
    avl_node_t * avl_insert_after(avl_tree_t *avltree, avl_node_t *node, avl_node_t *newnode);
    void avl_unlink_node(avl_tree_t *avltree, avl_node_t *avlnode);

    //int compare_node(const void *p1, const void* p2);
    int compare_tree_asc(const void *p1, const void *p2);

    dlnode * setup_cdllist(double *data, int d, int n);
    void free_cdllist(dlnode * head);

    void ndelete (dlnode *nodep, int dim, double * bound);

    #if VARIANT >= 2
    void delete_dom (dlnode *nodep, int dim);
    void reinsert_dom (dlnode *nodep, int dim);
    #endif

    void reinsert (dlnode *nodep, int dim, double * bound);

    double hv_recursive(dlnode *list, int dim, int c, const double * ref, double * bound);

    void filter_delete_node(dlnode *node, int d);
    int filter(dlnode *list, int d, int n, const double *ref);
#ifdef AVL_DEPTH
    unsigned char NODE_DEPTH(avl_node_t* n)  {if(n) {
                                                return (n)->depth;
                                                }
                                                return 0;}
    unsigned char L_DEPTH(avl_node_t* n)     {return NODE_DEPTH((n)->left);}
    unsigned char R_DEPTH(avl_node_t* n)     {return NODE_DEPTH((n)->right);}
    unsigned char CALC_DEPTH(avl_node_t* n)  {if(L_DEPTH(n)>R_DEPTH(n)) {
                                                    return L_DEPTH(n);
                                                }
                                                    return R_DEPTH(n) + 1;
                                             }
#endif

#ifdef EXPERIMENTAL
    double fpli_hv_order(double *data, int d, int n, const double *ref, int *order,
                     double *order_time, double *hv_time);
    #endif
};

}


#endif // CSWEEPHVINDICATOR_H
