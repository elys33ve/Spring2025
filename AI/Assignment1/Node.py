
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
