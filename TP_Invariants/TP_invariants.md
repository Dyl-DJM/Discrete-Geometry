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

