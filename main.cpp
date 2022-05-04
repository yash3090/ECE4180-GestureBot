#include "mbed.h"
#include "MMA8452.h"

// Mbed serial and pins
Serial pc(USBTX, USBRX);
Serial esp(p28, p27); // tx, rx
DigitalOut  led1(LED1);
DigitalOut  led2(LED2);
DigitalOut  led3(LED3);
DigitalOut  led4(LED4);
InterruptIn light(p16); // Light control
InterruptIn  speed(p17); // speed control
 
char ssid[32] = "Yashphone";     //  ssid from huzzahfruit wifi
char pwd [32] = "Yash3090"; // password
char command[10];
 
 
// code for sending/receiving data over serial huzzahfruit

volatile int rx_out=0;
volatile int tx_in=0;
volatile int rx_in=0;
volatile int tx_out=0;
const int buffer_size = 4095;
char tx_buffer[buffer_size+1];
char rx_buffer[buffer_size+1];
void send_line();
void read_line();
void Tx_interrupt();
void Rx_interrupt();
int DataRX;
int update;
char cmdbuff[1024];
char replybuff[4096];
char webdata[4096]; // This may need to be bigger depending on WEB browser used
char webbuff[4096];     // Currently using 1986 characters, Increase this if more web page data added
char timebuf[30];
void SendCMD(),getreply(),ReadWebData(),startserver();
char rx_line[1024];

void getcommand(double mx, double my, double mz);
double x, y, z;
int speed_control = 0; // 3 speed settings slow, medium, fast
bool light_control = false; // light control var


// interrupt function called when light button pressed 
// toggles the light
void light_button() {
    light_control = !light_control; 
    pc.printf("light_button hit");  
}

// interrupt function called when speed button pressed
// cycles through slow, medium, fast speed
void speed_button() {
    speed_control = (speed_control+1)%3;  
     pc.printf("speed_button hit"); 
}

int main()
{
    // SetUp
    MMA8452 acc(p9, p10, 100000);
    speed.mode(PullUp);
    speed.fall(&speed_button);
    light.mode(PullUp);
    light.fall(&light_button);
    pc.baud(9600);
    esp.baud(9600);
    led1=1,led2=0,led3=0, led4=0;
    // Setup a serial interrupt function to receive data
    esp.attach(&Rx_interrupt, Serial::RxIrq);
    // Setup a serial interrupt function to transmit data
    esp.attach(&Tx_interrupt, Serial::TxIrq);
    
    startserver();
    DataRX=0;
    while(1) {
        
        if(DataRX==1) {
            ReadWebData();
            esp.attach(&Rx_interrupt, Serial::RxIrq);
        }
        if(update==1) 
        {
            // get new command
            acc.readXYZGravity(&x,&y,&z);
            getcommand(x,y,z);
            
            // send new command
            sprintf(cmdbuff, "time,heading,analog2=%d,\"%s\"\r\n",timebuf,command);
            SendCMD();
            getreply();
            update=0;   
        }
    }
}
 
// Reads and processes GET and POST web data
void ReadWebData() {
    wait_ms(200);
    esp.attach(NULL,Serial::RxIrq);
    DataRX=0;
    memset(webdata, '\0', sizeof(webdata));
    strcpy(webdata, rx_buffer);
    memset(rx_buffer, '\0', sizeof(rx_buffer));
    rx_in = 0;
    rx_out = 0;
    // check web data for form information
    if( strstr(webdata, "POST") != NULL ) { // set update flag if POST request
        update=1;
    }
    if( strstr(webdata, "GET") != NULL && strstr(webdata, "favicon") == NULL ) { // set update flag for GET request but do not want to update for favicon requests
        update=1;
    }
}
// Starts webserver
void startserver(){
    
    pc.printf("++++++++++ Resetting ESP ++++++++++\r\n");
    strcpy(cmdbuff,"node.restart()\r\n");
    SendCMD();
    wait(2);
    getreply();
    
    pc.printf("\n++++++++++ Starting Server ++++++++++\r\n> ");
 
    sprintf(cmdbuff, "time,heading=\"%s\",\"%s\"\r\n",timebuf,command);
    SendCMD();
    getreply();
    wait(0.5);
 
    //create server
    sprintf(cmdbuff, "srv=net.createServer(net.TCP,%d)\r\n",4); // second arg is temout
    SendCMD();
    getreply();
    wait(0.5);
    
    strcpy(cmdbuff,"srv:listen(80,function(conn)\r\n");
    SendCMD();
    getreply();
    wait(0.3);
    
    strcpy(cmdbuff,"conn:on(\"receive\",function(conn,payload) \r\n");
    SendCMD();
    getreply();
    wait(0.3);
 
    //print data to mbed
    strcpy(cmdbuff,"print(payload)\r\n");
    SendCMD();
    getreply();
    wait(0.2);
 
    //web page data
    strcpy(cmdbuff,"conn:send('<!DOCTYPE html><html><body><h1>ESP8266 Mbed IoT Web Controller</h1>')\r\n");
    SendCMD();
    getreply();
    wait(0.4);
 
    strcpy(cmdbuff,"conn:send('Command: '..heading..' Robot<br><hr></div>')\r\n");
    SendCMD();
    getreply();
    wait(0.3);
    
    strcpy(cmdbuff,"conn:send('<form method=\"POST\" id =\"formid\"')\r\n");
    SendCMD();
    getreply();
    wait(0.3);
 
 
    strcpy(cmdbuff,"conn:send('<p><input type=\"submit\" value=\"press\"></form></body></html>')\r\n");
    SendCMD();
    getreply();
    wait(0.3);
 
    // end web page data
    strcpy(cmdbuff, "conn:on(\"sent\",function(conn) conn:close() end)\r\n"); // close connection
    SendCMD();
    getreply();
    wait(0.3);
    
    strcpy(cmdbuff, "end)\r\n");
    SendCMD();
    getreply();
    wait(0.2);
 
    strcpy(cmdbuff, "end)\r\n");
    SendCMD();
    getreply();
    wait(0.2);
 
    strcpy(cmdbuff, "tmr.alarm(0, 1000, 1, function()\r\n");
    SendCMD();
    getreply();
    wait(0.2);
    
    strcpy(cmdbuff, "if wifi.sta.getip() == nil then\r\n");
    SendCMD();
    getreply();
    wait(0.2);
    
    strcpy(cmdbuff, "print(\"Connecting to AP...\\n\")\r\n");
    SendCMD();
    getreply();
    wait(0.2);
    
    strcpy(cmdbuff, "else\r\n");
    SendCMD();
    getreply();
    wait(0.2);
    
    strcpy(cmdbuff, "ip, nm, gw=wifi.sta.getip()\r\n");
    SendCMD();
    getreply();
    wait(0.2);
    
    strcpy(cmdbuff,"print(\"IP Address: \",ip)\r\n"); //IP address to connect to for pi i.e of webserver
    SendCMD();
    getreply();
    wait(0.2);
    
    strcpy(cmdbuff,"tmr.stop(0)\r\n");
    SendCMD();
    getreply();
    wait(0.2);
    
    strcpy(cmdbuff,"end\r\n");
    SendCMD();
    getreply();
    wait(0.2);
    
    strcpy(cmdbuff,"end)\r\n");
    SendCMD();
    getreply();
    wait(0.2);
    
    pc.printf("\n\n++++++++++ Ready ++++++++++\r\n\n");
}
 
 
// ESP Command data send
void SendCMD(){
    int i;
    char temp_char;
    bool empty;
    i = 0;
    NVIC_DisableIRQ(UART1_IRQn);
    empty = (tx_in == tx_out);
    while ((i==0) || (cmdbuff[i-1] != '\n')) {
// Wait if buffer full
        if (((tx_in + 1) % buffer_size) == tx_out) {
            NVIC_EnableIRQ(UART1_IRQn);
            while (((tx_in + 1) % buffer_size) == tx_out) {
            }
            NVIC_DisableIRQ(UART1_IRQn);
        }
        tx_buffer[tx_in] = cmdbuff[i];
        i++;
        tx_in = (tx_in + 1) % buffer_size;
    }
    if (esp.writeable() && (empty)) {
        temp_char = tx_buffer[tx_out];
        tx_out = (tx_out + 1) % buffer_size;
        esp.putc(temp_char);
    }
    NVIC_EnableIRQ(UART1_IRQn);
    return;
}
 
