CXXFLAGS =	-O2 -g -Wall -fmessage-length=0

OBJS =		Robot.o raspi-robot.o

LIBS =		-lopencv_core -lopencv_contrib -lopencv_highgui -lraspicam -lraspicam_cv -lopencv_objdetect -lopencv_imgproc -lwiringPi -lStepperMotor28BYJ48

TARGET =	raspi-robot

$(TARGET):	$(OBJS)
	$(CXX) -o $(TARGET) $(OBJS) $(LIBS)

all:	$(TARGET)

clean:
	rm -f $(OBJS) $(TARGET)
