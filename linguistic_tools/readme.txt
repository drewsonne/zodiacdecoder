reader.cpp
v 1.0

Tanner Oakes
Linguistics
10.18.07

To use this program compile it:
	g++ reader.cpp -o reader
Or use the compiler of your choice.

Run the executable, type the name of the text file (no spaces in the name please) that you want to scan and press enter.

If you want to analyze additional files, you can do so, the reader will add the new data into its report. Once you have read all the files you want, type report and the program will output its data to the following files:
	bigraphs.txt
	trigraphs.txt
	tetragraphs.txt
	
This is the format (the actual reports have spaces instead of tabs):

	ABCD 		17 					0.0034
	
	n-graph		# of occurences		percentage of population
	
Please report any bugs or feature requests to tanneroakes@gmail.com