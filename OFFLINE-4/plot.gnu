# Set the input data file and output image file
datafile = 'output.csv'
outputfile = 'ratio_vs_mean.png'

# Set the plot properties
set title 'split ratio vs mean'
set xlabel 'split ratio'
set ylabel 'mean'

# Specify the input data format and delimiter
set datafile separator ','

# Create the scatter plot
plot datafile using 1:2 with points title 'split ratio vs mean'