'use strict';

const fs = require('fs');
var readlineSync = require('readline-sync');
const { simdjson } = require('bindings')('addon');

var files = ['apache_builds.json', 'canada.json', 'citm_catalog.json', 'github_events.json', 'gsoc-2018.json', 'instruments.json', 'marine_ik.json', 'mesh.json', 'mesh.pretty.json', 'numbers.json', 'random.json', 'twitter.json', 'twitterescaped.json', 'update-center.json']
var jsonexamplesPath = './jsonexamples/';

const numberOfIterations = 1000;
var file;
const GbSize = 1024 * 1024;
const NS_PER_MS = 1000000;

var start;
var diff;
var ns = 0;

/*************** simdjson#iterator ***************/
console.log("|      Fichier       |        simdjson#for(let..of)        |  unité |   iterations  |   resultat    |");

files.forEach(function(fileName) {
  for (let i = 0; i < numberOfIterations; i++) {
    var simdjsonObj = new simdjson({path : (jsonexamplesPath + fileName)});

    var result = [];
    start = process.hrtime();
    
    try {
      for (let item of simdjsonObj) {
        result.push(item);
      }
    } catch(error) {
      result = "not iterable";
    } finally {
      diff = process.hrtime(start);
      gc(); 
    }

    ns += diff[0] * NS_PER_MS + diff[1];
  }
  console.log("| " + fileName + " |       " +  ((ns / (NS_PER_MS * 1.0)) / numberOfIterations).toFixed(10) + "       |  ms     | " + numberOfIterations + " | " + result.toString().substring(0, 50) + '...');
  ns = 0; 

});

console.log();

/*************** JSON#iterator ***************/
console.log("|      Fichier       |        JSON#for(let..of)        |  unité |   iterations  |   resultat    |");
files.forEach(function(fileName) {
  file = fs.readFileSync(jsonexamplesPath + fileName, 'utf-8');
  for (let i = 0; i < numberOfIterations; i++) {
    var jsonOBJ = JSON.parse(file);

    var result = [];
    start = process.hrtime();
    
    try {
      for (let item of jsonOBJ) {
        result.push(item);
      }
    } catch(error) {
      result = "not iterable";
    } finally {
      diff = process.hrtime(start);
      gc(); 
    }

    ns += diff[0] * NS_PER_MS + diff[1];
  }
  console.log("| " + fileName + " |       " +  ((ns / (NS_PER_MS * 1.0)) / numberOfIterations).toFixed(10) + "       |  ms     | " + numberOfIterations + " | " + result.toString().substring(0, 50)+ '...');
  ns = 0; 

});
console.log();

/*************** simdjson#iterator ***************/
console.log("|      Fichier       |        simdjson#for(let..of)        |  unité |   iterations  |   resultat    |");

files.forEach(function(fileName) {
  var simdjsonObj = new simdjson({path : (jsonexamplesPath + fileName)});

  for (let i = 0; i < numberOfIterations; i++) {
    var result = [];
    start = process.hrtime();
    
    try {
      for (let item of simdjsonObj) {
        result.push(item);
      }
    } catch(error) {
      result = "not iterable";
    } finally {
      diff = process.hrtime(start);
      gc(); 
    }

    ns += diff[0] * NS_PER_MS + diff[1];
  }
  console.log("| " + fileName + " |       " +  ((ns / (NS_PER_MS * 1.0)) / numberOfIterations).toFixed(10) + "       |  ms     | " + numberOfIterations + " | " + result.toString().substring(0, 50) + '...');
  ns = 0; 

});

console.log();

/*************** JSON#iterator ***************/
console.log("|      Fichier       |        JSON#for(let..of)        |  unité |   iterations  |   resultat    |");
files.forEach(function(fileName) {
  file = fs.readFileSync(jsonexamplesPath + fileName, 'utf-8');
  var jsonOBJ = JSON.parse(file);

  for (let i = 0; i < numberOfIterations; i++) {
    var result = [];
    start = process.hrtime();
    
    try {
      for (let item of jsonOBJ) {
        result.push(item);
      }
    } catch(error) {
      result = "not iterable";
    } finally {
      diff = process.hrtime(start);
      gc(); 
    }

    ns += diff[0] * NS_PER_MS + diff[1];
  }
  console.log("| " + fileName + " |       " +  ((ns / (NS_PER_MS * 1.0)) / numberOfIterations).toFixed(10) + "       |  ms     | " + numberOfIterations + " | " + result.toString().substring(0, 50)+ '...');
  ns = 0; 

});
console.log();

function getFileSize(filePath) {
  var stats = fs.statSync(filePath);
  var size = stats["size"];
  return size / 1000;
}