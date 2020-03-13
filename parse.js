'use strict';

const fs = require('fs');
var readlineSync = require('readline-sync');
const { simdjson } = require('bindings')('addon');

const update_center = '../jsonexamples/update-center.json';

var files = ['apache_builds.json', 'apache_builds.json', 'canada.json', 'citm_catalog.json', 'github_events.json', 'gsoc-2018.json', 'instruments.json', 'marine_ik.json', 'mesh.json', 'mesh.pretty.json', 'numbers.json', 'random.json', 'twitter.json', 'twitterescaped.json', 'update-center.json']
var jsonexamplesPath = 'jsonexamples/';

const numberOfIterations = 1000;
const NS_PER_MS = 1000000;

var diff;
var ns = BigInt(0);
var file;
const GbSize = 1024 * 1024;
const NS_PER_SEC = 1e9;

/*************** simdjson#parse ***************/
console.log("|      Fichier       |        simdjson#parse        |  unité |  iterations ");

files.forEach(function(fileName){
  var fileSize = getFileSize(jsonexamplesPath + fileName);
  for (let i = 0; i < numberOfIterations; i++) {
      //file = fs.readFileSync(jsonexamplesPath + fileName, 'utf-8');
      var start = process.hrtime.bigint();
      //var simdjsonObj = new simdjson({doc : file});
      var simdjsonObj = new simdjson({path : (jsonexamplesPath + fileName)});
      var end = process.hrtime.bigint();
      //console.log( (end - start) );
      //console.log();
      ns += (end-start);
      //console.log(ns);
      gc(); 
    }
    var fileInGb = fileSize / GbSize;
    var timePerIteration = Number(ns) / numberOfIterations;
    var timePerIterationPerSecond = timePerIteration / (NS_PER_SEC * 1.0);
    /*console.log("stats");
    console.log("fileSize " + fileSize);
    console.log("fileInGb " + fileInGb);
    console.log("ns " + ns);
    console.log("timePerIteration " + timePerIteration);
    console.log("timePerIterationPerSecond " + timePerIterationPerSecond);*/
    
    console.log("| " + fileName + " |       " + ((fileInGb / timePerIterationPerSecond).toFixed(10)) + "       |  GB/s  | " + numberOfIterations);
    ns = BigInt(0); 
});

console.log();

/*************** JSON#parse ***************/
console.log("|      Fichier       |        JSON#parse        | unité |  iterations");
files.forEach(function(fileName){
  var fileSize = getFileSize(jsonexamplesPath + fileName);
  for (let i = 0; i < numberOfIterations; i++) {
      file = fs.readFileSync(jsonexamplesPath + fileName, 'utf-8');
      var start = process.hrtime.bigint();
      var JSONObj = JSON.parse(file);
      var end = process.hrtime.bigint();
      ns += (end-start);
      gc(); 
    }
    var fileInGb = fileSize / GbSize;
    var timePerIteration = Number(ns) / numberOfIterations;
    var timePerIterationPerSecond = timePerIteration / (NS_PER_SEC * 1.0);
    console.log("| " + fileName + " |       " + ((fileInGb / timePerIterationPerSecond).toFixed(10)) + "       |  GB/s  | " + numberOfIterations);
    ns = BigInt(0); 
});
console.log();




function getFileSize(filePath) {
    var stats = fs.statSync(filePath);
    var size = stats["size"];
    return size / 1000;
  }