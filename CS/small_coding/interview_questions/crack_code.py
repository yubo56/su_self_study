import numpy as np
def p1_1(in_str):
    '''
    determine if a string has all unique characters

    Notes: linear runtime, linear additional space, uses hash
    '''
    counts = set()
    for char in in_str:
        if char not in counts:
            counts.add(char)
        else:
            return False
    return True

def p1_1_constrained(in_str):
    '''
    solve P 1.1 w/o data structures

    Notes: w/o persist data store, can only examine locally, must sort
        n log n runtime, linear space (no in-place str sort), no data structures
    '''
    in_str = sorted(in_str)
    for idx, char in enumerate(in_str[ :-1]): # check successor, so drop last
        if char == in_str[idx + 1]:
            return False
    return True

def test_p1_1():
    solvers = [p1_1, p1_1_constrained]
    true_cases = ['abcdefg', '', 'a']
    false_cases = ['aaaaa', 'abb', 'aab']

    for solver in solvers:
        for true_case in true_cases:
            assert solver(true_case)
        for false_case in false_cases:
            assert not solver(false_case)

def p1_2(in_str):
    ''' reverses a c style string in place '''
    str_len = len(in_str[ :-1]) # length = w/o null char
    for i in range(str_len // 2): # ignores middle char for odd length
        a = in_str[i]
        in_str[i] = in_str[str_len - i - 1]
        in_str[str_len - i - 1] = a
    return in_str

def test_p1_2():
    def cstr(string):
        return list(string) + [None]

    assert p1_2(cstr('abc')) == cstr('cba')
    assert p1_2(cstr('abcd')) == cstr('dcba')
    assert p1_2([None]) == [None]

def to_tree(in_str, parent=None):
    '''
    converts string to tree as dict w/ nodes {'left', 'right', 'self'}
    'left', 'right' are further dicts, 'self' is a str
    '''
    # split half the time
    if np.random.rand() > 1 or len(in_str) < 3:
        return {'self': in_str, 'left': None, 'right': None, 'parent': parent}
    else:
        splits = sorted(np.random.choice(len(in_str) - 1, 2, replace=False))
        self = {'self': in_str[splits[0] + 1: splits[1] + 1],
                'parent': parent}
        self['left'] = to_tree(in_str[ : splits[0] + 1], self)
        self['right'] = to_tree(in_str[splits[1] + 1: ], self)
        return self

def reduce_tree(tree, reducer, ret):
    '''
    aggregates tree into ret using predicate reducer. preorder traversal
    mutate ret, no return
    '''
    if tree['left'] is None and tree['right'] is None:
        return reducer(tree['self'], ret)
    else:
        ret = reduce_tree(tree['left'], reducer, ret)
        ret = reducer(tree['self'], ret)
        ret = reduce_tree(tree['right'], reducer, ret)
        return ret

def p1_4(tree1, tree2):
    '''
    Extended: determine whether two parse trees are anagrams or not
    HACK: Use dict instead of class for now

    linear runtime, constant space
    '''
    def char_count(in_str, char_count):
        for c in in_str:
            if c not in char_count:
                char_count[c] = 1
            else:
                char_count[c] += 1
        return char_count

    chars1 = reduce_tree(tree1, char_count, {})
    chars2 = reduce_tree(tree2, char_count, {})

    for key in set(chars1.keys()).union(set(chars2.keys())):
        if key not in chars1 or key not in chars2\
                or chars1[key] != chars2[key]:
            return False

    return True

def print_tree(t, indent=''):
    if t is None:
        print('None')
        return
    print(indent + t['self'])
    if t['left'] is not None:
        print_tree(t['left'], indent=indent + '  ')
        print_tree(t['right'], indent=indent + '  ')

def p1_4_mod(tree1, tree2):
    ''' determines whether two parse trees are identical in place '''
    def get_next_char(node, idx):
        ''' given a node and an idx, return its next char + node/idx '''
        if idx < len(node['self']):
            return node['self'][idx], node, idx + 1

        if node['right']:
            child = node['right']
            while child['left'] is not None:
                child = child['left']
            return get_next_char(child, 0)

        # at this point, must be right leaf, go up parents until find something
        parent = node['parent']
        # keep
        while parent is not None and parent['right'] == node:
            node = parent
            parent = parent['parent']
        if parent is None:
            return None, None, None
        # we've just finished a left leg of the parent
        return get_next_char(parent, 0)

    node1 = tree1
    while node1['left'] is not None:
        node1 = node1['left']
    idx1 = 0
    char1, node1, idx1 = get_next_char(node1, idx1)
    node2 = tree2
    while node2['left'] is not None:
        node2 = node2['left']
    idx2 = 0
    char2, node2, idx2 = get_next_char(node2, idx2)

    while char1 is not None and char1 == char2:
        char1, node1, idx1 = get_next_char(node1, idx1)
        char2, node2, idx2 = get_next_char(node2, idx2)
    return char1 == char2

def test_p1_4():
    test_cases = ['abcdef', 'abc', 'a', '', 'abcdefghijklmn']
    for t in test_cases:
        assert p1_4(to_tree(t), to_tree(t))
        assert p1_4_mod(to_tree(t), to_tree(t))
    assert p1_4(to_tree('abcdef'), to_tree('debcfa'))

def arr_to_ll(arr):
    '''
    converts an array to a primitive linked list (use dict for now)
    Convention: empty ll = None
    '''
    next_node = None
    for i in range(len(arr)):
        node = {'next': next_node, 'val': arr[-i - 1]}
        next_node = node
    return next_node

def ll_to_arr(llist):
    arr = []
    while llist is not None:
        arr.append(llist['val'])
        llist = llist['next']
    return arr

def p2_1(llist):
    ''' removes duplicates from an unsorted linked list '''
    if llist is None:
        return llist

    curr = llist
    seen = set([curr['val']])
    while curr['next'] is not None:
        if curr['next']['val'] in seen:
            curr['next'] = curr['next']['next']
        else:
            seen.add(curr['val'])
            curr = curr['next']
    return llist

def test_p2_1():
    cases = [
        ([1, 2, 3, 1], [1, 2, 3]),
        ([1], [1]),
        ([], []),
        ([1, 1, 1, 2], [1, 2]),
        ([1, 2, 3], [1, 2, 3]),
    ]
    for case, res in cases:
        ret = ll_to_arr(p2_1(arr_to_ll(case)))
        assert ret == res, '%s, %s' % (str(ret), str(res))

def p4_3(arr):
    '''
    modified problem, just find indicies (instead of building graph), but make
    binary search tree

    IDEA: binary mapping: ends in 0 = last row, ends in 01 = 2nd to last, etc.
    0th row = 0 + 111...
    1st row = [0, 1] + 0 + 111...
    2nd row = [00, 01, 10, 11] + 0 + 111...
    '''
    total_depth = int(np.log2(len(arr) + 1))
    rows = []
    for row in range(total_depth):
        inc = 2 ** (total_depth - row)
        start = 2 ** (total_depth - row - 1) - 1
        rows.append([start + inc * i for i in range(2 ** row)])
    return rows

def test_p4_3():
    assert p4_3(np.arange(15)) == [[7], [3, 11], [1, 5, 9, 13],
                                   [0, 2, 4, 6, 8, 10, 12, 14]]

def p8_1(n):
    ''' generate nth fibb number, f0 = f1 = 1 '''
    idx = 1
    fibbs = [1, 1]
    while idx < n:
        idx += 1
        fibbs[idx % 2] = sum(fibbs)
    return fibbs[n % 2]

def test_p8_1():
    assert p8_1(0) == 1
    assert p8_1(1) == 1
    assert p8_1(4) == 5
    assert p8_1(5) == 8

def bin_search(arr, key):
    ''' practice algorithm '''
    l_idx = -1
    r_idx = len(arr)

    # don't get caught in l_idx = len(arr) - 1 + not found
    while l_idx < r_idx - 1:
        m_idx = (l_idx + r_idx) // 2
        if key == arr[m_idx]:
            return m_idx
        elif key < arr[m_idx]:
            r_idx = m_idx
        else:
            l_idx = m_idx
    raise ValueError('Not Found')

def p9_3(arr, key):
    '''
    arr is a sorted-then-rotated, find key in log(n)
    find rotational point (max element), then binary search w/ mod
    '''
    l_rot_idx = 0
    r_rot_idx = len(arr) - 1
    while arr[l_rot_idx] > arr[r_rot_idx]:
        m_rot_idx = (l_rot_idx + r_rot_idx) // 2
        if arr[l_rot_idx] >= arr[m_rot_idx]:
            r_rot_idx = m_rot_idx
        else:
            l_rot_idx = m_rot_idx
    # r_rot_idx = max, so search in (-len(arr) + r_rot_idx, r_rot_idx + 1)

    l_idx = -len(arr) + r_rot_idx
    r_idx = r_rot_idx + 1

    # don't get caught in l_idx = len(arr) - 1 + not found
    while l_idx < r_idx - 1:
        m_idx = (l_idx + r_idx) // 2
        if key == arr[m_idx % len(arr)]:
            return (m_idx + len(arr)) % len(arr)
        elif key < arr[m_idx % len(arr)]:
            r_idx = m_idx
        else:
            l_idx = m_idx
    raise ValueError('Not Found')


def test_bin_search():
    arr = np.arange(5)

    try:
        bin_search(arr, -1)
        assert False
    except ValueError:
        pass

    try:
        bin_search(arr, 200)
        assert False
    except ValueError:
        pass

    for i in arr:
        assert bin_search(arr, i) == i

def test_p9_3():
    test_arr = np.arange(5)
    def rotate(arr, idx):
        return [arr[i - idx] for i in range(len(arr))]
    for i in range(len(test_arr)):
        assert p9_3(rotate(test_arr, i), 2) == (i + 2) % 5

if __name__ == '__main__':
    # test_p1_1()
    # test_p1_2()
    # test_p1_4()
    # test_p2_1()
    # test_p4_3()
    # test_p8_1()
    # test_bin_search()
    test_p9_3()
