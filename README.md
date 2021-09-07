This is a C++ library to solve the art gallery problem.

## AGP

 We define the art gallery problem (AGP) as follows:

_Given a gallery or a museum find the minimum number of guards/cameras to watch the interior of the gallery._

A gallery is represented as a simple polygon \f$P\f$ without holes and guards as a set of points of \f$P\f$. In this library we sovle the AGP for stationary guards that can be placed, both in the vertices of the polygon or in its interior. 

\image html couto-60-20-polygon.png An example of a gallery. width=500cm

\image html couto-60-20-guarded.png A solution for the given gallery. width=500cm

## Algorithms

Following alogrithms are provided in the library:

 - An algorithm based on @ref fisk\_algo's proof,
 - @ref ghosh\_algo's approximation algorithm,
 - @ref couto\_algo's el al. exact algorithm,
 - @ref baumgartner\_algo's et al. algorithm that tackles the point AGP.

First three algorithms solve the vertex problem (when the guards can be placed only in the vertices). Baumgartner's approach tries to solve the general AGP. All the algorithms take a vector of points representing a gallery and return a vector of guards.

## Modules

The library consists of four modules:

- algo - containing all the available algorithms,
- arrangement - a module to handle gallery shapes,
- cplex - a wrap for the CPLEX package,
- plot - a module to plot algorithms results.

## Documentation

To generate the documentation using doxygen run:

```console
$ doxygen Doxyfile 
```

Two documentation formats will be available: html and latex. Both formats are located in `documentation` directory. In order to view the html documentation, type `<path-to-repo>/documentation/html/index.html` in your browser.


