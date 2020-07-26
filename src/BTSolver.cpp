#include"BTSolver.hpp"

using namespace std;

// =====================================================================
// Constructors
// =====================================================================

BTSolver::BTSolver ( SudokuBoard input, Trail* _trail,  string val_sh, string var_sh, string cc )
: sudokuGrid( input.get_p(), input.get_q(), input.get_board() ), network( input )
{
	valHeuristics = val_sh;
	varHeuristics = var_sh; 
	cChecks =  cc;

	trail = _trail;
}

// =====================================================================
// Consistency Checks
// =====================================================================

// Basic consistency check, no propagation done
bool BTSolver::assignmentsCheck ( void )
{
	for ( Constraint c : network.getConstraints() )
		if ( ! c.isConsistent() )
			return false;

	return true;
}

// =================================================================
// Arc Consistency
// =================================================================
bool BTSolver::arcConsistency ( void )
{
    vector<Variable*> toAssign;
    vector<Constraint*> RMC = network.getModifiedConstraints();
    for (int i = 0; i < RMC.size(); ++i)
    {
        vector<Variable*> LV = RMC[i]->vars;
        for (int j = 0; j < LV.size(); ++j)
        {
            if(LV[j]->isAssigned())
            {
                vector<Variable*> Neighbors = network.getNeighborsOfVariable(LV[j]);
                int assignedValue = LV[j]->getAssignment();
                for (int k = 0; k < Neighbors.size(); ++k)
                {
                    Domain D = Neighbors[k]->getDomain();
                    if(D.contains(assignedValue))
                    {
                        if (D.size() == 1)
                            return false;
                        if (D.size() == 2)
                            toAssign.push_back(Neighbors[k]);
                        trail->push(Neighbors[k]);
                        Neighbors[k]->removeValueFromDomain(assignedValue);
                    }
                }
            }
        }
    }
    if (!toAssign.empty())
    {
        for (int i = 0; i < toAssign.size(); ++i)
        {
            Domain D = toAssign[i]->getDomain();
            vector<int> assign = D.getValues();
            trail->push(toAssign[i]);
            toAssign[i]->assignValue(assign[0]);
        }
        return arcConsistency();
    }
    return network.isConsistent();
}

/**
 * Part 1 TODO: Implement the Forward Checking Heuristic
 *
 * This function will do both Constraint Propagation and check
 * the consistency of the network
 *
 * (1) If a variable is assigned then eliminate that value from
 *     the square's neighbors.
 *
 * Note: remember to trail.push variables before you change their domain
 * Return: a pair of a map and a bool. The map contains the pointers to all MODIFIED variables, mapped to their MODIFIED domain. 
 * 		   The bool is true if assignment is consistent, false otherwise.
 */
pair<map<Variable*,Domain>,bool> BTSolver::forwardChecking ( void )
{
	map<Variable*,Domain> myMap;
    for (Constraint* constraint: network.getModifiedConstraints())
    {
        for (Variable* variable: constraint->vars)
        {
            int value = variable->getAssignment();
            for (Variable* neighbor: network.getNeighborsOfVariable(variable))
            {
                if (neighbor->getDomain().contains(value))
                {
                    if (neighbor->getDomain().size() == 1)
                    {
                        return make_pair(myMap, false);
                    }
                    trail->push(neighbor);
                    neighbor->removeValueFromDomain(value);
                    if(myMap.count(neighbor) != 0)
                    {
                        myMap.at(neighbor) = neighbor->getDomain();
                    }
                    else
                    {
                        myMap.insert(pair<Variable*,Domain>(neighbor, neighbor->getDomain()));
                    }
                }
            }
        }
    }
    return make_pair(myMap, true);
}

/**
 * Part 2 TODO: Implement both of Norvig's Heuristics
 *
 * This function will do both Constraint Propagation and check
 * the consistency of the network
 *
 * (1) If a variable is assigned then eliminate that value from
 *     the square's neighbors.
 *
 * (2) If a constraint has only one possible place for a value
 *     then put the value there.
 *
 * Note: remember to trail.push variables before you change their domain
 * Return: a pair of a map and a bool. The map contains the pointers to all variables that were assigned during 
 *         the whole NorvigCheck propagation, and mapped to the values that they were assigned. 
 *         The bool is true if assignment is consistent, false otherwise.
 */
