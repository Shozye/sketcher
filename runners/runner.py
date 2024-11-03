
from dataclasses import dataclass
import os
import subprocess

class Constants:
    DATA_DIRNAME = "data"
    PATH_TO_BINARY=os.path.join(os.path.abspath(os.path.dirname(__file__)), "..", "build", "bin", "sketcher")
    OUTPUT_DIR_PATH = os.path.join(os.path.abspath(os.path.dirname(__file__)), DATA_DIRNAME)

@dataclass
class CommandLineArgs:
    command: str
    sketch_name: str
    weight_dist_seed: int = 0  # Default value: generates random seed if 0
    amount_elements: int = 10000  # Default elements
    amount_sketches: int = 1  # Default number of sketches
    weightDistType: str = "uniform_int"
    weightDistArg1: float = 1  # Default range for uniform_int: [1, 1]
    weightDistArg2: float = 1
    itemDist: str = "distinct"  # Default item distribution
    sketchSize: int = 400  # Default sketch size
    sketchStartSeed: int = 42  # Default starting seed for sketches
    qsketchStart: float = 0.25  # Optional QSketch specific
    qsketchEnd: float = 0.75  # Optional QSketch specific
    qsketchAmountPoints: int = 5000  # Optional QSketch specific

    def to_command(self) -> list[str]:
        command = (f"{Constants.PATH_TO_BINARY} {self.command} -name {self.sketch_name}"
                   f" -weightDist {self.weightDistType} {self.weightDistArg1} {self.weightDistArg2}"
                   f" -sketches {self.amount_sketches} -elems {self.amount_elements}"
                   f" -itemDist {self.itemDist} -weightSeed {self.weight_dist_seed}"
                   f" -sketchSize {self.sketchSize} -sketchSeed {self.sketchStartSeed}"
                   f" -qsketchStart {self.qsketchStart} -qsketchEnd {self.qsketchEnd}"
                   f" -qsketchAmountPoints {self.qsketchAmountPoints}")

        return command.split(" ")
    
def run_sketcher(args: CommandLineArgs, filename: str):

    with open(os.path.join(Constants.OUTPUT_DIR_PATH, filename), 'w+') as file:
        result = subprocess.run(args.to_command(), stdout=file)
        print(result)
    
def main():
    os.makedirs(Constants.DATA_DIRNAME, exist_ok=True)

    args = []
    for sketch_name in ["qsketch", "fastexpsketch"]:
        for amount_elems in [1, 10, 100, 1_000, 10_000, 100_000, 1_000_000, 10_000_000, 100_000_000, 1_000_000_000]:
            args.append((sketch_name, amount_elems))

    for i, (sketch_name, amount_elems) in enumerate(args):

        cmd = CommandLineArgs("test", sketch_name=sketch_name, amount_elements=amount_elems, amount_sketches=1)
        output_filename = f"{sketch_name}_uniform11_e{amount_elems}.txt"
        
        run_sketcher(cmd, output_filename)


    run_sketcher(cmd, "qsketch1.txt")

    

if __name__ == "__main__":
    main()