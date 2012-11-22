package org.jboss.narayana.blacktie.jnitest;

import java.nio.ByteBuffer;

public class NativeStuff
{
	public native boolean testNIOBuffer(ByteBuffer buf, int size);
	public native boolean testByteBuffer(byte[] buf, int size);
	public native void runServer(int port, int size);
}
