# simdjson, Node.js and Napi::ObjectWrap

## A Napi::ObjectWrap that allows to use C++ simdjson library with Node.js
This is a project wrapping the simdjson library for un use in Node. 
**Note** : This is a master projet and is **still in developement**. I am still woriking on some approaches to omptimze performances, but results so far seems interesting.  

## Requirements
When upgraded to simdjson 0.3.1 (coming soon!), any computer shoud be able to run projet. Performances are a lot more interesting on computers with an SIMD architecture. 

## Getting started
```
git clone https://github.com/croteaucarine/simdjson_node_objectwrap
cd simdjson_node_objectwrap
npm install
```

## Constructor
- new simdjson() : Creates an empty object. load and parse must be called in order to manipulate the object.
- new simdjson(string document) : Creates a fully manipulable object. 
- new simdjson(object path) : Key path must me used to send defined path as string (example : {path : 'mydoc.json'}). Creates a fully manipulable object. 
- new simdjson(object doc) : Key path must me used to send document as string (example : {doc : docVariable}). Creates a fully manipulable object. 

## Static Methods
simdjson.isValid will be implemented soon!

## Code usage and example
The library implements JSON parsing and basic JSON methods length, keys and iterator. The library also implements navigation through JSON pointer defined by the [RFC6901 standard](https://tools.ietf.org/html/rfc6901).

### Basic usage example
Example below is also available in repo under the folder examples and can be run with ```node examples/basic_usage.js```

```javascript
'use strict';

const fs = require('fs');
const { simdjson } = require('bindings')('addon');

const simdjsonOBJ = new simdjson({path : 'jsonexamples/github_events.json'});

// Display object content
console.log(simdjsonOBJ);

// Display Object Keys
console.log(simdjsonOBJ.keys());

// Display Object length
console.log(simdjsonOBJ.length);

// Display strignified Object
console.log(JSON.stringify(simdjsonOBJ));

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

### JSON Pointer example ([JavaScript Object Notation (JSON) Pointer (RFC 6901)](https://tools.ietf.org/html/rfc6901))
Example below and more JSON Pointer uses are also available in repo under the file jsonPointer.js and can be run with ```node examples/jsonPointer.js```

```javascript
var simdjsonOBJ = new simdjson({ path : 'jsonexamples/small/cars.json' });
console.log(simdjsonOBJ.getValue('0/tire_pressure/1'));   // outputs 39.9

var simdjsonOBJ = new simdjson({ path : 'jsonexamples/small/demo.json' });
console.log(simdjsonOBJ.getValue('Image/IDs/2'));  // outputs 234
console.log(simdjsonOBJ.getValue('Image/Width'));  // outputs 800
console.log(simdjsonOBJ.getValue('Image/Height')); // outputs 600
console.log(simdjsonOBJ.getValue('Image/Width') * simdjsonOBJ.getValue('/Image/Height')); // outputs 480000

simdjsonOBJ = new simdjson({ path : 'jsonexamples/twitter.json'});
console.log(simdjsonOBJ.getValue('statuses/0/id')); // outputs 505874924095815700
```

## Benchmarks
The script benchmarks contains multiple comparaisons between the native JavaScript JSON methods and equivalent simdjson methods (parse, length, keys, iterator and JSON Pointer). 
The benchmarks run with multiple JSON file formats that are located in the jsonexamples folder and results output in respective files in the benchmarks folder. 
To run benchmarks : ```npm run benchmarks```

### Benchmarks results in GB/s

|      Fichier       |         simdjson#parse  (GB/s)      |        JSON#parse (GB/s) |
| ------------- | ------------- | ------------- |
| apache_builds.json |       0.6582093176       |  0.3197537729  |
| canada.json |       0.5285019113       |  0.1369665620 |
| citm_catalog.json |   1.3283484553       |   0.3857911551 |
| github_events.json |      0.4011839264       |   0.3163136989 |
| gsoc-2018.json |         1.5753144939       |  0.5841688036 |
| instruments.json |     0.9355347016       |  0.3719788183 |
| marine_ik.json |    0.5655987608       |  0.1880935363 |
| mesh.json |        0.5523368471       |  0.2016259385   |
| mesh.pretty.json |      0.7585213936       |  0.3226030418 |
| numbers.json |      0.4698898210       |  0.2040313806 |
| random.json |          0.8534592398       |  0.2135693655 |
| twitter.json |          1.1542076927       |  0.2716755377 |
| twitterescaped.json |           0.7656334327       |  0.2160604291 |
| update-center.json |          0.9921201220       |  0.1903520690 |

## Performance results
![GBPS Graph](doc/gbps.png)

## Docker
```
docker build -t docker build -t simdjson_node_objectwrap . 
```

## License
This code is made available under the Apache License 2.0.




