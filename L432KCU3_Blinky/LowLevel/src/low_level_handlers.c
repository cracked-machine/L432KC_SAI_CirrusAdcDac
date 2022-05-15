

#include <stdint.h>


extern uint32_t g_systick;

// System tick timer
void SysTick_Handler(void) { ++g_systick; }

// Non maskable interrupt
void NMI_Handler(void) {  while(1) {} }
// Hard fault interrupt
void HardFault_Handler(void) {  while(1) {} }
// Memory management fault
void MemManage_Handler(void) { while(1) {} }
// Prefetch fault, memory access fault
void BusFault_Handler(void) { while(1) {} }
// Undefined instruction or illegal state
void UsageFault_Handler(void) { while(1) {} }
// System service call via SWI instruction
void SVC_Handler(void) { }
// Debug monitor
void DebugMon_Handler(void) { }
// Pendable request for system service
void PendSV_Handler(void) { }

// Pure virtual function call error handler. 
// Prevents linker bloating code if using pure virtual functions.
void __cxa_pure_virtual(void) { while(1); }

