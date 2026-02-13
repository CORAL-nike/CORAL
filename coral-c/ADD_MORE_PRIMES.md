# Instructions for adding more parameter sets

## Requirements

1. Clone [modarith](https://github.com/mcarrickscott/modarith) (and install its dependency [addchain](https://github.com/mmcloughlin/addchain))
2. In modarith, check out to commit `9cf4c7c20ae0d4b7e4f71330e2d9c42d7d26c933`
3. Install [Sage](https://github.com/sagemath/sage)  (version at least 10.5) 

## Procedure

1. Run the script `fp_all.py` with python3. Give as arguments: 
- The encoding size of the prime (usually its bitsize ceiled to the next full word) in bits
- The wordsize (in bits). It must be `64` given to hardcoded values in the precomputations (see below)
- The prime, in hexadecimal
- A path from your current location to the modarith folder
- A path from your current location to the C-code folder
- The name you give your new parameter set (any string, should not contain `lvl`)
2. Go into your build folder
3. Run `cmake -DSQISIGN_BUILD_TYPE=ref` 
4. Run `make precomp` (to speed up the process and run precomputations only for you new prime, you can temporarily modify the line `SET(SVARIANT_S "...")` in the highest `CMakeLists.txt` to only contain your new parameter name, don't forget to revert this chage afterwards)
5. Run `make`n and `make test` to check everything compiles and runs


## Warning

We did not script any fast way to remove a given parameter from your current code, even if that should be simple to do.
We recommend deleting affected folders and using git restore appropriately.