
import urllib.request

headStr = "http://image.slidesharecdn.com/cc2500-130410225125-phpapp01/95/cc2500-"
tailStr = "-1024.jpg?cb=1365661093"

for i in range(1, 43):
	#Input String
	linkStr = headStr + str(i) + tailStr
	
	#Output String
	if i < 10:
		outputFileStr = "00" + str(i) + ".png"
	elif i < 100:
		outputFileStr = "0" + str(i) + ".png"
	else:
		outputFileStr = str(i) + ".png"
	
	#Get Page/File ...
	r = urllib.request.urlopen(linkStr)
	
	#Save to file
	with open(outputFileStr, 'b+w') as f:
		f.write(r.read())
	
	print("Download: " + outputFileStr + " complete ...")
