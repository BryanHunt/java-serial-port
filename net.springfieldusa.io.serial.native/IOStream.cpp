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

#include <fcntl.h>
#include <sys/types.h>
#include <sys/uio.h>
#include <unistd.h>
#include <string.h>
#include <sys/ioctl.h>
#include <errno.h>

#include "net_springfieldusa_io_serial_IOStream.h"

#include "IOStream.h"

IOStream::IOStream(const char* device)
{
  fileDescriptor = ::open(device, O_RDWR | O_NOCTTY | O_NONBLOCK);
  ::ioctl(fileDescriptor, TIOCEXCL);
  ::fcntl(fileDescriptor, F_SETFL, 0);

  ::memset(&terminalConfig, 0, sizeof(terminalConfig));
  ::tcgetattr(fileDescriptor, &originalTerminalConfig);
  ::cfmakeraw(&terminalConfig);
  
  terminalConfig.c_cc[VMIN] = 0;
  terminalConfig.c_cc[VTIME] = 10;
  terminalConfig.c_cflag |= CLOCAL | CREAD | CS8;
  terminalConfig.c_cflag &= ~(PARENB | PARODD | CSTOPB);
  
  ::tcsetattr(fileDescriptor, TCSANOW, &terminalConfig);
  ::tcflush(fileDescriptor, TCIOFLUSH);
}

IOStream::~IOStream()
{
  ::tcdrain(fileDescriptor);
  ::tcsetattr(fileDescriptor, TCSANOW, &originalTerminalConfig);
  ::close(fileDescriptor);
}

ssize_t IOStream::read(void* buffer, size_t nbyte)
{
  return ::read(fileDescriptor, buffer, nbyte);
}

ssize_t IOStream::write(void* buffer, size_t nbyte)
{
  return ::write(fileDescriptor, buffer, nbyte);
}

int IOStream::flush(int flushMode)
{
  return ::tcflush(fileDescriptor, flushMode);
}

int IOStream::setBaud(int baud)
{
  int speed = ::cfsetspeed(&terminalConfig, baud);
  
  if (speed >= 0)
    ::tcsetattr(fileDescriptor, TCSANOW, &terminalConfig);
  
  return speed;
}

int IOStream::setParity(int parity)
{
  if(parity == 0)
  {
    terminalConfig.c_cflag &= ~PARENB; // No parity
  }
  else
  {
    terminalConfig.c_cflag |= PARENB; // Odd or even parity
    
    if(parity & 0x1)
      terminalConfig.c_cflag |= PARODD; // Odd parity
    else
      terminalConfig.c_cflag &= ~PARODD; // Even parity
  }
  
  return ::tcsetattr(fileDescriptor, TCSANOW, &terminalConfig);
}

int IOStream::setDataBits(int numberDataBits)
{
  terminalConfig.c_cflag &= ~CSIZE; // Mask off the size bits so we can set the correct size
  
  switch (numberDataBits)
  {
    case 5:
      terminalConfig.c_cflag |= CS5;
      break;
    case 6:
      terminalConfig.c_cflag |= CS6;
      break;
    case 7:
      terminalConfig.c_cflag |= CS7;
      break;
    case 8:
      terminalConfig.c_cflag |= CS8;
      break;
  }
  
  return ::tcsetattr(fileDescriptor, TCSANOW, &terminalConfig);
}

int IOStream::setStopBits(int numberStopBits)
{
  if (numberStopBits == 1)
    terminalConfig.c_cflag &= ~CSTOPB;
  else
    terminalConfig.c_cflag |= CSTOPB;
  
  return ::tcsetattr(fileDescriptor, TCSANOW, &terminalConfig);
}

int IOStream::setUseFlowControl(bool mode)
{
  if(mode)
    terminalConfig.c_cflag |= CRTSCTS;
  else
    terminalConfig.c_cflag &= ~CRTSCTS;
  
  return ::tcsetattr(fileDescriptor, TCSANOW, &terminalConfig);
}

int IOStream::setReadMode(int numberBytesToWaitFor, int timeoutBetweenBytes)
{
  terminalConfig.c_cc[VMIN] = numberBytesToWaitFor;
  terminalConfig.c_cc[VTIME] = timeoutBetweenBytes;
  return ::tcsetattr(fileDescriptor, TCSANOW, &terminalConfig);
}

