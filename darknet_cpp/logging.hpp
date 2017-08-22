/*
 *  Author: Maarten Vandersteegen EAVISE
 *  Description: Logging macros
 */

#ifndef LOGGING_HPP
#define LOGGING_HPP

#ifdef DEBUG
#define DPRINTF printf
#define EPRINTF printf
#else
#define DPRINTF
#define EPRINTF printf
#endif

#endif /* LOGGING_HPP */
