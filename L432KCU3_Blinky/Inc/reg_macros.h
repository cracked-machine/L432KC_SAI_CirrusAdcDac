#ifndef __REG_MACROS_HPP__
#define __REG_MACROS_HPP__

#define SET_BIT(REG, BIT)     ((REG) |= (BIT))

#define CLEAR_BIT(REG, BIT)   ((REG) &= ~(BIT))

#define READ_BIT(REG, BIT)    ((REG) & (BIT))

#define CLEAR_REG(REG)        ((REG) = (0x0))

#define WRITE_REG(REG, VAL)   ((REG) = (VAL))

#define READ_REG(REG)         ((REG))

#define MODIFY_REG(REG, CLEARMASK, SETMASK)  WRITE_REG((REG), (((READ_REG(REG)) & (~(CLEARMASK))) | (SETMASK)))



// void LL_mDelay(uint32_t Delay)
// {
//   __IO uint32_t  tmp = SysTick->CTRL;  /* Clear the COUNTFLAG first */
//    uint32_t tmpDelay; /* MISRAC2012-Rule-17.8 */
//   /* Add this code to indicate that local variable is not used */
//   ((void)tmp);
//   tmpDelay  = Delay;
//   /* Add a period to guaranty minimum wait */
//   if (tmpDelay  < LL_MAX_DELAY)
//   {
//     tmpDelay ++;
//   }

//   while (tmpDelay  != 0U)
//   {
//     if ((SysTick->CTRL & SysTick_CTRL_COUNTFLAG_Msk) != 0U)
//     {
//       tmpDelay --;
//     }
//   }
// }

#endif // __REG_MACROS_HPP__