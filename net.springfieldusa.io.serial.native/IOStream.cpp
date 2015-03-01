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
  
  tcgetattr(fileDescriptor, &terminalConfig);
  cfmakeraw(&terminalConfig);
  terminalConfig.c_cc[VMIN] = 1;
  terminalConfig.c_cc[VTIME] = 10;

  terminalConfig.c_cflag = B115200 | CLOCAL | CREAD | CS8;

  terminalConfig.c_cflag &= ~(PARENB | PARODD | CSTOPB | CRTSCTS);

  /*
  terminalConfig.c_iflag = IGNPAR | ICRNL;
  terminalConfig.c_oflag = 0;
  terminalConfig.c_lflag = 0;

  terminalConfig.c_cc[VMIN]  = 1;
  terminalConfig.c_cc[VTIME] = 0;
  */
  
  ::tcsetattr(fileDescriptor, TCSANOW, &terminalConfig);
}

IOStream::~IOStream()
{
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

int IOStream::setBaud(int baud)
{
  int speed = ::cfsetspeed(&terminalConfig, baud);
  
  if (speed >= 0)
    ::tcsetattr(fileDescriptor, TCSANOW, &terminalConfig);
  
  return speed;
}

int IOStream::getStatus()
{
  int status;
  ::ioctl(fileDescriptor, TIOCMGET, &status);
  return status;
}

JNIEXPORT jint JNICALL Java_net_springfieldusa_io_serial_IOStream_B115200(JNIEnv* env, jclass javaClass)
{
  return B115200;
}

JNIEXPORT jint JNICALL Java_net_springfieldusa_io_serial_IOStream_B9600(JNIEnv* env, jclass javaClass)
{
  return B9600;
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
    ::printf("errno: %d\n", errno);
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

JNIEXPORT void JNICALL Java_net_springfieldusa_io_serial_IOStream_setBaud(JNIEnv* env, jobject object, jlong pStream, jint baud)
{
  IOStream* stream = (IOStream*) pStream;

  if (stream->setBaud(baud) < 0)
  {
    ::printf("errno: %d\n", errno);
    jclass exception = env->FindClass("java/io/IOException");
    env->ThrowNew(exception, "Failed to set baud rate");
  }
}

JNIEXPORT jint JNICALL Java_net_springfieldusa_io_serial_IOStream_getStatus(JNIEnv* env, jobject object, jlong pStream)
{
  IOStream* stream = (IOStream*) pStream;
  return stream->getStatus();
}