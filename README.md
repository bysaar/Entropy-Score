# Entropy-Score Project

This project is a simulation of the proposed algorithm of the article: 

"Entropy-Score: A Method to Detect DDoS Attack and Flash Crowd".

## Overview
The article presents a problem that many popular servers are constantly facing with, the inability to distinguish between Normal Opreations, Flash Crowd and DDoS Attacks.

**DDoS Attack** - Distributed Denial of Service Attack is a cyber-attack in which the attacker can make the server unavailable for serving legitmate clients by flooding requests from multiple sources simultaneously.

**Flash Crowd** - A scenario which can be described as an sudden increase in traffic to a praticular server causing a dramatic growth in load and service time.
Usually occurs when a new launched feature is causing worldwide interest.

## Simulation Goal
The main goal of the simulation is to test the correctness and efficiency of the suggested method to differentiate between DDoS Attack and Flash Crowd.

## Detection Algorithm
blabla here.




## Implementation

### Assumptions:

- IP addresses are scattered randomly according to article's insight: H<sub>no attack</sub> < H<sub>DDoS Attack</sub> < H<sub>Flash Crowd</sub>.
- Center of each group is calculated dynamically per insertion.
- 'Distance' parameter was statically defined (29).
- Entropy threshold was set as the upper bound entropy value for the number of normal groups created.
- Packet-Score threshold was statically set as 15 according to article's results.
- Number of requests for each client is defined according to article described experiment.




### The Simulation:
blabla here