int IOStream::getStatus()
{
  int status;
  ::ioctl(fileDescriptor, TIOCMGET, &status);
  return status;
}

void IOStream::dumpConfig()
{
  ::printf("\n--------------------------------------------------------------------------------------------------------");
  ::printf("\n  c_ispeed : %ld", terminalConfig.c_ispeed);
  ::printf("\n  c_ospeed : %ld", terminalConfig.c_ospeed);
  
  ::printf("\n  c_cflag  : %016lx =>", terminalConfig.c_cflag);
  
  if((terminalConfig.c_cflag & CSIZE) == CS5)
    ::printf(" CS5");
  
  if((terminalConfig.c_cflag & CSIZE) == CS6)
    ::printf(" CS6");
  
  if((terminalConfig.c_cflag & CSIZE ) == CS7)
    ::printf(" CS7");
  
  if((terminalConfig.c_cflag & CSIZE) == CS8)
    ::printf(" CS8");
  
  if(terminalConfig.c_cflag & CSTOPB)
    ::printf(" CSTOPB");
  
  if(terminalConfig.c_cflag & CREAD)
    ::printf(" CREAD");
  
  if(terminalConfig.c_cflag & PARENB)
    ::printf(" PARENB");
  
  if(terminalConfig.c_cflag & PARODD)
    ::printf(" PARODD");
  
  if(terminalConfig.c_cflag & HUPCL)
    ::printf(" HUPCL");
  
  if(terminalConfig.c_cflag & CLOCAL)
    ::printf(" CLOCAL");
  
  if(terminalConfig.c_cflag & CRTSCTS)
    ::printf(" CRTSCTS");
  
  if(terminalConfig.c_cflag & CRTS_IFLOW)
    ::printf(" CRTS_IFLOW");
  
  if(terminalConfig.c_cflag & MDMBUF)
    ::printf(" MDMBUF");
  
  ::printf("\n  c_iflag  : %016lx =>", terminalConfig.c_iflag);
  
  if(terminalConfig.c_iflag & IGNBRK)
    ::printf(" IGNBRK");
  
  if(terminalConfig.c_iflag & BRKINT)
    ::printf(" BRKINT");
  
  if(terminalConfig.c_iflag & IGNPAR)
    ::printf(" IGNPAR");
  
  if(terminalConfig.c_iflag & PARMRK)
    ::printf(" PARMRK");
  
  if(terminalConfig.c_iflag & INPCK)
    ::printf(" INPCK");
  
  if(terminalConfig.c_iflag & ISTRIP)
    ::printf(" ISTRIP");
  
  if(terminalConfig.c_iflag & INLCR)
    ::printf(" INLCR");
  
  if(terminalConfig.c_iflag & IGNCR)
    ::printf(" IGNCR");
  
  if(terminalConfig.c_iflag & ICRNL)
    ::printf(" ICRNL");
  
  if(terminalConfig.c_iflag & IXON)
    ::printf(" IXON");
  
  if(terminalConfig.c_iflag & IXOFF)
    ::printf(" IXOFF");
  
  if(terminalConfig.c_iflag & IXANY)
    ::printf(" IXANY");
  
  if(terminalConfig.c_iflag & IMAXBEL)
    ::printf(" IMAXBEL");
  
  ::printf("\n  c_oflag  : %016lx =>", terminalConfig.c_oflag);
  
  if(terminalConfig.c_oflag & OPOST)
    ::printf(" OPOST");
  
  if(terminalConfig.c_oflag & ONLCR)
    ::printf(" ONLCR");
  
  if(terminalConfig.c_oflag & OXTABS)
    ::printf(" OXTABS");
  
  if(terminalConfig.c_oflag & ONOEOT)
    ::printf(" ONOEOT");
  
  if(terminalConfig.c_oflag & OCRNL)
    ::printf(" OCRNL");
  
  if(terminalConfig.c_oflag & ONOCR)
    ::printf(" ONOCR");
  
  if(terminalConfig.c_oflag & ONLRET)
    ::printf(" ONLRET");
  
  ::printf("\n  c_lflag  : %016lx =>", terminalConfig.c_lflag);
  
  if(terminalConfig.c_lflag & ECHOKE)
    ::printf(" ECHOKE");
  
  if(terminalConfig.c_lflag & ECHOE)
    ::printf(" ECHOE");
  
  if(terminalConfig.c_lflag & ECHO)
    ::printf(" ECHO");
  
  if(terminalConfig.c_lflag & ECHONL)
    ::printf(" ECHONL");
  
  if(terminalConfig.c_lflag & ECHOPRT)
    ::printf(" ECHOPRT");
  
  if(terminalConfig.c_lflag & ECHOCTL)
    ::printf(" ECHOCTL");
  
  if(terminalConfig.c_lflag & ISIG)
    ::printf(" ISIG");
  
  if(terminalConfig.c_lflag & ICANON)
    ::printf(" ICANON");
  
  if(terminalConfig.c_lflag & ALTWERASE)
    ::printf(" ALTWERASE");
  
  if(terminalConfig.c_lflag & IEXTEN)
    ::printf(" IEXTEN");
  
  if(terminalConfig.c_lflag & EXTPROC)
    ::printf(" EXTPROC");
  
  if(terminalConfig.c_lflag & TOSTOP)
    ::printf(" TOSTOP");
  
  if(terminalConfig.c_lflag & FLUSHO)
    ::printf(" FLUSHO");
  
  if(terminalConfig.c_lflag & NOKERNINFO)
    ::printf(" NOKERNINFO");
  
  if(terminalConfig.c_lflag & PENDIN)
    ::printf(" PENDIN");
  
  if(terminalConfig.c_lflag & NOFLSH)
    ::printf(" NOFLSH");
  
  ::printf("\n--------------------------------------------------------------------------------------------------------\n");
  ::fflush(stdout);
}

