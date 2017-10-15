doc:baiduDoc.cpp
	g++ -g baiduDoc.cpp -lcurl -o doc

clean:
	rm -rf doc
	rm -rf *.swf
