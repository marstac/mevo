# include <stdio.h>
# include <stdlib.h>
# include <math.h>
# include <string.h>

# include "../OmniOptimizer.h"

void OmniOptimizer::onthefly_display (OmniOptimizer::population *pop, FILE *gp, int ii)
{
    int i;
    int flag;
    FILE *fpt;
    fpt = fopen("plotp.out","w");
    flag = 0;
    for (i=0; i<popsize; i++)
    {
        if (pop->ind[i]->getConstrViolation()==0.0)
        {
            if (choice==0)
            {
                fprintf(fpt,"%e\t%e\n",pop->ind[i]->getRealVar()[0],pop->ind[i]->getObj()[0]);
            }
            else
            {
                if (choice==1 || choice==2)
                {
                    fprintf(fpt,"%e\t%e\n",pop->ind[i]->getObj()[obj1-1],pop->ind[i]->getObj()[obj2-1]);
                }
                else
                {
                    fprintf(fpt,"%e\t%e\t%e\n",pop->ind[i]->getObj()[obj1-1],pop->ind[i]->getObj()[obj2-1],pop->ind[i]->getObj()[obj3-1]);
                }
            }
            fflush(fpt);
            flag = 1;
        }
    }
    if (flag==0)
    {
        printf("\n No feasible soln in this pop, hence no display");
    }
    else
    {
        if (choice!=3)
            fprintf(gp,"set title 'Generation #%d'\n unset key\n plot 'plotp.out' w points pointtype 6 pointsize 1\n",ii);
        else
            fprintf(gp,"set title 'Generation #%d'\n set view %d,%d\n unset key\n splot 'plotp.out' w points pointtype 6 pointsize 1\n",ii,angle1,angle2);
        fflush(gp);
    }
    fclose(fpt);
    return;
}
