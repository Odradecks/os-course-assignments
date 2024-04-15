#!/bin/bash

# Define file names
src="src"
dst="dst"

# Open source file for reading
exec 3<"$src"

# Create destination file for reading
exec 4>"$dst"

while IFS= read -r -n 4096 data; do  # reads from file descriptor 3 (&3) in chunks of 4096 bytes and stores in `data` variable
	printf '%s' "$data" >&4  # prints the content of the `data` variable to file descriptor 4 (&4).
done <&3

# Close files
exec 3>&-
exec 4>&-

echo "File copied successfully."
