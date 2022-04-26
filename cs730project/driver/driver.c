#include <linux/module.h>
#include <linux/cdev.h>
#include <linux/device.h>
#include <linux/kernel.h>
#include <linux/uaccess.h>
#include <linux/fs.h>
#include<linux/ioctl.h>
#include<linux/kernel.h>
#include<linux/mm.h>
#include<linux/mm_types.h>
#include<linux/file.h>
#include<linux/path.h>
#include<linux/slab.h>
#include<linux/dcache.h>
#include<linux/sched.h>
#include<linux/fs_struct.h>
#include <asm/tlbflush.h>
#include <linux/hugetlb.h>
#include<linux/huge_mm.h>

#include <asm/pgalloc.h>
#include <asm/uaccess.h>
#include <asm/pgtable.h>
#include <linux/syscalls.h>


#define DEVNAME "pgdv"


//struct used by ioctl to pass arguemnts
typedef struct {

        int pid;

}pinfo;

#define CLEAR_DIRTY _IOW ('p',1,pinfo *)
#define TRACK_DIRTY _IOW ('p',2,pinfo *)
#define COW _IOW ('p',3,pinfo *)



struct file *dirty_track_file;




static int major;
atomic_t  device_opened;
static struct class *pgdv_class;
struct device *pgdv_device;


extern void(*cow_dirty_track)(struct mm_struct *mm);
void cow_track(struct mm_struct *mm);



static int pgdv_open(struct inode *inode, struct file *file)
{
        atomic_inc(&device_opened);
        try_module_get(THIS_MODULE);
	cow_dirty_track=&cow_track;
        printk(KERN_INFO "Device opened successfully\n");
        return 0;
}

static int pgdv_release(struct inode *inode, struct file *file)
{
        atomic_dec(&device_opened);
        module_put(THIS_MODULE);
	cow_dirty_track = NULL;
        printk(KERN_INFO "Device closed successfully\n");

        return 0;
}

static ssize_t pgdv_read(struct file *filp,
                           char *buffer,
                           size_t length,
                           loff_t * offset)
{
        printk(KERN_INFO "In read\n");
	return 0;
}

static ssize_t pgdv_get(struct file *filp, const char *buff, size_t len, loff_t * off)
{

	
	int path;
        printk(KERN_INFO "In pgdv_get\n");

        if(copy_from_user(&path, buff, len))
             return -1;

	printk(" pid is : %s \n",buff);	
	
	
/*
	FILE *f;
	int i;
*/
/*
        //f = fopen(path, "rb");
        //fseek(f, -2, SEEK_END);
        while(1){
                char c = fgetc(f);
                if(c == -1)
                        return 0;

                if(c)
                        printk(KERN_INFO "%ld : 0x%X\n", ftell(f), c);
                //
        }
*/
	return 0;
}

static int count;

loff_t pos = 0;

