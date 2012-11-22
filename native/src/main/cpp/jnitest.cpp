#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>

#include <jni.h>
#include <nativeStuff.h>

static bool checkData(jbyte* data, jsize size, jsize expectSize)
{
	if(size != expectSize){
		printf("buffer size is %d but expect %d\n", size, expectSize);
		return false;
	} else {
		/*
		for(int i = 0; i < size; i++) {
			printf("%x ", data[i]);
		}
		printf("\n");
		*/
		int a = ntohl((*(int*)&data[0]));
		//printf("a = %x\n", a);
		int b = ntohl((*(int*)&data[4]));
		//printf("b = %x\n", b);
		float c;
		uint32_t value = ntohl((*(uint32_t*)&data[8]));
		memcpy(&c, &value, sizeof(c));
		//printf("c = %f\n", c);
		
		return a == 1 && b == 2 && c == 1.0f;
	}

	return false;
}

JNIEXPORT jboolean JNICALL Java_org_jboss_narayana_blacktie_jnitest_NativeStuff_testByteBuffer(JNIEnv * env, jobject obj, jbyteArray buf, jint expectSize) 
{
	bool success = false;
	jbyte* data = env->GetByteArrayElements(buf, 0);
	jsize size = env->GetArrayLength(buf);

	if(data == NULL) {
		printf("can not get array buf\n");
	} else {
		success = checkData(data, size, expectSize);
	}
}

JNIEXPORT jboolean JNICALL Java_org_jboss_narayana_blacktie_jnitest_NativeStuff_testNIOBuffer(JNIEnv * env, jobject obj, jobject buf, jint expectSize)
{
	bool success = false;
	jbyte* data = (jbyte*)env->GetDirectBufferAddress(buf);
	jsize size = env->GetDirectBufferCapacity(buf); 

	if(data == NULL) {
		printf("can not get direct buffer address\n");
	} else {
		success = checkData(data, size, expectSize);
	}
	return success;
}

JNIEXPORT void JNICALL Java_org_jboss_narayana_blacktie_jnitest_NativeStuff_runServer(JNIEnv * env, jobject obj, jint port, jint size)
{
	bool success = false;
	int listenfd = 0, connfd = 0;
	struct sockaddr_in serv_addr; 

	env->MonitorEnter(obj);
	jbyte* rcvbuf = (jbyte*)malloc(sizeof(jbyte) * size);

	listenfd = socket(AF_INET, SOCK_STREAM, 0);
	memset(&serv_addr, 0, sizeof(serv_addr));
	memset(rcvbuf, 0, sizeof(jbyte) * size); 

	serv_addr.sin_family = AF_INET;
	serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
	serv_addr.sin_port = htons(port); 

	int on = 1;
	setsockopt(listenfd, SOL_SOCKET, SO_REUSEADDR, &on, sizeof(int));
	bind(listenfd, (struct sockaddr*)&serv_addr, sizeof(serv_addr)); 
	listen(listenfd, 10); 

	jclass cls = env->GetObjectClass(obj);
	jmethodID mid = env->GetMethodID(cls, "notify", "()V");
	if(mid != 0) {
		env->CallVoidMethod(obj, mid);
		//printf("call notify\n");
	}
	env->MonitorExit(obj);

	connfd = accept(listenfd, (struct sockaddr*)NULL, NULL); 

	int remain = size;
	int rcvlen = 0;
	int len;

	while(remain > 0) {
		len = read(connfd, rcvbuf + rcvlen, remain);

		//printf("read %d bytes\n", len);
		if(len <= 0) {
			//printf("break read\n");
			break;
		} else {
			rcvlen += len;
			remain -= len;
		}
	}

	if(remain == 0) {
		success = checkData(rcvbuf, rcvlen, size);
	} else {
		printf("read expect=%d, but rcvlen=%d, remain=%d\n", size, rcvlen, remain);
	}

	free(rcvbuf);

	char rv[1];
	if(success) {
		rv[0] = 0;
	} else {
		rv[0] = 1;
	}

	//printf("write rv with rv[0] = %d\n", rv[0]);
	write(connfd, rv, 1); 

	close(connfd);
	close(listenfd);
}
