#include <Python.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <fcntl.h>
#include <linux/fb.h>
#include <sys/mman.h>
#include <sys/ioctl.h>

static PyObject* px(PyObject* self,  PyObject *args)
{
    int x, y, r, g, b;

    if (!PyArg_ParseTuple(args, "iiiii", &x, &y, &r, &g, &b)) {
       return NULL;
    }
   
    int fbfd = 0;
    struct fb_var_screeninfo vinfo;
    struct fb_fix_screeninfo finfo;
    long int screensize = 0;
    char *fbp = 0;
    long int location = 0;

    // Open the file for reading and writing
    fbfd = open("/dev/fb0", O_RDWR);
    if (fbfd == -1) {
        perror("Error: cannot open framebuffer device");
        exit(1);
    }

    // Get fixed screen information
    if (ioctl(fbfd, FBIOGET_FSCREENINFO, &finfo) == -1) {
        perror("Error reading fixed information");
        exit(2);
    }

    // Get variable screen information
    if (ioctl(fbfd, FBIOGET_VSCREENINFO, &vinfo) == -1) {
        perror("Error reading variable information");
        exit(3);
    }


    // Figure out the size of the screen in bytes
    screensize = vinfo.xres * vinfo.yres * vinfo.bits_per_pixel / 8;

    // Map the device to memory
    fbp = (char *)mmap(0, screensize, PROT_READ | PROT_WRITE, MAP_SHARED, fbfd, 0);
    if ((int)fbp == -1) {
        perror("Error: failed to map framebuffer device to memory");
        exit(4);
    }

    location = (x+vinfo.xoffset) * (vinfo.bits_per_pixel/8) +
               (y+vinfo.yoffset) * finfo.line_length;

    *(fbp + location + 2) = r;
    *(fbp + location + 1) = g;
    *(fbp + location + 0) = b;
    *(fbp + location + 3) = 0;// No transparency

    munmap(fbp, screensize);
    close(fbfd);
    return Py_BuildValue("s", "done");
}

static char px_docs[] =
   "draw pixel on coordinates x, y colored r, g, b\n";

static PyObject* fill(PyObject* self,  PyObject *args)
{
    int x1, y1, x2, y2, r, g, b, x, y;

    if (!PyArg_ParseTuple(args, "iiiiiii", &x1, &y1, &x2, &y2, &r, &g, &b)) {
       return NULL;
    }
   
    int fbfd = 0;
    struct fb_var_screeninfo vinfo;
    struct fb_fix_screeninfo finfo;
    long int screensize = 0;
    char *fbp = 0;
    long int location = 0;

    // Open the file for reading and writing
    fbfd = open("/dev/fb0", O_RDWR);
    if (fbfd == -1) {
        perror("Error: cannot open framebuffer device");
        exit(1);
    }

    // Get fixed screen information
    if (ioctl(fbfd, FBIOGET_FSCREENINFO, &finfo) == -1) {
        perror("Error reading fixed information");
        exit(2);
    }

    // Get variable screen information
    if (ioctl(fbfd, FBIOGET_VSCREENINFO, &vinfo) == -1) {
        perror("Error reading variable information");
        exit(3);
    }


    // Figure out the size of the screen in bytes
    screensize = vinfo.xres * vinfo.yres * vinfo.bits_per_pixel / 8;

    // Map the device to memory
    fbp = (char *)mmap(0, screensize, PROT_READ | PROT_WRITE, MAP_SHARED, fbfd, 0);
    if ((int)fbp == -1) {
        perror("Error: failed to map framebuffer device to memory");
        exit(4);
    }

    for (y = y1; y < y2; y++)
        for (x = x1; x < x2; x++) {
            location = (x+vinfo.xoffset) * (vinfo.bits_per_pixel/8) +
                       (y+vinfo.yoffset) * finfo.line_length;

            *(fbp + location + 2) = r;
            *(fbp + location + 1) = g;
            *(fbp + location + 0) = b;
            *(fbp + location + 3) = 0;// No transparency
    }

    munmap(fbp, screensize);
    close(fbfd);
    return Py_BuildValue("s", "done");
}

static char fill_docs[] =
   "draw rectangle on coordinates x1, y1 to x2, y2 colored r, g, b\n";

static PyMethodDef helloworld_funcs[] = {
   {"px", (PyCFunction)px, 
      METH_VARARGS, px_docs},
   {"fill", (PyCFunction)fill, 
      METH_VARARGS, fill_docs},
      {NULL}
};

void initfbpx(void) {
   Py_InitModule3("fbpx", helloworld_funcs,
                  "framebuffer pixel drawing");
}
