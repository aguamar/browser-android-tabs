// Copyright 2018 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

package org.chromium.chromecast.base;

class TestUtils {
    // Use to test consumption of superclasses of some generic parameter.
    public static class Base {
        @Override
        public String toString() {
            return "Base";
        }
    }

    // Use to test production of subclasses of some generic parameter.
    public static class Derived extends Base {
        @Override
        public String toString() {
            return "Derived";
        }
    }
}
