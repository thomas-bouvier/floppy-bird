# Genetic Algorithm

A genetic algorithm is used when you have *to optimize a set of parameters*.
This algorithm is divided in two parts:
* Initialisation
* Iterative optimization to find the ideal set of parameters
## Part 1: The population
A *population* is composed of N *members* which contain a unique set of parameters randomly generated known as the member's *DNA. The differences between each member create the *variation* of the population.
The *genotype* corresponds to the DNA structure.
The *phenotype* corresponds to the way that the DNA is visualized.
## Part 2: The selection
### Evaluate the fitness
The next step is to evaluate the "fitness" of each member and select which members are fit to be selected. The fitness is computed by a function which evaluate numerically any given possible DNA.
### Create a mating pool
You save each DNA with the higher fitness coefficient in a mating pool.
Several methods to create your mating pool :
*elitist method
*50% method
*probabilistic method: Express as a percentage the fitness coefficient, then randomly select parents based on this percentage
## Part 3: The Reproduction
### Crossover
You pick randomly two members in the mating pool, those will became parents to create another member based on its parents' DNA.
Several methods to give the DNA :
* asexual reproduction: The child is exactly one parent
* concatenation: You choose a random midpoint and concatenate parents
* randomly: We randomly chose each letter.
### Mutation
It's the final process before adding the new child to the population
You define a percentage that a parameter mutate to another value generated after the crossover.
This adds variety to the population.
# Find a String Exemple
You want to find a string of several characters. Brute force methods are too slow to be efficient.
The DNA is a sentence.
The member contains this DNA.
The population is consequently a set of different sentences carried by each member.
The fitness coefficient is computed when you compare the difference between the target and the DNA.
When the fitness reach a certain number, you stop the algorithm.

----

Created: 12/03/2017
Update: 12/03/2017
Author: Théo Chatelier
Floppy-Bird