/* DO NOT EDIT THIS FILE - it is machine generated */
#include <jni.h>
/* Header for class net_springfieldusa_io_serial_IOStream */

#ifndef _Included_net_springfieldusa_io_serial_IOStream
#define _Included_net_springfieldusa_io_serial_IOStream
#ifdef __cplusplus
extern "C" {
#endif
/*
 * Class:     net_springfieldusa_io_serial_IOStream
 * Method:    B115200
 * Signature: ()I
 */
JNIEXPORT jint JNICALL Java_net_springfieldusa_io_serial_IOStream_B115200
  (JNIEnv *, jclass);

/*
 * Class:     net_springfieldusa_io_serial_IOStream
 * Method:    B9600
 * Signature: ()I
 */
JNIEXPORT jint JNICALL Java_net_springfieldusa_io_serial_IOStream_B9600
  (JNIEnv *, jclass);

/*
 * Class:     net_springfieldusa_io_serial_IOStream
 * Method:    open
 * Signature: (Ljava/lang/String;)J
 */
JNIEXPORT jlong JNICALL Java_net_springfieldusa_io_serial_IOStream_open
  (JNIEnv *, jobject, jstring);

/*
 * Class:     net_springfieldusa_io_serial_IOStream
 * Method:    read
 * Signature: (J[B)I
 */
JNIEXPORT jint JNICALL Java_net_springfieldusa_io_serial_IOStream_read
  (JNIEnv *, jobject, jlong, jbyteArray);

/*
 * Class:     net_springfieldusa_io_serial_IOStream
 * Method:    write
 * Signature: (J[B)I
 */
JNIEXPORT jint JNICALL Java_net_springfieldusa_io_serial_IOStream_write
  (JNIEnv *, jobject, jlong, jbyteArray);

/*
 * Class:     net_springfieldusa_io_serial_IOStream
 * Method:    close
 * Signature: (J)V
 */
JNIEXPORT void JNICALL Java_net_springfieldusa_io_serial_IOStream_close
  (JNIEnv *, jobject, jlong);

/*
 * Class:     net_springfieldusa_io_serial_IOStream
 * Method:    setBaud
 * Signature: (JI)V
 */
JNIEXPORT void JNICALL Java_net_springfieldusa_io_serial_IOStream_setBaud
  (JNIEnv *, jobject, jlong, jint);

/*
 * Class:     net_springfieldusa_io_serial_IOStream
 * Method:    getStatus
 * Signature: (J)I
 */
JNIEXPORT jint JNICALL Java_net_springfieldusa_io_serial_IOStream_getStatus
  (JNIEnv *, jobject, jlong);

#ifdef __cplusplus
}
#endif
#endif
