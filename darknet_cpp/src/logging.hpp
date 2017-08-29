/*
 *  Author: Maarten Vandersteegen EAVISE
 *  Description: Logging macros
 */

#ifndef LOGGING_HPP
#define LOGGING_HPP

#ifdef NDEBUG
#define DPRINTF(...)
#define EPRINTF(fmt, ...)   fprintf(stderr, fmt, ## __VA_ARGS__);
#else
#define DPRINTF(fmt, ...)   printf("%s:%d:%s(): " fmt, __FILE__, __LINE__, __func__, ## __VA_ARGS__);
#define EPRINTF(fmt, ...)   fprintf(stderr, "%s:%d:%s(): " fmt, __FILE__, __LINE__, __func__, ## __VA_ARGS__);
#endif

#endif /* LOGGING_HPP */