// Get Command and ESP status replies
void getreply() {
    read_line();
    sscanf(rx_line,replybuff);
}
 
// Read a line from the large rx buffer from rx interrupt routine
void read_line() {
    int i;
    i = 0;
    NVIC_DisableIRQ(UART1_IRQn);
    while ((i==0) || (rx_line[i-1] != '\r')) {
        if (rx_in == rx_out) {
            NVIC_EnableIRQ(UART1_IRQn);
            while (rx_in == rx_out) {
            }
            NVIC_DisableIRQ(UART1_IRQn);
        }
        rx_line[i] = rx_buffer[rx_out];
        i++;
        rx_out = (rx_out + 1) % buffer_size;
    }
    NVIC_EnableIRQ(UART1_IRQn);
    rx_line[i-1] = 0;
    return;
}
 

// Interupt Routine to read in data from serial port
void Rx_interrupt() {
    DataRX=1;
// Stop if buffer full
    while ((esp.readable()) && (((rx_in + 1) % buffer_size) != rx_out)) {
        rx_buffer[rx_in] = esp.getc();

        pc.putc(rx_buffer[rx_in]);
        rx_in = (rx_in + 1) % buffer_size;
    }
    return;
}
 
 
// Interupt Routine to write out data to serial port
void Tx_interrupt() {
// Stop if buffer empty
    while ((esp.writeable()) && (tx_in != tx_out)) {
        esp.putc(tx_buffer[tx_out]);
        tx_out = (tx_out + 1) % buffer_size;
    }
    return;
}
 

 
// Heading calculation
void getcommand(double mx, double my, double mz){
    int dir = 0; // DETERMINES COMMAND 0-29
    if (  (-0.85 < x) && (x < -0.5) && (-0.35 < y) && (y < 0.1) && (0.5 < z) && (z<0.8) ){ // right
            dir = 3;
             led1 = 1;
        led2 = 0;
        led3 = 0;
        led4 = 0;
            }else if ((0.55 < x) and  (x < 0.95) && (-0.15 < y) and (y < 0.15) and (0.4<z) and (z<0.8)) { // left
            dir = 4;
            led1 = 0;
        led2 = 1;
        led3 = 0;
        led4 = 0;
            }else if ((-0.05 < x) and  (x < 0.15) && (-0.95 < y) and (y < -0.7) and (0.35<z) and (z<0.65)) { // forward
            dir = 1;
            led1 = 0;
        led2 = 0;
        led3 = 1;
        led4 = 0;
            }else if ((-0.1 < x) and  (x < 0.2) && (0.4 < y) and (y < 0.8) and (0.65<z) and (z<1.1)) { // backward
            dir = 2;
            led1 = 0;
        led2 = 0;
        led3 = 0;
        led4 = 1;
            } else { //stationary
            dir = 0;
            led1 = 0;
        led2 = 0;
        led3 = 0;
        led4 = 0;
    }
    
    if(light_control) {
        dir += 5;  
    } 
    dir += (speed_control*10);
    pc.printf("%d", speed_control);
    sprintf(command, "%d", dir);
     
    }