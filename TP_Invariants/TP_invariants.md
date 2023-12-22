# TP2 : Topological invariant calculation

### STEP 1: DOWNLOAD AND OBSERVE SEGMENTED IMAGES



If we take the 26-adjency : 

**Fertility** image seems to have only two tunnels/cavities and a single connected component.

**Filigree** has multiple tunnels/cavities, all the cavities aren't tunnels (the onrnements are cavities but not tunnels) and forms a single component.

**Dragon** owns a single tunnel ans represents a single connected component.

**Torus** is a single connected component and creates some tunnels.



### STEP 2: CALCULATE THE NUMBERS OF CONNECTED COMPONENTS AND CAVITIES

By taking the following pairs adjencies for the foreground andf the background : 

- First (26 Fg, 6 Bg)
- Second (6 Fg, 26 Bg)


I used the WriteComponents method to check the connected components, and I ran my programm on all my images to get the following results : 


```
 === fertility analysis ===
(26, 6) Adjency : 
Number of components : 1
Number of cavities : 1
(6, 26) Adjency : 
Number of components : 1
Number of cavities : 1

 === dragon analysis ===
(26, 6) Adjency : 
Number of components : 1
Number of cavities : 1
(6, 26) Adjency : 
Number of components : 2
Number of cavities : 1

 === filigree analysis ===
(26, 6) Adjency : 
Number of components : 1
Number of cavities : 1
(6, 26) Adjency : 
Number of components : 1
Number of cavities : 1

 === torus analysis ===
(26, 6) Adjency : 
Number of components : 1
Number of cavities : 1
(6, 26) Adjency : 
Number of components : 1
Number of cavities : 1
```

I got similar results for these two different adjencies, the only one that differs is for the dragon image, it considers that there are two components with the 6 adjency.


### STEP 3: CALCULATE THE EULER CHARACTERISTIC

After I made a KSpace for my 3D points, I constructed a cubical complex object thanks to the digital set of the foreground.

The function nbCells(n) could retrieve me the amount of cells of the n dimension.

*Ex :* 0-cells <=> complex.nbCells(0)


```
 === fertility analysis ===
0-cells : 144487
1-cells : 413834
2-cells : 394745
3-cells : 125401

 === dragon analysis ===
0-cells : 133107
1-cells : 377782
2-cells : 357002
3-cells : 112328

 === filigree analysis ===
0-cells : 65647
1-cells : 177483
2-cells : 158495
3-cells : 46774

 === torus analysis ===
0-cells : 546268
1-cells : 1588497
2-cells : 1539188
3-cells : 496959
```


Then, the euler function of the cubical complex object computed the euler charactéristic. Moreover, thanks to the different amount of cells I could make my own computation of the Euler characteristic by using this formula : 0-cells - 1-cells + 2-cells - 3-cells

With : 

- 0-cells = vertices
- 1-cells = segments
- 2-cells = surface of a plan
- 3-cells = 3D surface

```
 === fertility analysis ===
0-cells : 144487
1-cells : 413834
2-cells : 394745
3-cells : 125401

Euler (with complex.euler()) : -3
Euler (with formula) : -3

 === dragon analysis ===
0-cells : 133107
1-cells : 377782
2-cells : 357002
3-cells : 112328

Euler (with complex.euler()) : -1
Euler (with formula) : -1

 === filigree analysis ===
0-cells : 65647
1-cells : 177483
2-cells : 158495
3-cells : 46774

Euler (with complex.euler()) : -115
Euler (with formula) : -115

 === torus analysis ===
0-cells : 546268
1-cells : 1588497
2-cells : 1539188
3-cells : 496959

Euler (with complex.euler()) : 0
Euler (with formula) : 0
```


### STEP 4: CALCULATE THE NUMBER OF TUNNELS

To compute the amount of tunnels, we can follow this formula : nbTunnels = eulerCharacteristic + nbCavities -nbComponents

I got the following results : 

```
 === fertility analysis ===
Nb tunnels (26 Adjency): -3
Nb tunnels (6 Adjency): -3

 === dragon analysis ===
Nb tunnels (26 Adjency): -1
Nb tunnels (6 Adjency): -2

 === filigree analysis ===
Nb tunnels (26 Adjency): -115
Nb tunnels (6 Adjency): -115

 === torus analysis ===
Nb tunnels (26 Adjency): 0
Nb tunnels (6 Adjency): 0
```