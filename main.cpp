#include "EthInterface.h"
#include "EthernetInterface.h"
#include "Filter.h"
#include "LSM9DS1.h"
#include "Mutex.h"
#include "Quaternion.h"
#include "TCPSocket.h"
#include "ThisThread.h"
#include "Thread.h"
#include "USBSerial.h"
#include "html.h"
#include "mbed.h"
#include "nsapi_types.h"
#include <cstdio>

EthernetInterface eth;
Serial pc(USBTX, USBRX); // tx, rx

SocketAddress ip("192.168.1.2");
SocketAddress netmask("255.255.255.0");
SocketAddress gateway("192.168.1.1");

Thread filterthread;
Mutex filtermutex;
Filter filter;
LSM9DS1 imu(p9, p10, 0xD6, 0x3C);
void filter_thread(void);

#define POST_DATA_LEN 50
char postdata[POST_DATA_LEN];

// main() runs in its own thread in the OS
int main() {
  pc.printf("\n\n\n\n");

  if (imu.begin()) {
    pc.printf("Successfully communicated with LSM9DS1.\n");
  } else {
    pc.printf("Failed to communicate with LSM9DS1.\n");
  }

  pc.printf("Calibrating IMU\n");
  imu.calibrate(1);

  nsapi_error_t status;
  pc.printf("Initializing mbed ethernet\n");

  status = eth.set_network(ip, netmask, gateway);
  pc.printf("Set ethernet network parameters with status: %d\n", status);

  status = eth.connect();
  pc.printf("Brought up ethernet port with status: %d\n", status);

  TCPSocket sock;

  // Start filtering thread
  filterthread.start(filter_thread);
  filterthread.set_priority(osPriorityHigh);

reset:

  status = sock.open(&eth);
  pc.printf("Opened socket on eth with status: %d\n", status);

  status = sock.bind(80);
  pc.printf("Binded socket to port 80 with status: %d\n", status);

  status = sock.listen();
  pc.printf("Listening on socket with status: %d\n", status);

  while (true) {
    TCPSocket *client = sock.accept(&status);
    pc.printf("Accepted client  with status: %d\n", status);

    if (status < 0) {
      // Sometimes we start to run into some odd issues.
      // In this case, we reset the listening socket
      sock.close();
      goto reset;
    }

    char method;

    client->recv(&method, 1);
    pc.printf("Recieved request with method %c\n", method);

    if (method == 'G') {
      status = client->send(gethtml, strlen(gethtml));
      pc.printf("Responded to GET request ");
    } else {
      if (method == 'P') {
        pc.printf("Responded to POST request ");
        status = client->send(postheader, strlen(postheader));
      } else {
        pc.printf("Responded to raw request ");
      }

      float roll, pitch, yaw;

      filtermutex.lock();
      filter.euler(&roll, &pitch, &yaw);
      filtermutex.unlock();

      int responsesize =
          snprintf(postdata, POST_DATA_LEN, "%f, %f, %f", roll, pitch, yaw);
      status = client->send(postdata, responsesize);
    }
    pc.printf("with status: %d\n", status);

  cleanup:
    status = client->close();
    pc.printf("Closed client socket with status: %d\n", status);
  }
}

void filter_thread(void) {
  while (true) {
    ThisThread::sleep_for(TS_MS);
    // pc.printf("Running filter step\n");
    imu.readAccel();
    imu.readGyro();
    filtermutex.lock();
    filter.step(imu.calcAccel(imu.ax), imu.calcAccel(imu.ay),
                imu.calcAccel(imu.az), imu.calcGyro(imu.gx) * DEG_TO_RAD,
                imu.calcGyro(imu.gy) * DEG_TO_RAD,
                imu.calcGyro(imu.gz) * DEG_TO_RAD);
    filtermutex.unlock();
    // pc.printf("Running step, gyro %f, %f, %f, %f, %f, %f, %f\n", imu.gx,
    //           imu.gy, imu.gz, filter.state.w, filter.state.x, filter.state.y,
    //           filter.state.z);
  }
}