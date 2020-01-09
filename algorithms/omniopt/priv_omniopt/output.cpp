#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "../OmniOptimizer.h"

void OmniOptimizer::output (FILE *fpt1, FILE *fpt2, FILE *fpt3, FILE *fpt4, FILE *fpt5)
{
    int i;
    fprintf(fpt1,"# This file contains the data of initial OmniOptimizer::population\n");
    fprintf(fpt2,"# This file contains the data of final OmniOptimizer::population\n");
    fprintf(fpt3,"# This file contains the data of final feasible OmniOptimizer::population\n");
    fprintf(fpt4,"# This file contains the data of all generations (if required)");
    fprintf(fpt5,"# This file contains the data of inputs given and other simulation parameters\n");

    fprintf(fpt5,"\n OmniOptimizer::population size = %d",popsize);
    fprintf(fpt5,"\n Number of objective functions = %d",nobj);
    fprintf(fpt5,"\n Number of constraints = %d",ncon);
    fprintf(fpt5,"\n Number of real variables = %d",nreal);
    if (nreal!=0)
    {
        for (i=0; i<nreal; i++)
        {
            fprintf(fpt5,"\n Lower limit of real variable %d = %e",i+1,min_realvar[i]);
            fprintf(fpt5,"\n Upper limit of real variable %d = %e",i+1,max_realvar[i]);
        }

    }
    fprintf(fpt5,"\n Number of binary variables = %d",nbin);
    if (nbin!=0)
    {
        for (i=0; i<nbin; i++)
        {
		  //  fprintf(fpt5,"\n Number of bits for binary variable %d = %d",i+1,nbits[i]);
            fprintf(fpt5,"\n Lower limit of binary variable %d = %e",i+1,min_binvar[i]);
            fprintf(fpt5,"\n Upper limit of binary variable %d = %e",i+1,max_binvar[i]);
        }


    }
       // bitlength = 0;
    if (nbin!=0)
    {
//		for (i=0; i<nbin; i++)
//		{
//			bitlength += nbits[i];
//        }
    }
  //  fprintf(fpt1,"# of objectives = %d, # of constraints = %d, # of real_var = %d, # of bits of bin_var = %d, constr_violation, rank, variable_crowding_distance, objective_crowding_distance, overall_crowding_distance\n",nobj,ncon,nreal,bitlength);
   // fprintf(fpt2,"# of objectives = %d, # of constraints = %d, # of real_var = %d, # of bits of bin_var = %d, constr_violation, rank, variable_crowding_distance, objective_crowding_distance, overall_crowding_distance\n",nobj,ncon,nreal,bitlength);
  //  fprintf(fpt3,"# of objectives = %d, # of constraints = %d, # of real_var = %d, # of bits of bin_var = %d, constr_violation, rank, variable_crowding_distance, objective_crowding_distance, overall_crowding_distance\n",nobj,ncon,nreal,bitlength);
  //  fprintf(fpt4,"# of objectives = %d, # of constraints = %d, # of real_var = %d, # of bits of bin_var = %d, constr_violation, rank, variable_crowding_distance, objective_crowding_distance, overall_crowding_distance\n",nobj,ncon,nreal,bitlength);
    fprintf(fpt5,"\n Selection restriction type chosen = %d",mate);
    fprintf(fpt5,"\n Variable niching option = %d",var_option);
    fprintf(fpt5,"\n Objective niching option = %d",obj_option);
    //fprintf(fpt5,"\n Delta for loose-domination = %e",delta);
    fprintf(fpt5,"\n Input data type is given as : %d",input_type);
    fprintf(fpt5,"\n Run mode defined for this run was : %d",run_mode);
    fprintf(fpt5,"\n Frquency of saving the OmniOptimizer::population = %d",frequency);
	fprintf(fpt5,"\n choice = %d",choice);
    return;
}
