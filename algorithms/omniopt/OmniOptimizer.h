#ifndef OmniOptimizer_H_
#define OmniOptimizer_H_

#include "../../core/CGeneticAlgorithm.h"
#include "../../core/CIndividual.h"

#include "../../core/CPopulation.h"

class OmniOptimizer : virtual public MEVO::CGeneticAlgorithm {

public:
    OmniOptimizer();

    virtual ~OmniOptimizer();

    const char* getName() {return "Omni Optimizer";}
	void run();
    bool checkConf();

private:

	void alocateMemory();

    class OmniIndividual : public MEVO::CIndividual
	{
        public:
        OmniIndividual(int _numVar, int _numIntVar, int _numObj, int _numConstr);
        virtual ICIndividual* copy();
        virtual ~OmniIndividual();
        int rank;
        double realvar_crowd_dist;
		double binvar_crowd_dist;
    };

	typedef struct
	{
        OmniIndividual **ind;
	} population;

	typedef struct lists
	{
		int index;
		struct lists *parent;
		struct lists *child;
	} list;


	int nreal;
	int nbin;
	int nobj;
	int ncon;
	int popsize;

    int nbinmut;
    int nrealmut;
    int nbincross;
    int nrealcross;

    double *min_realvar;
	double *max_realvar;
	double *min_binvar;
	double *max_binvar;
	double *min_obj;
	double *max_obj;
//	double *epsilon;
//	double delta;
	int mate;
	int input_type;
	int run_mode;
	int var_option;
	int obj_option;
	int frequency;
	int var1;
	int obj1;
	int obj2;
	int obj3;
	int angle1;
	int angle2;
	int choice;

    void allocate_memory_pop (population *&, int);
    void allocate_memory_ind (OmniIndividual *&);
	void deallocate_memory_pop (population *, int);
    void deallocate_memory_ind (OmniIndividual *);

    void crossover (OmniIndividual *, OmniIndividual *, OmniIndividual *, OmniIndividual *);
    void realcross (OmniIndividual *, OmniIndividual *, OmniIndividual *, OmniIndividual *);
    void bincross (OmniIndividual *, OmniIndividual *, OmniIndividual *, OmniIndividual *);

	void assign_crowding_distance_list (population *, list *, int);
	void assign_crowding_distance_list_obj (population *, list *, int);
	void assign_crowding_distance_list_realvar (population *, list *, int);
	void assign_crowding_distance_list_binvar (population *, list *, int);
	void assign_crowding_distance_indices (population *, int, int);
	void assign_crowding_distance_indices_obj (population *, int, int);
	void assign_crowding_distance_indices_realvar (population *, int, int);
	void assign_crowding_distance_indices_binvar (population *, int, int);

	void decode_pop (population *);
    void decode_ind (OmniIndividual *);

	void onthefly_display (population *, FILE *, int);

	void define_epsilon (population *, int , double *);

	void evaluate_pop (population *);
    void evaluate_ind (OmniIndividual *);

	void fill_nondominated_sort (population *, population *);
	void crowding_fill (population *, population *, int , int , list *);

	void ga_optimizer(void);

	void input(void);

	void initialize_pop (population *);
	void initialize_random_pop (population *);
	void initialize_latin_pop (population *);
	void initialize_file_pop (population *);

	void insert (list *, int);
	list* del (list *);

	void merge(population *, population *, population *);
    void copy_ind (OmniIndividual *, OmniIndividual *);

	void mutation_pop (population *);
    void mutation_ind (OmniIndividual *);
    void real_mutate_ind (OmniIndividual *);

	void output (FILE *, FILE *, FILE *, FILE *, FILE *);

	void pdefinit(void);

	void test_problem (double *, double *, int **, double *, double *);

	void assign_rank_and_crowding_distance (population *);

	void report_pop (population *, FILE *);
	void report_feasible (population *, FILE *);
	void report_var (population *, FILE *);
	void report_obj (population *, FILE *);

	void quicksort_front_obj(population *, int , int *, int);
	void q_sort_front_obj(population *, int , int *, int , int );
	void quicksort_front_realvar(population *, int , int *, int );
	void q_sort_front_realvar(population *, int , int *, int , int );
	void quicksort_front_binvar(population *, int , int *, int );
	void q_sort_front_binvar(population *, int , int *, int , int );
	void quicksort_dist(population *, int *, int );
	void q_sort_dist(population *, int *, int , int );

	void selection (population *, population *);
	void restricted_selection (population *, population *);
    OmniIndividual* tournament (OmniIndividual *, OmniIndividual *);
	list* search_nearest (population *, list *, list *, double *, double *, double *, double *);
	list* search_second_nearest (population *, list *, list *, list *, double *, double *, double *, double *);
	double calc_distance (population *, list *, list *, double *, double *, double *, double *);

	double maximum (double , double);
	double minimum (double , double);
	double square (double);
	double modulus (double *, int);
	double dot (double *, double *, int);
	double mean (double *, int);

    std::deque<double> FEpsilon;


};

#endif /* OmniOptimizer_H_ */
