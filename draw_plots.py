import matplotlib.pyplot as plt
import numpy as np
import os
from scipy.optimize import curve_fit
from matplotlib.backends.backend_pdf import PdfPages

def read_data(filename):
    try:
        with open(filename, 'r') as f:
            content = f.read().strip()
    except FileNotFoundError:
        print(f"Error: File '{filename}' not found.")
        return None
    except Exception as e:
        print(f"Error reading file: {e}")
        return None
    if not content:
        print("Error: File is empty.")
        return None
    try:
        values = list(map(float, content.split()))
    except ValueError as e:
        print(f"Error parsing data: {e}")
        return None

    if len(values) % 5 != 0:
        print(f"Error: Expected multiples of 5 values, got {len(values)} values.")
        return None
    data = np.array(values).reshape(-1, 5)
    print(f"Successfully read {data.shape[0]} data points")
    return data

def log_func(x, a, b):
    return a * np.log(x) + b

def linear_func(x, a, b):
    return a * x + b

def plot_with_fit(ax, x, y, color, label, func):
    # Точки данных
    ax.scatter(x, y, color=color, label=f'{label} Data', alpha=0.6)
    if len(x) > 1:
        popt, _ = curve_fit(func, x, y)
        x_fit = np.linspace(min(x), max(x), 100)
        y_fit = func(x_fit, *popt)
        ax.plot(x_fit, y_fit, '--', color=color,
                label=f'{label} Fit: {func.__name__}')
        ax.text(0.05, 0.95, f'a={popt[0]:.6f}, b={popt[1]:.6f}',
                transform=ax.transAxes, ha='left', va='top',
                bbox=dict(facecolor='white', alpha=0.8))

def create_operation_plots(nodes, insert_times, search_times, delete_times, in_order_times):
    figs = []

    fig1, ax1 = plt.subplots(figsize=(8, 6))
    plot_with_fit(ax1, nodes, insert_times, 'blue', 'Insert', log_func)
    fig1.suptitle('Insert Operation Analysis')
    figs.append(fig1)

    fig2, ax2 = plt.subplots(figsize=(8, 6))
    plot_with_fit(ax2, nodes, search_times, 'red', 'Search', log_func)
    fig2.suptitle('Search Operation Analysis')
    figs.append(fig2)

    fig3, ax3 = plt.subplots(figsize=(8, 6))
    plot_with_fit(ax3, nodes, delete_times, 'green', 'Delete', log_func)

    fig3.suptitle('Delete Operation Analysis')
    figs.append(fig3)

    fig4, ax4 = plt.subplots(figsize=(8, 6))
    plot_with_fit(ax4, nodes, in_order_times, 'purple', 'InOrder', linear_func)
    fig4.suptitle('InOrder Traversal Analysis')
    figs.append(fig4)

    return figs

def save_plots_to_pdf(filename, figures):
    with PdfPages(filename) as pdf:
        for fig in figures:
            pdf.savefig(fig, bbox_inches='tight')
            plt.close(fig)

def main():
    filename = input("Enter the data file name (or 'q' to quit): ").strip()
    if filename.lower() == 'q':
        return

    if not os.path.exists(filename):
        print(f"Error: File '{filename}' does not exist. Please try again.")
        return

    data = read_data(filename)
    if data is not None:
        nodes = data[:, 0]
        insert_times = data[:, 1]
        search_times = data[:, 2]
        delete_times = data[:, 3]
        in_order_times = data[:, 4]

        figures = create_operation_plots(nodes, insert_times, search_times, delete_times, in_order_times)

        output_pdf = filename.split('.')[0] + '_plots.pdf'
        save_plots_to_pdf(output_pdf, figures)
        print(f"All plots saved to {output_pdf}")

        plt.show()

if __name__ == "__main__":
    main()
