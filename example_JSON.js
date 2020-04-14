'use strict';

const fs = require('fs');
const github_events = 'jsonexamples/github_events.json';
var file = fs.readFileSync(github_events, 'utf-8');
var JSONObj = JSON.parse(file);

// Display object content
//console.log(JSONObj);

// Display Object Keys
//console.log(Object.keys(JSONObj));

// Display Object length
//console.log(JSONObj.length);

// Display strignified Object
//console.log(JSON.stringify(JSONObj));

// Display Own Property Names
console.log(Object.getOwnPropertyNames(JSONObj));

// Display Own Property Descriptors
console.log(Object.getOwnPropertyDescriptors(JSONObj));

// Loop through JSON Array 
/*try {
    var i  = 1;
    for (let item of JSONObj) {
        console.log(item);
        i++;
    }
} catch(error) {
    console.error(error);
}*/

// Display nested elements
/*try {
    var i  = 1;
    for (let item of simdjsonOBJ) {
        console.log(item.actor.login);
        i++;
    }
} catch(error) {
    console.error(error);
}*/