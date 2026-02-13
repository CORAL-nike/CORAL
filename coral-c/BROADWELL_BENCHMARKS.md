# How to generate benchmarks with assembly-optimizations

## Warning

Assembly-optimizations for intel broadwell architecture are only available for parameter sets taken from the SQIsign NIST implementation (v2).
The only parameter which is meaningfull for CORAL is the 500-bit prime from the SQIsign `lvl5` parameter set.

## Steps

1. Edit the top-level `CMakeLists.txt` file in the following way:
- In the line starting with `SET(SVARIANT_S ` delete all parameter sets except `lvl1`, `lvl3` and `lvl5`. In fact, deleting all except `lvl5` speeds up the subsequent steps.
-  Uncomment the lines `#elseif (${SQISIGN_BUILD_TYPE} MATCHES "broadwell")` and `#  add_compile_definitions(SQISIGN_BUILD_TYPE_BROADWELL SQISIGN_GF_IMPL_BROADWELL)`
2. In your build folder, run `cmake -DSQISIGN_BUILD_TYPE=broadwell -DCMAKE_BUILD_TYPE=Release ..`
3. In your build folder, run `make clean` then `make`
4. Run the benchmarks

## Reverting

1. Revert the top-level `CMakeLists.txt` to its previous state
2. Run cmake with whatever parameters you need