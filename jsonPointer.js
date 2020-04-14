'use strict';

const fs = require('fs');
const { simdjson } = require('bindings')('addon');

console.log("Cars document");
var simdjsonOBJ = new simdjson({ path : 'jsonexamples/small/cars.json' });

console.log("JSON pointer : /Image/Height");
console.log(simdjsonOBJ.getValue('0/tire_pressure/1'));

console.log("Demo document");
var simdjsonOBJ = new simdjson({ path : 'jsonexamples/small/demo.json' });

console.log("JSON pointer : Image/Width");
console.log(simdjsonOBJ.getValue('Image/Width'));

console.log("JSON pointer : Image/Height");
console.log(simdjsonOBJ.getValue('Image/Height'));
console.log("");

console.log("JSON pointer : Image/Width * Image/Height");
console.log(simdjsonOBJ.getValue('Image/Width') * simdjsonOBJ.getValue('Image/Height'));
console.log("");

console.log("JSON pointer : Image/IDs/2");
console.log(simdjsonOBJ.getValue('Image/IDs/2'));
console.log("");

console.log("Twitter document");
simdjsonOBJ = new simdjson({ path : 'jsonexamples/twitter.json'});

console.log("JSON pointer : statuses/0/id");
console.log(simdjsonOBJ.getValue('statuses/0/id'));
console.log("");

console.log("JSON pointer : statuses/1/id");
console.log(simdjsonOBJ.getValue('statuses/1/id'));
console.log("");

console.log("JSON pointer : statuses/2/id");
console.log(simdjsonOBJ.getValue('statuses/2/id'));
console.log("");

console.log("JSON pointer : statuses/3/id");
console.log(simdjsonOBJ.getValue('statuses/3/id'));
console.log("");

console.log("JSON pointer : statuses/4/id");
console.log(simdjsonOBJ.getValue('statuses/4/id'));
console.log("");

console.log("JSON pointer : statuses/5/id");
console.log(simdjsonOBJ.getValue('statuses/5/id'));
console.log("");


try {
  console.log(simdjsonOBJ.getValue());  // Error is expected
} catch(err) {  
  console.log(err.message);
}
console.log("");

try {
  console.log(simdjsonOBJ.getValue(1));  // Error is expected
} catch(err) {  
  console.log(err.message);
}
console.log("");

console.log("Testing invalid string");
try {
  console.log(simdjsonOBJ.getValue("QWERTY"));
} catch(err) {  
  console.log(err.message);
}