# simdjson, Node.js and Napi::ObjectWrap

## A Napi:ObjectWrap that allows to use C++ simdjson library with Node.js
...

## Performance results
![GBPS Graph](doc/gbps.png)

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
To run benchmarks : ```node --exopse-gc benchmarks.js```

### Benchmarks results in GB/s
|      File       |        JSON#parse        | simdjson#parse |
| ------------- | ------------- |------------- |
| apache_builds.json |       0.3508060654       |  1.2776726171       |
| canada.json |       0.1411976798       | 0.6375911861       |
| citm_catalog.json |       0.4489479678       |  0.3123227330       |
| github_events.json |       0.3431752242       |  0.8627043830       |
| gsoc-2018.json |       0.6820689779       |  1.2901085179       |
| instruments.json |       0.3263017406       |  1.0613046511       |
| marine_ik.json |       0.1985107722       |  0.5801906399       |
| mesh.json |       0.2001960761       |  0.5928486893       |
| mesh.pretty.json |       0.3109946339       |  0.5928486893       |
| numbers.json |       0.2051137580       |  0.6850210674       |
| random.json |       0.2188974147       |  0.2741147806       | 
| twitter.json |       0.2819563119       |  0.3169782538       | 
| twitterescaped.json |       0.2307891846       |   0.8102025150       |
| update-center.json |       0.1986242831       |  0.2975296458       | 







