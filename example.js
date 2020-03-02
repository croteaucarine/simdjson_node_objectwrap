'use strict';

const fs = require('fs');
const { simdjson } = require('bindings')('addon');

const deo = 'jsonexamples/github_events.json';
const content = fs.readFileSync(github_events, 'utf-8');
const simdjsonOBJ = new simdjson(content);

// Display object content
console.log(simdjsonOBJ);

// Display Object Keys
console.log(simdjsonOBJ.keys());

// Display Object length
console.log(simdjsonOBJ.length);

// Display strignified Object
console.log(JSON.stringify(simdjsonOBJ));

// Display Own Property Names
console.log(Object.getOwnPropertyNames(simdjsonOBJ));

// Display Own Property Descriptors
console.log(Object.getOwnPropertyDescriptors(simdjsonOBJ));

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