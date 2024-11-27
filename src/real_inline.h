#ifndef ATMEGA_SNAKE_REAL_INLINE_H_
#define ATMEGA_SNAKE_REAL_INLINE_H_

#ifdef __OPTIMIZE__
#define REAL_INLINE __attribute__((always_inline)) inline
#else
#define REAL_INLINE inline
#endif

#endif /* ATMEGA_SNAKE_REAL_INLINE_H_ */
