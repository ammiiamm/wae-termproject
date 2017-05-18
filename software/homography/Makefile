# the program to build
NAME		:= homography

# Build tools and flags
CXX			:= /usr/bin/g++
LD			:= /usr/bin/g++

PKG_CONFIG_LIBS = opencv

export PKG_CONFIG_PATH=/usr/local/lib/pkgconfig

CPPFLAGS	:= -I /usr/local/include \
				-I /usr/include \
				-I/usr/local/ \
                                -I ../../include \
				`pkg-config --cflags $(PKG_CONFIG_LIBS)` \
				-I ./ 

CXXFLAGS	:=  -O3 #e.g., CXXFLAGS=-g -O0 for debugging

# To keep the makefile for 32 and 64 in sync we add 64 and 32-bit library paths
# If you are targeting only 32 bit for you can remove the entries containing "64"
LDFLAGS		:= `pkg-config --cflags $(PKG_CONFIG_LIBS)` \
				-L/usr/local/lib \
				-L/usr/lib \
	                        -L ../../lib \
				-L. 

LIBS		:=              -lopencv_core -lopencv_highgui -lopencv_video -lopencv_imgproc \
				-lvlccore -lvlc -lboost_thread -lboost_system -lboost_filesystem\
				`pkg-config --libs $(PKG_CONFIG_LIBS)` -lzmq

all		    : $(NAME)

$(NAME)		: 	$(NAME).o

	$(LD) $(LDFLAGS) -o $@ $^ $(LIBS)
	
ProjectIncludes := ./*.h	
%.o 		: %.cpp ${ProjectIncludes}
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -c -o $@ $<

clean		:
	$(RM) *.o $(NAME)
