/*******************************************************************************
 * Copyright (c) 2015 Bryan Hunt
 *
 * All rights reserved. This program and the accompanying materials
 * are made available under the terms of the Eclipse Public License v1.0
 * which accompanies this distribution, and is available at
 * http://www.eclipse.org/legal/epl-v10.html
 *
 * Contributors:
 *     Bryan Hunt - initial API and implementation
 *******************************************************************************/

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
