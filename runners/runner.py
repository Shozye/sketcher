
from dataclasses import dataclass
import os
import subprocess


@dataclass
class CommandLineArgs:
    path_to_binary: str
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
    qsketchEnd: float = 1  # Optional QSketch specific
    qsketchAmountPoints: int = 5000  # Optional QSketch specific

    def to_command(self) -> list[str]:
        command = (f"{self.path_to_binary} {self.command} -name {self.sketch_name}"
                   f" -weightDist {self.weightDistType} {self.weightDistArg1} {self.weightDistArg2}"
                   f" -sketches {self.amount_sketches} -elems {self.amount_elements}"
                   f" -itemDist {self.itemDist} -weightSeed {self.weight_dist_seed}"
                   f" -sketchSize {self.sketchSize} -sketchSeed {self.sketchStartSeed}"
                   f" -qsketchStart {self.qsketchStart} -qsketchEnd {self.qsketchEnd}"
                   f" -qsketchAmountPoints {self.qsketchAmountPoints}")

        return command.split(" ")
    
def main():
    DATA_DIRNAME = "data"
    os.makedirs(DATA_DIRNAME, exist_ok=True)

    path_to_binary=os.path.join(os.path.abspath(os.path.dirname(__file__)), "..", "build", "bin", "sketcher")
    output_path = os.path.join(os.path.abspath(os.path.dirname(__file__)), DATA_DIRNAME, "output.txt")
    cmd = CommandLineArgs(path_to_binary, "test", "qsketch")

    with open(output_path, 'w+') as file:
        result = subprocess.run(cmd.to_command(), stdout=file)
    print(result)

if __name__ == "__main__":
    main()