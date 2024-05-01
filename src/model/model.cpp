#include "./model.hpp"

using namespace operations_research;

Model::Model(Instance i) : instance(i) {
    this->buildModel();
}

void Model::buildModel(int hmaxValue, bool rebuild) {
    int numProfessors = this->instance.getNumProfessors();
    int numSlots = this->instance.getNumSlots();
    std::vector<int> numPapersProfessors(numProfessors, 0);
    for(int i = 0; i < numProfessors; i++) {
        numPapersProfessors[i] = this->instance.getProfessor(i)->getMinimumSlots();
    }

    this->model = std::unique_ptr<MPSolver>(MPSolver::CreateSolver("CBC"));

    //initialize h_max variable
    MPVariable* h_max = model->MakeIntVar(0, model->infinity(), "h_max");

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
            std::snprintf(varName, 100, "h(%d,%d)", i, s);
            h[i][s] = model->MakeBoolVar(varName);

            std::snprintf(varName, 100, "x(%d,%d)", i, s);
            x[i][s] = model->MakeBoolVar(varName);

            std::snprintf(varName, 100, "l(%d,%d)", i, s);
            l[i][s] = model->MakeBoolVar(varName);

            std::snprintf(varName, 100, "y(%d,%d)", i, s);
            y[i][s] = model->MakeBoolVar(varName);
        }
    }

    // add constraint 1 professor must be an advisor 
    for (int s = 1; s < numSlots+1; s++) {
        MPConstraint* constraint;
        char name[100];
        for (int i = 0; i < numProfessors; i++) {
            std::snprintf(name, 100, "slot(%d)_has(%d)", s, i);
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
        std::snprintf(name, 100, "prof(%d)_hasC", i);
        constraint = model->MakeRowConstraint(c, c, name);
        for (int s = 1; s < numSlots+1; s++) {
            constraint->SetCoefficient(y[i][s], 1);
        }
    }

    // add constraint every slot has 1 advisor
    for (int s = 1; s < numSlots+1; s++) {
        MPConstraint* constraint;
        char name[100];
        std::snprintf(name, 100, "slot(%d)_has1", s);
        constraint = model->MakeRowConstraint(1, 1, name);
        for (int i = 0; i < numProfessors; i++) {
            constraint->SetCoefficient(y[i][s], 1);
        }
    }
    
    //add constraint every slot must have 3 professors
    for (int s = 1; s < numSlots+1; s++) {
        MPConstraint* constraint;
        char name[100];
        std::snprintf(name, 100, "slot(%d)_has3", s);
        constraint = model->MakeRowConstraint(3, 3, name);
        for (int i = 0; i < numProfessors; i++) {
            constraint->SetCoefficient(x[i][s], 1);
        }
    }

    //add constraint every professor must be in less than d slots and more than c
    int bound = ceil((2.0*numSlots)/numProfessors);
    for (int i = 0; i < numProfessors; i++) {
        int numProfessorPapers = numPapersProfessors[i];
        int c = numProfessorPapers;
        int d = this->instance.getProfessor(i)->getMaximumSlots();

        MPConstraint* constraint;

        char name[100];
        std::snprintf(name, 100, "professor(%d)_has(%d)_avals", i, c);
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
            std::snprintf(name, 100, "count_gap_of(%d)_til(%d)", i, s);
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
    for (int s = 0; s < numSlots; s++) {
        for (int i = 0; i < numProfessors; i++) {
            for (int sLine = s+1; sLine < numSlots+1; sLine++) {
                MPConstraint* constraint;
                char name[100];

                std::snprintf(name, 100, "slot(%d)_isLastOf(%d)_and(%d)isnt", s, i, sLine);
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
        std::snprintf(name, 100, "h(%d, 0)", i);
        constraint = model->MakeRowConstraint(0, 0, name);
        constraint->SetCoefficient(h[i][0], 1);
    }

    //add constraint h_max should be the maximum gap
    for (int i = 0; i < numProfessors; i++) {
        for (int s = 1; s < numSlots+1; s++) {
            MPConstraint* constraint;
            char name[100];
            std::snprintf(name, 100, "h_max_is_greater_than_h(%d,%d)", i, s);
            constraint = model->MakeRowConstraint(0, model->infinity(), name);
            constraint->SetCoefficient(h_max, 1);
            constraint->SetCoefficient(h[i][s], -1);
        }
    }

    // FO
    MPObjective* objective = model->MutableObjective();

    if (rebuild) {
        //add constraint h_max should be the maximum gap
        for (int i = 0; i < numProfessors; i++) {
            for (int s = 1; s < numSlots+1; s++) {
                MPConstraint* constraint;
                char name[100];
                std::snprintf(name, 100, "h_max_value_is_greater_than_h(%d,%d)", i, s);
                constraint = model->MakeRowConstraint(0, hmaxValue, name);
                constraint->SetCoefficient(h[i][s], -1);
            }
        }

        //rewrites the objective function
        for (int i = 0; i < numProfessors; i++) {
            for (int s = 0; s < numSlots + 1; s++) {
                objective->SetCoefficient(h[i][s], 1);
            }
        }
    } else {
        objective->SetCoefficient(h_max, 1);
    }

    objective->SetMinimization();
}

int Model::solve() {
    MPSolver::ResultStatus status = this->model->Solve();
    if (status == MPSolver::OPTIMAL) {
        int value = this->model->Objective().Value();
        return value;
    } else {
        return -1;
    }
}

void Model::rebuildModel(int hmaxValue) {
    this->buildModel(hmaxValue, true);
}

void Model::printSolution() {
    int numProfessors = this->instance.getNumProfessors();
    int numSlots = this->instance.getNumSlots();
    std::vector<std::vector<int>> solution(numProfessors, std::vector<int>(numSlots+1, 0));
    std::vector<MPVariable*> variables = this->model->variables();
    for (int i = 0; i < numProfessors; i++) {
        for (int s = 0; s < numSlots+1; s++) {
            for (int j = 0; (size_t) j < variables.size(); j++) {
                if (variables[j]->name() == "x(" + std::to_string(i) + "," + std::to_string(s) + ")" && variables[j]->solution_value() == 1) {
                    solution[i][s] = variables[j]->solution_value();
                    break;
                }
            }
        }
    }

    for (int s = 1; s < numSlots+1; s++) {
        for (int i = 0; i < numProfessors; i++) {
            if (solution[i][s] == 1) {
                std::cout << i+1 << " ";
            }
        }
        std::cout << std::endl;
    }
}

std::string Model::getSolutionAsJSON() {
    int numProfessors = this->instance.getNumProfessors();
    int numSlots = this->instance.getNumSlots();
    std::vector<std::vector<int>> solution(numProfessors, std::vector<int>(numSlots+1, 0));
    std::vector<MPVariable*> variables = this->model->variables();
    int solutionValue = this->model->Objective().Value();
    for (int i = 0; i < numProfessors; i++) {
        for (int s = 0; s < numSlots+1; s++) {
            for (int j = 0; (size_t) j < variables.size(); j++) {
                if (variables[j]->name() == "x(" + std::to_string(i) + "," + std::to_string(s) + ")" && variables[j]->solution_value() == 1) {
                    solution[i][s] = variables[j]->solution_value();
                    break;
                }
            }
        }
    }

    std::string json = "{\n";
    json += "\"instance\": \"" + instance.toString() + "\",\n";
    json += "\"solution\": ";
    json += "{\n";
    json += "\"value\": " + std::to_string(solutionValue) + ",\n";
    json += "\"assignment\": ";
    json += "[\n";
    for (int s = 1; s < numSlots+1; s++) {
        json += "[";
        for (int i = 0; i < numProfessors; i++) {
            if (solution[i][s] == 1) {
                json += std::to_string(i+1) + ", ";
            }
        }
        json.pop_back();
        json.pop_back();
        json += "],\n";
    }
    json.pop_back();
    json.pop_back();
    json += "\n]\n}\n}";

    return json;
}