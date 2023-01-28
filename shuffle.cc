/*
 * Example of using the <algorithm> std::shuffle and seeding a random number generator with either
 * the command-line argument provided seed integer, or the current system time if there is no
 * argument provided. Based off of the example at: 
 *
 *    http://www.cplusplus.com/reference/algorithm/shuffle/
 *
 *  Last updated: 2020-10-06
 *  Copyright 2020 UW. All rights reserved.
 *
 */
//

/*

#include <iostream>
#include <string>
#include <stdexcept>
#include <vector>
#include <algorithm>
#include <random>
#include <chrono>

void print( std::vector<int> const & v ) {
	for ( int i : v ) std::cout << i << ' ';
	std::cout << std::endl;
}

int main( int argc, char * argv[] ) {
	std::vector<int> v = { 1, 2, 3, 4, 5, 6, 7, 8, 9 };

	// use a time-based seed for the default seed value
	unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
	
	if ( argc > 1 ) {
		try {
			seed = std::stoi( std::string{ argv[1] } );
		} catch( std::invalid_argument & e ) {
			std::cerr << e.what() << std::endl;
			return 1;
		} catch( std::out_of_range & e ) {
			std::cerr << e.what() << std::endl;
			return -1;
		} // catch
	} // if

	std::default_random_engine rng{seed};

	for ( int i = 0; i < 1000; i++ ) {
		//std::shuffle( v.begin(), v.end(), std::default_random_engine(seed) );
		std::shuffle( v.begin(), v.end(), rng );
		print(v);		
	} // for

	return 0;
} // main

*/