pair<map<Variable*,int>,bool> BTSolver::norvigCheck ( void )
{
    // (1) If a variable is assigned then eliminate that value from
    // the variable's neighbours: Forward Checking
    map<Variable*,int> myMap;
    for (Constraint* constraint: network.getModifiedConstraints())
    {
        for (Variable* variable: constraint->vars)
        {
            int value = variable->getAssignment();
            for (Variable* neighbor: network.getNeighborsOfVariable(variable))
            {
                if (neighbor->getDomain().contains(value))
                {
                    if (neighbor->getDomain().size() == 1)
                    {
                        return make_pair(myMap, false);
                    }
                    trail->push(neighbor);
                    neighbor->removeValueFromDomain(value);
                }
            }
        }
    }

    // (2) If a constraint has only one possible place for a value
    // then put the value there
    vector<int> count;
    count.resize(sudokuGrid.get_n(), 0);

    // initialize an array counting # of occurrences for each value
    // for each variable in the constraint, find its domain, increment the
    // count for each value in the domain by 1
    for (Constraint constraint: network.getConstraints()) {
        for(Variable* var: constraint.vars)
        {
            if(!var->isAssigned())
            {
                Domain::ValueSet values = var->getDomain().getValues();
                for ( int i = 0; i < values.size(); i++ )
                {
                    int val = values[i];
                    count[val-1] = count[val-1] + 1;
                }
            }
        }
        // iterate through the count vector: if count = 1, figure out the variable,
        // trail push and make assignment
        for(int i = 0; i < sudokuGrid.get_n(); i++)
        {
            if (count[i] == 1)
            {
                for (Variable* var: constraint.vars)
                {
                    if (var->getDomain().contains(i+1))
                    {
                        trail->push(var);
                        var->assignValue(i+1);
                        myMap[var] = i+1;
                        return make_pair(myMap, network.isConsistent());
                    }
                }
            }
        }
    }
    // Check consistency of the network
    bool isConsistent = network.isConsistent();

    return make_pair(myMap, isConsistent);
}

/**
 * Optional TODO: Implement your own advanced Constraint Propagation
 *
 * Completing the three tourn heuristic will automatically enter
 * your program into a tournament.
 */
bool BTSolver::getTournCC ( void )
{
	// (1) If a variable is assigned then eliminate that value from
    // the variable's neighbours: Forward Checking
    for (Constraint* constraint: network.getModifiedConstraints())
    {
        for (Variable* variable: constraint->vars)
        {
            int value = variable->getAssignment();
            for (Variable* neighbor: network.getNeighborsOfVariable(variable))
            {
                if (neighbor->getDomain().contains(value))
                {
                    if (neighbor->getDomain().size() == 1)
                    {
                        return false;
                    }
                    trail->push(neighbor);
                    neighbor->removeValueFromDomain(value);
                }
            }
        }
    }

    // (2) If a constraint has only one possible place for a value
    // then put the value there
    vector<int> count;
    count.resize(sudokuGrid.get_n(), 0);

    // initialize an array counting # of occurrences for each value
    // for each variable in the constraint, find its domain, increment the
    // count for each value in the domain by 1
    for (Constraint constraint: network.getConstraints()) {
        for(Variable* var: constraint.vars)
        {
            if(!var->isAssigned())
            {
                Domain::ValueSet values = var->getDomain().getValues();
                for ( int i = 0; i < values.size(); i++ )
                {
                    int val = values[i];
                    count[val-1] = count[val-1] + 1;
                }
            }
        }
        // iterate through the count vector: if count = 1, figure out the variable,
        // trail push and make assignment
        for(int i = 0; i < sudokuGrid.get_n(); i++)
        {
            if (count[i] == 1)
            {
                for (Variable* var: constraint.vars)
                {
                    if (var->getDomain().contains(i+1))
                    {
                        trail->push(var);
                        var->assignValue(i+1);
                        return network.isConsistent();
                    }
                }
            }
        }
    }
    // Check consistency of the network
    bool isConsistent = network.isConsistent();

    return isConsistent;
}

// =====================================================================
// Variable Selectors
// =====================================================================

// Basic variable selector, returns first unassigned variable
Variable* BTSolver::getfirstUnassignedVariable ( void )
{
	for ( Variable* v : network.getVariables() )
		if ( !(v->isAssigned()) )
			return v;

	// Everything is assigned
	return nullptr;
}

/**
 * Part 1 TODO: Implement the Minimum Remaining Value Heuristic
 *
 * Return: The unassigned variable with the smallest domain
 */
