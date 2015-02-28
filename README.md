# EarthquakeSimulator
Program simulates the following system:

/ / / / / / / DRAGGING SURFACE / / / / / / (v_drag)-->
-------------------------------------------
'        |               |   
'      spring1       spring3      
'        |               |
... -|block1|-spring2-|block2|-spring4 ...
-------------------------------------------
/ / / / / / / FRICTIONAL SURFACE/ / / / / /


A number of blocks (with identical masses: M_0) are connected to each other by springs with spring constant = k_0
They rest on the Frictional Surface which adds a force componant based on a friction function (varies depending on the experiment)
The ceiling to the experiment (the Dragging Surface) is traveling in the positive direction at speed v_drag
It is connected to each block individually by a spring with spring constant = k_p

As the system progresses from t=0, energy builds in the inter-spring blocks and small scale slips occur. 
The system approaches a state of self-organised criticality where more and more slips occur.

Measuring the sizes of the events by looking at the power released over large samples of blocks (10^3 range) shows an
pproximation to the Guttenburg-Richter scaling law
