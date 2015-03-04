/*
 * IOStream.cpp
 *
 *  Created on: Feb 27, 2015
 *      Author: bhunt
 */

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
  fcntl(fileDescriptor, F_SETFL, 0);
  
  tcgetattr(fileDescriptor, &originalTerminalConfig);
  cfmakeraw(&terminalConfig);
  
  terminalConfig.c_cc[VMIN] = 1;
  terminalConfig.c_cc[VTIME] = 10;
  terminalConfig.c_cflag |= CLOCAL | CREAD | CS8;
  terminalConfig.c_cflag &= ~(PARENB | PARODD | CSTOPB);
  
  ::tcsetattr(fileDescriptor, TCSANOW, &terminalConfig);
}

IOStream::~IOStream()
{
  ::close(fileDescriptor);
  ::tcsetattr(fileDescriptor, TCSANOW, &originalTerminalConfig);
}

ssize_t IOStream::read(void* buffer, size_t nbyte)
{
  return ::read(fileDescriptor, buffer, nbyte);
}

ssize_t IOStream::write(void* buffer, size_t nbyte)
{
  return ::write(fileDescriptor, buffer, nbyte);
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
    terminalConfig.c_cflag &= ~CLOCAL;
  else
    terminalConfig.c_cflag |= CLOCAL;
  
  return ::tcsetattr(fileDescriptor, TCSANOW, &terminalConfig);
}

int IOStream::getStatus()
{
  int status;
  ::ioctl(fileDescriptor, TIOCMGET, &status);
  return status;
}

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

JNIEXPORT jint JNICALL Java_net_springfieldusa_io_serial_IOStream_getStatus(JNIEnv* env, jobject object, jlong pStream)
{
  IOStream* stream = (IOStream*) pStream;
  return stream->getStatus();
}