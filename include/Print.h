//
//  Print.h
//  Compression
//
//  Created by admin on 2019-02-24.
//  Copyright © 2019 maximpuchkov. All rights reserved.
//

#ifndef Print_h
#define Print_h

#include <iostream>


// Fold expressions are C++17


/* Helper functions to print to console */

// Print any arguments
template<typename ...T>
void print(T &&...ts);


// Print any arguments, and add new lines
template<typename ...T>
void print_spaced(int lines, T &&...ts);









/* Implementation */

const char NEW_LINE = '\n';


template<typename ...T>
void print(T &&...ts) {
    (std::cout << ... << std::forward<T>(ts)) << std::endl;
}


// Print any arguments, and add new lines
template<typename ...T>
void print_spaced(int lines, T &&...ts) {
    print(std::forward<T>(ts)...);
    
    for (int i = 0; i < lines; i ++) {
        print(NEW_LINE);
    }
}




#endif /* Print_h */
