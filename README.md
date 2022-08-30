# Entropy-Score Project

This project is a simulation of the detection algorithm mentioned in the article:

"Entropy-Score: A Method to Detect DDoS Attack and Flash Crowd" By **THE AUTHORS**

The implementation done in C++ over OMNet++ framework

## Overview
The article presents a problem that many popular servers are constantly faced with, which is the inability to distinguish between legitimate flash crowd traffic as a result of a new feature being uploaded and massive incoming traffic because of a DDoS attack.

The need to differentiate between the two is necessary because early detection of a DDoS attack can help prevent it. Moreover, the service for flash crowd traffic is often very important, therefore false-positive detection might not be acceptable.

To solve this problem, the article proposes a combined method consisting of packet scoring and entropy calculation, with the help of which it will be possible to distinguish between malicious network traffic and mass crowd traffic.

### Definitions:

- **Entropy** – Measurable property that describes the degree of disorder, randomness or uncertainty of the system.

- **DDoS Attack** – Distributed Denial of Service Attack is a cyber-attack in which the attacker can make the server unavailable for serving legitimate clients by flooding requests from multiple sources.
When the attack is activated, all the sources of the attack generate massive traffic toward the server simultaneously, therefore, if the server cannot distinguish between legitimate requests and malicious ones it is impossible to stop this attack simply by blocking its source.

- **Flash Crowd** – A flash crowd scenario is a sudden increase in traffic to a particular common server causing a dramatic growth in server load and service time. Usually occurs when a newly launched feature is causing worldwide interest.

### Simulation Goal:
The main goal of the simulation is to test the correctness and efficiency of the suggested method to differentiate between DDoS Attack and Flash Crowd.

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
#### Each group is classified according to the algorithm description.
#### Correctness is verified by comparing the classification tag versus the pre-defined group type.

## Results

<img width="618" alt="image" src="https://user-images.githubusercontent.com/90688449/187318113-27c4980c-5a9c-48a2-a120-b0a7e446760b.png">

> <sup>*</sup> The expected results are shown on the left side (taken from the article), and our results are shown in right.

There are slight differences in the group's probability distribution, we assume that this is a result of our implementation and the fact that the creation of groups and randomness of IP addresses are not specified in the article. In our simulation, 3 DDoS groups were created instead of 2 in the article results.

We would like to note that one of the article's assumptions (equation 2) is that DDoS attackers have IP addresses that are more random than normal clients, so we assume that 3 uniform distributed groups created are reflecting more randomness and do not necessarily contradict the article's results. In addition, the behavior of our graphs matches the article’s results.

<img width="525" alt="image" src="https://user-images.githubusercontent.com/90688449/187320302-1b026004-d15b-42de-9e90-120ab673c3e0.png">

> <sup>*</sup> The expected results are shown on the left side (taken from the article), and our results are shown in right.

From the graphs, it can be shown that the entropy values are not equal.
Since the amount and type of requests during the 30sec interval were not specified, we decided to measure the entropy values at the end of each phase. In our opinion, this is the cause of the difference in value, although the behavior of the graphs is the same.

<img width="530" alt="image" src="https://user-images.githubusercontent.com/90688449/187320230-7c1b8b9e-4278-48bb-b34b-bdb7cd8a89ce.png">

> <sup>*</sup> The expected results are shown on the left side (taken from the article), and our results are shown in right.

As we expected, our results fit the article's results. The DDoS groups acquire a high packet score whereas the FC group's scores remain low.

## Conclusions

- The algorithm method seems to be effcient and correct.
- In order to implement the algorithm correctly, it is required to know the system's normal behavior and set the right threshold parameters.
- In real time scenario, the algorithm can detect DDoS attacks in a relatively short time interval and can be used to prevent it without blocking FC traffic.



