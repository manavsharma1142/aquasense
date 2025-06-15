# This Python file uses the following encoding: utf-8

# if __name__ == "__main__":
#     pass
import os
import pandas as pd
import matplotlib.pyplot as plt
import seaborn as sns

# Read the dataset from the CSV file
df = pd.read_csv('../data/data.csv')

# Create a folder named "charts" if it doesn't exist
if not os.path.exists('charts'):
    os.makedirs('charts')

# Histograms for each parameter
for column in df.columns[:-1]:  # Exclude the 'Potability' column
    plt.figure(figsize=(8, 6))
    plt.hist(df[column], bins=10, color='skyblue', edgecolor='black')
    plt.title(f'{column} Distribution')
    plt.xlabel(column)
    plt.ylabel('Frequency')
    plt.grid(True)
    plt.savefig(f'charts/{column}_histogram.png')
    plt.close()

# Box plots for each parameter
for column in df.columns[:-1]:
    plt.figure(figsize=(8, 6))
    df.boxplot(column=column)
    plt.title(f'{column} Box Plot')
    plt.ylabel(column)
    plt.grid(True)
    plt.savefig(f'charts/{column}_boxplot.png')
    plt.close()

# Scatter plot between 'Potability' and each parameter
for column in df.columns[:-1]:
    plt.figure(figsize=(8, 6))
    plt.scatter(df[column], df['Potability'], color='skyblue', alpha=0.6)
    plt.title(f'{column} vs Potability')
    plt.xlabel(column)
    plt.ylabel('Potability')
    plt.grid(True)
    plt.savefig(f'charts/{column}_vs_potability_scatter.png')
    plt.close()

# Bar plot for 'Potability'
plt.figure(figsize=(6, 4))
df['Potability'].value_counts().plot(kind='bar', color='skyblue')
plt.title('Potability Distribution')
plt.xlabel('Potability')
plt.ylabel('Count')
plt.xticks(rotation=0)
plt.grid(True)
plt.savefig('charts/potability_barplot.png')
plt.close()

# Correlation Matrix
plt.figure(figsize=(10, 8))
sns.heatmap(df.corr(), annot=True, cmap='coolwarm')
plt.title('Correlation Matrix Heatmap')
plt.savefig('charts/correlation_matrix_heatmap.png')
plt.close()

print("Visualizations generated and saved successfully in the 'charts' folder!")

