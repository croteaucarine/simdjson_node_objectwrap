'use strict';

const fs = require('fs');
var readlineSync = require('readline-sync');
const { simdjson } = require('bindings')('addon');

/*const apache_builds = 'jsonexamples/apache_builds.json';
const canada = 'jsonexamples/canada.json';
const citm_catalog = 'jsonexamples/citm_catalog.json';
const github_events = 'jsonexamples/github_events.json';
const gsoc_2018 = 'jsonexamples/gsoc-2018.json';
const instruments = 'jsonexamples/instruments.json';
const marine_ik = 'jsonexamples/marine_ik.json';
const mesh = 'jsonexamples/mesh.json';
const mesh_pretty = 'jsonexamples/mesh.pretty.json';
const numbers = 'jsonexamples/numbers.json';
const random = 'jsonexamples/random.json';
const twitter = 'jsonexamples/twitter.json';
const twitterescaped = 'jsonexamples/twitterescaped.json';
const update_center = 'jsonexamples/update-center.json';*/


var files = ['apache_builds.json', 'canada.json', 'citm_catalog.json', 'github_events.json', 'gsoc-2018.json', 'instruments.json', 'marine_ik.json', 'mesh.json', 'mesh.pretty.json', 'numbers.json', 'random.json', 'twitter.json', 'twitterescaped.json', 'update-center.json']
var jsonexamplesPath = 'jsonexamples/';

const numberOfIterations = 1000;
var start, end;
var time = 0;
var file;
const GbSize = 1024 * 1024;

/*************** JSON#parse ***************/
console.log("|      Fichier       |        JSON#parse        | unité |");
files.forEach(function(fileName){
  var fileSize = getFileSize(jsonexamplesPath + fileName);
  for (let i = 0; i < numberOfIterations; i++) {
      file = fs.readFileSync(jsonexamplesPath + fileName, 'utf-8');
      start = new Date();
      var JSONbuffer = JSON.parse(file);
      end = new Date();
      time += end.getTime() - start.getTime();
      gc(); 
    }
    console.log("| " + fileName + " |       " + (((fileSize / GbSize)) / ((time / 1000) / numberOfIterations)).toFixed(10) + "       |  GB/s  |");
    time = 0; 
});
console.log();


/*************** simdjson#parse ***************/
console.log("|      Fichier       |        simdjson#parse        |  unité |");

files.forEach(function(fileName){
  var fileSize = getFileSize(jsonexamplesPath + fileName);
  for (let i = 0; i < numberOfIterations; i++) {
      file = fs.readFileSync(jsonexamplesPath + fileName, 'utf-8');
      start = new Date();
      var simsdjsonbuffer = new simdjson(file);
      end = new Date();
      time += end.getTime() - start.getTime();
      gc(); 
    }
    
    console.log("| " + fileName + " |       " + (((fileSize / GbSize)) / ((time / 1000) / numberOfIterations)).toFixed(10) + "       |  GB/s  |");
    time = 0; 
});

console.log();

function getFileSize(filePath) {
    var stats = fs.statSync(filePath);
    var size = stats["size"];
    return size / 1000;
  }