Variable* BTSolver::getMRV ( void )
{
    Variable* var = getfirstUnassignedVariable();
    if(!var)
    {
        return nullptr;
    }
    int size = var->size();
    for(Variable* v: network.getVariables())
    {
        int tmpSize = v->size();
        if(tmpSize<size && !(v->isAssigned()))
        {
            size = v->size();
            var = v;
        }
    }
    return var;
}

/**
 * Part 2 TODO: Implement the Minimum Remaining Value Heuristic
 *                with Degree Heuristic as a Tie Breaker
 *
 * Return: The unassigned variable with the smallest domain and affecting the most unassigned neighbors.
 * 		   If there are multiple variables that have the same smallest domain with the same number 
 * 		   of unassigned neighbors, add them to the vector of Variables.
 *         If there is only one variable, return the vector of size 1 containing that variable.
 */
vector<Variable*> BTSolver::MRVwithTieBreaker ( void )
{
    vector<Variable*> res;
    Variable* var = getfirstUnassignedVariable();
    if (!var)
    {
        res.push_back(nullptr);
        return res;
    }
    
    //get all variables with the same smallest domains
    vector<Variable*> smallestVars;
    int minSize = var->size();
    for(Variable* v: network.getVariables())
    {
        if(!v->isAssigned())
        {
            int tmpSize = v->size();
            if(tmpSize < minSize)
            {
                smallestVars.clear();
                smallestVars.push_back(v);
                minSize = tmpSize;
            }
            else if(tmpSize == minSize)
            {
                smallestVars.push_back(v);
            }
        }
    }
    
    //get the variables that have the same smallest domain with the same number of unassigned neighbors
    int maxSize = 0;
    for(Variable* sv: smallestVars)
    {
        int count = 0;
        for(Variable* nv: network.getNeighborsOfVariable(sv))
        {
            if(!nv->isAssigned())
            {
                count++;
            }
        }
        if(count > maxSize)
        {
            res.clear();
            res.push_back(sv);
            maxSize = count;
        }
        else if(count == maxSize)
        {
            res.push_back(sv);
        }
    }

    return res;
}

/**
 * Optional TODO: Implement your own advanced Variable Heuristic
 *
 * Completing the three tourn heuristic will automatically enter
 * your program into a tournament.
 */
Variable* BTSolver::getTournVar ( void )
{
	Variable* var = getfirstUnassignedVariable();
    if(!var)
    {
        return nullptr;
    }
    int size = var->size();
    for(Variable* v: network.getVariables())
    {
        int tmpSize = v->size();
        if(tmpSize<size && !(v->isAssigned()))
        {
            size = v->size();
            var = v;
        }
    }
    return var;
}

// =====================================================================
// Value Selectors
// =====================================================================

// Default Value Ordering
vector<int> BTSolver::getValuesInOrder ( Variable* v )
{
	vector<int> values = v->getDomain().getValues();
	sort( values.begin(), values.end() );
	return values;
}

/**
 * Part 1 TODO: Implement the Least Constraining Value Heuristic
 *
 * The Least constraining value is the one that will knock the least
 * values out of it's neighbors domain.
 *
 * Return: A list of v's domain sorted by the LCV heuristic
 *         The LCV is first and the MCV is last
 */
vector<int> BTSolver::getValuesLCVOrder ( Variable* v )
{
    //<value, how many unassigned neighbors have this value>
    map<int, int> countMap;
    Domain::ValueSet valueSet = v->getDomain().getValues();

    //initialize the counter
    for (int value: valueSet)
        countMap.insert(std::pair<int, int>(value, 0));

    //get all the neighbours
    ConstraintNetwork::VariableSet neighbors = network.getNeighborsOfVariable(v);

    //for each neighbour, count occurrence of the values v has, the more counts, the more constrained
    for (Variable* neigh: neighbors)
    {
        Domain::ValueSet neighValues = neigh->getValues();
        for (int domainVal: neighValues)
            if (countMap.count(domainVal) > 0)
                countMap[domainVal]++;
    }

    //sort the map

    //reverse key-value in a vector, so as to sort by map's value, which is vector's key
    vector<std::pair<int, int>> reverseMap;
    for (std::map<int,int>::iterator it=countMap.begin(); it!=countMap.end(); ++it)
    {
        std::pair<int, int> tmp;
        tmp.first = it->second;
        tmp.second = it->first;
        reverseMap.push_back(tmp);
    }

    //sort
    std::sort(reverseMap.begin(), reverseMap.end());

    vector<int> res;

    // Iterate over the vector of pairs using range base for loop
    for (std::pair<int, int> element: reverseMap)
        res.push_back(element.second);

    return res;
}

