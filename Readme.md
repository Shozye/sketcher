
# Sketcher

## Compilation

Just use
```bash
mkdir build
cd build
cmake ..
make
```
executable is `/build/bin/sketcher` 

## Quick Start

```bash
./sketcher test -name fastexpsketch
```
The command above tests fastexpsketch with default parameters.
So it will create 1 fastexpsketch of size 400 and add 10000 elements to it. 
Elements will be distinct and have the same weight - 1.


## Usage

**Sketcher** offers three main commands:

### `test`
Run quick tests with various sketches and generate a basic analysis.
```bash
./sketcher test -name <sketch_name> 
```

### `sketch` [IN DEVELOPMENT]
Create and save sketches to a file, which can be paused and resumed if the application is stopped.
```bash
./sketcher sketch <filename> -name <sketch_name> 
```


### `analyse` [IN DEVELOPMENT]
Load sketches from a saved file and perform an analysis.
```bash
./sketcher analyse <filename>
```


## Parameters

- **-name `<sketch_name>`**: Type of sketch to use. Options:
  - `fastexpsketch`
  - `qsketch`

- **-elems `<amount_elements>`**: Number of elements to process in each sketch. Default: `10000`

- **-sketches `<amount_sketches>`**: Number of sketches to create. Default: `1`

- **-weightDist `<weight_distribution>`**: Distribution type for element weights. Default: `uniform_int 1 1`. Options:
  - `uniform_int a b`: Uniformly distributes weights between [`a`, `b`]. Weights are only integers.
  - `uniform_real a b`: Uniformly distributes weights between [`a`, `b`]. `b` is not included. If `a >= b`, then `uniform_real 0 1` will be applied
  - `normal e v`: Distributes weights according to a normal distribution with mean `e` and variance `v`.

  Note: If the weight distribution returns a negative value, it will be recalculated until a positive value is found.

- **-itemDist `<item_distribution>`**: Specifies item distribution type. Options:
  - `distinct` (default)
  - `repeated`

- **-weightSeed `<seed_weight_distribution>`**: Seed for generating weight distribution. Must be an integer. If set to `0`, a random seed will be generated.

- **-sketchSize `<size>`**: Specifies the size of the sketch. Default: `400`

- **-sketchSeed `<seed>`**: First sketch will be initialized with this seed, and following sketches will increment on this seed. Default: `42`

- **-qsketchStart `<start_value>`**: For private estimations. Starting point for QSketch.

- **-qsketchEnd `<end_value>`**: For private estimations. Ending point for QSketch.

- **-qsketchAmountPoints `<number>`**: For private estimations. Number of points for QSketch.


Make sure to replace the placeholders with accurate descriptions if needed. Let me know if you want to add or modify anything!