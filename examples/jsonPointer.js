'use strict';

const fs = require('fs');
const { simdjson } = require('bindings')('addon');
const colors = require('colors');

console.log(colors.blue("Cars document"));
console.log("");
var simdjsonOBJ = new simdjson({ path : './jsonexamples/small/cars.json' });

console.log("JSON pointer : 0/tire_pressure/1");
console.log(simdjsonOBJ.getValue('0/tire_pressure/1'));
console.log("");

console.log("");
console.log(colors.blue("Demo document"));
console.log("");

var simdjsonOBJ = new simdjson({ path : './jsonexamples/small/demo.json' });

console.log("JSON pointer : Image/Width");
console.log(simdjsonOBJ.getValue('Image/Width'));
console.log("");

console.log("JSON pointer : Image/Height");
console.log(simdjsonOBJ.getValue('Image/Height'));
console.log("");

console.log("JSON pointer : Image/Width * Image/Height");
console.log(simdjsonOBJ.getValue('Image/Width') * simdjsonOBJ.getValue('Image/Height'));
console.log("");

console.log("JSON pointer : Image/IDs/2");
console.log(simdjsonOBJ.getValue('Image/IDs/2'));
console.log("");

console.log("");
console.log(colors.blue("Numbers document"));
console.log("");
simdjsonOBJ = new simdjson({ path : './jsonexamples/numbers.json'});

console.log("JSON pointer : 0");
console.log(simdjsonOBJ.getValue('0'));
console.log("");

console.log("JSON pointer : 10000");
console.log(simdjsonOBJ.getValue('10000'));
console.log("");

console.log("JSON pointer : 10001 - Error is expected");
try {
  console.log(simdjsonOBJ.getValue('10001'));  // Error is expected
} catch(err) {  
  console.log(colors.red(err.message));
}
console.log("");

console.log("");
console.log(colors.blue("Twitter document"));
console.log("");
simdjsonOBJ = new simdjson({ path : './jsonexamples/twitter.json'});

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

console.log("JSON pointer : calling getValue witout argument - Error is expected");
try {
  console.log(simdjsonOBJ.getValue());  // Error is expected
} catch(err) {  
  console.log(colors.red(err.message));
}
console.log("");

console.log("JSON pointer : calling getValue with invalid argument - Error is expected");
try {
  console.log(simdjsonOBJ.getValue(1));  // Error is expected
} catch(err) {  
  console.log(colors.red(err.message));
}
console.log("");

console.log("Testing non-existing string in document - Error is expected");
try {
  console.log(simdjsonOBJ.getValue("QWERTY"));
} catch(err) {  
  console.log(colors.red(err.message));
}