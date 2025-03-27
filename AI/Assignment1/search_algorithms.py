import math, heapq


"""
- adjust initial_state and goal_state in main for the starting and end cities
- run python search_algorithms.py
- outputs the path and cities visited for each of the 4 functions
""" 

### ===========================================================================

"""
    Node class
        (state, parent, action, path_cost)
        f - f value = path_cost + heuristic
        expand(problem) - returns successor nodes
        child_node(problem, next_state) - creates and returns new child node
        solution() - returns sequence of actions to reach this node
        path() - returns list of nodes from root to self
        f_value(problem) - calculates the f value from cost + heuristic, updates self.f
"""

class Node:
    def __init__(self, state, parent=None, action=None, path_cost=0):
        self.state = state          # city/state this node represents
        self.parent = parent
        self.action = action        # action that led to this state
        self.path_cost = path_cost  # cost to reach this node from the start
        self.f = None

    def __lt__(self, other):
        """ compares two nodes based on their f-values """
        return self.f < other.f

    def expand(self, problem):
        """ generates successor nodes by applying actions """
        return [self.child_node(problem, action)
                for action in problem.actions(self.state)]

    def child_node(self, problem, next_state):
        """ creates a single child node """
        next_cost = problem.step_cost(self.state, next_state)
        return Node(next_state, self, next_state, self.path_cost + next_cost)

    def solution(self):
        """ returns the sequence of actions to reach this node """
        return [node.state for node in self.path()[0:]]

    def path(self):
        """ returns a list of nodes forming the path from the root to this node """
        node = self
        path_back = []
        while node:
            path_back.append(node)
            node = node.parent
        return list(reversed(path_back))
    
    def f_value(self, problem):
        """ calculates f value """
        self.f = self.path_cost + problem.heuristic(self.state)  
        return self.f

### ===========================================================================


"""
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

****heuristic(state, heuristic_type=1): Computes heuristics using:
    Type 1: Triangle inequality using Bucharest as an intermediary.
    Type 2: Minimum edge cost from the current state and goal state.
"""


