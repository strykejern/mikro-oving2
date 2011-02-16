import os

sourcefolder = "src"
headerfolder = "headers"

def validFile(file): return file.endswith(".c")

for srcFile in filter(validFile, os.listdir(sourcefolder)):
	
	sourcefile = open(sourcefolder + "/" + srcFile)
	
	functionList = []
	
	line = sourcefile.readline()
	while line:
		if line.startswith("/**"):
			line = sourcefile.readline()
			functionList.append(line[:line.find(")")+1])
		
		line = sourcefile.readline()
		
	print sourcefile.name
	for func in functionList:
		print "\t" + func
		
	if len(functionList):
		headername = sourcefile.name.split("/")[1][:-2] + ".h"
		headerfile = None
		
		try:
			headerfile = open(headerfolder + "/" + headername)
		except:
			headerfile = open(headerfolder + "/" + headername, 'w')
			
			headerfile.write("#ifndef " + headername[:-2].upper() + "_HEADER_INCLUDED\n")
			headerfile.write("#define " + headername[:-2].upper() + "_HEADER_INCLUDED\n")
			
			for func in functionList:
				headerfile.write(func + ";\n")
				
			headerfile.write("#endif\n")
			
		headerfile.close()
		
		print headername
	
