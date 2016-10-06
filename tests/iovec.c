/*

  Copyright (c) 2016 Martin Sustrik

  Permission is hereby granted, free of charge, to any person obtaining a copy
  of this software and associated documentation files (the "Software"),
  to deal in the Software without restriction, including without limitation
  the rights to use, copy, modify, merge, publish, distribute, sublicense,
  and/or sell copies of the Software, and to permit persons to whom
  the Software is furnished to do so, subject to the following conditions:

  The above copyright notice and this permission notice shall be included
  in all copies or substantial portions of the Software.

  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
  IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
  FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL
  THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
  LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
  FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS
  IN THE SOFTWARE.

*/

#include <assert.h>
#include <string.h>

#include "../dsock.h"

int main() {

    int h[2];
    int rc = unix_pair(h);
    struct iovec iov[3];
    iov[0].iov_base = "AB";
    iov[0].iov_len = 2;
    iov[1].iov_base = "CD";
    iov[1].iov_len = 2;
    iov[2].iov_base = "EF";
    iov[2].iov_len = 2;
    rc = bsendmsg(h[0], iov, 3, -1);
    assert(rc == 0);
    iov[0].iov_base = "GH";
    iov[0].iov_len = 2;
    iov[1].iov_base = "IJ";
    iov[1].iov_len = 2;
    rc = bsendmsg(h[0], iov, 2, -1);
    assert(rc == 0);

    char buf[10];
    iov[0].iov_base = buf;
    iov[0].iov_len = 2;
    iov[1].iov_base = buf + 2;
    iov[1].iov_len = 3;
    rc = brecvmsg(h[1], iov, 2, -1);
    assert(rc == 0);
    iov[0].iov_base = buf + 5;
    iov[0].iov_len = 4;
    iov[1].iov_base = buf + 9;
    iov[1].iov_len = 1;
    rc = brecvmsg(h[1], iov, 2, -1);
    assert(rc == 0);
    assert(memcmp(buf, "ABCDEFGHIJ", 10) == 0);

    rc = hclose(h[0]);
    assert(rc == 0);
    rc = hclose(h[1]);
    assert(rc == 0);

    return 0;
}

