#include "ec.h"
#include "ptab.h"
#include "string.h"
#include "stdbool.h"

#define MAX 0xC0000000
#define SIZE_OF_PAGE 0x1000
#define ZERO 0x0

typedef enum {
    sys_print      = 1,
    sys_sum        = 2,
    sys_break      = 3,
    sys_thr_create = 4,
    sys_thr_yield  = 5,
} Syscall_numbers;

void Ec::syscall_handler (uint8 a)
{
    // Get access to registers stored during entering the system - see
    // entry_sysenter in entry.S
    Sys_regs * r = current->sys_regs();
    Syscall_numbers number = static_cast<Syscall_numbers> (a);
    // mword attrs = Ptab::PRESENT | Ptab::RW | Ptab::USER;

    switch (number) {
        case sys_print: {
            char *data = reinterpret_cast<char*>(r->esi);
            unsigned len = r->edi;
            for (unsigned i = 0; i < len; i++)
                printf("%c", data[i]);
            break;
        }
        case sys_sum: {
            // Naprosto nepotřebné systémové volání na sečtení dvou čísel
            int first_number = r->esi;
            int second_number = r->edi;
            r->eax = first_number + second_number;
            break;
        }
        case sys_break: {

            bool result = true;

            mword attr_alloc = (Ptab::PRESENT | Ptab::RW | Ptab::USER);
            mword attr_dealloc = ~ (Ptab::RW | Ptab::USER);

            r->eax = Ec::break_current;

            mword last_break = Ec::break_current;
            mword request_break = r->esi;
            
            // return the current break value
            if (request_break == 0) {
                break;
            }

            // control of min/max break value
            if ((request_break < Ec::break_min) || (request_break > MAX)) {
                r->eax = 0;
                break;
            }

            // request 
            if (request_break == last_break) {
                Ec::break_current = request_break;
                break;
            }
            /* ALOKACE */
            if (request_break > last_break) {

                /* VYNULOVANI */
                void* p = reinterpret_cast<void*>(last_break);

                mword count1 = request_break - last_break;
                mword count2 = PAGE_SIZE - (last_break % PAGE_SIZE);
                
                if (((last_break / PAGE_SIZE) == (request_break / PAGE_SIZE)) && (last_break % PAGE_SIZE) != ZERO) {
                    memset(p, ZERO, count1);
                    Ec::break_current = request_break;
                    break;
                }
                if (count2 != PAGE_SIZE ) {
                    memset(p, ZERO, count2);
                }
                //rounds a number up
                mword current_break = ((last_break % PAGE_SIZE) == 0) ? last_break : (last_break - (last_break % PAGE_SIZE)) + PAGE_SIZE;
                int page_counter = 0;

                while (current_break < request_break) {
                    
                    void* page = Kalloc::allocator.alloc_page(1, Kalloc::FILL_0);
                    // if alloc is failed -> deallocate all allocated pages
                    if(!page) {

                    DEALLOC:
                        while (page_counter > 0) {
                            current_break -= PAGE_SIZE;

                            mword physic_adress = Ptab::get_mapping(current_break);
                            physic_adress = physic_adress & ~PAGE_MASK;
                            Ptab::insert_mapping(current_break, physic_adress, attr_dealloc);
                            
                            void* virtual_adress = Kalloc::phys2virt(physic_adress);
                            Kalloc::allocator.free_page(virtual_adress);

                            page_counter--;
                        }  

                        r->eax = 0;
                        result = false;
                        break;   

                    }    
                    else {  // if alloc is okay
                        
                        mword physic_adress = Kalloc::virt2phys(page);
                        
                        if(!Ptab::insert_mapping(current_break, physic_adress, attr_alloc)) {
                            r->eax = 0;
                            result = false;
                            goto DEALLOC;
                        }
                        current_break += PAGE_SIZE;
                        page_counter++;
                    }  
                    
                }

                /* FINISH ALLOCATE */
                if (result) {
                    r->eax = last_break;
                    Ec::break_current = r->esi;
                }

                break;    
            }
            /* DEALOKACE */
            if (request_break < last_break) {

                /* VYNULOVANI */
                void* p = reinterpret_cast<void*>(request_break);

                mword count1 = last_break - request_break;
                mword count2 = PAGE_SIZE - (request_break % PAGE_SIZE);
                
                if ((last_break / PAGE_SIZE) == (request_break / PAGE_SIZE)) {
                    memset(p, ZERO, count1);
                    Ec::break_current = r->esi;
                    break;
                }
                if (count2 != PAGE_SIZE ) {
                    memset(p, ZERO, count2);
                    request_break += count2;
                }

                //rounds a number up
                mword current_break = ((last_break % PAGE_SIZE) == 0) ? last_break : (last_break - (last_break % PAGE_SIZE)) + PAGE_SIZE;
               
                while (current_break > request_break) {
                    current_break -= PAGE_SIZE;

                    mword physic_adress = Ptab::get_mapping(current_break);
                    physic_adress = physic_adress & ~PAGE_MASK;
                    Ptab::insert_mapping(current_break, physic_adress, attr_dealloc);

                    void* virtual_adress = Kalloc::phys2virt(physic_adress);
                    Kalloc::allocator.free_page(virtual_adress);

                } 

                /* FINISH DEALLOCATE */
                if (result) {
                    Ec::break_current = r->esi;
                }

                break;
            }
            break;
        }
        default:
            printf ("unknown syscall %d\n", number);
            break;
    };

    ret_user_sysexit();
}
