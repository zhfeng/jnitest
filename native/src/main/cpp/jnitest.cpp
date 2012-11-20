#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <arpa/inet.h>
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
