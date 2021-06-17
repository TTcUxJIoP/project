all: qwe

run: 
	g++ fps.cpp -o DisplayImage `pkg-config opencv --cflags --libs`
	./DisplayImage	
test: 
	g++ qwe.cpp -o DisplayImage `pkg-config opencv --cflags --libs`
	./DisplayImage
