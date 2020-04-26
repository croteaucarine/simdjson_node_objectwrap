'use strict';

const fs = require('fs');
const { simdjson } = require('bindings')('addon');

const github_events = 'jsonexamples/github_events.json';

const simdjsonOBJ = new simdjson({path : github_events});
//simdjsonOBJ.load({path : github_events});
//simdjsonOBJ.parse();

// Display object content
console.log(simdjsonOBJ);

// Display Object Keys
console.log(simdjsonOBJ.keys());

// Display Object length
console.log(simdjsonOBJ.length);

// Display strignified Object
//console.log(JSON.stringify(simdjsonOBJ));

// Loop through JSON Array 
// try-catch intercepts error if object is not an array
try {
    var i  = 1;
    for (let item of simdjsonOBJ) {
        console.log(item);
        i++;
    }
} catch(error) {
    console.error(error);
}

// Display nested elements
try {
    var i  = 1;
    for (let item of simdjsonOBJ) {
        console.log(item.actor.login);
        i++;
    }
} catch(error) {
    console.error(error);
}