/*
 * spinlock.h
 *
 *  Created on: 2015-9-8
 *      Author: gd
 */

#ifndef __SPINLOCK_H__
#define __SPINLOCK_H__

#ifdef __cplusplus
extern "C" {
#endif

/*
 * spinlock_lock
 *
 * 自旋锁 加锁
 */
extern void spinlock_lock(uint8_t num);
/*
 * spinlock_unlock
 *
 * 自旋锁 解锁
 */
extern void spinlock_unlock(uint8_t num);

#ifdef __cplusplus
}
#endif

#endif /* __SPINLOCK_H__ */
