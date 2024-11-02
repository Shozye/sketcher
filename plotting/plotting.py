from matplotlib import pyplot as plt

def main(filename_ending: str):
    qsketch_filename = "../qsketch"+filename_ending
    fastexpsketch_filename = "../fastexpsketch"+filename_ending
    qsketch_relweight = 0
    fastexpsketch_relweight = 0
    average_relative_errors = []

    with open(fastexpsketch_filename) as fesketchfile:
        while True:
            line = fesketchfile.readline()
            if line == "":
                break
            if line.startswith("Average Relative Error"):
                fastexpsketch_relweight = float(line.split()[4][:-1])

    with open(qsketch_filename) as file:
        args_line = file.readline()[:-1]
        sketch_name = args_line.split()[3][:-1]
        elements = f"{int(args_line.split()[7][:-1]):.0e}"
        sketches = f"{int(args_line.split()[9][:-1]):.0e}"

        a = float(args_line.split()[13][:-1])
        a = int(a) if a == int(a) else a
        b = float(args_line.split()[15][:-1])
        b = int(b) if b == int(b) else b
        distribution = args_line.split()[11][:-1] + "(" + str(a) + ","+str(b) + ")"
        while True:
            line = file.readline()
            if line == "":
                break

            if line.startswith("Average Relative Error"):
                qsketch_relweight = float(line.split()[4][:-1])
                
            elif line.startswith("Average relative errors:"):
                for relweight in line.split()[3:]:
                    average_relative_errors.append(float(relweight[:-1]))


    min_average_error = min(average_relative_errors)
    x_of_min_average_error = average_relative_errors.index(min_average_error)/(len(average_relative_errors)-1)

    rangeA = 4500
    rangeB = 6001
    xs = [x/(len(average_relative_errors)-1) for x in range(len(average_relative_errors))][rangeA:rangeB]
    fast_exp_sketch_ys = [fastexpsketch_relweight for _ in xs]
    qsketch_newton_ys = [qsketch_relweight for _ in xs]
    qsketch_normal_ys = average_relative_errors[rangeA:rangeB]


    # Plotting the data
    plt.figure(figsize=(8, 5))
    plt.plot(xs, fast_exp_sketch_ys, label="FastExpSketch Relative Error [%]", linestyle="--", color="blue")
    plt.plot(xs, qsketch_newton_ys, label="QSketch Relative Error with Newton estimation [%]", linestyle="--", color="green")
    plt.plot(xs, qsketch_normal_ys, label="QSketch Relative Error with direct estimation [%]", color="red")
    plt.scatter([x_of_min_average_error], [min_average_error], color="red", label=f"QSketch min ({x_of_min_average_error}, {min_average_error})")

    # Adding text annotations for constant values
    plt.text(xs[0], fastexpsketch_relweight + 0.05, f"FESketch = {fastexpsketch_relweight:.3f}", color="blue")
    plt.text(xs[0], qsketch_relweight + 0.05, f"QSketch (Newton) = {qsketch_relweight:.3f}", color="green")

    
    # Adding QoL features
    plt.xlabel("Argument of QSketch.estimate, Translates S[i] into 2^{-S[i]-x};")
    plt.ylabel("Relative Error of total weight estimation in %")

    title = f"Comparison of QSketch Estimators and FESketch. \nelements={elements}, sketches={sketches}, distribution={distribution}, size={400}"
    plt.title(title)
    plt.legend()
    plt.grid(True)
    plt.tight_layout()

    # Display the plot
    plt.savefig(filename_ending.split(".")[0]+"_new.png")

if __name__ == "__main__":
    import sys
    print(sys.argv[1])
    main(sys.argv[1])