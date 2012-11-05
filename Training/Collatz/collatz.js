#!/usr/bin/env node

/*
	A simple command line utility to follow the Collatz conjecture.

	@params

	argv[2] => seed
	argv[3] => logging file (* optional)
*/

(function(){

	(function main(){

		var argc = process.argv.length-1;


		if ( argc !== 2 && argc !== 3 ) {

			console.log('Invalid/missing input.');

			return 1;
		}


		var num = process.argv[2];

		if ( num < 0 ) {

			console.log('No natural number.');

			return 2;
		}


		var steps = collatz( num );

		console.log('\nsteps: ', steps.length );


		if ( argc === 3 ) {

			var fs = require('fs'),

				file = process.argv[3],

				msg = steps.join('');

			fs.writeFile( file, msg, function ( err ) {

				if ( err ) throw err;

				console.log('\nlogged in: "' + file +'"' );
			});
		}

		return 0;

	})();



	function collatz ( num ) {

		var steps = [];

		do {

			if ( num%2 === 0 ) {

				num /= 2;

			} else {

				num = num * 3 + 1;
			}

			steps.push( '\n'+num );

		} while ( num !== 1 );


		return steps;
	}

})();
