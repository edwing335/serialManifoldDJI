#include <iostream>
#include <stdio.h>
#include <string.h>
#include <string>

#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include <termios.h>

#include <sys/ioctl.h>
#include <linux/serial.h>

using namespace std;
int armedFromAirspaceVar = 1;
int fd;

int init_serial() {
  fd = open("/dev/ttyS0", O_RDWR | O_NOCTTY | O_NDELAY);
  if (fd == -1) {
    perror("open_port: Unable to open /dev/ttyS0 - ");
    return (-1);
  }
  else {
    struct termios tty;
    tcgetattr(fd, &tty);
    cfsetispeed(&tty, B230400);
    cfsetospeed(&tty, B230400);
    tty.c_cflag |= (CLOCAL | CREAD);
    tty.c_cflag &= ~PARENB;
    tty.c_cflag &= ~CSTOPB;
    tty.c_cflag &= ~CSIZE;
    tty.c_cflag |= CS8;
    tcsetattr(fd, TCSANOW, &tty);

    /////////////
    /* Setting other Port Stuff */
    // tty.c_cflag     &=  ~PARENB;            // Make 8n1
    // tty.c_cflag     &=  ~CSTOPB;
    // tty.c_cflag     &=  ~CSIZE;
    // tty.c_cflag     |=  CS8;

    // tty.c_cflag     &=  ~CRTSCTS;           // no flow control
    // tty.c_cc[VMIN]   =  1;                  // read doesn't block
    // tty.c_cc[VTIME]  =  5;                  // 0.5 seconds read timeout
    // tty.c_cflag     |=  CREAD | CLOCAL;     // turn on READ & ignore ctrl lines

    // /* Make raw */
    // cfmakeraw(&tty);

    // /* Flush Port, then applies attributes */
    // tcflush( fd, TCIFLUSH );
    // if ( tcsetattr ( fd, TCSANOW, &tty ) != 0) {
    //    std::cout << "Error " << errno << " from tcsetattr" << std::endl;
    // }
  }
  // Turn off blocking for reads, use (fd, F_SETFL, FNDELAY) if you want that
  // fcntl(fd, F_SETFL, FNDELAY);
  // fcntl(fd, F_SETFL, 0);//blocking serial port flow
}



unsigned char read_serial() {
  char buf[10];
  std::cout << "Read init: " << endl;
  for (int i = 0; i < 100; i++) {
    n = read( fd, &buf , 2);
    // n=read( fd, &buf , 10);
    std::cout << "Num of chars:" << n << " Read: " << buf[0] << endl;
  }
}


// unsigned char read_serial() {
//    static char c;
//     read(fd, &c, 1);
//     std::cout << "Charter read:" << c << std::endl;
//     return c;


// }

/*int read_serial(char &read_char) {
  // TODO: we need to send aaaa to read only one a, improve this!
  // Read up to 255 characters from the port if they are there
  char buf[256]={0};
  int n = read(fd, (void*)buf, 4);
  if (n < 0) {
    perror("Read failed - ");
    return -1;
  }
  else if (n == 0) {
    // printf("No data on port\n");
    std::cout << "No data on Serial port" << std::endl;
    // usleep(500000);
  }
  else {

    read_char = buf[0];
    std::cout << "Charter read:" << read_char << std::endl;




    // buf[n] = '\0';
    // printf("%i bytes read : %s", n, buf);
    // usleep(500000);

    //  if (read_char == '1')
    // {
    //   armedFromAirspaceVar = 1;
    //   cout << "********** armedFromAirspaceVar=1" << endl;
    // }
    // else if (read_char == '0')
    // {
    //   armedFromAirspaceVar = 0;
    //   cout << "********** armedFromAirspaceVar=0" << endl;
    // }

  }
  }
*/

std::string readLine()
{

  //     std::string m_line;
  //     std::string m_lineQ;

  //     string line;
  //     int n;
  //     char buf[255];

  //     n = read(fd, buf, 255);
  //     if(n > 0) {
  //             // read up to \n
  //             for(int i=0; i < n; i++) {
  //                     if(buf[i] == '\r') {
  //                             // m_lineQ.push(m_line);
  //                             m_line.clear();
  //                     } else {
  //                             if(buf[i] != 0 && buf[i] != '\n')
  //                                     m_line += buf[i];
  //                     }
  // }
  //     }
  //     if(m_lineQ.size() > 0) {
  //             // line = m_lineQ.front();
  // // Debugging
  // //fprintf(stderr, "SERIAL LINE: %s\n", line.c_str());
  //             // m_lineQ.pop();
  //     }
  //     return line;
}

int main()
{
  init_serial();
  std::cout << "::::Running serial reader to Fire/Sensor module:" << std::endl;
  // write_serial((void*)"v");//ask version
  write(fd, "v", 1);

  write(fd, "TEST-", 5);


  static char read_char;
  int fired = 0;
  // static char c;
  int n = 0,
      spot = 0;
  char buf = '\0';

  /* Whole response*/
  char response[1024];
  memset(response, '\0', sizeof response);
  while (1) {
    cout << "********** in the while..." << endl;


    do {
      n = read( fd, &buf, 1 );
      cout << "********** serial charters:" << n << endl;
      // sprintf( &response[spot], "%c", buf );
      spot += n;
    } while ( buf != '\r' && n > 0);
    cout << "********** serial read" << buf << endl;

    // if(read(fd, &read_char, 1))
    // {

    cout << "********** serial read stop1...:" << read_char << endl;
    usleep(500000);
    // }
    // else
    // {
    // cout<<"no read "<<endl;
    // }
  }



  while (1) { //

    //////////////////////////////////////////////////////////////////
    read(fd, &read_char, 1);
    if (read_char == 'f')
    {
      fired = 1;
      cout << "********** Net fired!" << endl;
    }
    else
    {
      fired = 0;//TODO
      if (armedFromAirspaceVar && !fired)
      {
        // write_serial((void*)"a");//send arm to aislan device
        write(fd, "a", 1);
        // read_char=read_serial();
        // read_serial(read_char);
        while (armedFromAirspaceVar)
        {
          cout << "********** in the while..." << endl;
          read(fd, &read_char, 1);
          cout << "********** serial read stop...:" << read_char << endl;
          // read_serial(read_char);
          if (read_char == 'f') {
            //repeate while
          }
          else {
            fired = 1;
            cout << "**********fired received, must to go out of the while..." << endl;
            // exit(1);//TODO
            usleep(2000000);
          }
        }
      }
      // write_serial((void*)"d");//send disarm to aislan device
      write(fd, "d", 1);
      //TODO: goto home/main after here
    }

  }




  // Don't forget to clean up
  close(fd);
  return 0;
}