# the program to build
NAME		:= CamViewTest

# Build tools and flags
CXX			:= /usr/bin/g++
LD			:= /usr/bin/g++
CPPFLAGS	:= -I /usr/local/include \
				-I /usr/local/include/opencv \
				-I ../../include \
				-I /usr/include \
				-I /usr/include/ImageMagick \
				-I ./

CXXFLAGS	:=  -O3 #e.g., CXXFLAGS=-g -O0 for debugging

# To keep the makefile for 32 and 64 in sync we add 64 and 32-bit library paths
# If you are targeting only 32 bit for you can remove the entries containing "64"
LDFLAGS		:= -L/usr/local/opencv-2.4.9/lib \
				-L/usr/lib \
				-L ../../lib \
				-L.

LIBS		:= -lVGL -lcurl -lopencv_flann -lopencv_core -lopencv_highgui -lopencv_calib3d -lopencv_video -lopencv_imgproc  \
				-lopencv_ml -lopencv_objdetect -lopencv_features2d\
				-lvlccore -lvlc -lopencv_nonfree -lopencv_legacy -lopencv_calib3d\
				-lboost_thread -lboost_system -ltbb -lboost_date_time -lMagick++ \
				-lMagickCore `pkg-config --libs openssl`

all		    : $(NAME)

$(NAME)		: 	$(NAME).o CamView.o				
	$(LD) $(LDFLAGS) -o $@ $^ $(LIBS)

ProjectIncludes:= ./*.h	
%.o 		: %.cpp ${ProjectIncludes}
	$(CXX) $(CPPFLAGS) $(CXXFLAGS)  -c -o $@ $<

clean		:
	$(RM) *.o $(NAME) 
