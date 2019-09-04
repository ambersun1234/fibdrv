#include <linux/cdev.h>
#include <linux/device.h>
#include <linux/fs.h>
#include <linux/init.h>
#include <linux/kdev_t.h>
#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/mutex.h>

MODULE_LICENSE("Dual MIT/GPL");
MODULE_AUTHOR("National Cheng Kung University, Taiwan");
MODULE_DESCRIPTION("Fibonacci engine driver");
MODULE_VERSION("0.1");

#define DEV_FIBONACCI_NAME "fibonacci"

/* MAX_LENGTH is set to 92 because
 * ssize_t can't fit the number > 92
 */
#define MAX_LENGTH 92

static dev_t fib_dev = 0;
static struct cdev *fib_cdev;
static struct class *fib_class;
static DEFINE_MUTEX(fib_mutex);

static struct u64 {
	unsigned long long lsl;
	unsigned long long msl;
}

static uint32_t clz(uint32_t input)
{
    // use binary search method to check
    uint8_t count = 0;

    if ((input & 0xFFFF0000) == 0) {
        input <<= 16;
        count += 16;
    }  // 1111 1111 1111 1111
    if ((input & 0xFF000000) == 0) {
        input <<= 8;
        count += 8;
    }  // 1111 1111
    if ((input & 0xF0000000) == 0) {
        input <<= 4;
        count += 4;
    }  // 1111
    if ((input & 0xC0000000) == 0) {
        input <<= 2;
        count += 2;
    }  // 1100
    if ((input & 0x80000000) == 0) {
        input <<= 0;
        count += 1;
    }  // 1000
    return count;
}

static struct u64 *adder(struct u64 *input1, struct u64 *input2)
{
    struct u64 *r = kmalloc(sizeof(struct u64), GFP_KERNEL);
    r->lsl = 0;
    r->msl = 0;

    unsigned long long diff = 0;
    r->lsl = input1->lsl;
    r->msl = input1->msl + input2->msl;

    diff = ULLONG_MAX - r->lsl;
    if (input2->lsl >= diff) {
        r->lsl += input2->lsl;
        r->msl += 1;
    } else {
        r->lsl += input2->lsl;
    }

    return r;
}

static struct u64 *subtracter(struct u64 *input1, struct u64 *input2)
{
    struct u64 *r = kmalloc(sizeof(struct u64), GFP_KERNEL);

    if (input1->lsl < input2->lsl) {
        unsigned long long mycarry = ULLONG_MAX;
        r->lsl = mycarry + input1->lsl - input2->lsl + 1;
        r->msl = input1->msl - input2->msl - 1;
    } else {
        r->lsl = input1->lsl - input2->lsl;
        r->msl = input1->msl - input2->msl;
    }
    return r;
}

static struct u64 *multiplier(struct u64 *input1, struct u64 *input2)
{
    struct u64 *r = kmalloc(sizeof(struct u64), GFP_KERNEL);
    r->lsl = 0;
    r->msl = 0;

    size_t width = 8 * sizeof(unsigned long long);
    // 8 bits * how many bytes
    for (size_t i = 0; i < width; i++) {
        if ((input2->lsl >> i) & 0x1) {
            struct u64 tmp;

            r->msl += input1->msl << i;

            tmp.lsl = (input1->lsl << i);
            tmp.msl = i == 0 ? 0 : (input1->lsl >> (width - i));
            r = adder(r, &tmp);
        }
    }

    for (size_t i = 0; i < width; i++) {
        if ((input2->msl >> i) & 0x1) {
            r->msl += (input1->lsl << i);
        }
    }
    return r;
}

static struct u64 fibonacci(uint32_t input)
{
    unsigned int msb = clz(input);
    unsigned int mask = (1 << (31 - msb - 1));
    struct u64 current = {.msl = 0, .lsl = 1}, next = {.msl = 0, .lsl = 1};
    struct u64 mul = {.msl = 0, .lsl = 2}, zero = {.msl = 0, .lsl = 0};

    /* fast doubling formula
     * f(2k) = f(k)[2f(k + 1) - f(k)]
     * f(2k + 1) = f(k + 1)^2 + f(k)^2
     */

    if (input == 0)
        return zero;
    if (input >= 1 && input <= 2)
        return current;

