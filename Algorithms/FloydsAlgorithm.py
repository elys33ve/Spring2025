from math import inf



D = [   [0,inf,-2,inf,inf,inf],
        [3,0,inf,inf,inf,inf],
        [inf,4,0,inf,1,3],
        [inf,inf,-1,0,1,inf],
        [inf,inf,inf,2,0,1],
        [4,inf,4,inf,inf,0]]
n = len(D)


def S_values():
    S = [row[:] for row in D]

    for k in range(n):
        print(f"\nS{k} =")
        for row in S:
            #print(["∞" if x == inf else x for x in row])
            print([inf if x == inf else x for x in row])
        new_S = [row[:] for row in S]

        for i in range(n):
            for j in range(n):
                if S[i][k] != inf and S[k][j] != inf:
                    if S[i][j] > S[i][k] + S[k][j]:
                        new_S[i][j] = S[i][k] + S[k][j]
        S = new_S


def P_values():
    S = [row[:] for row in D]
    P = [[None if i == j or D[i][j] == inf else i for j in range(n)] for i in range(n)]

    for k in range(n):
        print(f"P{k} =")
        for row in P:
            print([('-' if x is None else x) for x in row])

        new_S = [row[:] for row in S]
        new_P = [row[:] for row in P]

        for i in range(n):
            for j in range(n):
                if S[i][k] != inf and S[k][j] != inf:
                    if S[i][j] > S[i][k] + S[k][j]:
                        new_S[i][j] = S[i][k] + S[k][j]
                        new_P[i][j] = P[k][j]
        S = new_S
        P = new_P

def print_D():
    print("D=")
    for i in range(n):
        print(D[i])

#P_values()
#S_values()
print_D()
