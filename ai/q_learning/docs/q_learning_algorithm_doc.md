# Q-Learning Algorithm
Q-learning is a AI method from the branch of Reinforcement Learning used to solve state-action problem.

The basis is to find the best action to perform in the current state of the program. The best action is chosen thanks to a trial and error method. We test an action in our current state, if the following state is incorrect, then we give a negative reward to the last chosen action (linked to the last state). In the other case, if the following state is correct, we enhance the last action with a positive reward.

```
#INPUT#
S is a set of states
A is a set of actions
Q[S,A] contains Q-values
y is the discount
alpha is the learning rate
#LOOP#
s' = previous_state
a' = previous_action

s = current_state

r = compute_reward

optimal_next_reward = max(a)Q[s,a]

Q[s',a'] = Q[s',a'] + alpha * (r + y * optimal_next_reward - Q[s',a'])

perform_next_action
```
We can manage a more complex algorithm in order to converge faster to a state-action matrix fully completed thanks to few upgrades :

#### Propagation
It's the fact to save a determined quantity of the last action-state combinaisons in order to propage the current reward through each last chosen combinaisons. This helps to propagate negative reward faster.

#### Random Exploration
It could be relevant to add a parameter corresponding to a probability to chose randomly an action and unfollow the Q-Reward value for one action. This allows the program to explore more possibilities and avoid to stay stuck with a unique answer.

## Flappy Bird Example
In our application, a state is the position of the bird compared to the next pipe according to the x-axis and y-axis coordinates. An action is *jump* or *do nothing*.

When we encounter a new state, we initialize randomly the chance to chose one action or the other one. Then, according to the following state, we can increase or decrease the chance to chose the previous action when we are in the previous state.

We will have at the end a matrix with every possible state and an action to each of them.

## Variable Parameters
* Discount : Reduce the propagation between each state
* Learning_rate : Reduce the modification of the old Q-value
* Death Reward : The value given if the bird dies
* Life Reward : The value given if the bird is alive
* Higher Reward Limit : The higher limit of the Q-value
* Lower Reward Limit : The lower limit of the Q-value
* Random Reward Initialization : New Q-values will be initialized randomly between 0 and this value
* Distance Accuracy : the values collected by the game (distances) will be divided by this parameter to divide the quantity of different states
* Quantity of saved states (and actions) : Quantity of states saved to propagate the reward values
* Quantity of Frame before next Action : Delay in frame between two actions (the game runs at 60FPS)

----
Created: 20/04/2017

Update: 20/04/2017

Author: Théo Chatelier

Floppy-Bird
