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
# it into awk, which converts the number from kilobytes into
# megabytes and prints the result.
TOTAL=$(grep MemTotal /proc/meminfo | awk '{print $2/1000}')
FREE=$(grep MemFree /proc/meminfo | awk '{print $2/1000}')
echo "memory: $FREE MB/$TOTAL MB"

# Display partition storage
# The awk command adds 0 to the field to convert it to a number
SIZE=$(df -H / | grep /dev/sda2 | awk '{print $2+0}')
AVAIL=$(df -H / | grep /dev/sda2 | awk '{print $4+0}')
echo "primary partition storage: $AVAIL GB/$SIZE GB"
