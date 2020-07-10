LIBS =  -lpng
IOSRC = src/io_png
BIN = bin
RASRC = src/ra
IMGSRC = src/image
MSSRC = src/ms
EXECUTABLENAME = meanshift
EXECUTABLENAMEFILTER = msfilter
CFLAGS = -O2 -ansi -pedantic -Wall -Wextra
CC = g++ 



all: $(BIN)/$(EXECUTABLENAME)  $(BIN)/$(EXECUTABLENAMEFILTER)

$(BIN)/$(EXECUTABLENAME): src/meanshift.o $(MSSRC)/ms.o  $(RASRC)/raList.o $(IMGSRC)/image.o $(IOSRC)/io_png.o $(RASRC)/TransitiveClosure.o  
	$(CC) $(CFLAGS) src/meanshift.o  $(RASRC)/raList.o $(RASRC)/TransitiveClosure.o $(IOSRC)/io_png.o $(IMGSRC)/image.o $(MSSRC)/ms.o -o bin/$(EXECUTABLENAME) $(LIBS)
	
$(BIN)/$(EXECUTABLENAMEFILTER):  src/msfilter.o $(MSSRC)/ms.o  $(RASRC)/raList.o $(IMGSRC)/image.o $(IOSRC)/io_png.o $(RASRC)/TransitiveClosure.o  
	$(CC) $(CFLAGS) src/msfilter.o  $(RASRC)/raList.o $(RASRC)/TransitiveClosure.o $(IOSRC)/io_png.o $(IMGSRC)/image.o $(MSSRC)/ms.o -o bin/$(EXECUTABLENAMEFILTER) $(LIBS)

meanshift.o: src/meanshift.cpp 
	$(CC) $(CFLAGS)  -c src/meanshift.cpp $(LIBS) -o $(BIN)/meanshift
	
msfilter.o: src/msfilter.cpp 
	$(CC) $(CFLAGS)  -c src/msfilter.cpp $(LIBS) -o $(BIN)/msfilter

$(MSSRC)/ms.o: $(MSSRC)/ms.cpp $(MSSRC)/ms.h 
	$(CC) $(CFLAGS)  -c $(MSSRC)/ms.cpp -o $(MSSRC)/ms.o
	
$(RASRC)/raList.o: $(RASRC)/RAList.cpp $(RASRC)/RAList.h 
	$(CC) $(CFLAGS)  -c $(RASRC)/RAList.cpp  -o $(RASRC)/raList.o
	
$(RASRC)/TransitiveClosure.o: $(RASRC)/TransitiveClosure.cpp $(RASRC)/TransitiveClosure.h
	$(CC) $(CFLAGS)  -c $(RASRC)/TransitiveClosure.cpp  -o $(RASRC)/TransitiveClosure.o
		
$(IMGSRC)/image.o: $(IMGSRC)/image.cpp $(IMGSRC)/image.h
	$(CC) $(CFLAGS)  -c $(IMGSRC)/image.cpp  -o $(IMGSRC)/image.o

$(IOSRC)/io_png.o: $(IOSRC)/ $(IOSRC)/io_png.c $(IOSRC)/io_png.h
	$(CC) $(CFLAGS)  -c $(IOSRC)/io_png.c -o$(IOSRC)/io_png.o

	
.PHONY: clean
clean:
	rm src/msfilter.o src/meanshift.o $(MSSRC)/*.o $(RASRC)/*.o $(IOSRC)/*.o $(IMGSRC)/*.o bin/$(EXECUTABLENAME) bin/$(EXECUTABLENAMEFILTER)