//-----------------------------------------------------------------------------------------------------------------------
//
//  JNI
//
//-----------------------------------------------------------------------------------------------------------------------

JNIEXPORT jlong JNICALL Java_net_springfieldusa_io_serial_IOStream_open(JNIEnv* env, jobject object, jstring deviceName)
{
  const char* device = env->GetStringUTFChars(deviceName, 0);
  IOStream* stream = new IOStream(device);
  env->ReleaseStringUTFChars(deviceName, device);

  if(stream->getFileDescriptor() == -1)
  {
    ::printf("errno: %d\n", errno);
    jclass exception = env->FindClass("java/io/IOException");
    env->ThrowNew(exception, "Failed to open from stream");
  }

  return (long) stream;
}

JNIEXPORT jint JNICALL Java_net_springfieldusa_io_serial_IOStream_read(JNIEnv* env, jobject object, jlong pStream, jbyteArray byteArray)
{
  IOStream* stream = (IOStream*) pStream;
  jbyte* buffer = env->GetByteArrayElements(byteArray, NULL);
  int nbyte = env->GetArrayLength(byteArray);
  
  ssize_t read = stream->read(buffer, nbyte);
  
  env->ReleaseByteArrayElements(byteArray, buffer, 0);

  if(read == -1)
  {
    ::printf("errno: %d\n", errno);
    jclass exception = env->FindClass("java/io/IOException");
    env->ThrowNew(exception, "Failed to read from stream");
  }

  return (int) read;
}

JNIEXPORT jint JNICALL Java_net_springfieldusa_io_serial_IOStream_write(JNIEnv* env, jobject object, jlong pStream, jbyteArray byteArray)
{
  IOStream* stream = (IOStream*) pStream;
  jbyte* buffer = env->GetByteArrayElements(byteArray, NULL);
  int nbyte = env->GetArrayLength(byteArray);
  
  ssize_t written = stream->write(buffer, nbyte);
  
  env->ReleaseByteArrayElements(byteArray, buffer, 0);
  
  if(written == -1)
  {
    jclass exception = env->FindClass("java/io/IOException");
    env->ThrowNew(exception, "Failed to write to stream");
  }

  return (int) written;
}

JNIEXPORT void JNICALL Java_net_springfieldusa_io_serial_IOStream_close(JNIEnv* env, jobject object, jlong pStream)
{
  IOStream* stream = (IOStream*) pStream;
  delete stream;
}

