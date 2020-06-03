'use strict';

const fs = require('fs');
const { simdjson } = require('bindings')('addon');

const path = './jsonexamples/small/themoviedb.json';

const simdjsonOBJ = new simdjson({path : path});

// Display object content
const test = simdjsonOBJ.toString();
console.log(test);
//console.log(simdjsonOBJ.toJSON());

// Display Object Keys
//console.log(simdjsonOBJ.keys());

// Display Object length
//console.log(simdjsonOBJ.length);

// Display strignified Object
//console.log(JSON.stringify(simdjsonOBJ));

// Loop through JSON Array 
// try-catch intercepts error if object is not an array
/*try {
    var i  = 1;
    for (let item of simdjsonOBJ) {
        console.log(i);
        console.log(item.genre_ids);
        i++;
    }
} catch(error) {
    console.error(error);
}*/

// Affichage des objets imbriqués
/*try {
    for (let item of simdjsonOBJ) {
        console.log(item);
        console.log(item.genre_ids);
    }
} catch(error) {
    console.error(error);
}*/