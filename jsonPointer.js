'use strict';

const fs = require('fs');
const { simdjson } = require('bindings')('addon');

const demo = 'jsonexamples/small/demo.json';
var content = fs.readFileSync(demo, 'utf-8');
var simdjsonOBJ = new simdjson(content);

console.log("Demo document");
console.log("JSON pointer : /Image/Width");
console.log(simdjsonOBJ.getValue('/Image/Width'));
console.log("");

console.log("JSON pointer : /Image/Height");
console.log(simdjsonOBJ.getValue('/Image/Height'));
console.log("");

console.log("JSON pointer : /Image/IDs/2");
console.log(simdjsonOBJ.getValue('/Image/IDs/2'));
console.log("");

const twitter = 'jsonexamples/twitter.json';
content = fs.readFileSync(twitter, 'utf-8');
simdjsonOBJ = new simdjson(content);

console.log("Twitter document");
console.log("JSON pointer : /statuses/0/id");
console.log(simdjsonOBJ.getValue('/statuses/0/id'));
console.log("");

console.log("JSON pointer : /statuses/1/id");
console.log(simdjsonOBJ.getValue('/statuses/1/id'));
console.log("");

console.log("JSON pointer : /statuses/2/id");
console.log(simdjsonOBJ.getValue('/statuses/2/id'));
console.log("");

console.log("JSON pointer : /statuses/3/id");
console.log(simdjsonOBJ.getValue('/statuses/3/id'));
console.log("");

console.log("JSON pointer : /statuses/4/id");
console.log(simdjsonOBJ.getValue('/statuses/4/id'));
console.log("");

console.log("JSON pointer : /statuses/5/id");
console.log(simdjsonOBJ.getValue('/statuses/5/id'));
console.log("");