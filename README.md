# Data Structures And Algorithms

This repo contains all the code made by me for the IIC2133 course as a teacher asistant.

# Programs

## Dynamic programming

* Pastillas: A total probabilities problem that includes several repeated calls and can be solved using dynamic programming.
* Palindromo: This program finds the minimum number of palindroms that can write a given word using dynamic programming.
* SubString: This program finds the common orderer characters of two strings using dynamic programming.

## Midi Soul

This program is the solution to a homework asigned to the students of the 2017-1 Data Structures and Algorithms course. The problem was to find the most repeated melody (of at least n notes) on a one track midi file. The aproach of the solution is to use a `sorted suffix array` of notes so the most repeated melody is the longuest sequence of repeated prefixes of the suffixes of at least n notes.

The suffix array structure is a simplification of the `suffix trie`, and is commonly used to find the longest repeated subtring on a large string.

## KDTree

This program is the solution of a homework assigned to me when i was a student on Data tructures and Algorithms. The base code of the womework is a fully working `RayTracer` made by the teacher assistant [https://github.com/Ghoeb Vicente Errázuriz].
The objective of the homework was to optimize the RayTracing by creating a KDTree to organize the triangles of a scene so the that there are fewer calls to the Ray-Triangle collition detection method.

The final KDTree had big results and it could render scenes with hundreds of thousands triangles.

## Prim

This program is the solution of a homework assigned to me when i was a student on Data tructures and Algorithms. The objective was to find the maximum flow tree from a source to a group of sinks (all nodes of a graph). The solution used an adaptation of the prim algorithm and the implementation includes a min heap.