void clean_dirty_ptes(struct mm_struct *mm, int clear)
{
        pgd_t *pgd;
        pud_t *pud;
        pmd_t *pmd;
        pte_t *pte;
        pte_t *page_table;
        spinlock_t *ptl;
        unsigned long address;
        unsigned long i,j,k,l;
        unsigned long user = 0;
        unsigned long mask = _PAGE_DIRTY | _PAGE_PRESENT | _PAGE_USER;	// changed to dirty bit mask
        struct vm_area_struct *vma;
        pgd_t *base = mm->pgd;

        ssize_t ccode=0;
        
	dirty_track_file = filp_open("/home/nikhila/cs730project/dirty", O_CREAT | O_TRUNC | O_WRONLY | O_LARGEFILE, 0666);
	if (IS_ERR(dirty_track_file) || dirty_track_file == NULL) {
        
		printk("Could not open file dirty.log\n");
                BUG_ON(IS_ERR(dirty_track_file) || dirty_track_file == NULL);
        
	}







	for(i=0; i<PTRS_PER_PGD; i++)
        {
                pgd = base + i;
                if((pgd_flags(*pgd) & mask) != mask)
                        continue;
                for(j=0; j<PTRS_PER_PUD; j++)
                {
                        pud = (pud_t *)pgd_page_vaddr(*pgd) + j;
                        if((pud_flags(*pud) & mask) != mask)
                                continue;
                        address = (i<<PGDIR_SHIFT) + (j<<PUD_SHIFT);
			/*			
                        if(vma && pud_huge(*pud) && is_vm_hugetlb_page(vma))
                        {
                                spin_lock(&mm->page_table_lock);
                                page_table = huge_pte_offset(mm, address,1<<30 );
                                // *page_table = pte_mkreserve(*page_table);
                                *page_table = pte_clear_flags(*page_table,_PAGE_DIRTY);
                                printk(KERN_INFO "after clear from pud page: [%lx]\n", pte_val(*page_table));

				

                                spin_unlock(&mm->page_table_lock);
                                continue;
                        }*/
                        for(k=0; k<PTRS_PER_PMD; k++)
                        {
                                pmd = (pmd_t *)pud_page_vaddr(*pud) + k;
                                if((pmd_flags(*pmd) & mask) != mask)
                                        continue;
                                address = (i<<PGDIR_SHIFT) + (j<<PUD_SHIFT) + (k<<PMD_SHIFT);
                                vma = find_vma(mm, address);
				/*				
                                if(vma && pmd_huge(*pmd) && ( (transparent_hugepage_active(vma)) ||is_vm_hugetlb_page(vma)))
                                {
                                        spin_lock(&mm->page_table_lock);
                                        // *pmd = pmd_mkreserve(*pmd);
                                        *pmd = pmd_clear_flags(*pmd,_PAGE_DIRTY);
                                        printk(KERN_INFO "after clear from pmd page: [%lx]\n", pmd_val(*pmd));

                                        spin_unlock(&mm->page_table_lock);
                                        continue;
                                }
				*/
                                for(l=0; l<PTRS_PER_PTE; l++)
                                {
                                        pte = (pte_t *)pmd_page_vaddr(*pmd) + l;
                                        if((pte_flags(*pte) & mask) != mask)
                                                continue;
/*
					*pte = pte_set_flags(*pte, _PAGE_DIRTY);

					asm("sfence":::"memory");
					printk(KERN_INFO "after set: [%lx]\n", pte_val(*pte));

					*pte = pte_clear_flags(*pte,_PAGE_DIRTY);

					asm("sfence":::"memory");
					printk(KERN_INFO "after clear: [%lx]\n", pte_val(*pte));
*/										
                                        address = (i<<PGDIR_SHIFT) + (j<<PUD_SHIFT) + (k<<PMD_SHIFT) + (l<<PAGE_SHIFT);
                                        vma = find_vma(mm, address);
										
                                        if(vma)
                                        {
                                                page_table = pte_offset_map_lock(mm, pmd, address, &ptl);
                                               // *pte = pte_mkreserve(*pte);
					if(clear == 1) {

						*pte = pte_clear_flags(*pte,_PAGE_DIRTY);
						printk(KERN_INFO "after clear: [%lx]\n", pte_val(*pte));
					}
		     			if(clear == 0) {

						count++;

				                ccode = kernel_write(dirty_track_file, (void *)&count, 4, &pos);

						pos+=4;
						//printk(KERN_ALERT" %d",count);
					
					}			

					if(clear == 2) //at the initial signal to clear the write bit of all the entries
					{
					
					*pte = pte_clear_flags(*pte, _PAGE_RW);
					
					}
					
					if(clear == 3) //for page fault handler to revert read bit
					{
					
					*pte = pte_set_flags(*pte, _PAGE_RW);

                                        ccode = kernel_write(dirty_track_file, (void *)&count, 4, &pos);

					pos+=4;					
					}
				
					pte_unmap_unlock(page_table, ptl);
                                        }
                                        user++;
                                }
                        }
                }
        }
}


void cow_track(struct mm_struct *mm) {

	
	clean_dirty_ptes(mm,3);



}



static long ioctl_redirect(struct file *f, unsigned int cmd, unsigned long arg) {

        pinfo p;
        if(copy_from_user(&p, (pinfo *)arg, sizeof(pinfo))) {
                printk("couldnt copy error \n");
        }

	switch(cmd) {
	
		case CLEAR_DIRTY : 
			{
			
			
				printk(KERN_ALERT"\npid is %d\n",p.pid);

				clean_dirty_ptes(current->mm, 1);
				
				break;

			}
		case TRACK_DIRTY : 
			{
			
			
				printk(KERN_ALERT"\npid is %d\n",p.pid);

				clean_dirty_ptes(current->mm, 0);
				
			        filp_close(dirty_track_file, NULL);

				printk(KERN_ALERT"number of pages dirtied after clearing are : %d ", count);	
				break;

			}
		case COW :
			{
				
				clean_dirty_ptes(current->mm, 2);
				
				break;
			}

	}
return p.pid;

}


static struct file_operations fops = {
        .read = pgdv_read,
        .write = pgdv_get,
        .open = pgdv_open,
        .release = pgdv_release,
	.unlocked_ioctl = ioctl_redirect
};

static char *pgdv_devnode(struct device *dev, umode_t *mode)
{
        if (mode && dev->devt == MKDEV(major, 0))
                *mode = 0666;
        return NULL;
}

int init_module(void)
{
	int err;
	major = register_chrdev(0, DEVNAME, &fops);
	err = major;
	if (err < 0) {
		printk(KERN_ALERT "Registering char device failed with %d\n", major);
		return  err;
	}

	pgdv_class = class_create(THIS_MODULE, DEVNAME);
	err = PTR_ERR(pgdv_class);
	if (IS_ERR(pgdv_class)){
		unregister_chrdev(major, DEVNAME);
		return -1;
	}

	pgdv_class->devnode = pgdv_devnode;

	pgdv_device = device_create(pgdv_class, NULL, MKDEV(major, 0), NULL, DEVNAME);
	err = PTR_ERR(pgdv_device);
	if (IS_ERR(pgdv_device)){
		class_destroy(pgdv_class);
		return -1;
	}

	printk(KERN_INFO "I was assigned major number %d. To talk to\n", major);
	atomic_set(&device_opened, 0);
	return 0;

}

void cleanup_module(void)
{
        device_destroy(pgdv_class, MKDEV(major, 0));
        class_destroy(pgdv_class);
        unregister_chrdev(major, DEVNAME);
	printk(KERN_INFO "Goodbye kernel\n");
}

MODULE_AUTHOR("tabisha@cse.iitk.ac.in");
MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("project modules");
