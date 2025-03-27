import math

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
    
        


