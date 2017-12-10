# monte-carlo-renderer

## Compile
```
$ cd monte-carlo-renderer-master
$ mkdir build
$ cd build
$ cmake ..
$ make
```

## Run
```
- ./render -input ../data/<name> -output <output_name> -size <w> <h> [-shadows] [-bounces <num>] 
- ./render -input ../data/scene06_bunny_1k.txt -output 06.png -size 300 300 -bounces 1
```
