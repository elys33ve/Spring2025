import math, heapq
from Node import *
from Problem import *

"""
- adjust initial_state and goal_state in main for the starting and end cities

--- search algorithm functions:

    BFS     breadth_first_search(problem)

    DFS     depth_first_search(problem, limit)

    Best-First      best_first_search(problem)

    A*      a_star_search(problem)

--- used classes:
    (Node.py) Node class
    (Problem.py) RomanianMapProblem class

"""

### ===========================================================================


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



