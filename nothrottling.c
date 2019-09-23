#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/notifier.h>
#include <linux/suspend.h>
MODULE_LICENSE("GPL");
MODULE_AUTHOR("sanek9");
MODULE_DESCRIPTION("A simple example Linux module.");
MODULE_VERSION("0.01");


int no_throttling(void){
 unsigned int ecx = 0x1FC;
 unsigned int edx = 0, eax = 0;
 unsigned long long result = 0;
 __asm__ __volatile__("rdmsr" : "=a"(eax), "=d"(edx) : "c"(ecx));
 result = eax | (unsigned long long)edx << 0x20;
 printk(KERN_ALERT  "Module msrdrv: Read 0x%016llx (0x%08x:0x%08x) from MSR 0x%08x\n", result, edx, eax, ecx);
 eax = eax & 0xFFFFFFFE;
 printk(KERN_ALERT "Module msrdrv: Writing 0x%08x:0x%08x to MSR 0x%04x\n", edx, eax, ecx);
 __asm__ __volatile__("wrmsr" : : "c"(ecx), "a"(eax), "d"(edx));
 printk(KERN_INFO "Hello, World!\n");
 return 0;
}

int notify_key(struct notifier_block *nblock, unsigned long code, void *__param) {
    /* Add in this block for actual working notifier */
    no_throttling();
    printk(KERN_INFO "I'm Batman!\n");
    return (NOTIFY_OK);
}

static struct notifier_block nb = {
        .notifier_call = notify_key
};

static int __init lkm_example_init(void) {
 register_pm_notifier(&nb);
 no_throttling();
 return 0;
}

static void __exit lkm_example_exit(void) {
 unregister_pm_notifier(&nb);
 printk(KERN_INFO "Goodbye, World!\n");
}

module_init(lkm_example_init);
module_exit(lkm_example_exit);
