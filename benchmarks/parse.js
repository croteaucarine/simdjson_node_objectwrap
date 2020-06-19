'use strict';

const fs = require('fs');
var readlineSync = require('readline-sync');
const { simdjson } = require('bindings')('addon');

var files = ['apache_builds.json', 'canada.json', 'citm_catalog.json', 'github_events.json', 'github_events_100.json', 'github_events_1000.json', 'gsoc-2018.json', 'instruments.json', 'marine_ik.json', 'mesh.json', 'mesh.pretty.json', 'numbers.json', 'random.json', 'twitter.json', 'twitterescaped.json', 'update-center.json']
var jsonexamplesPath = './jsonexamples/';

const numberOfIterations = 1000;

var diff;
var ns = 0;
var file;
const GbSize = 1024 * 1024;
const NS_PER_SEC = 1e9;
const NS_PER_MS = 1e6;
const MS_PER_SEC = 1000.0;


console.log("| Méthode |   Fichier   | Taille  |   (ms)   |  (GB/s)  | iterations ");
/*************** simdjson#load & simdjson#parseAfterObjectLoaded ***************/

files.forEach(function(fileName){
  var fileSize = getFileSize(jsonexamplesPath + fileName);
  for (let i = 0; i < numberOfIterations; i++) {
      var simdjsonObj = new simdjson();
      simdjsonObj.load({path : (jsonexamplesPath + fileName)});
      
      var start = process.hrtime.bigint();
      simdjsonObj.parse();
      var end = process.hrtime.bigint();
      ns += Number(end-start);

      gc(); 
    }
    var fileInGb = fileSize / GbSize;
    var timePerIteration = Math.round(ns / numberOfIterations * 1000) / 1000;
    console.log("| Méthode 1 | " + fileName + "|" + fileInGb.toFixed(15).replace('.', ',') + " |  " + (timePerIteration / NS_PER_MS).toFixed(3).replace('.', ',') + " | " + (fileInGb / (timePerIteration / NS_PER_SEC)).toFixed(2).replace('.', ',') + " | " + numberOfIterations);
    ns = 0; 
});

console.log();

/*************** simdjson#parseWithPath ***************/

files.forEach(function(fileName){
  var fileSize = getFileSize(jsonexamplesPath + fileName);
  for (let i = 0; i < numberOfIterations; i++) {
      var start = process.hrtime.bigint();
      var simdjsonObj = new simdjson({path : (jsonexamplesPath + fileName)});
      var end = process.hrtime.bigint();
      ns += Number(end-start);

      gc(); 
    }
    var fileInGb = fileSize / GbSize;
    var timePerIteration = Math.round(ns / numberOfIterations * 1000) / 1000;
    console.log("| Méthode 2 | " + fileName + "|" + fileInGb.toFixed(15).replace('.', ',') + " |  " + (timePerIteration / NS_PER_MS).toFixed(3).replace('.', ',') + " | " + (fileInGb / (timePerIteration / NS_PER_SEC)).toFixed(2).replace('.', ',') + " | " + numberOfIterations);
    ns = 0; 
});

console.log();

/*************** simdjson#parseWithString ***************/

files.forEach(function(fileName){
  var fileSize = getFileSize(jsonexamplesPath + fileName);
  for (let i = 0; i < numberOfIterations; i++) {
      var start = process.hrtime.bigint();
      file = fs.readFileSync(jsonexamplesPath + fileName, 'utf-8');
      var simdjsonObj = new simdjson(file);
      var end = process.hrtime.bigint();
      ns += Number(end-start);

      gc(); 
    }
    var fileInGb = fileSize / GbSize;
    var timePerIteration = Math.round(ns / numberOfIterations * 1000) / 1000;
    console.log("| Méthode 3 | " + fileName + "|" + fileInGb.toFixed(15).replace('.', ',') + " |  " + (timePerIteration / NS_PER_MS).toFixed(3).replace('.', ',') + " | " + (fileInGb / (timePerIteration / NS_PER_SEC)).toFixed(2).replace('.', ',') + " | " + numberOfIterations);
    ns = 0; 
});

console.log();

/*************** JSON#parse ***************/

files.forEach(function(fileName){
  var fileSize = getFileSize(jsonexamplesPath + fileName);
  for (let i = 0; i < numberOfIterations; i++) {
      var startLoad = process.hrtime.bigint();
      file = fs.readFileSync(jsonexamplesPath + fileName, 'utf-8');
      var endLoad = process.hrtime.bigint();
      var start = process.hrtime.bigint();
      var JSONObj = JSON.parse(file);
      var end = process.hrtime.bigint();
      ns += Number(end-start);

      gc(); 
    }
    var fileInGb = fileSize / GbSize;
    var timePerIteration = Math.round(ns / numberOfIterations * 1000) / 1000;
    console.log("| Méthode 4 | " + fileName + "|" + fileInGb.toFixed(15).replace('.', ',') + " |  " + (timePerIteration / NS_PER_MS).toFixed(3).replace('.', ',') + " | " + (fileInGb / (timePerIteration / NS_PER_SEC)).toFixed(2).replace('.', ',') + " | " + numberOfIterations);
    ns = 0; 
});
console.log();




function getFileSize(filePath) {
    var stats = fs.statSync(filePath);
    var size = stats["size"];
    return size / 1000;
  }
