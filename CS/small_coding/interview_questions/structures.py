#!/usr/bin/env python3
'''
key data structs:
    built-in: associative array/hash, list, [set]
    lib: queue, pqueue
    simple implement: stack <- list
    linked list, directed/undirected graph
'''
import queue

# hash (64 bit)
a = {}
a[1] = 1
print(1 in a, 2 in a, a.get(1, 0), a.get(2, 0))
del a[1]

# array (python doesn't exactly have, numpy?)
# set (subtraction/addition)
a = set()
a.add(1)
print(1 in a, 2 in a)
a.remove(1)

# stack
a = []
a.append(3)
a.append(5)
a.append(4)
print(a.pop(), a.pop(), a.pop())
try:
    a.pop()
except IndexError:
    pass

# queue
q = queue.Queue()
q.put(3)
q.put(5)
q.put(4)
print(q.get_nowait(), q.get_nowait(), q.get_nowait())
try:
    q.get_nowait()
except queue.Empty:
    pass

# priority queue
pq = queue.PriorityQueue()
pq.put(3)
pq.put(5)
pq.put(4)
print(pq.get_nowait(), pq.get_nowait(), pq.get_nowait())
try:
    pq.get_nowait()
except queue.Empty:
    pass

# linked list
class ListElem(object):
    def __init__(self, **kwargs):
        super(ListElem, self).__init__()
        self.value = kwargs.get('value', None)
        self.child = kwargs.get('child', None)

list_tail = ListElem(value=1)
list_head = ListElem(value=0, child=list_tail)
print(list_head.value, list_head.child.value)

# directed graph
class DGraphNode(object):
    def __init__(self, **kwargs):
        super(DGraphNode, self).__init__()
        self.value = kwargs.get('value', None)
        self.children = kwargs.get('children', [])

    def print(self, indent=''):
        print(indent, self.value)
        for ch in self.children:
            ch.print(indent=indent + '  ')

leaf_node_1 = DGraphNode(value=2)
leaf_node_2 = DGraphNode(value=0)
root = DGraphNode(value=1, children=[leaf_node_1, leaf_node_2])
print(root.value, root.children[0].value, root.children[1].value)
root.print()

# build tree pre/in/post order
print()
print('TREE ALGS')
print()
def build_tree_pre(depth, elems):
    assert len(elems) == 2 ** depth - 1
    if depth == 1:
        return DGraphNode(value=elems[0])
    child_size = len(elems) // 2
    left = build_tree_pre(depth - 1, elems[1:child_size + 1])
    right = build_tree_pre(depth - 1, elems[child_size + 1: ])
    return DGraphNode(value = elems[0],
                      children=[left, right])

def build_tree_in(depth, elems):
    assert len(elems) == 2 ** depth - 1
    if depth == 1:
        return DGraphNode(value=elems[0])
    child_size = len(elems) // 2
    left = build_tree_in(depth - 1, elems[ :child_size])
    right = build_tree_in(depth - 1, elems[child_size + 1: ])
    return DGraphNode(value = elems[child_size],
                      children=[left, right])

def build_tree_post(depth, elems):
    assert len(elems) == 2 ** depth - 1
    if depth == 1:
        return DGraphNode(value=elems[0])
    child_size = len(elems) // 2
    left = build_tree_post(depth - 1, elems[ :child_size])
    right = build_tree_post(depth - 1, elems[child_size:-1])
    return DGraphNode(value = elems[-1],
                      children=[left, right])
build_tree_pre(3, range(2 ** 3 - 1)).print()
build_tree_in(3, range(2 ** 3 - 1)).print()
build_tree_post(3, range(2 ** 3 - 1)).print()

# weighted graph
class WDGraphNode(object):
    def __init__(self, **kwargs):
        super(WDGraphNode, self).__init__()
        self.id = kwargs['id']
        self.value = kwargs.get('value', None)
        self.edges = kwargs.get('edges', [])

node1 = WDGraphNode(value=1, id=1)
node2 = WDGraphNode(value=2, id=2)
node3 = WDGraphNode(value=3, id=3)
node4 = WDGraphNode(value=4, id=4)
node5 = WDGraphNode(value=5, id=5)
node6 = WDGraphNode(value=6, id=6)
node1.edges = [(node2, 7), (node3, 9), (node6, 14)]
node2.edges = [(node1, 7), (node3, 10), (node4, 15)]
node3.edges = [(node1, 9), (node2, 10), (node4, 14), (node6, 2)]
node4.edges = [(node2, 15), (node3, 11), (node5, 6)]
node5.edges = [(node4, 6), (node6, 9)]
node6.edges = [(node1, 14), (node3, 2), (node5, 9)]

# Dijkstra's, with hax
pq = queue.PriorityQueue()
pq.put((0, [node1]))
distances = {}
i = 0.01 # HACK HACK pq compares second elements and errors if priorities are eq
while not pq.empty():
    curr_val, curr_path = pq.get_nowait()
    node = curr_path[-1]
    if node.id not in distances or distances[node.id][0] > curr_val:
        distances[node.id] = (curr_val, curr_path)
        for target, edge_w in node.edges:
            pq.put((curr_val + edge_w + i, curr_path + [target]))
            i += 0.01
for key, (dist, path) in distances.items():
    print(key, int(dist), [node.id for node in path])
