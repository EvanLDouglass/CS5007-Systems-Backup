# Basic Bash script
# Author: Evan Douglass
# Created: March 12 2019

# Display number of processors
# The command used here was found at:
# https://www.ostechnix.com/find-number-cpu-cores-commandline-linux/
PROC=$(nproc)
echo "cores: $PROC"

# Display system memory properties
# Regex syntax for bash found at
# https://www.geeksforgeeks.org/awk-command-unixlinux-examples/
# grep portion finds a line with the memory type and pipes
# it into awk, which prints the 2nd and 3rd fields in the
# line, the number and units
TOTAL=$(grep MemTotal /proc/meminfo | awk '{print $2,$3}')
FREE=$(grep MemFree /proc/meminfo | awk '{print $2,$3}')
echo "memory: $FREE/$TOTAL"