class RomanianMapProblem:
    def __init__(self, initial_state, goal_state):
        self.initial_state = initial_state
        self.goal_state = goal_state

        self.map = {
            'Arad': {'Zerind': 75, 'Sibiu': 140, 'Timisoara': 118},
            'Zerind': {'Arad': 75, 'Oradea': 71},
            'Oradea': {'Zerind': 71, 'Sibiu': 151},
            'Sibiu': {'Arad': 140, 'Oradea': 151, 'Fagaras': 99, 'Rimnicu Vilcea': 80},
            'Timisoara': {'Arad': 118, 'Lugoj': 111},
            'Lugoj': {'Timisoara': 111, 'Mehadia': 70},
            'Mehadia': {'Lugoj': 70, 'Drobeta': 75},
            'Drobeta': {'Mehadia': 75, 'Craiova': 120},
            'Craiova': {'Drobeta': 120, 'Rimnicu Vilcea': 146, 'Pitesti': 138},
            'Rimnicu Vilcea': {'Sibiu': 80, 'Craiova': 146, 'Pitesti': 97},
            'Fagaras': {'Sibiu': 99, 'Bucharest': 211},
            'Pitesti': {'Rimnicu Vilcea': 97, 'Craiova': 138, 'Bucharest': 101},
            'Bucharest': {'Fagaras': 211, 'Pitesti': 101, 'Giurgiu': 90, 'Urziceni': 85},
            'Giurgiu': {'Bucharest': 90},
            'Urziceni': {'Bucharest': 85, 'Vaslui': 142, 'Hirsova': 98},
            'Hirsova': {'Urziceni': 98, 'Eforie': 86},
            'Eforie': {'Hirsova': 86},
            'Vaslui': {'Urziceni': 142, 'Iasi': 92},
            'Iasi': {'Vaslui': 92, 'Neamt': 87},
            'Neamt': {'Iasi': 87}
        }

        # straight-line distances (SLD) to Bucharest. h_SLD
        self.sld_to_bucharest = {
            'Arad': 366, 'Bucharest': 0, 'Craiova': 160, 'Drobeta': 242, 'Eforie': 161,
            'Fagaras': 176, 'Giurgiu': 77, 'Hirsova': 151, 'Iasi': 226, 'Lugoj': 244,
            'Mehadia': 241, 'Neamt': 234, 'Oradea': 380, 'Pitesti': 100, 'Rimnicu Vilcea': 193,
            'Sibiu': 253, 'Timisoara': 329, 'Urziceni': 80, 'Vaslui': 199, 'Zerind': 374
        }

    def actions(self, state):
        """ returns the neighboring cities from the given city """
        return list(self.map.get(state, {}).keys())

    def goal_test(self, state):
        """ checks if the given city is the goal city """
        return state == self.goal_state

    def step_cost(self, state, next_state):
        """ returns the distance between current city and next city """
        return self.map.get(state, {}).get(next_state, 0)

    def heuristic(self, state, goal=None, reference_city=None):
        """ calculates the heuristic value for the given state (estimated distance to the goal) """
        if goal == None: 
            goal = self.goal_state  # default to self.goal

        # if the state or goal not in SLD values, return inf
        if state not in self.sld_to_bucharest or goal not in self.sld_to_bucharest:
            return float('inf')

        # if goal is Bucharest, use predefined SLD values
        if goal == "Bucharest":
            return self.sld_to_bucharest.get(state, float('inf'))

        # if goal is not Bucharest, estimate the SLD
        if state in self.sld_to_bucharest:
            # approach 1: use Bucharest as an intermediate reference
            #   SLD(state, goal) >=  | self.sld_to_bucharest[state] - self.sld_to_bucharest[goal] | 
            if reference_city == None:
                return abs(self.sld_to_bucharest[state] - self.sld_to_bucharest[goal])
            # approach 2: use a reference city to estimate SLD
            #   calculates absolute difference in distances to the reference_city 
            #   from the state and the goal, providing a lower bound on the actual distance 
            #   between state and goal.
            elif reference_city in self.sld_to_bucharest:
                return abs(self.sld_to_bucharest[state] - self.sld_to_bucharest[reference_city]) + \
                    abs(self.sld_to_bucharest[goal] - self.sld_to_bucharest[reference_city])

        return math.inf # invalid 
    

### ===========================================================================


"""

Search algorithm functions:
    BFS     breadth_first_search(problem)
    DFS     depth_first_search(problem, limit)
    Best-First      best_first_search(problem)
    A*      a_star_search(problem)

"""


### ========== Breadth-First Search (BFS) ========== ###
bfs_visited = 0
def breadth_first_search(problem):
    """
    - expand shallowest unexpanded node
    - first-in, first-out (FIFO) queue, successors at end
    """
    global bfs_visited
    bfs_visited = 0
    node = Node(problem.initial_state)
    if problem.goal_test(node.state):
        return node  # return solution path

    queue = [node]  # FIFO queue
    explored = set()

    while queue:
        node = queue.pop(0)  # remove the first element
        explored.add(node.state)
        bfs_visited += 1    # incr counter for each visited city

        for child in node.expand(problem):
            if child.state not in explored and child not in queue:  # prevents revisiting **
                if problem.goal_test(child.state):
                    return child
                queue.append(child)

    return None  # no solution found



### ========== Depth-First Search (DFS) ========== ###
dfs_visited = 0
def depth_first_search(problem, limit):
    """
    - expand deepest unexpanded node
    - last-in, first-out (LIFO) queue, successors at front
    """
    global dfs_visited
    dfs_visited = 0
    node = Node(problem.initial_state)
    queue = [(node, limit)]  # LIFO queue
    explored = set()

    while queue:
        node, node_limit = queue.pop()  # pop the last element (LIFO)
        dfs_visited += 1  # increment the counter for each visited city
        if problem.goal_test(node.state):
            return node  # solution found
        if node_limit > 0:
            explored.add(node.state)
            for child in node.expand(problem):
                if child.state not in explored:
                    queue.append((child, node_limit - 1))  # add child with decremented limit

    return None  # no solution found


