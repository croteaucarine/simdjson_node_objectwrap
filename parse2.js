'use strict';

const fs = require('fs');
var readlineSync = require('readline-sync');
const { simdjson } = require('bindings')('addon');

const update_center = '../jsonexamples/update-center.json';

var files = ['apache_builds.json', 'canada.json', 'citm_catalog.json', 'github_events.json', 'gsoc-2018.json', 'instruments.json', 'marine_ik.json', 'mesh.json', 'mesh.pretty.json', 'numbers.json', 'random.json', 'twitter.json', 'twitterescaped.json', 'update-center.json']
var jsonexamplesPath = 'jsonexamples/';

const numberOfIterations = 1000;
const NS_PER_MS = 1000000;

var start;
var diff;
var ns = 0;
var file;
const GbSize = 1024 * 1024;
const NS_PER_SEC = 1e9;

/*************** simdjson#parse ***************/
console.log("|      Fichier       |        simdjson#parse        |  unité |  iterations ");

files.forEach(function(fileName){
  var fileSize = getFileSize(jsonexamplesPath + fileName);
  for (let i = 0; i < numberOfIterations; i++) {
      var start = process.hrtime();
      const simdjsonOBJ = new simdjson({path : (jsonexamplesPath + fileName)});
      var diff = process.hrtime(start);
      ns += diff[0] * NS_PER_SEC + diff[1];
      gc(); 
    }
    
    console.log("| " + fileName + " |       " + (((fileSize / GbSize)) / ((ns / (NS_PER_SEC * 1.0)) / numberOfIterations)).toFixed(10) + "       |  GB/s  | " + numberOfIterations);
    ns = 0; 
});

console.log();

/*************** JSON#parse ***************/
console.log("|      Fichier       |        JSON#parse        | unité |  iterations");
files.forEach(function(fileName){
  var fileSize = getFileSize(jsonexamplesPath + fileName);
  for (let i = 0; i < numberOfIterations; i++) {
      file = fs.readFileSync(jsonexamplesPath + fileName, 'utf-8');
      var start = process.hrtime();
      var JSONObj = JSON.parse(file);
      var diff = process.hrtime(start);
      ns += diff[0] * NS_PER_SEC + diff[1];
      gc(); 
    }
    console.log("| " + fileName + " |       " + (((fileSize / GbSize)) / ((ns / (NS_PER_SEC * 1.0)) / numberOfIterations)).toFixed(10) + "       |  GB/s  | " + numberOfIterations);
    ns = 0; 
});
console.log();




function getFileSize(filePath) {
    var stats = fs.statSync(filePath);
    var size = stats["size"];
    return size / 1000;
  }