'use strict';

const fs = require('fs');
var readlineSync = require('readline-sync');
const { simdjson } = require('bindings')('addon');

var files = ['apache_builds.json', 'canada.json', 'citm_catalog.json', 'github_events.json', 'gsoc-2018.json', 'instruments.json', 'marine_ik.json', 'mesh.json', 'mesh.pretty.json', 'numbers.json', 'random.json', 'twitter.json', 'twitterescaped.json', 'update-center.json']
var jsonexamplesPath = './jsonexamples/';

const numberOfIterations = 1;
var file;
const NS_PER_MS = 1000000;

var start;
var diff;
var ns = 0;

/*************** simdjson#keys ***************/
console.log("|      Fichier       |        simdjson#keys        |  unité  |   iterations  |   resultat    |");
files.forEach(function(fileName) {
  for (let i = 0; i < numberOfIterations; i++) {
    var simdjsonObj = new simdjson({path : (jsonexamplesPath + fileName)});
    var keys;
    try {
      start = process.hrtime();
      keys = simdjsonObj.keys();
      diff = process.hrtime(start);
      ns += diff[0] * NS_PER_MS + diff[1];
      
    } catch(error) {
      console.log("error" + error);
    }

    gc(); 
  }

  console.log("| " + fileName + " |       " +  ((ns / (NS_PER_MS * 1.0)) / numberOfIterations).toFixed(10).replace('.', ',') + "       |  ms     | " + numberOfIterations + " | " + keys.length + " keys : " + keys.toString().substring(0, 50)+ "...");
  
  ns = 0; 
});

console.log();

/*************** JSON#keys ***************/
console.log("|      Fichier       |        JSON#keys        |  unité  |   iterations  |   resultat    |");
files.forEach(function(fileName) {
  file = fs.readFileSync(jsonexamplesPath + fileName, 'utf-8');
  
  for (let i = 0; i < numberOfIterations; i++) {
    var jsonOBJ = JSON.parse(file);
  
    var keys;
      try {
        start = process.hrtime();
        keys = Object.keys(jsonOBJ);
        diff = process.hrtime(start);
        ns += diff[0] * NS_PER_MS + diff[1];
        
      } catch(error) {
        console.log("error" + error);
      }

      gc(); 
    }

    console.log("| " + fileName + " |       " +  ((ns / (NS_PER_MS * 1.0)) / numberOfIterations).toFixed(10).replace('.', ',') + "       |  ms     | " + numberOfIterations + " | " + keys.length + " keys : " + keys.toString().substring(0, 50) + "...");
    
    ns = 0; 

});
console.log();

/*************** simdjson#keys ***************/
console.log("|      Fichier       |        simdjson#keys        |  unité  |   iterations  |   resultat    |");
files.forEach(function(fileName) {
  var simdjsonObj = new simdjson({path : (jsonexamplesPath + fileName)});
  
  for (let i = 0; i < numberOfIterations; i++) {
    var keys;
    try {
      start = process.hrtime();
      keys = simdjsonObj.keys();
      diff = process.hrtime(start);
      ns += diff[0] * NS_PER_MS + diff[1];
      
    } catch(error) {
      console.log("error" + error);
    }

    gc(); 
  }

  console.log("| " + fileName + " |       " +  ((ns / (NS_PER_MS * 1.0)) / numberOfIterations).toFixed(10).replace('.', ',') + "       |  ms     | " + numberOfIterations + " | " + keys.length + " keys : " + keys.toString().substring(0, 50)+ "...");
  //console.log(keys.toString());
  ns = 0; 
});

console.log();

/*************** JSON#keys ***************/
console.log("|      Fichier       |        JSON#keys        |  unité  |   iterations  |   resultat    |");
files.forEach(function(fileName) {
  file = fs.readFileSync(jsonexamplesPath + fileName, 'utf-8');
  var jsonOBJ = JSON.parse(file);
  
  for (let i = 0; i < numberOfIterations; i++) {
    var keys;
    try {
      start = process.hrtime();
      keys = Object.keys(jsonOBJ);
      diff = process.hrtime(start);
      ns += diff[0] * NS_PER_MS + diff[1];
      
    } catch(error) {
      console.log("error" + error);
    }

    gc(); 
  }

  console.log("| " + fileName + " |       " +  ((ns / (NS_PER_MS * 1.0)) / numberOfIterations).toFixed(10).replace('.', ',') + "       |  ms     | " + numberOfIterations + " | " + keys.length + " keys : " + keys.toString().substring(0, 50) + "...");
  //console.log(keys.toString());
  
  ns = 0; 

});
console.log();