/*
 * JBoss, Home of Professional Open Source.
 * Copyright 2011, Red Hat, Inc., and individual contributors
 * as indicated by the @author tags. See the copyright.txt file in the
 * distribution for a full listing of individual contributors.
 *
 * This is free software; you can redistribute it and/or modify it
 * under the terms of the GNU Lesser General Public License as
 * published by the Free Software Foundation; either version 2.1 of
 * the License, or (at your option) any later version.
 *
 * This software is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this software; if not, write to the Free
 * Software Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA
 * 02110-1301 USA, or see the FSF site: http://www.fsf.org.
 */

package org.jboss.narayana.blacktie.jnitest;

import static org.junit.Assert.*;

import java.nio.ByteBuffer;

import org.junit.Before;
import org.junit.Test;

/**
 * @author zhfeng
 *
 */
public class TestByteBuffer {
    private NativeStuff test;

    static {
        System.loadLibrary("jnitest");
    }

    @Before
    public void setUp() throws Exception {
        test = new NativeStuff();
    }

    @Test
    public void test_byte_buffer() {
        int size = 100*1024 *1024;
        byte[] data = new byte[size];
        ByteBuffer buf = ByteBuffer.wrap(data);
        //ByteBuffer buf = ByteBuffer.allocate(size);
        buf.putInt(1);
        buf.putInt(2);
        buf.putFloat(1.0f);
        buf.put((byte) 3);
        boolean result = test.testByteBuffer(data, size);
        assertTrue(result);
    }

}
