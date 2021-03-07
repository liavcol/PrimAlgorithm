# Prim's Algorithm Implementation
A little project demonstrating a C++ implementation of a graph data structure and Prim's algorithm for finding a minimal spanning tree (MST) in a weighted undirected graph.
Also includes a way of recalculating an MST after a new edge has been added to the graph in O(|V|).

## Introduction
Initially was given as an assignment in a course in Computer Science BSc.
The problem: Calcuting an MST using Prim's algorithm cost O(|E|log(|V|)). So given an MST of a graph and a new edge that needs to be added to the graph we want to recalculate the
new MST in O(|V|), much cheaper than running Prim's algorithm.
I decided to use the opportunity to add an implementaion of a graph data structure, complete with methods to easilly manage it.

## In-Depth (Just a Little)
A spanning tree of a graph G(V, E) is a graph that has all the vertices of G and |V|-1 edges of the edges og G such that the graph is connected.
In simpler terms, it is a way to represent G in a way where you can get from any vertex to any other vertex with a minimal number of edges. Obviously, there could be many ways to
represent a graph that way, and as such - many spanning trees for that graph.

A minimal spanning tree (MST) of a weighted graph is the a spanning tree that out of all other spanning trees of that graph has the lowest sum of all edges' weights.
An MST might not be unique, in cases we have different edges with equal weights. In fact, an MST is unique only if the grpah has distinct weights on the edges.

Prim's algorithm is an algorithm for calculating an MST for a graph. The general idea of the algorithm is to build an MST by having 2 sets of vertices, the first set is of vertices
that are part of the MST (starting with only onw vertex in it) and the other set is of vertices that are not yet in the MST. At each iteration the algorithm is connecting the sets 
by using the cheapest edge possible (a greedy algorithm) until all the vertices are part of the MST.
The algorithm uses a priority queue to keep track of the cheapest vertex to get to at each iteration and retrieving it in O(1). The set of vertices that are already part of
the MST are the ones that are out of the queue, while the vertices inside the queue are the ones that has not been added to the MST yet. Also, at each iteration, after "adding" 
a vertex to the MST that algorithm checkes all of its adjacent vertices to see if there is a cheaper way to connect them to the MST and update that way accordingly.

Although retrieving the cheapest vertex cost O(1), updating a vertex cost O(log(|V|)) for inserting it into the priority queue. Since we will update the vertices at most O(|E|)
times, we get an overall time complexity of O(|E|log(|V|)). 
Note that in my implementation there may be multiple entries for the same vertex (for each time it was updated before getting out of the queue). However the time complexity stays
O(|E|log(|V|))! That is because the most elements that can be in the priority queue are |E| (based on the updates) and |E| is O(|V|^2) and O(log(|V|^2)) = O(log(|V|)).
