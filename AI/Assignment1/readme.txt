
Uses 4 different search algorithms (BFS, DFS, Greedy, A*) to find path 
between a starting Romanian city (initial_state) (from map in RomanianMapProblem)
and a goal city (goal_state).
Outputs the results (path, path cost, number of cities visited) for each search algorithm.

main.py
    in main function:
        initial_state
        goal_state
        limit (for DFS)

    breadth_first_search(problem)
    depth_first_search(problem, limit)
    best_first_search(problem)
    a_star_search(problem)

Node.py:
    Node class
        (state, parent, action, path_cost)
        f - f value = path_cost + heuristic
        expand(problem) - returns successor nodes
        child_node(problem, next_state) - creates and returns new child node
        solution() - returns sequence of actions to reach this node
        path() - returns list of nodes from root to self
        f_value(problem) - calculates the f value from cost + heuristic, updates self.f

Problem.py:
    RomanianMapProblem class
        (initial_state, goal_state)
        map - dictionary of each city, with value as a dict of neighboring cities and distances
        sld_to_bucharest - dictionary of each city and SLD to Bucharest, from textbook
        actions(state) - returns neighboring cities from given city
        goal_test(state) - boolean, checks if given state is goal_state
        step_cost(state, next_state) - returns distance between state and next
        heuristic(state, goal=<[self.goal_state]>) - calculates heuristic value
            estimates SLD to goal city using triangle inequality
            - approach 1: uses Bucharest as reference
            - approach 2: uses a difference reference city to estimate SLD and calculate


