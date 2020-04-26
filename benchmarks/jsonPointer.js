'use strict';

const fs = require('fs');
const { simdjson } = require('bindings')('addon');
const json_pointer = require('json-pointer');

const numberOfIterations = 1000;
const NS_PER_MS = 1e6;

var start;
var diff;
var end;
var ns = 0;
var jsonexamplesPath = './jsonexamples/';

const tests = [
    { file: 'small/cars.json', query: '0/tire_pressure/1', output:null, timeInMs:null },
    { file: 'small/demo.json', query: 'Image/Width', output:null, timeInMs:null },
    { file: 'small/demo.json', query: 'Image/Height', output:null, timeInMs:null },
    { file: 'small/demo.json', query: 'Image/IDs/2', output:null, timeInMs:null },
    { file: 'numbers.json', query: '0', output:null, timeInMs:null },
    { file: 'numbers.json', query: '10000', output:null, timeInMs:null },
    { file: 'numbers.json', query: '100001', output:null, timeInMs:null },
    { file: 'twitter.json', query: 'statuses/0/id', output:null, timeInMs:null },
    { file: 'twitter.json', query: 'statuses/5/id', output:null, timeInMs:null },
    { file: 'twitter.json', query: 'statuses/10/id', output:null, timeInMs:null },
    { file: 'twitter.json', query: null, output:null, timeInMs:null },
    { file: 'twitter.json', query: '', output:null, timeInMs:null },
    { file: 'twitter.json', query: 'qwerty', output:null, timeInMs:null }
];

/*************** simdjson#jsonPointer ***************/
console.log("|      Fichier      |  Requête  |        simdjson#jsonPointer        |  unité  |   iterations  |   resultat    |");
tests.forEach(function(test) {
  for (let i = 0; i < numberOfIterations; i++) {
    var simdjsonObj = new simdjson({path : (jsonexamplesPath + test.file)});

    start = process.hrtime.bigint();
    try {
      test.output = simdjsonObj.getValue(test.query);
    } catch(err) {  
        test.output = err.message;
    }
    var end = process.hrtime.bigint();
    ns += Number(end-start);
    gc();
  }
  test.timeInMs = (ns / numberOfIterations) / NS_PER_MS;
  console.log(" | " + test.file + " | " + test.query + " | " + test.timeInMs.toFixed(10) + " | ms | " + numberOfIterations + " | " + test.output );
  ns = 0; 
});

console.log("");

/*************** npm#json-pointer ***************/
console.log("|      Fichier       |        npm#json-pointer        |  unité  |   iterations  |   resultat    |");
tests.forEach(function(test) {
  var file = fs.readFileSync(jsonexamplesPath + test.file, 'utf-8');

  for (let i = 0; i < numberOfIterations; i++) {
    var JSONObj = JSON.parse(file);

    start = process.hrtime.bigint();
    try {
      test.output = json_pointer.get(JSONObj, "/" + test.query);
    } catch(err) {  
        test.output = err.message;
    }
    var end = process.hrtime.bigint();
    ns += Number(end-start);
    gc();
  }
  test.timeInMs = (ns / numberOfIterations) / NS_PER_MS;
  console.log(" | " + test.file + " | " + test.query + " | " + test.timeInMs.toFixed(10) + " | ms | " + numberOfIterations + " | " + test.output );
  ns = 0; 
});