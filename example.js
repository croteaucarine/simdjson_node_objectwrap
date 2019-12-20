'use strict';

const fs = require('fs');
var readlineSync = require('readline-sync');
const { simdjson } = require('bindings')('addon');

const apache_builds = 'jsonexamples/apache_builds.json';
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
const update_center = 'jsonexamples/update-center.json';


/*const files = [apache_builds, canada, citm_catalog, github_events, gsoc_2018, instruments, marine_ik, 
    mesh, mesh_pretty, numbers, random, twitter, twitterescaped, update_center];
*/

const files = [github_events];

    files.forEach(file => {
        console.log("***************  " + file + "  ***************");
        const content = fs.readFileSync(file, 'utf-8');

        const simdjsonOBJ = new simdjson(content);
        const jsonOBJ = JSON.parse(content);
        
        /*console.log('1.1 Affichage de l\'objet simdjson');
        console.log(simdjsonOBJ);
        console.log();

        console.log('1.2 Affichage de l\'objet JSON');
        console.log(jsonOBJ);
        console.log();

        console.log("2.1 simdjsonOBJ.keys())");
        console.log(simdjsonOBJ.keys());

        console.log()

        console.log("2.2 Object.keys(jsonOBJ))");
        console.log(Object.keys(jsonOBJ));  

        console.log("3.1 simdjsonOBJ.length");
        console.log(simdjsonOBJ.length);

        console.log("3.2 jsonOBJ.length");
        console.log(jsonOBJ.length);

        console.log()*/

        console.log(simdjsonOBJ[0]);

        /*try {
            console.log("4.1 itération de l\'objet simdjson");
            var i  = 1;
            for (let item of simdjsonOBJ) {
                console.log(i + " - " + item);

                console.log(item);
                i++;
            }
        } catch(error) {
            console.error(error);
        }
        
        var response = readlineSync.question('Press enter to continue... q to quit');

        try {
            console.log("4.2 itération de l\'objet JSON");
            var i  = 1;
            for (let item of jsonOBJ) {
                console.log(i + " - " + item);

                console.log(item);

                i++;
            }
        } catch(error) {
            console.error(error);
        }

        console.log();

        /*console.log('Affichage de l\'objet - JSON.strignify');
        console.log(JSON.stringify(simdjsonOBJ));

        console.log();
        console.log('Affichage des propriétés de l\'objet (Object.getOwnPropertyNames)');
        console.log(Object.getOwnPropertyNames(simdjsonOBJ));

        console.log();
        console.log('Affichage des propriétés de l\'objet (Object.getOwnPropertyDescriptors)');
        console.log(Object.getOwnPropertyDescriptors(simdjsonOBJ));*/

        console.log();
        //var response = readlineSync.question('Press enter to continue... q to quit');
      }
    );

/*


console.log();
console.log('test itérateur');
for (let item of simdjsonOBJ) {
    console.log("item : " + item);
  }

  */
//console.log(JSON.parse(JSON.stringify(example)));
/*console.log(example.value);
console.log(example.test);
// It prints 11
/*example.value = 19;
console.log(example.value);
// It prints 19
example.readOnlyProp = 500;
console.log(example.readOnlyProp);
// Unchanged. It prints 19*/

function getAllFuncs(obj) {
    var props = [];

    do {
        props = props.concat(Object.getOwnPropertyNames(obj));
    } while (obj = Object.getPrototypeOf(obj));

    return props.sort().filter(function(e, i, arr) { 
        if(e) {
            console.log(e);
       if (e!=arr[i+1] && typeof obj[e] == 'function') return true;
    }
    });
}