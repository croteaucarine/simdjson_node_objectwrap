# simdjson, Node.js and Napi::ObjectWrap

## A Napi:ObjectWrap that allows to use C++ simdjson library with Node.js
...

## Performance results
...

## Requirements
...

## Getting started
```
git clone https://github.com/croteaucarine/simdjson_node_objectwrap
cd simdjson_node_objectwrap
npm install
```

## Code usage and example
Example below is also available in repo under the file example.js and can be run with ```node example.js```
```javascript
'use strict';

const fs = require('fs');
const { simdjson } = require('bindings')('addon');

const github_events = 'jsonexamples/github_events.json';
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
```

## Benchmarks
The script benchmarks.js contains the comparaison between the default JavaScript method JSON.parse and parsing with simdjson. 
The benchmarks runs with multiple JSON file formats that are located in the jsonexamples folder. 
To run benchmarks : ``node --exopse-gc benchmarks.js```

### Benchmarks results in GB/s