/**
 * Optional TODO: Implement your own advanced Value Heuristic
 *
 * Completing the three tourn heuristic will automatically enter
 * your program into a tournament.
 */
vector<int> BTSolver::getTournVal ( Variable* v )
{
	//<value, how many unassigned neighbors have this value>
    map<int, int> countMap;
    Domain::ValueSet valueSet = v->getDomain().getValues();

    //initialize the counter
    for (int value: valueSet)
        countMap.insert(std::pair<int, int>(value, 0));

    //get all the neighbours
    ConstraintNetwork::VariableSet neighbors = network.getNeighborsOfVariable(v);

    //for each neighbour, count occurrence of the values v has, the more counts, the more constrained
    for (Variable* neigh: neighbors)
    {
        Domain::ValueSet neighValues = neigh->getValues();
        for (int domainVal: neighValues)
            if (countMap.count(domainVal) > 0)
                countMap[domainVal]++;
    }

    //sort the map

    //reverse key-value in a vector, so as to sort by map's value, which is vector's key
    vector<std::pair<int, int>> reverseMap;
    for (std::map<int,int>::iterator it=countMap.begin(); it!=countMap.end(); ++it)
    {
        std::pair<int, int> tmp;
        tmp.first = it->second;
        tmp.second = it->first;
        reverseMap.push_back(tmp);
    }

    //sort
    std::sort(reverseMap.begin(), reverseMap.end());

    vector<int> res;

    // Iterate over the vector of pairs using range base for loop
    for (std::pair<int, int> element: reverseMap)
        res.push_back(element.second);

    return res;
}

// =====================================================================
// Engine Functions
// =====================================================================

int BTSolver::solve ( float time_left)
{
	if (time_left <= 60.0)
		return -1;
	double elapsed_time = 0.0;
    clock_t begin_clock = clock();

	if ( hasSolution )
		return 0;

	// Variable Selection
	Variable* v = selectNextVariable();

	if ( v == nullptr )
	{
		for ( Variable* var : network.getVariables() )
		{
			// If all variables haven't been assigned
			if ( ! ( var->isAssigned() ) )
			{
				return 0;
			}
		}

		// Success
		hasSolution = true;
		return 0;
	}

	// Attempt to assign a value
	for ( int i : getNextValues( v ) )
	{
		// Store place in trail and push variable's state on trail
		trail->placeTrailMarker();
		trail->push( v );

		// Assign the value
		v->assignValue( i );

		// Propagate constraints, check consistency, recurse
		if ( checkConsistency() ) {
			clock_t end_clock = clock();
			elapsed_time += (float)(end_clock - begin_clock)/ CLOCKS_PER_SEC;
			double new_start_time = time_left - elapsed_time;
			int check_status = solve(new_start_time);
			if(check_status == -1) {
			    return -1;
			}
			
		}

		// If this assignment succeeded, return
		if ( hasSolution )
			return 0;

		// Otherwise backtrack
		trail->undo();
	}
	return 0;
}

bool BTSolver::checkConsistency ( void )
{
	if ( cChecks == "forwardChecking" )
		return forwardChecking().second;

	if ( cChecks == "norvigCheck" )
		return norvigCheck().second;

	if ( cChecks == "tournCC" )
		return getTournCC();

	return assignmentsCheck();
}

Variable* BTSolver::selectNextVariable ( void )
{
	if ( varHeuristics == "MinimumRemainingValue" )
		return getMRV();

	if ( varHeuristics == "MRVwithTieBreaker" )
		return MRVwithTieBreaker()[0];

	if ( varHeuristics == "tournVar" )
		return getTournVar();

	return getfirstUnassignedVariable();
}

vector<int> BTSolver::getNextValues ( Variable* v )
{
	if ( valHeuristics == "LeastConstrainingValue" )
		return getValuesLCVOrder( v );

	if ( valHeuristics == "tournVal" )
		return getTournVal( v );

	return getValuesInOrder( v );
}

bool BTSolver::haveSolution ( void )
{
	return hasSolution;
}

SudokuBoard BTSolver::getSolution ( void )
{
	return network.toSudokuBoard ( sudokuGrid.get_p(), sudokuGrid.get_q() );
}

ConstraintNetwork BTSolver::getNetwork ( void )
{
	return network;
}
