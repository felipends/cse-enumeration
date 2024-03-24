#include "./model.hpp"

using namespace operations_research;

Model::Model(Instance i) : instance(i) {
    this->buildModel();
}

void Model::buildModel() {
    int numProfessors = this->instance.getNumProfessors();
    int numSlots = this->instance.getNumSlots();
    std::vector<int> numPapersProfessors(numProfessors, 0);
    for(int i = 0; i < numSlots; i++) {
        numPapersProfessors[this->instance.advisorInSlot[i]]++;
    }

    this->model = new MPSolver("ProfessorsAllocation", MPSolver::CBC_MIXED_INTEGER_PROGRAMMING);

    //initialize variables h_is, x_is, l_is and y_is
    std::vector<std::vector<MPVariable*>> h(numProfessors);
    std::vector<std::vector<MPVariable*>> x(numProfessors);
    std::vector<std::vector<MPVariable*>> l(numProfessors);
    std::vector<std::vector<MPVariable*>> y(numProfessors);
    for (int i = 0; i < numProfessors; i++) {
        h[i] = std::vector<MPVariable*>(numSlots+1);  
        x[i] = std::vector<MPVariable*>(numSlots+1);  
        l[i] = std::vector<MPVariable*>(numSlots+1);  
        y[i] = std::vector<MPVariable*>(numSlots+1);  
        for (int s = 0; s < numSlots+1; s++) {
            char varName[100];
            sprintf(varName, "h(%d,%d)", i, s);
            h[i][s] = model->MakeBoolVar(varName);

            sprintf(varName, "x(%d,%d)", i, s);
            x[i][s] = model->MakeBoolVar(varName);

            sprintf(varName, "l(%d,%d)", i, s);
            l[i][s] = model->MakeBoolVar(varName);

            sprintf(varName, "y(%d,%d)", i, s);
            y[i][s] = model->MakeBoolVar(varName);
        }
    }

    // add constraint 1 professor must be an advisor 
    for (int s = 1; s < numSlots+1; s++) {
        MPConstraint* constraint;
        char name[100];
        for (int i = 0; i < numProfessors; i++) {
            sprintf(name, "slot(%d)_has(%d)", s, i);
            constraint = model->MakeRowConstraint(-model->infinity(), 0, name);
            constraint->SetCoefficient(y[i][s], 1);
            constraint->SetCoefficient(x[i][s], -1);
        }
    }

    // add constraint professors are advisor of their papers  
    for (int i = 0; i < numProfessors; i++) {
        int c = numPapersProfessors[i];
        MPConstraint* constraint;
        char name[100];
        sprintf(name, "prof(%d)_hasC", i);
        constraint = model->MakeRowConstraint(c, c, name);
        for (int s = 1; s < numSlots+1; s++) {
            constraint->SetCoefficient(y[i][s], 1);
        }
    }

    // add constraint every slot has 1 advisor
    for (int s = 1; s < numSlots+1; s++) {
        MPConstraint* constraint;
        char name[100];
        sprintf(name, "slot(%d)_has1", s);
        constraint = model->MakeRowConstraint(1, 1, name);
        for (int i = 0; i < numProfessors; i++) {
            constraint->SetCoefficient(y[i][s], 1);
        }
    }
    
    //add constraint every slot must have 3 professors
    for (int s = 1; s < numSlots+1; s++) {
        MPConstraint* constraint;
        char name[100];
        sprintf(name, "slot(%d)_has3", s);
        constraint = model->MakeRowConstraint(3, 3, name);
        for (int i = 0; i < numProfessors; i++) {
            constraint->SetCoefficient(x[i][s], 1);
        }
    }

    //add constraint every professor must be in less than d slots and more than c
    int bound = ceil((2.0*numSlots)/numProfessors);
    for (int i = 0; i < numProfessors; i++) {
        int numProfessorPapers = numPapersProfessors[i];
        int c = numProfessorPapers > 3 ? numProfessorPapers : 3;
        int d = std::max(numSlots, numProfessorPapers + bound) >= 3 ?std::max(numSlots, numProfessorPapers + bound) : 3;

        MPConstraint* constraint;

        char name[100];
        sprintf(name, "professor(%d)_has(%d)_avals", i, c);
        constraint = model->MakeRowConstraint(c, d, name);

        for (int s = 1; s < numSlots+1; s++) {
            constraint->SetCoefficient(x[i][s], 1);
        }
    }

    //add constraint to count slots gap
    for (int s = 1; s < numSlots+1; s++) {
        for (int i = 0; i < numProfessors; i++) {
            MPConstraint* constraint;

            char name[100];
            sprintf(name, "count_gap_of(%d)_til(%d)", i, s);
            constraint = model->MakeRowConstraint(-(model->infinity()), 1, name);

            for (int sLine = 0; sLine <= s-1; sLine++) {
                constraint->SetCoefficient(x[i][sLine], (double) 1.0/numSlots);
            }
            constraint->SetCoefficient(x[i][s], -numSlots);
            constraint->SetCoefficient(l[i][s], 1);
            constraint->SetCoefficient(h[i][s-1], 1);
            constraint->SetCoefficient(h[i][s], -1);
        }
    }

    //add constraint to know if its the last slot of professor
    for (int s = 1; s < numSlots+1; s++) {
        for (int i = 0; i < numProfessors; i++) {
            MPConstraint* constraint;
            char name[100];
            for (int sLine = s+1; sLine < numSlots+1; sLine++) {
                sprintf(name, "slot(%d)_isLastOf(%d)_and(%d)isnt", s, i, sLine);
                constraint = model->MakeRowConstraint(0, model->infinity(), name);
                constraint->SetCoefficient(x[i][sLine], -1);
                constraint->SetCoefficient(l[i][s], 1);
            }
        }
    }

    //for every professor slot 0 has 0 gaps
    for (int i = 0; i < numProfessors; i++) {
        MPConstraint* constraint;
        char name[100];
        sprintf(name, "h(%d, 0)", i);
        constraint = model->MakeRowConstraint(0, 0, name);
        constraint->SetCoefficient(h[i][0], 1);
    }

    // FO
    MPObjective* objective = model->MutableObjective();
    for (int i = 0; i < numProfessors; i++) {
        for (int s = 0; s < numSlots + 1; s++) {
            objective->SetCoefficient(h[i][s], 1);
        }
    }

    objective->SetMinimization();
}

int Model::solve() {
    MPSolver::ResultStatus status = this->model->Solve();
    if (status == MPSolver::OPTIMAL) {
        return 1;
    } else {
        return 0;
    }
}