    while (mask > 0) {
        if (mask & input) {
            // bit = 1: fast doubling then iterate 1
            struct u64 *t0, *t1, *t2, *temp, *temp2;
            // t0 = current * (2 * next - current);
            // t1 = next * next + current * current;

            temp = multiplier(&next, &mul);
            temp = subtracter(temp, &current);
            t0 = multiplier(&current, temp);

            t2 = &next;
            temp = multiplier(&next, t2);
            t2 = &current;
            temp2 = multiplier(&current, t2);
            t1 = adder(temp, temp2);

            current = *t0;
            next = *t1;

            // iterate 1
            temp = adder(&current, &next);
            current = next;
            next = *temp;
        } else {
            // bit = 0: fast doubling
            struct u64 *t0, *t1, *t2, *temp, *temp2;
            // t0 = current * (2 * next - current);
            // t1 = next * next + current * current;

            temp = multiplier(&next, &mul);
            temp = subtracter(temp, &current);
            t0 = multiplier(&current, temp);

            t2 = &next;
            temp = multiplier(&next, t2);
            t2 = &current;
            temp2 = multiplier(&current, t2);
            t1 = adder(temp, temp2);

            current = *t0;
            next = *t1;
        }
        mask >>= 1;
    }
    return current;
}

static long long fib_sequence(long long k)
{
    /* FIXME: use clz/ctz and fast algorithms to speed up */
    long long f[k + 2];

    f[0] = 0;
    f[1] = 1;

    for (int i = 2; i <= k; i++) {
        f[i] = f[i - 1] + f[i - 2];
    }

    return f[k];
}

static int fib_open(struct inode *inode, struct file *file)
{
    if (!mutex_trylock(&fib_mutex)) {
        printk(KERN_ALERT "fibdrv is in use");
        return -EBUSY;
    }
    return 0;
}

static int fib_release(struct inode *inode, struct file *file)
{
    mutex_unlock(&fib_mutex);
    return 0;
}

/* calculate the fibonacci number at given offset */
static ssize_t fib_read(struct file *file,
                        char *buf,
                        size_t size,
                        loff_t *offset)
{
	struct u64 result = fibonacci(1);
    return 0;
}

/* write operation is skipped */
static ssize_t fib_write(struct file *file,
                         const char *buf,
                         size_t size,
                         loff_t *offset)
{
    return 1;
}

static loff_t fib_device_lseek(struct file *file, loff_t offset, int orig)
{
    loff_t new_pos = 0;
    switch (orig) {
    case 0: /* SEEK_SET: */
        new_pos = offset;
        break;
    case 1: /* SEEK_CUR: */
        new_pos = file->f_pos + offset;
        break;
    case 2: /* SEEK_END: */
        new_pos = MAX_LENGTH - offset;
        break;
    }

    if (new_pos > MAX_LENGTH)
        new_pos = MAX_LENGTH;  // max case
    if (new_pos < 0)
        new_pos = 0;        // min case
    file->f_pos = new_pos;  // This is what we'll use now
    return new_pos;
}

const struct file_operations fib_fops = {
    .owner = THIS_MODULE,
    .read = fib_read,
    .write = fib_write,
    .open = fib_open,
    .release = fib_release,
    .llseek = fib_device_lseek,
};

static int __init init_fib_dev(void)
{
    int rc = 0;

    mutex_init(&fib_mutex);

    // Let's register the device
    // This will dynamically allocate the major number
    rc = alloc_chrdev_region(&fib_dev, 0, 1, DEV_FIBONACCI_NAME);

    if (rc < 0) {
        printk(KERN_ALERT
               "Failed to register the fibonacci char device. rc = %i",
               rc);
        return rc;
    }

    fib_cdev = cdev_alloc();
    if (fib_cdev == NULL) {
        printk(KERN_ALERT "Failed to alloc cdev");
        rc = -1;
        goto failed_cdev;
    }
    cdev_init(fib_cdev, &fib_fops);
    rc = cdev_add(fib_cdev, fib_dev, 1);

    if (rc < 0) {
        printk(KERN_ALERT "Failed to add cdev");
        rc = -2;
        goto failed_cdev;
    }

    fib_class = class_create(THIS_MODULE, DEV_FIBONACCI_NAME);

    if (!fib_class) {
        printk(KERN_ALERT "Failed to create device class");
        rc = -3;
        goto failed_class_create;
    }

    if (!device_create(fib_class, NULL, fib_dev, NULL, DEV_FIBONACCI_NAME)) {
        printk(KERN_ALERT "Failed to create device");
        rc = -4;
        goto failed_device_create;
    }
    return rc;
failed_device_create:
    class_destroy(fib_class);
failed_class_create:
    cdev_del(fib_cdev);
failed_cdev:
    unregister_chrdev_region(fib_dev, 1);
    return rc;
}

static void __exit exit_fib_dev(void)
{
    mutex_destroy(&fib_mutex);
    device_destroy(fib_class, fib_dev);
    class_destroy(fib_class);
    cdev_del(fib_cdev);
    unregister_chrdev_region(fib_dev, 1);
}

module_init(init_fib_dev);
module_exit(exit_fib_dev);
