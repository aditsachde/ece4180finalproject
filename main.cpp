#include "EthInterface.h"
#include "EthernetInterface.h"
#include "TCPSocket.h"
#include "ThisThread.h"
#include "Thread.h"
#include "mbed.h"
#include "nsapi_types.h"
#include "USBSerial.h"
#include "LSM9DS1.h"

EthernetInterface eth;
Serial pc(USBTX, USBRX); // tx, rx

SocketAddress ip("192.168.1.2");
SocketAddress netmask("255.255.255.0");
SocketAddress gateway("192.168.1.1");

char const *httpresponse = "HTTP/1.1 200 Everything Is Just Fine\n\
Server: netcat!\n\
Content-Type: text/html; charset=UTF-8\n\
\n\
<!doctype html>\n\
<html>\n\
<body>\n\
<h1>A webpage served with netcat</h1>\n\
</body>\n\
</html>";

// main() runs in its own thread in the OS
int main()
{
    pc.printf("\n\n\n\n");

    LSM9DS1 imu(p9, p10, 0xD6, 0x3C);
    if (imu.begin()) {
        pc.printf("Successfully communicated with LSM9DS1.\n");
    } else {
        pc.printf("Failed to communicate with LSM9DS1.\n");
    }

    pc.printf("Calibrating IMU\n");
    imu.calibration();

    nsapi_error_t status;
    pc.printf("Initializing mbed ethernet\n");
    
    status = eth.set_network(ip, netmask, gateway);
    pc.printf("Set ethernet network parameters with status: %d\n", status);

    status = eth.connect();
    pc.printf("Brought up ethernet port with status: %d\n", status);

    TCPSocket sock;

    status = sock.open(&eth);
    pc.printf("Opened socket on eth with status: %d\n", status);

    status = sock.bind(80);
    pc.printf("Binded socket to port 80 with status: %d\n", status);

    status = sock.listen();
    pc.printf("Listening on socket with status: %d\n", status);

    while (true) {
        TCPSocket *client = sock.accept(&status);
        pc.printf("Accepted client  with status: %d\n", status);

        status = client->send(httpresponse, strlen(httpresponse));
        pc.printf("Sent client data with status: %d\n", status);

        status = client->close();
        pc.printf("Closed client socket with status: %d\n", status);

    }
}

