BINPATH = 
OBJPATH = obj/
INCS = 
VPATH = src/

CXXFLAGS = -O2 $(INCS)
LIBS = c:\MinGW\lib\libuser32.a
DLLS = 
OBJS =  $(OBJPATH)algorithm.o $(OBJPATH)kalman.o\
		$(OBJPATH)main.o $(OBJPATH)SetCursorTest.o 

ShapeHuman : $(DLLS) $(OBJS)
	g++ $(CXXFLAGS) $(DLLS) $(OBJS) -o MIMUPC $(LIBS) 2>error.txt
	@echo make successfully!

$(OBJPATH)main.o : main.cpp
	@echo building object $@
	@g++ $(CXXFLAGS) -c $< -o $@ 2>error.txt

$(OBJPATH)%.o : %.cpp %.h
	@echo building object $@
	@g++ $(CXXFLAGS) -c $< -o $@ 2>error.txt
	
clean:
	@echo "clear"
	@rm -rf $(BIN_PATH)