JNIEXPORT void JNICALL Java_net_springfieldusa_io_serial_IOStream_flush(JNIEnv* env, jobject object, jlong pStream, jint flushMode)
{
  IOStream* stream = (IOStream*) pStream;
  int nativeFlushMode;
  
  switch(flushMode)
  {
    case 0:
      nativeFlushMode = TCIFLUSH;
      break;
      
    case 1:
      nativeFlushMode = TCOFLUSH;
      break;
      
    case 2:
      nativeFlushMode = TCIOFLUSH;
      break;
      
    default:
      jclass exception = env->FindClass("java/io/IOException");
      env->ThrowNew(exception, "Unknown flush mode");
  }
  
  if(stream->flush(nativeFlushMode) != 0)
  {
    jclass exception = env->FindClass("java/io/IOException");
    env->ThrowNew(exception, "Flush failed");
  }
}

JNIEXPORT void JNICALL Java_net_springfieldusa_io_serial_IOStream_setBaud(JNIEnv* env, jobject object, jlong pStream, jint baudRate)
{
  IOStream* stream = (IOStream*) pStream;

  if (stream->setBaud(baudRate) < 0)
  {
    jclass exception = env->FindClass("java/io/IOException");
    env->ThrowNew(exception, "Failed to set baud rate");
  }
}

JNIEXPORT void JNICALL Java_net_springfieldusa_io_serial_IOStream_setParity(JNIEnv* env, jobject object, jlong pStream, jint parity)
{
  IOStream* stream = (IOStream*) pStream;
  
  if(stream->setParity(parity) < 0)
  {
    jclass exception = env->FindClass("java/io/IOException");
    env->ThrowNew(exception, "Failed to set parity");
  }
}

JNIEXPORT void JNICALL Java_net_springfieldusa_io_serial_IOStream_setDataBits(JNIEnv* env, jobject object, jlong pStream, jint numberDataBits)
{
  IOStream* stream = (IOStream*) pStream;
  
  if(stream->setDataBits(numberDataBits) < 0)
  {
    jclass exception = env->FindClass("java/io/IOException");
    env->ThrowNew(exception, "Failed to set data bits");
  }
}

JNIEXPORT void JNICALL Java_net_springfieldusa_io_serial_IOStream_setStopBits(JNIEnv* env, jobject object, jlong pStream, jint numberStopBits)
{
  IOStream* stream = (IOStream*) pStream;
  
  if(stream->setStopBits(numberStopBits) < 0)
  {
    jclass exception = env->FindClass("java/io/IOException");
    env->ThrowNew(exception, "Failed to set stop bits");
  }
}

JNIEXPORT void JNICALL Java_net_springfieldusa_io_serial_IOStream_setUseFlowControl(JNIEnv* env, jobject object, jlong pStream, jboolean mode)
{
  IOStream* stream = (IOStream*) pStream;
  
  if(stream->setUseFlowControl(mode) < 0)
  {
    jclass exception = env->FindClass("java/io/IOException");
    env->ThrowNew(exception, "Failed to set flow control");
  }
}

JNIEXPORT void JNICALL Java_net_springfieldusa_io_serial_IOStream_setReadMode(JNIEnv* env, jobject object, jlong pStream, jint numberBytesToWaitFor, jint timeoutBetweenBytes)
{
  IOStream* stream = (IOStream*) pStream;
  
  if(stream->setReadMode(numberBytesToWaitFor, timeoutBetweenBytes) < 0)
  {
    jclass exception = env->FindClass("java/io/IOException");
    env->ThrowNew(exception, "Failed to set read mode");
  }
}

JNIEXPORT jint JNICALL Java_net_springfieldusa_io_serial_IOStream_getStatus(JNIEnv* env, jobject object, jlong pStream)
{
  IOStream* stream = (IOStream*) pStream;
  return stream->getStatus();
}

JNIEXPORT void JNICALL Java_net_springfieldusa_io_serial_IOStream_dumpConfig(JNIEnv* env, jobject object, jlong pStream)
{
  IOStream* stream = (IOStream*) pStream;
  stream->dumpConfig();
}

