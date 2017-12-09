# monte-carlo-renderer

## Compile
```
$ cd <path to zero>
  
$ mkdir build

$ cd build

$ cmake ..

$ make
```
## Run
```
$ ./a4 -input ../data/<name> -output <output_name> -size <w> <h> [-shadows] [-bounces <num>] 
  
$ ./a4 -input ../data/scene06_bunny_1k.txt -output 06.png -size 300 300 -bounces 1
```
