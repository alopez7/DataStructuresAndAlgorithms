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
