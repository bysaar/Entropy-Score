# Entropy-Score Project

This project is a simulation of the detection algorithm mentioned in the article:

"Entropy-Score: A Method to Detect DDoS Attack and Flash Crowd" By Akshat Gaurav, Awadhesh Kumar Singh.

The implementation done in C++ over OMNet++ framework.


## Overview
The article presents a problem that many popular servers are constantly faced with, which is the inability to distinguish between legitimate flash crowd traffic as a result of a new feature being uploaded and massive incoming traffic because of a DDoS attack.

The need to differentiate between the two is necessary because early detection of a DDoS attack can help prevent it. Moreover, the service for flash crowd traffic is often very important, therefore false-positive detection might not be acceptable.

To solve this problem, the article proposes a combined method consisting of packet scoring and entropy calculation, with the help of which it will be possible to distinguish between malicious network traffic and mass crowd traffic.

### Definitions:

- **Entropy** – Measurable property that describes the degree of disorder, randomness or uncertainty of the system.

- **DDoS Attack** – Distributed Denial of Service Attack is a cyber-attack in which the attacker can make the server unavailable for serving legitimate clients by flooding requests from multiple sources.
When the attack is activated, all the sources of the attack generate massive traffic toward the server simultaneously, therefore, if the server cannot distinguish between legitimate requests and malicious ones it is impossible to stop this attack simply by blocking its source.

- **Flash Crowd** – A flash crowd scenario is a sudden increase in traffic to a particular common server causing a dramatic growth in server load and service time. Usually occurs when a newly launched feature is causing worldwide interest.

### Simulation's Goal:
The main goal of the simulation is to test the correctness and efficiency of the suggested method to differentiate between DDoS Attacks and Flash Crowd scenarios.

## Detection Algorithm
- **Phase 1**: Incoming packets will be divided into groups according to 'Distance' parameter and their IP addresses.

- **Phase 2**: Entropy calculation of the system which is done according to Equation 1. in the article:

&emsp;&emsp;&emsp;&emsp;&emsp;&emsp;&emsp;&emsp;&emsp;&emsp;&emsp;&emsp;&emsp; <img src="https://user-images.githubusercontent.com/90688449/187313035-e4dcec9b-b5fa-4b29-86c7-48dfac333fac.gif" alt="centered image">


&emsp;&emsp;&emsp;&emsp;&emsp;&emsp; Where X is the total sample space and p(x<sub>i</sub>) is the probability of i<sup>th</sup> sample occurence.

&emsp;&emsp;**Insight: &emsp;&emsp;&emsp;&emsp;&emsp;&emsp;&emsp;  H<sub>no attack</sub> < H<sub>DDoS Attack</sub> < H<sub>Flash Crowd</sub>**
&emsp;&emsp;&emsp;&emsp;&emsp;&emsp; 
> Means that IP addresses scattering is the lowest for Normal Operations and highest for Flash Crowd.

- **Phase 3**: Compare entropy value versus the threshold (Th):

&emsp;&emsp;&emsp;&emsp;&emsp;&emsp;&nbsp; **If** the entropy is less than the threshold, then traffic is classified as normal.

&emsp;&emsp;&emsp;&emsp;&emsp;&emsp;&nbsp; **Else**, further distinguishing between DDoS and FC traffic is required.

- **Phase 4**: Score of each group is calculated and compared with Packet Score Threshold (T):

&emsp;&emsp;&emsp;&emsp;&emsp;&emsp;&nbsp; **If** the score is less than the threshold, then traffic is classified as Flash Crowd.

&emsp;&emsp;&emsp;&emsp;&emsp;&emsp;&nbsp; **Else**, traffic is classified as DDoS Attack.


## Implementation

### Network:

<tab><img width="842" alt="צילום מסך 2022-08-30 ב-2 53 24" src="https://user-images.githubusercontent.com/90688449/187318629-f58f488d-8b2a-4dde-b4f1-c3ad1d7f6771.png"></tab>

#### Topology - High-level:
<img width="546" alt="image" src="https://user-images.githubusercontent.com/90688449/187316465-126f90b3-2d7a-4352-b09c-09d6ef733ff6.png">

#### Topology - Deep Look:
<img width="549" alt="image" src="https://user-images.githubusercontent.com/90688449/187316494-ff4b4527-70e9-4238-8b86-3789b63fe505.png">


### Assumptions:

- IP addresses are scattered randomly according to article's insight
- Center of each group is calculated dynamically per insertion.
- 'Distance' parameter was statically defined (29).
- Entropy threshold was set as the upper bound entropy value for the number of normal groups created.
- Packet-Score threshold was statically set as 15 according to article's results.
- Number of requests for each client is defined according to article described experiment.

> The values of the parameters were taken from TABLE II. in the article.

### The Simulation:

The simulation is divided into four different parts:

- **Initiallization**: Clients are divided to three different types - Normal, DDoS and FC.

- **First Phase - Normal Operation**: - 50 normal clients generate requests towards the server causing a dynamic groups creation. At the end of this phase, the system's entropy value is calculated and recorded, then the entropy threshold is set.

- **Second Phase - DDoS Attack**: - 15 DDoS clients generate requests towards the server. At the end of this phase, the system's entropy value is calculated as well as the packet score values for each DDoS group.

- **Third Phase - FC Scenario**: - 40 FC clients generate requests towards the server. At then end of this phase, the system's entropy value is calculated as well as the packet score values for each FC group.

### **Classification**:
During the simulation Phase 1 is used as the learning phase of the system - since the entropy threshold should be based on the normal traffic, therefore, in a real scenario normal traffic should not cross the system's entropy threshold.

For the next two phases (2,3), if the system's entropy will be higher than the threshold, then it should be either a DDoS attack or an FC scenario. The differentiation between them using the packet score calculated for each group and set the classification tag accordingly.

- Each group is classified according to the algorithm description.
- Correctness is verified by comparing the classification tag versus the pre-defined group type (client_type variable).

## Results

### Probability Distributions:

<img width="935" alt="image" src="https://user-images.githubusercontent.com/90688449/187956988-a70181cc-1430-4681-b6a8-f157b699835b.png">

<img width="932" alt="image" src="https://user-images.githubusercontent.com/90688449/187957142-2d2fae15-d1be-4f81-8d92-831bd856153b.png">

### System's Entropy Values:

<img width="933" alt="image" src="https://user-images.githubusercontent.com/90688449/187957343-420c9715-e04e-441a-8cf8-05cd0a399cc4.png">

### Packet Score Values:

<img width="825" alt="image" src="https://user-images.githubusercontent.com/90688449/187957429-1dbdf652-a931-4f77-862f-56216410e4c2.png">


## Conclusions

- The algorithm method seems to be efficient and correct.
- In order to implement the algorithm correctly, it is required to know the system's normal behavior and set the right threshold parameters.
- In a real-time scenario, the algorithm can detect DDoS attacks in a relatively short time interval and can be used to prevent them without blocking flash crowd traffic.



