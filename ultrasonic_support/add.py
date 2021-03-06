#Libraries
import RPi.GPIO as GPIO
import time
import mysql.connector

mydb = mysql.connector.connect(
	host = "172.20.10.6",
	user = "cam2",
	password = "XSW@2wsx",
	database = "SmartCampus"
)

mycursor = mydb.cursor()
 
#GPIO Mode (BOARD / BCM)
GPIO.setmode(GPIO.BCM)
 
#set GPIO Pins
GPIO_TRIGGER = 18
GPIO_ECHO = 24
 
#set GPIO direction (IN / OUT)
GPIO.setup(GPIO_TRIGGER, GPIO.OUT)
GPIO.setup(GPIO_ECHO, GPIO.IN)

#alreadyDetected = None
 
def motionSensor():
    global alreadyDetected
    # set Trigger to HIGH
    GPIO.output(GPIO_TRIGGER, True)
 
    # set Trigger after 0.01ms to LOW
    time.sleep(0.00001)
    GPIO.output(GPIO_TRIGGER, False)
 
    StartTime = time.time()
    StopTime = time.time()
 
    # save StartTime
    while GPIO.input(GPIO_ECHO) == 0:
        StartTime = time.time()
 
    # save time of arrival
    while GPIO.input(GPIO_ECHO) == 1:
        StopTime = time.time()
 
    # time difference between start and arrival
    TimeElapsed = StopTime - StartTime
    # multiply with the sonic speed (34300 cm/s)
    # and divide by 2, because there and back
    distance = (TimeElapsed * 34300) / 2

    if(distance < 60):
	#print("Motion Detected")
	if(alreadyDetected == False):
	    #global alreadyDetected
	    alreadyDetected = True
	    print("Motion Detected")
	    sql = "UPDATE Campus SET Population = Population + 1 WHERE Location = 'Cafeteria'"
	    mycursor.execute(sql)
	    mydb.commit()
	    return distance
	else:
	    return distance
    else:
	#global alreadyDetected
	alreadyDetected = False
	return distance
 
if __name__ == '__main__':
    try:
        while True:
            dist = motionSensor()
            #print ("Measured Distance = %.1f cm" % dist)
            time.sleep(0.1)
 
        # Reset by pressing CTRL + C
    except KeyboardInterrupt:
        print("Measurement stopped by User")
        GPIO.cleanup()
