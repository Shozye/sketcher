
# Sketcher

## Compilation

Just use
```bash
make
```

## Quick Start

```bash
./sketcher test -name fastexpsketch -e 1000 -s 1 -w uniform_int 1 1 -i distinct
```
The command above creates 1 FastExpSketch with 1000 distinct elements, where each element has a weight of 1. Then it performs a basic analysis of the created sketch.

## Usage

**Sketcher** offers three main commands:

### `test`
Run quick tests with various sketches and generate a basic analysis.
```bash
./sketcher test -name <sketch_name> -e <amount_elements> -s <amount_sketches> -w <weight_distribution> -i <item_distribution>
```

- **Example**: `./sketcher test -name fastexpsketch -e 500 -s 5 -w normal 5 1 -i distinct`
  - This generates 5 FastExpSketches using 500 distinct elements, with weights following a normal distribution (mean 5, variance 1) and makes a basic analysis.

### `sketch`
Create and save sketches to a file, which can be paused and resumed if the application is stopped.
```bash
./sketcher sketch <filename> -name <sketch_name> -e <amount_elements> -s <amount_sketches> -w <weight_distribution> -i <item_distribution>
```

- **Example**: `./sketcher sketch sketches.dat -name qsketch -e 1000 -s 10 -w uniform_int 1 10 -i distinct`
  - This command creates 10 QSketches with 1000 distinct elements and uniform weights between 1 and 10 and saves them to `sketches.dat`.

### `analyse`
Load sketches from a saved file and perform an analysis.
```bash
./sketcher analyse <filename>
```

- **Example**: `./sketcher analyse sketches.dat`


## Parameters

- **-name `<sketch_name>`**: Type of sketch to use. Options:
  - `fastexpsketch`
  - `qsketch`

- **-e `<amount_elements>`**: Number of elements to process in each sketch. Default: `1000`

- **-s `<amount_sketches>`**: Number of sketches to create. Default: `1`

- **-w `<weight_distribution>`**: Distribution type for element weights. Default: `uniform_int 1 1`. Options:
  - `uniform_int a b`: Uniformly distributes weights between [`a`, `b`]. Weights are only integers.
  - `uniform_real a b`: Uniformly distributes weights between [`a`, `b`]. `b` not included.
  - `normal e v`: Distributes weights according to a normal distribution with mean `e` and variance `v`.

  Note that in case weight distribution will return negative value, it will be computed until positive value found. 

- **-i `<item_distribution>`**: Specifies item distribution type. Options:
  - `distinct` (default)
  - `repeated`

- **-seed `<seed_weight_distribution>**: Specifies seed for generating weight distribution. Integer.
