#include "schaffer.h"

void solve_SCH() {
    //Problem declaration
    CProblem p;

    //adding objective functions
    p.addObjectiveFnc(new F1);
    p.addObjectiveFnc(new F2);

    //adding constraint functions
    p.addConstraintFnc(new C1);

    //setting real variables size
    p.setRealVarSize(1);

    //setting variable bounds
    p.setRealLowLimit(-1000,0);
    p.setRealHighLimit(1000,0);

    //algorithm
    NSGA2 nsga;
    nsga.setProblem(&p);

    //computation start
    nsga.run();

    nsga.getLastPop()->save("results_sch.txt");

    //Indicator object
    CSweepHVIndicator hvi;

    //asessement of the results
    double nsga2res = hvi.assessPopulation(nsga.getLastPop());

    //delcaration of the second algorithm
    AMGA2 amga2;

    //setting a problem to the second algorithm
    amga2.setProblem(&p);

    //second algorithm computation start
    amga2.run();

    //comparison of the results for the two algorithms
    double comp=hvi.comparePopulations(amga2.getLastPop(), nsga.getLastPop());
}
