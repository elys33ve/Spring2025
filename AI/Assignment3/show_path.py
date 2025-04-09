"""
Generate the block steps for the report
"""

on='on'
table='table'
clear='clear'
a='🄰'
b='🄱'
c='🄲'
d='🄳'
e='🄴'
f='🄵'

n = 6
path = [[[on,e,d],[on,d,a],[on,a,table],[on,b,c],[on,c,table],[on,f,table],[clear,e],[clear,b],[clear,f]],
[[clear,d],[on,e,table],[on,d,a],[on,a,table],[on,b,c],[on,c,table],[on,f,table],[clear,e],[clear,b],[clear,f]],
[[clear,d],[on,e,table],[on,d,f],[on,a,table],[on,b,c],[on,c,table],[on,f,table],[clear,e],[clear,b],[clear,a]],
[[clear,d],[on,e,table],[on,d,f],[on,a,b],[on,b,c],[on,c,table],[on,f,table],[clear,e],[clear,a]],
[[clear,d],[on,e,a],[on,d,f],[on,a,b],[on,b,c],[on,c,table],[on,f,table],[clear,e]]]


def blockspace(tblocks, onblocks, n):
    space = [[0 for _ in range(n)] for _ in range(n)]
    for x in tblocks:
        idx = space[n-1].index(0)
        space[n-1][idx] = x
 
    put_blocks = []
    for on in onblocks:
        put = False
        for i in range(n-1,-1,-1):
            if on[1] in space[i]:   # found
                space[i-1][space[i].index(on[1])] = on[0]   
                put = True
                if on in put_blocks:
                    put_blocks.remove(on)
        if put == False and on not in put_blocks:
            put_blocks.append(on)
    # this is terrible but i dont feel like fixing en
    while put_blocks != []:
        for on in put_blocks:
            put = False
            for i in range(n-1,-1,-1):
                if on[1] in space[i]:   # found
                    space[i-1][space[i].index(on[1])] = on[0]   
                    put = True
                    if on in put_blocks:
                        put_blocks.remove(on)
            if put == False and on not in put_blocks:
                put_blocks.append(on)

    return space


def print_steps(arr):
    n = len(arr[0])
    num_arrays = len(arr)
    n_cols = 7

    for start_index in range(0, num_arrays, n_cols):  # 5 states at once / per line
        end_index = min(start_index + n_cols, num_arrays)
        current_arrays = arr[start_index:end_index]

        # print header for current chunk
        header = "\t".join(f"state[{i}]" for i in range(start_index, end_index))
        print(header)

        for row_index in range(n):  # go thru rows
            row_line = ""
            for array in current_arrays:
                row = array[row_index]
                s = " ".join(map(str, row)) + "\t|\t"
                row_line += s.replace('0'," ")
            print(row_line)
        print()

def print_steps2(arr):
    n = len(arr[0])
    num_arrays = len(arr)
    n_cols = 5

    for start_index in range(0, num_arrays, n_cols):  # 5 states at once / per line
        end_index = min(start_index + n_cols, num_arrays)
        current_arrays = arr[start_index:end_index]

        # print header for current chunk
        header = "\t".join(f"state[{i}]" for i in range(start_index, end_index))
        print(header)

        for row_index in range(n):  # go thru rows
            row_line = ""
            for array in current_arrays:
                row = array[row_index]
                s = " ".join(map(str, row)) + "\t"
                row_line += s.replace('0',"🅉")
            print(row_line)
        print()

def print_steps3(arr):
    for i in range(len(arr)):
        state = arr[i]
        print(f"\nstate[{i}]")
        for row in state:
            s = " ".join(map(str, row))
            print(s.replace('0',"🅉"))

def blocksWorldPath(path, n):
    steps = []
    for state in path:
        table_blocks = []
        on_blocks = []

        for x in state:
            if table in x:
                table_blocks.append(x[1])
            elif clear not in x:
                on_blocks.append([x[1], x[2]])
        
        steps.append(blockspace(table_blocks, on_blocks, n))
    print_steps2(steps)
    print(len(path))


blocksWorldPath(path, n)


