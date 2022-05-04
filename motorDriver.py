import RPi.GPIO as GPIO
import threading
import time
from selenium import webdriver
from webdriver_manager.chrome import ChromeDriverManager
import time
global finished

GPIO.setwarnings(False)
motor_a1 = 5
motor_a2 = 6
motor_b1 = 23
motor_b2 = 24
pwm_motor_a = 12
pwm_motor_b = 18
button = 22
input_val = 0
#Setting up GPIO pins
GPIO.setmode(GPIO.BCM)
GPIO.setup(motor_a1, GPIO.OUT)
GPIO.setup(motor_a2, GPIO.OUT)
GPIO.setup(motor_b1, GPIO.OUT)
GPIO.setup(motor_b2, GPIO.OUT)
GPIO.setup(pwm_motor_a, GPIO.OUT)
GPIO.setup(pwm_motor_b, GPIO.OUT)


#Scrapes web to get command value frmo nodeJS page
def scrape_web():
    global input_val
    chrome_options = webdriver.ChromeOptions()
        #chrome_options.add_argument('--no-sandbox')
    chrome_options.add_argument('-headless')
        #chrome_options.add_argument('--disable-dev-shm-usage')
        #driver = webdriver.Chrome(r"C:\Users\siddh\Desktop\CollegeWork\Semester 6\ECE4180\chromedriver")
    driver = webdriver.Chrome("/usr/lib/chromium-browser/chromedriver", chrome_options=chrome_options)
    try:
       
        driver.get("http://192.168.237.221/")
        start = time.time()
        while True:
            
            if (time.time() > start + 100):
               break
            page = driver.page_source
            a = page.split(":")
            b = a[1].split(" ")
            command = b[1]
            driver.find_element_by_id("formid").submit()
            input_val = int(command)
            print(input_val)
            #print("button pressed")
            #prichromium-browsernt(command)
            #ime.sleep(0.1)
    finally:
        driver.close()


#Test function
def readinput():
    global input_val
    while True:
        input_val = int(input("what should it do:\n"))
        time.sleep(1.5)

#Motor Driver Functions:
def stop():
    GPIO.output(motor_a1, GPIO.LOW)
    GPIO.output(motor_a2, GPIO.LOW)
    GPIO.output(motor_b1, GPIO.LOW)
    GPIO.output(motor_b2, GPIO.LOW)
    pwm_a = GPIO.PWM(pwm_motor_a, 190000)
    pwm_b = GPIO.PWM(pwm_motor_b, 190000)
    pwm_a.start(0)
    pwm_b.start(0) 
    time.sleep(5)

def moveForward(speed):
    GPIO.output(motor_a1, GPIO.LOW)
    GPIO.output(motor_a2, GPIO.HIGH)
    GPIO.output(motor_b1, GPIO.LOW)
    GPIO.output(motor_b2, GPIO.HIGH)
    pwm_a = GPIO.PWM(pwm_motor_a, 190000)
    pwm_b = GPIO.PWM(pwm_motor_b, 190000)
    
    pwm_a.start(speed)
    pwm_b.start(speed)
    time.sleep(0.5)


def moveBackward(speed):
    GPIO.output(motor_a1, GPIO.HIGH)
    GPIO.output(motor_a2, GPIO.LOW)
    GPIO.output(motor_b1, GPIO.HIGH)
    GPIO.output(motor_b2, GPIO.LOW)
    pwm_a = GPIO.PWM(pwm_motor_a, 190000)
    pwm_b = GPIO.PWM(pwm_motor_b, 190000)
    
    pwm_a.start(speed)
    pwm_b.start(speed)
    time.sleep(0.5)
    

def moveLeft(speed):
    GPIO.output(motor_a1, GPIO.LOW)
    GPIO.output(motor_a2, GPIO.HIGH)
    GPIO.output(motor_b1, GPIO.LOW)
    GPIO.output(motor_b2, GPIO.LOW)
    pwm_a = GPIO.PWM(pwm_motor_a, 190000)
    pwm_b = GPIO.PWM(pwm_motor_b, 190000)
    
    pwm_a.start(speed)
    pwm_b.start(speed)
    time.sleep(0.5)

def moveRight(speed):
    GPIO.output(motor_a1, GPIO.LOW)
    GPIO.output(motor_a2, GPIO.LOW)
    GPIO.output(motor_b1, GPIO.LOW)
    GPIO.output(motor_b2, GPIO.HIGH)
    pwm_a = GPIO.PWM(pwm_motor_a, 190000)
    pwm_b = GPIO.PWM(pwm_motor_b, 190000)
    
    pwm_a.start(speed)
    pwm_b.start(speed)
    time.sleep(0.5)


#Runs motor at mentioned speed based on different values
def runMotors():
    global input_val
    print(str(input_val) + "in run motors")
    start = time.time()
    while True:
        direction = int(input_val) % 10
        speed = int(input_val) // 10
        if (speed == 0):
            speed = 50
        elif (speed == 1):
            speed = 75
        elif (speed == 2):
            speed = 100

        if (time.time() > start + 120):
           break
        if(input_val == 1):
            print("Moving forward")
            moveForward(speed)
        elif(input_val == 2):
            print("Moving backward")
            moveBackward(speed)
        elif(input_val == 3):
            print("Moving right")
            moveRight(speed)
        elif(input_val == 4):
            print("Moving left")
            moveLeft(speed)


def main():
    #starts motor thread
    motor_thread = threading.Thread(target = runMotors, name = "motor_thread")
    #starts web scrapping thread
    web_thread = threading.Thread(target = scrape_web, name = "web_thread")
    motor_thread.start()
    web_thread.start()
    motor_thread.join()
    web_thread.join()

    print("power off")
    GPIO.cleanup()

if __name__ == "__main__":
    main()
