import os
from matplotlib import pyplot as plt

class Constants:
    PATH_TO_DATA_DIR = os.path.join(os.path.abspath(os.path.dirname(__file__)), "..", "runners", "data")
    PATH_TO_IMAGE_DIR = os.path.join(os.path.abspath(os.path.dirname(__file__)), "imgs")

def main():
    grouped_files = []
    filenames = os.listdir(Constants.PATH_TO_DATA_DIR)
    for filename in filenames:
        if filename.startswith("fastexpsketch_"):
            suffix = filename.removeprefix("fastexpsketch_")
            fesketch_filepath = os.path.join(Constants.PATH_TO_DATA_DIR, "fastexpsketch_"+suffix)
            qsketch_filepath = os.path.join(Constants.PATH_TO_DATA_DIR, "qsketch_"+suffix)
            grouped_files.append((fesketch_filepath, qsketch_filepath))

    for fesketch_filepath, qsketch_filepath in grouped_files:
        make_a_graph_from_two_files(fesketch_filepath, qsketch_filepath)

def get_relweight_from_fastexpsketch(filename: str) -> float:
    with open(filename) as file:
        while True:
            line = file.readline()
            if line == "":
                break
            line = line[:-1] # remove newline
            if line.startswith("[Average_Relative_Error]"):
                return float(line.split()[1])
    return 0

def get_info_from_qsketch(filename: str) -> list[float]:
    amount_elements = 0
    amount_sketches = 0
    distribution = ""
    average_relative_errors = []
    qsketch_relative_weight = 0
    START, END, AMOUNT_POINTS = 0, 1, 0

    with open(filename) as file:
        args_line = file.readline()[:-1]
        amount_elements = f"{int(args_line.split()[7][:-1]):.0e}"
        amount_sketches = f"{int(args_line.split()[9][:-1]):.0e}"

        a = float(args_line.split()[13][:-1])
        a = int(a) if a == int(a) else a
        b = float(args_line.split()[15][:-1])
        b = int(b) if b == int(b) else b
        distribution = args_line.split()[11][:-1] + "(" + str(a) + ","+str(b) + ")"
        while True:
            line = file.readline()
            if line == "":
                break
            line = line[:-1] # remove newline
            if line.startswith("[Average_Relative_Error]"):
                qsketch_relative_weight = float(line.split()[1])
            
            elif line.startswith("[START_END_AMOUNTPOINTS]"):
                START = float(line.split()[1])
                END = float(line.split()[2])
                AMOUNT_POINTS = int(line.split()[3])
                
            elif line.startswith("[Average_Relative_Errors]"):
                for relweight in line.split()[1:]:
                    average_relative_errors.append(float(relweight))
        
    return amount_elements, amount_sketches, distribution, (START, END, AMOUNT_POINTS), qsketch_relative_weight, average_relative_errors

def make_a_graph_from_two_files(fastexpsketch_filepath: str, qsketch_filepath: str):
    fesketch_relweight = get_relweight_from_fastexpsketch(fastexpsketch_filepath)
    amount_elems, amount_sketches, distribution, (START, END, AMOUNT_POINTS), qsketch_relative_weight, average_relative_errors = get_info_from_qsketch(qsketch_filepath)

    ONE_INDEX_LENGTH = ((END-START) / AMOUNT_POINTS)

    min_avg_error = min(average_relative_errors)
    argmin_avg_error = average_relative_errors.index(min_avg_error) * ONE_INDEX_LENGTH

    all_xs = [x * ONE_INDEX_LENGTH for x in range(len(average_relative_errors))]

    PLOT_START_X = 0.4
    PLOT_END_X = 0.65

    PLOT_START_X_INDEX = int(len(all_xs)*PLOT_START_X)
    PLOT_END_X_INDEX = int(len(all_xs)*PLOT_END_X)

    xs = all_xs[PLOT_START_X_INDEX:PLOT_END_X_INDEX]
    fast_exp_sketch_ys = [fesketch_relweight for _ in xs]
    qsketch_newton_ys = [qsketch_relative_weight for _ in xs]
    qsketch_estimator_ys = average_relative_errors[PLOT_START_X_INDEX:PLOT_END_X_INDEX]


    # Plotting the data
    plt.figure(figsize=(8, 5))
    plt.plot(xs, fast_exp_sketch_ys, label="FastExpSketch Relative Error", linestyle="--", color="blue")
    plt.plot(xs, qsketch_newton_ys, label="QSketch Relative Error with Newton estimation", linestyle="--", color="green")
    plt.plot(xs, qsketch_estimator_ys, label="QSketch Relative Error with direct estimation", color="red")
    plt.scatter([argmin_avg_error], [min_avg_error], color="red", label=f"min ({argmin_avg_error}, {min_avg_error})")

    # Adding text annotations for constant values
    plt.text(xs[0], fesketch_relweight + 0.0005, f"FESketch = {fesketch_relweight:.3f}", color="blue")
    plt.text(xs[0], qsketch_relative_weight + 0.0005, f"QSketch (Newton) = {qsketch_relative_weight:.3f}", color="green")

    
    # Adding QoL features
    plt.xlabel("Argument of QSketch.estimate, Translates S[i] into 2^{-S[i]-x};")
    plt.ylabel("Relative Error of total weight estimation")

    title = f"Comparison of QSketch Estimators and FESketch. \nelements={amount_elems}, sketches={amount_sketches}, distribution={distribution}, size={400}"
    plt.title(title)
    plt.legend()
    plt.grid(True)
    plt.tight_layout()

    output_filename = os.path.basename(qsketch_filepath).removeprefix("qsketch_").removesuffix('.txt') + ".png"
    output_filepath= (os.path.join(Constants.PATH_TO_IMAGE_DIR, output_filename))
    plt.savefig(output_filepath)

if __name__ == "__main__":
    main()