### ========== Best-First Search (Greedy Algorithm) ========== ###
greedy_visited = 0
def best_first_search(problem):
    """
    - expand the node that appears closest to goal
    - evaluation function h(n) (heuristic) = estimate cost from n to closest goal
    """
    global greedy_visited
    greedy_visited = 0  # reset the counter for each search
    node = Node(problem.initial_state)
    node.f_value(problem)
    queue = [(node.f, node)]  # use a list as a priority queue
    explored = set()

    while queue:
        _, node = min(queue, key=lambda x: x)  # get node with lowest f-value
        queue.remove((_, node))  # remove the best node from the frontier
        greedy_visited += 1  # increment the counter for each visited city
        if problem.goal_test(node.state):
            return node  # solution found

        explored.add(node.state)
        for child in node.expand(problem):
            child.f_value(problem)
            if child.state not in explored:
                queue.append((child.f, child))  # add child to the frontier

    return None  # no solution found


### ========== A* Search ========== ###
a_star_visited = 0
def a_star_search(problem):
    """
    - avoid expanding paths that are already expensive
    - uses f(n) = g(n) + h(n) for evaluation
        - g(n): cost to reach node n
        - h(n): estimated cost to goal from n
    - utilizes an admissible heuristic
    """
    global a_star_visited
    a_star_visited = 0
    node = Node(problem.initial_state)
    node.f_value(problem)  # compute first f-value (f(n) = path_cost(n) + heuristic(n))

    queue = [(node.f, node)]  # priority queue using heapq
    heapq.heapify(queue)
    explored = set()    # visited cities
    cost_tracker = {node.state: node.f}  # track lowest f-cost for each state

    while queue:
        _, node = heapq.heappop(queue)  # get node with lowest f-cost
        if problem.goal_test(node.state):
            return node     # return solution
        
        explored.add(node.state)
        a_star_visited += 1
        for child in node.expand(problem):
            child.f_value(problem)      # set child node's f-value

            if child.state in explored:
                continue  # ignore already explored states

            if child.state not in cost_tracker or child.f < cost_tracker[child.state]:
                # if the new path is better, update the queue and tracker
                cost_tracker[child.state] = child.f
                heapq.heappush(queue, (child.f, child))    
    return None     # no solution found


### ===========================================================================
### ===========================================================================

if __name__=="__main__":
    initial_state = 'Neamt'
    goal_state = 'Arad'
    problem = RomanianMapProblem(initial_state, goal_state)
    heuristic_value = problem.heuristic(initial_state, goal_state)#, reference_city='Pitesti')

    print(f"Heuristic value ({initial_state}, {goal_state}): {heuristic_value}\n")

    # BFS
    print("===== Breadth-First Search")
    bfs = breadth_first_search(problem)
    if bfs:
        print(f"path: {bfs.solution()} \ncost: {bfs.path_cost} \nvisited: {bfs_visited}")

    # DFS
    print("===== Depth-First Search")
    limit = 20
    dfs = depth_first_search(problem, limit)
    if dfs == 'cutoff':
        print(f"cutoff {limit}")
    elif dfs:
        print(f"path: {dfs.solution()} \ncost: {dfs.path_cost} \nvisited: {dfs_visited}")

    # Best-First (Greedy Algorithm)
    print("===== Best-First (Greedy Algorithm)")
    greedy = best_first_search(problem)
    if type(greedy) == str:
        print(greedy)
    else:
        print(f"path: {greedy.solution()} \ncost: {greedy.path_cost} \nvisited: {greedy_visited}")

    # A* Search
    print("===== A* Search")
    a_star = a_star_search(problem)
    if a_star:
        print(f"path: {a_star.solution()} \ncost: {a_star.path_cost} \nvisited: {a_star_visited}")



