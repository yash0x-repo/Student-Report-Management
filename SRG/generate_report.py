# generate_report.py
import sys
import pandas as pd
import matplotlib.pyplot as plt

def create_report_card(df, output_image):
    fig, ax = plt.subplots(figsize=(12, 0.6 + 0.3 * len(df)))
    ax.axis('off')
    table = ax.table(cellText=df.values, colLabels=df.columns, loc='center', cellLoc='center')
    table.auto_set_font_size(False)
    table.set_fontsize(10)
    table.scale(1, 1.5)
    plt.title("Student Report Card", fontsize=14, weight='bold')
    plt.savefig(output_image, bbox_inches='tight')
    print(f"Saved image to {output_image}")

if __name__ == "__main__":
    roll = sys.argv[1]  # Take roll number from command-line
    filename = f"transcript_{roll}.csv"
    output_image = f"report_card_{roll}.png"
    try:
        df = pd.read_csv(filename)
        create_report_card(df, output_image)
    except Exception as e:
        print(f"Error generating report: {e}")
