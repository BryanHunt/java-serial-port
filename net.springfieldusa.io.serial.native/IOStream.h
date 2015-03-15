/*
 * IOStream.h
 *
 *  Created on: Feb 27, 2015
 *      Author: bhunt
 */

#ifndef IOSTREAM_H_
#define IOSTREAM_H_

#include <termios.h>

class IOStream
{
  private:

  int fileDescriptor;
  termios originalTerminalConfig;
  termios terminalConfig;

  public:

  IOStream(const char* device);
  virtual ~IOStream();

  ssize_t read(void* buffer, size_t nbyte);
  ssize_t write(void* buffer, size_t nbyte);
  
  int flush(int flushMode);
  
  int setBaud(int baud);
  int setParity(int parity);
  int setDataBits(int numberDataBits);
  int setStopBits(int numberStopBits);
  int setUseFlowControl(bool mode);
  int setReadMode(int numberBytesToWaitFor, int timeoutBetweenBytes);
  
  int getFileDescriptor() { return fileDescriptor; }
  int getStatus();
  void dumpConfig();
};

#endif /* IOSTREAM_H_ */
