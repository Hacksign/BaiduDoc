doc:baiduDoc.cpp
	g++ -g -lcurl baiduDoc.cpp -o doc

clean:
	rm -rf doc
	rm -rf *.swf
