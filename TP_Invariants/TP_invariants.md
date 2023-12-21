# TP2 : Topological invariant calculation

### STEP 1: DOWNLOAD AND OBSERVE SEGMENTED IMAGES




**Fertility** image seems to have only two tunnels...





### STEP 2: CALCULATE THE NUMBERS OF CONNECTED COMPONENTS AND CAVITIES

By taking the following pairs adjencies for the foreground andf the background : 

- First (26 Fg, 6 Bg)
- Second (6 Fg, 26 Bg)


I used the WriteComponents method to check the connected components, and I ran my programm on all my images to get the following results : 


```cpp
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


### STEP 3: CALCULATE THE EULER CHARACTERISTIC




```cpp
 === fertility analysis ===
(26, 6) Adjency : 
Number of components : 1
Number of cavities : 1
(6, 26) Adjency : 
Number of components : 1
Number of cavities : 1

0-cells : 144487
1-cells : 413834
2-cells : 394745
3-cells : 125401

 === dragon analysis ===
(26, 6) Adjency : 
Number of components : 1
Number of cavities : 1
(6, 26) Adjency : 
Number of components : 2
Number of cavities : 1

0-cells : 133107
1-cells : 377782
2-cells : 357002
3-cells : 112328

 === filigree analysis ===
(26, 6) Adjency : 
Number of components : 1
Number of cavities : 1
(6, 26) Adjency : 
Number of components : 1
Number of cavities : 1

0-cells : 65647
1-cells : 177483
2-cells : 158495
3-cells : 46774

 === torus analysis ===
(26, 6) Adjency : 
Number of components : 1
Number of cavities : 1
(6, 26) Adjency : 
Number of components : 1
Number of cavities : 1

0-cells : 546268
1-cells : 1588497
2-cells : 1539188
3-cells : 496959
```


```cpp
 === fertility analysis ===
(26, 6) Adjency : 
Number of components : 1
Number of cavities : 1
(6, 26) Adjency : 
Number of components : 1
Number of cavities : 1

0-cells : 144487
1-cells : 413834
2-cells : 394745
3-cells : 125401

Euler (with complex.euler()) : -3

Euler (with formula) : -3

 === dragon analysis ===
(26, 6) Adjency : 
Number of components : 1
Number of cavities : 1
(6, 26) Adjency : 
Number of components : 2
Number of cavities : 1

0-cells : 133107
1-cells : 377782
2-cells : 357002
3-cells : 112328

Euler (with complex.euler()) : -1

Euler (with formula) : -1

 === filigree analysis ===
(26, 6) Adjency : 
Number of components : 1
Number of cavities : 1
(6, 26) Adjency : 
Number of components : 1
Number of cavities : 1

0-cells : 65647
1-cells : 177483
2-cells : 158495
3-cells : 46774

Euler (with complex.euler()) : -115

Euler (with formula) : -115

 === torus analysis ===
(26, 6) Adjency : 
Number of components : 1
Number of cavities : 1
(6, 26) Adjency : 
Number of components : 1
Number of cavities : 1

0-cells : 546268
1-cells : 1588497
2-cells : 1539188
3-cells : 496959

Euler (with complex.euler()) : 0

Euler (with formula) : 0
```


```cpp
 === fertility analysis ===
(26, 6) Adjency : 
Number of components : 1
Number of cavities : 1
(6, 26) Adjency : 
Number of components : 1
Number of cavities : 1

0-cells : 144487
1-cells : 413834
2-cells : 394745
3-cells : 125401

Euler (with complex.euler()) : -3
Euler (with formula) : -3

Nb tunnels (26 Adjency): -3
Nb tunnels (6 Adjency): -3

 === dragon analysis ===
(26, 6) Adjency : 
Number of components : 1
Number of cavities : 1
(6, 26) Adjency : 
Number of components : 2
Number of cavities : 1

0-cells : 133107
1-cells : 377782
2-cells : 357002
3-cells : 112328

Euler (with complex.euler()) : -1
Euler (with formula) : -1

Nb tunnels (26 Adjency): -1
Nb tunnels (6 Adjency): -2

 === filigree analysis ===
(26, 6) Adjency : 
Number of components : 1
Number of cavities : 1
(6, 26) Adjency : 
Number of components : 1
Number of cavities : 1

0-cells : 65647
1-cells : 177483
2-cells : 158495
3-cells : 46774

Euler (with complex.euler()) : -115
Euler (with formula) : -115

Nb tunnels (26 Adjency): -115
Nb tunnels (6 Adjency): -115

 === torus analysis ===
(26, 6) Adjency : 
Number of components : 1
Number of cavities : 1
(6, 26) Adjency : 
Number of components : 1
Number of cavities : 1

0-cells : 546268
1-cells : 1588497
2-cells : 1539188
3-cells : 496959

Euler (with complex.euler()) : 0
Euler (with formula) : 0

Nb tunnels (26 Adjency): 0
Nb tunnels (6 Adjency): 0
```