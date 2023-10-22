/*
 * ======================================================== *
 * Name:        error.h                                     *
 * Author:      RADIM SAFAR - FIT VUT                       *
 * Email:       xsafar27(at)stud.fit.vutbr.cz               *
 * Last modif:  03/14/2022                                  *
 * ======================================================== *
 */


#ifndef ERROR_H
#define ERROR_H

/**
 * @brief Prints out input message on stderr and exits the program.
 * 
 * Prints out formatted string the same way as printf would on stderr.
 * Adds "Chyba: " in front of the message.
 * Exists program with exit(1)
 * 
 * @param fmt 
 * @param ... 
 */
void error_exit(const char *fmt, ...);

/**
 * @brief Print out input message on stderr.
 * 
 * Prints out formatted string the same waz as printf would on stderr.
 * Adds "Chyba: " in front of the message.
 * 
 * @param fmt 
 * @param ... 
 */
void warning_msg(const char *fmt, ...);

#endif // end ERROR_H