package org.jboss.narayana.blacktie.jnitest;

import static org.junit.Assert.assertTrue;

import java.io.DataInputStream;
import java.io.DataOutputStream;
import java.net.Socket;
import java.nio.ByteBuffer;

import org.junit.After;
import org.junit.Before;
import org.junit.Test;

public class TestSocketBuffer {
    private NativeStuff test;
    private int port = 12345;
    private int size = 100*1024*1024;
    private Thread thr = null;

    static {
        System.loadLibrary("jnitest");
    }

    @Before
    public void setUp() throws Exception {
        test = new NativeStuff();
        thr = new Thread(){

            public void run() {
                //System.out.println("run server");

                //server_running = true;
                //test.notify();
                test.runServer(port, size);

            }
        };
        thr.start();
    }

    @After
    public void tearDown() throws Exception {
        if(thr != null) {
            thr.join();
        }
    }

    @Test
    public  void test_socket_buffer() throws Exception {
        //Thread.sleep(2000);
        synchronized (test) {
            test.wait();
        }

        //System.out.println("connect to localhost:" + port);
        Socket socket = new Socket("localhost", port);
        //System.out.println("connect to localhost:" + port + " ok");
        DataOutputStream out =  new DataOutputStream(socket.getOutputStream());
        DataInputStream in = new DataInputStream(socket.getInputStream());

        ByteBuffer buf = ByteBuffer.allocate(size);
        buf.putInt(1);
        buf.putInt(2);
        buf.putFloat(1.0f);
        buf.put((byte) 3);
        out.write(buf.array());


        byte rv = in.readByte();
        assertTrue(rv == 0);
        out.close();
        in.close();
        socket.close();
    }

}
